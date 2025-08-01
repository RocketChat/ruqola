/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authentication/loginjob.h"
#include "envutils.h"
#include "restapimethod.h"
#include <QCoreApplication>
#include <QDir>
#include <QNetworkAccessManager>
#include <QObject>
#include <functional>

class LoginManager : public QObject
{
    Q_OBJECT
public:
    explicit LoginManager(QObject *parent = nullptr);
    void login(const QString &serverUrl, QNetworkAccessManager *networkManager);

Q_SIGNALS:
    void loginSucceeded(const QString &authToken, const QString &userId);
    void loginFailed(const QString &error);

private:
    RocketChatRestApi::LoginJob *loginJob = nullptr;
    RocketChatRestApi::RestApiMethod *restApiMethod = nullptr;
};