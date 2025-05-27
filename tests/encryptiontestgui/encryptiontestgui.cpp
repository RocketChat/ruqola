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
        EncryptionUtils::RSAKeyPair rsaKeyPair = EncryptionUtils::generateRSAKey();
        qDebug() << rsaKeyPair.publicKey;
        qDebug() << rsaKeyPair.privateKey;
    });
    auto pushButtonGenerateSessionKey = new QPushButton(QStringLiteral("Generate Session Key"), this);
    mainLayout->addWidget(pushButtonGenerateSessionKey);
    connect(pushButtonGenerateSessionKey, &QPushButton::clicked, this, []() {
        // test
    });

    auto pushButtonEncode = new QPushButton(QStringLiteral("Encode"), this);
    mainLayout->addWidget(pushButtonEncode);
    connect(pushButtonEncode, &QPushButton::clicked, this, []() {
        // test
    });
    auto pushButtonDecode = new QPushButton(QStringLiteral("Decode"), this);
    mainLayout->addWidget(pushButtonDecode);
    connect(pushButtonDecode, &QPushButton::clicked, this, []() {
        // test
    });

    auto pushButtonReset = new QPushButton(QStringLiteral("Reset"), this);
    mainLayout->addWidget(pushButtonReset);
    connect(pushButtonReset, &QPushButton::clicked, this, []() {
        EncryptionUtils::generateRSAKey();
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
