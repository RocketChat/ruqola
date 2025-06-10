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
 * @brief Tests the determinism of private key encoding and decoding using the master key.
 *
 * Definitions:
 *   - x = master key
 *
 *   - y = initial private key
 *
 *   - z = encode(x, y) = encoded private key
 *
 *   - w = decode(x, z) = decoded private key
 *
 * The test verifies:
 *
 *   If the same master key x and private key y are used,
 *     then decoding the encoded key yields the original key:
 *
 *   - decode(x, encode(x, y)) = y = initial private key
 *
 *   In other words, w = y iff x and y are unchanged.
 */
void RsaPairTest::encodeDecodeDeterminismTest()
{
    EncryptionUtils::RSAKeyPair rsaPair1;
    EncryptionUtils::RSAKeyPair rsaPair2;

    QByteArray privateKey1;
    QByteArray privateKey2;

    QByteArray masterKey;

    QByteArray encodedPrivateKey1;
    QByteArray encodedPrivateKey2;
    QByteArray decodedPrivateKey1;
    QByteArray decodedPrivateKey2;

    for (int i = 0; i <= 10; i++) {
        rsaPair1 = EncryptionUtils::generateRSAKey();
        rsaPair2 = EncryptionUtils::generateRSAKey();

        privateKey1 = rsaPair1.privateKey;
        privateKey2 = rsaPair2.privateKey;

        masterKey = EncryptionUtils::getMasterKey(EncryptionUtils::generateRandomText(32), EncryptionUtils::generateRandomText(32));

        encodedPrivateKey1 = EncryptionUtils::encodePrivateKey(rsaPair1.privateKey, masterKey);
        encodedPrivateKey2 = EncryptionUtils::encodePrivateKey(rsaPair2.privateKey, masterKey);

        decodedPrivateKey1 = EncryptionUtils::decodePrivateKey(encodedPrivateKey1, masterKey);
        decodedPrivateKey2 = EncryptionUtils::decodePrivateKey(encodedPrivateKey2, masterKey);

        QVERIFY(decodedPrivateKey1 == privateKey1);
        QVERIFY(decodedPrivateKey2 == privateKey2);
    }
}

#include "moc_rsapairtest.cpp"