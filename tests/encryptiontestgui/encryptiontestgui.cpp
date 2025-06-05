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
            // test
        }

        delete dialog;
    });

    auto pushButtonGenerateRSAKey = new QPushButton(QStringLiteral("Generate RSA Pair"), this);
    mainLayout->addWidget(pushButtonGenerateRSAKey);
    connect(pushButtonGenerateRSAKey, &QPushButton::clicked, this, [this]() {
        const auto rsaKeyPair = EncryptionUtils::generateRSAKey();
        qDebug() << "Public Key: " << rsaKeyPair.publicKey << "Private Key: " << rsaKeyPair.privateKey;
        mTextEditResult->setPlainText(QString::fromUtf8(rsaKeyPair.publicKey) + QString::fromUtf8(rsaKeyPair.privateKey));
    });

    auto pushButtonEncodePrivateKey = new QPushButton(QStringLiteral("Encode Private Key"), this);
    mainLayout->addWidget(pushButtonEncodePrivateKey);
    connect(pushButtonEncodePrivateKey, &QPushButton::clicked, this, [this]() {
        mPrivateKey = EncryptionUtils::generateRSAKey().privateKey;
        mEncodedPrivateKey = EncryptionUtils::encodePrivateKey(mPrivateKey, QStringLiteral("root"), QStringLiteral("admin"));
        qDebug() << mEncodedPrivateKey.toBase64();
        mTextEditResult->setPlainText(QString::fromUtf8(mEncodedPrivateKey.toBase64()));
    });

    auto pushButtonDecodePrivateKey = new QPushButton(QStringLiteral("Decode Private Key"), this);
    mainLayout->addWidget(pushButtonDecodePrivateKey);
    connect(pushButtonDecodePrivateKey, &QPushButton::clicked, this, [this]() {
        qDebug() << mEncodedPrivateKey.toBase64() << "encoded private key";
        qDebug() << mEncodedPrivateKey.size() << "encoded private key size";
        mDecodedPrivateKey = EncryptionUtils::decodePrivateKey(mEncodedPrivateKey);
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
