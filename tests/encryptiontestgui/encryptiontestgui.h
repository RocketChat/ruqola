/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
<<<<<<< HEAD
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <montel@kde.org>
=======
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
>>>>>>> gsoc2025

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "encryption/encryptionutils.h"
#include <QWidget>

class QTextEdit;
class EncryptionTestGui : public QWidget
{
    Q_OBJECT
public:
    explicit EncryptionTestGui(QWidget *parent = nullptr);
    ~EncryptionTestGui() override = default;

private:
    QTextEdit *const mTextEdit;
    QTextEdit *const mTextEditResult;
<<<<<<< HEAD
    QByteArray mMasterKey = nullptr;
    QString password;
    QString userId;
    QByteArray mEncodedPrivateKey = nullptr;
    QByteArray mDecodedPrivateKey = nullptr;
    EncryptionUtils::RSAKeyPair mRsaKeyPair;
=======
    QByteArray mMasterKey;
    QString mPassword;
    QString mUserId;
>>>>>>> gsoc2025
};
