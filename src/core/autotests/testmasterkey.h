/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class TestMasterKey : public QObject
{
    Q_OBJECT
public:
    explicit TestMasterKey(QObject *parent = nullptr);
    ~TestMasterKey() override = default;

private Q_SLOTS:
    void testMasterKeyDeterminism();
    void testMasterKeyEmptyUserId();
    void testMasterKeyEmptyPassword();
    void testImportRawKey();
};
