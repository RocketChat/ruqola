/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "localdatabasebase.h"

class E2EKeyStore : public LocalDatabaseBase
{
public:
    E2EKeyStore();
    ~E2EKeyStore() override;

    bool saveKey(const QString &userId, const QByteArray &encryptedPrivateKey, const QByteArray &publicKey);
    bool loadKey(const QString &userId, QByteArray &encryptedPrivateKey, QByteArray &publicKey);
    bool deleteKey(const QString &userId);
    bool hasKey(const QString &userId);

    QString schemaDataBase() const override;

private:
    static const char s_schemaE2EKeyStore[];
};