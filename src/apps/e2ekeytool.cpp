/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

/*

test my existance...of course within server, otherwise I still exist :)

curl -X POST http://localhost:3000/api/v1/login \
  -H "Content-Type: application/json" \
  -d '{"user": "", "password": ""}'
*/

#include "authentication/loginjob.h"
#include "e2e/fetchmykeysjob.h"
#include "e2e/setuserpublicandprivatekeysjob.h"
#include "encryptionutils.h"
#include "restapimethod.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <qfile.h>

using namespace EncryptionUtils;
using namespace RocketChatRestApi;

const QString url = QStringLiteral("http://localhost:3000");

QHash<QString, QString> loadCredentials(const QString &filePath)
{
    QHash<QString, QString> creds;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open credentials file:" << filePath;
        return creds;
    }

    while (!file.atEnd()) {
        const QString line = QString::fromUtf8(file.readLine()).trimmed();

        if (line.startsWith(QLatin1Char('#')) || !line.contains(QLatin1Char('=')))
            continue;

        const auto parts = line.split(QLatin1Char('='), Qt::KeepEmptyParts);
        if (parts.size() == 2)
            creds[parts[0].trimmed()] = parts[1].trimmed();
    }

    return creds;
}

void uploadKeys(const QString &authToken, const QString &userId, const QString &password, QNetworkAccessManager *networkManager)
{
    RSAKeyPair keyPair = generateRSAKey();
    QByteArray masterKey = getMasterKey(password, QStringLiteral("salt"));
    QByteArray encryptedPrivateKey = encryptPrivateKey(keyPair.privateKey, masterKey);

    auto *uploadJob = new SetUserPublicAndPrivateKeysJob();

    auto *restApiMethod = new RestApiMethod();
    restApiMethod->setServerUrl(url);

    uploadJob->setRestApiMethod(restApiMethod);
    uploadJob->setNetworkAccessManager(networkManager);

    uploadJob->setAuthToken(authToken);
    uploadJob->setUserId(userId);

    SetUserPublicAndPrivateKeysJob::SetUserPublicAndPrivateKeysInfo info;
    info.rsaPublicKey = QString::fromUtf8(keyPair.publicKey);
    info.rsaPrivateKey = QString::fromUtf8(encryptedPrivateKey.toBase64());
    info.force = true;
    uploadJob->setSetUserPublicAndPrivateKeysInfo(info);

    QObject::connect(uploadJob, &SetUserPublicAndPrivateKeysJob::setUserPublicAndPrivateKeysDone, uploadJob, []() {
        qDebug() << "Key upload successful!";
        QCoreApplication::quit();
    });

    QObject::connect(uploadJob, &SetUserPublicAndPrivateKeysJob::failed, uploadJob, [](const QString &err) {
        qCritical() << "Key upload failed: " << err;
        QCoreApplication::quit();
    });

    uploadJob->start();
}

void downloadKeys(const QString &password)
{
    auto *fetchJob = new FetchMyKeysJob();

    QObject::connect(fetchJob, &FetchMyKeysJob::fetchMyKeysDone, fetchJob, [password](const QJsonObject &jsonObj) {
        QString publicKey = jsonObj["public_key"_L1].toString();
        QString encryptedPrivateKeyB64 = jsonObj["private_key"_L1].toString();

        qDebug() << "Downloaded Public Key:" << publicKey;

        QByteArray encryptedPrivateKey = QByteArray::fromBase64(encryptedPrivateKeyB64.toUtf8());
        QByteArray masterKey = getMasterKey(password, QStringLiteral("salt"));
        QByteArray decryptedPrivateKey = decryptPrivateKey(encryptedPrivateKey, masterKey);

        qDebug() << "Decrypted Private Key:\n" << QString::fromUtf8(decryptedPrivateKey);

        QCoreApplication::quit();
    });

    QObject::connect(fetchJob, &RestApiAbstractJob::failed, nullptr, [=](const QString &err) {
        qCritical() << "Key fetch failed:" << err;
        QCoreApplication::quit();
    });

    fetchJob->start();
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QNetworkAccessManager *networkManager = new QNetworkAccessManager(&app);

    auto *loginJob = new LoginJob(&app);
    auto *restApiMethod = new RestApiMethod();

    auto creds = loadCredentials(QStringLiteral("/home/edc/rocketchat/ruqola/src/apps/credentials.txt"));

    restApiMethod->setServerUrl(url);

    loginJob->setRestApiMethod(restApiMethod);
    loginJob->setNetworkAccessManager(networkManager);

    loginJob->setUserName(creds.value(QStringLiteral("USERNAME")));
    loginJob->setPassword(creds.value(QStringLiteral("PASSWORD")));

    QObject::connect(loginJob, &LoginJob::loginDone, &app, [networkManager](const QString &authToken, const QString &userId) {
        qDebug() << "Login successful! Auth token:" << authToken << "UserId:" << userId;
        // uploadKeys(authToken, userId, QStringLiteral("mypassword123"), networkManager);
    });

    QObject::connect(loginJob, &RestApiAbstractJob::failed, &app, [](const QString &err) {
        qCritical() << "Login failed:" << err;
        QCoreApplication::quit();
    });

    loginJob->start();

    return app.exec();
}