/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rsapairtest.h"
#include "encryption/encryptionutils.h"
#include <QTest>

QTEST_GUILESS_MAIN(RsaPairTest)
RsaPairTest::RsaPairTest(QObject *parent)
    : QObject(parent)
{
}

void RsaPairTest::rsaPairGenerationNonDeterminismTest()
{
    EncryptionUtils::RSAKeyPair rsaPair1;
    EncryptionUtils::RSAKeyPair rsaPair2;

    for (int i = 0; i <= 10; i++) {
        rsaPair1 = EncryptionUtils::generateRSAKey();
        rsaPair2 = EncryptionUtils::generateRSAKey();
        QVERIFY(rsaPair1.publicKey != rsaPair2.publicKey);
        QVERIFY(rsaPair1.privateKey != rsaPair2.privateKey);
    }
}

/**
 * @brief Tests the determinism of private key encryption and decryption using the master key.
 *
 * Definitions:
 *   - x = master key
 *
 *   - y = initial private key
 *
 *   - z = encrypt(x, y) = encrypted private key
 *
 *   - w = decrypt(x, z) = decrypted private key
 *
 * The test verifies:
 *
 *   If the same master key x and private key y are used,
 *     then decrypting the encrypted key yields the original key:
 *
 *   - decrypt(x, encrypt(x, y)) = y = initial private key
 *
 *   In other words, w = y iff x and y are unchanged.
 */
void RsaPairTest::encryptDecryptDeterminismTest()
{
    EncryptionUtils::RSAKeyPair rsaPair1;
    EncryptionUtils::RSAKeyPair rsaPair2;

    QByteArray privateKey1;
    QByteArray privateKey2;

    QByteArray masterKey;

    QByteArray encryptedPrivateKey1;
    QByteArray encryptedPrivateKey2;
    QByteArray decryptedPrivateKey1;
    QByteArray decryptedPrivateKey2;

    for (int i = 0; i <= 10; i++) {
        rsaPair1 = EncryptionUtils::generateRSAKey();
        rsaPair2 = EncryptionUtils::generateRSAKey();

        privateKey1 = rsaPair1.privateKey;
        privateKey2 = rsaPair2.privateKey;

        masterKey = EncryptionUtils::getMasterKey(EncryptionUtils::generateRandomText(32), EncryptionUtils::generateRandomText(32));

        encryptedPrivateKey1 = EncryptionUtils::encryptPrivateKey(rsaPair1.privateKey, masterKey);
        encryptedPrivateKey2 = EncryptionUtils::decryptPrivateKey(rsaPair2.privateKey, masterKey);

        decryptedPrivateKey1 = EncryptionUtils::decryptPrivateKey(encryptedPrivateKey1, masterKey);
        decryptedPrivateKey2 = EncryptionUtils::decryptPrivateKey(encryptedPrivateKey2, masterKey);

        QVERIFY(decryptedPrivateKey1 == privateKey1);
        QVERIFY(decryptedPrivateKey2 == privateKey2);
    }
}

#include "moc_rsapairtest.cpp"