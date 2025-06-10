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
        layout->addWidget(new QLabel(QStringLiteral("UserId: "), dialog), 0, 0);
        layout->addWidget(userIdEdit, 0, 1);
        layout->addWidget(new QLabel(QStringLiteral("Password: "), dialog), 1, 0);
        layout->addWidget(passwordEdit, 1, 1);

        auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
        layout->addWidget(buttonBox, 2, 0, 1, 2);

        connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

        if (dialog->exec()) {
            mUserId = userIdEdit->text();
            mPassword = passwordEdit->text();
            mMasterKey = EncryptionUtils::getMasterKey(mPassword, mUserId);
            qDebug() << "Derived Master Key:" << mMasterKey.toBase64();
            mTextEditResult->setPlainText((QStringLiteral("Master Key derivation succeeded!\n") + QString::fromUtf8(mMasterKey.toBase64())));
        }
        delete dialog;
    });

    auto pushButtonGenerateRSAKey = new QPushButton(QStringLiteral("Generate RSA Pair"), this);
    mainLayout->addWidget(pushButtonGenerateRSAKey);
    connect(pushButtonGenerateRSAKey, &QPushButton::clicked, this, [this]() {
        mRsaKeyPair = EncryptionUtils::generateRSAKey();
        qDebug() << "Public Key:\n " << mRsaKeyPair.publicKey << "Private Key:\n " << mRsaKeyPair.privateKey;
        mTextEditResult->setPlainText(QStringLiteral("RSA pair generation succeded!\n") + QString::fromUtf8(mRsaKeyPair.publicKey)
                                      + QString::fromUtf8(mRsaKeyPair.privateKey));
    });

    auto pushButtonEncodePrivateKey = new QPushButton(QStringLiteral("Encode Private Key"), this);
    mainLayout->addWidget(pushButtonEncodePrivateKey);
    connect(pushButtonEncodePrivateKey, &QPushButton::clicked, this, [this]() {
        mEncodedPrivateKey = EncryptionUtils::encodePrivateKey(mRsaKeyPair.privateKey, mMasterKey);
        qDebug() << mEncodedPrivateKey.toBase64() << "encoded private key ";
        mTextEditResult->setPlainText(QStringLiteral("Private key encryption succeded!\n") + QString::fromUtf8(mEncodedPrivateKey.toBase64()));
    });

    auto pushButtonDecodePrivateKey = new QPushButton(QStringLiteral("Decode Private Key"), this);
    mainLayout->addWidget(pushButtonDecodePrivateKey);
    connect(pushButtonDecodePrivateKey, &QPushButton::clicked, this, [this]() {
        mDecodedPrivateKey = EncryptionUtils::decodePrivateKey(mEncodedPrivateKey, mMasterKey);
        mTextEditResult->setPlainText(QStringLiteral("Private key decryption succeded!\n") + QString::fromUtf8(mDecodedPrivateKey));
        qDebug() << mDecodedPrivateKey << "decoded private key '\n' ";
        qDebug() << mRsaKeyPair.privateKey << "init private key '\n' ";

        /* QString orig = QString::fromUtf8(mRsaKeyPair.privateKey).trimmed().replace("\r\n", "\n");
        QString decoded = QString::fromUtf8(mDecodedPrivateKey).trimmed().replace("\r\n", "\n");
        qDebug() << orig << "decoded private key '\n' ";
        qDebug() << mRsaKeyPair.privateKey << "init private key '\n' "; */

        // mTextEditResult->setPlainText(QString::fromUtf8(mDecodedPrivateKey.toBase64()));
    });

    auto pushButtonGenerateSessionKey = new QPushButton(QStringLiteral("Generate Session Key"), this);
    mainLayout->addWidget(pushButtonGenerateSessionKey);
    connect(pushButtonGenerateSessionKey, &QPushButton::clicked, this, []() {
        // test
    });

    auto pushButtonEncodeMessage = new QPushButton(QStringLiteral("Encode Message"), this);
    mainLayout->addWidget(pushButtonEncodeMessage);
    connect(pushButtonEncodeMessage, &QPushButton::clicked, this, []() {
        // test
    });

    auto pushButtonDecodeMessage = new QPushButton(QStringLiteral("Decode Message"), this);
    mainLayout->addWidget(pushButtonDecodeMessage);
    connect(pushButtonDecodeMessage, &QPushButton::clicked, this, []() {
        // test
    });

    auto pushButtonReset = new QPushButton(QStringLiteral("Reset"), this);
    mainLayout->addWidget(pushButtonReset);
    connect(pushButtonReset, &QPushButton::clicked, this, []() {
        // test
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
