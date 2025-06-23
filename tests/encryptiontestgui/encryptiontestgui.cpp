/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptiontestgui.h"
#include "encryption/encryptionutils.h"
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

EncryptionTestGui::EncryptionTestGui(QWidget *parent)
    : QWidget{parent}
    , mTextEdit(new QTextEdit(this))
    , mTextEditResult(new QTextEdit(this))

{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    auto labelInput = new QLabel(QStringLiteral("Input"), this);
    mainLayout->addWidget(labelInput);
    mainLayout->addWidget(mTextEdit);

    auto pushButtonDeriveMasterKey = new QPushButton(QStringLiteral("Derive Master Key"), this);
    mainLayout->addWidget(pushButtonDeriveMasterKey);

    connect(pushButtonDeriveMasterKey, &QPushButton::clicked, this, [this]() {
        auto *dialog = new QDialog(this);
        dialog->setWindowTitle(QStringLiteral("Credentials"));

        auto *userIdEdit = new QLineEdit(dialog);
        auto *passwordEdit = new QLineEdit(dialog);
        passwordEdit->setEchoMode(QLineEdit::Password);

        auto *layout = new QGridLayout(dialog);
        layout->addWidget(new QLabel(QStringLiteral("Username: "), dialog), 0, 0);
        layout->addWidget(userIdEdit, 0, 1);
        layout->addWidget(new QLabel(QStringLiteral("Password: "), dialog), 1, 0);
        layout->addWidget(passwordEdit, 1, 1);

        auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
        layout->addWidget(buttonBox, 2, 0, 1, 2);

        connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

        if (dialog->exec()) {
            mUsername = userIdEdit->text();
            mPassword = passwordEdit->text();
            mMasterKey = EncryptionUtils::getMasterKey(mPassword, mUsername);
            qDebug() << "Derived Master Key:" << mMasterKey.toBase64();
            mTextEditResult->setPlainText((QStringLiteral("Master Key derivation succeeded!\n") + QString::fromUtf8(mMasterKey.toBase64())));
        }
        delete dialog;
    });

    auto pushButtonGenerateRSAKey = new QPushButton(QStringLiteral("Generate RSA Pair"), this);
    mainLayout->addWidget(pushButtonGenerateRSAKey);
    connect(pushButtonGenerateRSAKey, &QPushButton::clicked, this, [this]() {
        mRsaKeyPair = EncryptionUtils::generateRSAKey();
        qDebug() << mRsaKeyPair.publicKey;
        qDebug() << mRsaKeyPair.privateKey;
    });

    auto pushButtonGenerateSessionKey = new QPushButton(QStringLiteral("Generate Session Key"), this);
    mainLayout->addWidget(pushButtonGenerateSessionKey);
    connect(pushButtonGenerateSessionKey, &QPushButton::clicked, this, [this]() {
        mSessionKey = EncryptionUtils::generateSessionKey();
        qDebug() << "Derived Session Key:" << mSessionKey.toBase64();
        mTextEditResult->setPlainText((QStringLiteral("Session key generation succeeded!\n") + QString::fromUtf8(mSessionKey.toBase64())));
    });

    auto pushButtonEncryptSessionKey = new QPushButton(QStringLiteral("Encrypt Session Key"), this);
    mainLayout->addWidget(pushButtonEncryptSessionKey);
    connect(pushButtonEncryptSessionKey, &QPushButton::clicked, this, [this]() {
        auto publicKey = mRsaKeyPair.publicKey.toUtf8();
        if (publicKey.isEmpty())
            mTextEditResult->setPlainText((QStringLiteral("Public key is empty, session key encryption failed!\n")));
        else {
            RSA *publicKeyfromPem = EncryptionUtils::publicKeyFromPEM(publicKey);
            mEncryptedSessionKey = EncryptionUtils::encryptSessionKey(mSessionKey, publicKeyfromPem);
            qDebug() << "Public Key from PEM:" << publicKeyfromPem;
            qDebug() << "Encrypted Session Key:" << mEncryptedSessionKey.toBase64();
            mTextEditResult->setPlainText((QStringLiteral("Session key encryption succeeded!\n") + QString::fromUtf8(mEncryptedSessionKey.toBase64())));
        }
    });

    auto pushButtonDecryptSessionKey = new QPushButton(QStringLiteral("Decrypt Session Key"), this);
    mainLayout->addWidget(pushButtonDecryptSessionKey);
    connect(pushButtonDecryptSessionKey, &QPushButton::clicked, this, [this]() {
        auto privateKey = mRsaKeyPair.privateKey.toUtf8();
        if (privateKey.isEmpty())
            mTextEditResult->setPlainText((QStringLiteral("Private key is empty, session key decryption failed!\n")));
        else {
            RSA *privateKeyfromPem = EncryptionUtils::privateKeyFromPEM(privateKey);
            mDecryptedSessionKey = EncryptionUtils::decryptSessionKey(mEncryptedSessionKey, privateKeyfromPem);
            qDebug() << "Private Key from PEM:" << privateKeyfromPem;
            qDebug() << "Decrypted Session Key:" << mDecryptedSessionKey.toBase64();
            mTextEditResult->setPlainText((QStringLiteral("Session key decryption succeeded!\n") + QString::fromUtf8(mDecryptedSessionKey.toBase64())));
        }
    });
    auto pushButtonEncrypt = new QPushButton(QStringLiteral("Encrypt message"), this);
    mainLayout->addWidget(pushButtonEncrypt);
    connect(pushButtonEncrypt, &QPushButton::clicked, this, [this]() {
        const auto text = mTextEdit->toPlainText();
        if (text.isEmpty()) {
            mTextEditResult->setPlainText((QStringLiteral("Text cannot be null, message encryption failed!\n")));
        } else {
            mEncryptedMessage = EncryptionUtils::encryptMessage(text.toUtf8(), mSessionKey);
            qDebug() << "Encrypted message:" << mEncryptedMessage.toBase64();
            mTextEditResult->setPlainText((QStringLiteral("Message encryption succeeded!\n") + QString::fromUtf8(mEncryptedMessage.toBase64())));
            mTextEdit->clear();
        }
    });
    auto pushButtonDecrypt = new QPushButton(QStringLiteral("Decrypt message"), this);
    mainLayout->addWidget(pushButtonDecrypt);
    connect(pushButtonDecrypt, &QPushButton::clicked, this, [this]() {
        qDebug() << "Session key:" << mSessionKey;
        if (QString::fromUtf8(mEncryptedMessage).isEmpty()) {
            mTextEditResult->setPlainText((QStringLiteral("Encrypted message is null, message decryption failed!\n")));
            return;
        }
        mDecryptedMessage = EncryptionUtils::decryptMessage(mEncryptedMessage, mSessionKey);
        qDebug() << "Decrypted message:" << mDecryptedMessage;
        mTextEditResult->setPlainText((QStringLiteral("Message decryption succeeded!\n") + QString::fromUtf8(mDecryptedMessage)));
    });

    auto pushButtonReset = new QPushButton(QStringLiteral("Reset"), this);
    mainLayout->addWidget(pushButtonReset);
    connect(pushButtonReset, &QPushButton::clicked, this, [this]() {
        mTextEdit->clear();
        mTextEditResult->clear();
        mMasterKey.clear();
        mPassword = QStringLiteral("");
        mUsername = QStringLiteral("");
        mRsaKeyPair.privateKey.clear();
        mRsaKeyPair.publicKey.clear();
        mSessionKey.clear();
        mEncryptedSessionKey.clear();
        mDecryptedSessionKey.clear();
        mEncryptedMessage.clear();
        mDecryptedMessage.clear();
        qDebug() << "Master Key: " << mMasterKey << "\nusername: " << mUsername << "\npassword: " << mPassword << "\nprivatekey: " << mRsaKeyPair.privateKey
                 << "\npublickey: " << mRsaKeyPair.publicKey << "\nencrypted session key: " << mEncryptedSessionKey
                 << "\ndecrypted session key: " << mDecryptedSessionKey << "\nencrypted message: " << mEncryptedMessage
                 << "\ndecrypted message: " << mDecryptedMessage;
        mTextEditResult->setPlainText((QStringLiteral("Reset succeded!\n")));
    });

    mTextEditResult->setReadOnly(true);
    auto labelOutput = new QLabel(QStringLiteral("Output"), this);
    mainLayout->addWidget(labelOutput);
    mainLayout->addWidget(mTextEditResult);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    EncryptionTestGui w;
    w.resize(800, 600);
    w.show();
    return app.exec();
}

#include "moc_encryptiontestgui.cpp"
