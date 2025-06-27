/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageencryptiondecryptiontest.h"
#include "encryption/encryptionutils.h"
#include <QTest>

QTEST_GUILESS_MAIN(MessageEncryptionDecryptionTest)
MessageEncryptionDecryptionTest::MessageEncryptionDecryptionTest(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief Tests symmetric encryption and decryption of a message using a session key.
 *
 * Verifies that for a message `m` and session key `k`, decryption function `D`
 * and encryption function `E` the property holds:
 *
 *     `D(E(m, k), k) = m`
 */
void MessageEncryptionDecryptionTest::messageEncryptionDecryptionTest()
{
    const QByteArray sessionKey1 = EncryptionUtils::generateSessionKey();
    const QByteArray sessionKey2 = EncryptionUtils::generateSessionKey();
    auto message = QStringLiteral("This is GSoC 2025, Andro Ranogajec got to the end of 'phase1' :)").toUtf8();
    QByteArray encryptedMessage = EncryptionUtils::encryptMessage(message, sessionKey1);
    QByteArray decryptedMessage = EncryptionUtils::decryptMessage(encryptedMessage, sessionKey1);

    QVERIFY(message == decryptedMessage);

    for (int i = 1; i <= 10; ++i) {
        message = EncryptionUtils::generateRandomText(i).toUtf8();
        encryptedMessage = EncryptionUtils::encryptMessage(message, sessionKey1);
        decryptedMessage = EncryptionUtils::decryptMessage(encryptedMessage, sessionKey2);
        QVERIFY(message != decryptedMessage);
    }
}

#include "moc_messageencryptiondecryptiontest.cpp"