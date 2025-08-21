/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sessionkeydistributiontest.h"
#include "e2e/acceptsuggestedgroupkeyjob.h"
#include "e2e/provideuserswithsuggestedgroupkeysjob.h"
#include "e2e/rejectsuggestedgroupkeyjob.h"
#include "encryption/encryptionutils.h"
#include "loginmanager.h"
#include "uploaddownloadrsakeypair.h"
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QTest>
QTEST_GUILESS_MAIN(SessionKeyDistributionTest)

void SessionKeyDistributionTest::testSessionKeyDistribution()
{
    const auto app = QCoreApplication::instance();
    const auto networkManager = new QNetworkAccessManager(app);
    const auto url = QStringLiteral("http://localhost:3000");
    const auto password = QStringLiteral("mypassword123");
    const auto roomId = QStringLiteral("123"); // Replace with a real room ID

    // Step 1: Login as two user
    auto loginManager0 = new LoginManager(app);
    auto loginManager1 = new LoginManager(app);
    QString user1Id, user2Id, user1Auth, user2Auth;
    EncryptionUtils::RSAKeyPair user1KeyPair, user2KeyPair;
    QByteArray sessionKey;
    auto testPassed = false;

    // Helper: proceed when both users are ready
    int readyCount = 0;
    auto proceed = [&]() {
        if (++readyCount < 2)
            return;

        // Step 2: Generate session key (AES-128)
        sessionKey = EncryptionUtils::generateSessionKey();

        // Step 3: Encrypt session key with each user's public key
        QVector<RocketChatRestApi::SuggestedGroupKey> suggestedKeys;
        const auto encryptedSessionKeyForUser1 = EncryptionUtils::encryptSessionKey(sessionKey, EncryptionUtils::publicKeyFromPEM(user1KeyPair.publicKey));
        const auto encryptedSessionKeyForUser2 = EncryptionUtils::encryptSessionKey(sessionKey, EncryptionUtils::publicKeyFromPEM(user2KeyPair.publicKey));
        suggestedKeys.append({user1Id, QString::fromLatin1(encryptedSessionKeyForUser1.toBase64())});
        suggestedKeys.append({user2Id, QString::fromLatin1(encryptedSessionKeyForUser2.toBase64())});

        // Step 4: Distribute encrypted keys using API
        const auto provideJob = new RocketChatRestApi::ProvideUsersWithSuggestedGroupKeysJob(app);
        provideJob->setRoomId(roomId);
        provideJob->setKeys(suggestedKeys);
        QObject::connect(
            provideJob,
            &RocketChatRestApi::ProvideUsersWithSuggestedGroupKeysJob::provideUsersWithSuggestedGroupKeysDone,
            app,
            [&](const QJsonObject &) {
                // Simulate user1 receiving and accepting the key
                const auto encKey1 = QByteArray::fromBase64(suggestedKeys[0].encryptedKey.toUtf8());
                const auto decKey1 = EncryptionUtils::decryptSessionKey(encKey1, EncryptionUtils::privateKeyFromPEM(user1KeyPair.privateKey));
                QCOMPARE(decKey1, sessionKey);

                const auto acceptJob1 = new RocketChatRestApi::AcceptSuggestedGroupKeyJob(app);
                acceptJob1->setRoomId(roomId);
                QObject::connect(acceptJob1, &RocketChatRestApi::AcceptSuggestedGroupKeyJob::acceptSuggestedGroupKeyDone, app, [&](const QJsonObject &) {
                    // Simulate user2 receiving and rejecting the key
                    const auto encKey2 = QByteArray::fromBase64(suggestedKeys[1].encryptedKey.toUtf8());
                    const auto decKey2 = EncryptionUtils::decryptSessionKey(encKey2, EncryptionUtils::privateKeyFromPEM(user2KeyPair.privateKey));
                    QCOMPARE(decKey2, sessionKey);

                    const auto rejectJob2 = new RocketChatRestApi::RejectSuggestedGroupKeyJob(app);
                    rejectJob2->setRoomId(roomId);
                    QObject::connect(rejectJob2, &RocketChatRestApi::RejectSuggestedGroupKeyJob::rejectSuggestedGroupKeyDone, app, [&](const QJsonObject &) {
                        testPassed = true;
                        app->quit();
                    });
                    rejectJob2->start();
                });
                acceptJob1->start();
            });
        provideJob->start();
    };

    // Step 1a: Login and upload keys for user1
    QObject::connect(loginManager0,
                     &LoginManager::loginSucceeded,
                     this,
                     [=, &user1Id, &user1Auth, &user1KeyPair](const QString &authToken, const QString &userId) {
                         user1Id = userId;
                         user1Auth = authToken;
                         uploadKeys(authToken, url, userId, password, networkManager, [&](const QString &, const EncryptionUtils::RSAKeyPair &keypair) {
                             user1KeyPair = keypair;
                             proceed();
                         });
                     });
    loginManager0->login(url, networkManager, 0);

    // Step 1b: Login and upload keys for user2
    QObject::connect(loginManager1,
                     &LoginManager::loginSucceeded,
                     this,
                     [=, &user2Id, &user2Auth, &user2KeyPair](const QString &authToken, const QString &userId) {
                         user2Id = userId;
                         user2Auth = authToken;
                         uploadKeys(authToken, url, userId, password, networkManager, [&](const QString &, const EncryptionUtils::RSAKeyPair &keypair) {
                             user2KeyPair = keypair;
                             proceed();
                         });
                     });
    loginManager1->login(url, networkManager, 1);

    // Handle login failures
    QObject::connect(loginManager0, &LoginManager::loginFailed, this, [=](const QString &err) {
        QFAIL(qPrintable(QStringLiteral("User1 login failed: %1").arg(err)));
        app->quit();
    });
    QObject::connect(loginManager1, &LoginManager::loginFailed, this, [=](const QString &err) {
        QFAIL(qPrintable(QStringLiteral("User2 login failed: %1").arg(err)));
        app->quit();
    });

    app->exec();
    QVERIFY(testPassed);
}

void SessionKeyDistributionTest::testJsonPayload()
{
    RocketChatRestApi::ProvideUsersWithSuggestedGroupKeysJob job;
    job.setRoomId("123");
    const QVector<RocketChatRestApi::SuggestedGroupKey> suggestedGroupKeys = {{"userA", "base64keyA"}, {"userB", "base64keyB"}};
    job.setKeys(suggestedGroupKeys);

    const QJsonDocument doc = job.json();
    const QJsonObject obj = doc.object();
    QCOMPARE(obj["rid"].toString(), QStringLiteral("123"));
    QJsonArray arr = obj["keys"].toArray();
    QCOMPARE(arr.size(), 2);
    QCOMPARE(arr[0].toObject()["userId"].toString(), QStringLiteral("userA"));
    QCOMPARE(arr[0].toObject()["key"].toString(), QStringLiteral("base64keyA"));
}

void SessionKeyDistributionTest::testCanStartValidation()
{
    RocketChatRestApi::ProvideUsersWithSuggestedGroupKeysJob job;
    QVERIFY(!job.canStart()); // No roomId or keys

    job.setRoomId("room123");
    QVERIFY(!job.canStart()); // No keys

    QVector<RocketChatRestApi::SuggestedGroupKey> keys = {{"userA", "base64keyA"}};
    job.setKeys(keys);
    QVERIFY(job.canStart()); // Now valid
}

#include "sessionkeydistributiontest.moc"