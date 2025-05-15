/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptionutils.h"
#include <QDebug>
#include <QTest>

class TestMasterKey : public QObject
{
    /// @brief This class is testing master key functionality.
    Q_OBJECT
private slots:
    void testMasterKeyDeterminism();
    void testImportRawKey();
};

/**
 * @brief This methods to going to test the determinism the master key.
 *
 * if
 * n == n1 and m == m1
 * then
 * getMasterKey(n, m) == getMasterKey(n1, m1)
 */
void TestMasterKey::testMasterKeyDeterminism()
{
}

/**
 * @brief This methods to going to test the determinism the base key.
 *
 * if
 * n == n1 and m == m1
 * then
 * importRawKey(n, m) == importRawKey(n1, m1)
 */
void TestMasterKey::testImportRawKey()
{
}