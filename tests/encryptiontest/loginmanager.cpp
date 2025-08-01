/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loginmanager.h"

LoginManager::LoginManager(QObject *parent)
    : QObject(parent)
{
}

void LoginManager::login(const QString &serverUrl, QNetworkAccessManager *networkManager)
{
    auto envPath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("../../.env"));
    auto creds = loadEnvFile(envPath);

    loginJob = new RocketChatRestApi::LoginJob(this);
    restApiMethod = new RocketChatRestApi::RestApiMethod();
    restApiMethod->setServerUrl(serverUrl);

    loginJob->setRestApiMethod(restApiMethod);
    loginJob->setNetworkAccessManager(networkManager);
    loginJob->setUserName(creds.value(QStringLiteral("USERNAME")));
    loginJob->setPassword(creds.value(QStringLiteral("PASSWORD")));

    QObject::connect(loginJob, &RocketChatRestApi::LoginJob::loginDone, this, [this](const QString &authToken, const QString &userId) {
        Q_EMIT loginSucceeded(authToken, userId);
    });

    QObject::connect(loginJob, &RocketChatRestApi::RestApiAbstractJob::failed, this, [this](const QString &err) {
        Q_EMIT loginFailed(err);
    });

    loginJob->start();
}