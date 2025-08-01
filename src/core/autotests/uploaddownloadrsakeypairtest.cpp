#include "uploaddownloadrsakeypair.h"
#include "loginmanager.h"
#include "uploaddownloadrsakeypairtest.h"
#include <QCoreApplication>
#include <QObject>
#include <QTest>

void UploadDownloadRsaKeyPairTest::uploadDownloadCompare()
{
    auto app = QCoreApplication::instance();

    LoginManager loginManager;
    QNetworkAccessManager networkManager;
    const QString url = QStringLiteral("http://localhost:3000");
    const QString password = QStringLiteral("mypassword123");
    bool testPassed = false;

    QObject::connect(&loginManager, &LoginManager::loginSucceeded, this, [&](const QString &authToken, const QString &userId) {
        uploadKeys(authToken, url, userId, password, &networkManager, [&](const QString &message, const EncryptionUtils::RSAKeyPair &keypair) {
            downloadKeys(authToken, url, userId, password, &networkManager, [&](const QString &publicKey, const QString &decryptedPrivateKey) {
                QCOMPARE(publicKey, QString::fromUtf8(keypair.publicKey));
                QCOMPARE(decryptedPrivateKey, QString::fromUtf8(keypair.privateKey));
                testPassed = true;
                app->quit();
            });
        });
    });

    QObject::connect(&loginManager, &LoginManager::loginFailed, this, [&](const QString &err) {
        QFAIL(qPrintable(QStringLiteral("Login failed: %1").arg(err)));
        app->quit();
    });

    loginManager.login(url, &networkManager);
    app->exec();

    QVERIFY(testPassed);
}

QTEST_GUILESS_MAIN(UploadDownloadRsaKeyPairTest)
#include "uploaddownloadrsakeypairtest.moc"
