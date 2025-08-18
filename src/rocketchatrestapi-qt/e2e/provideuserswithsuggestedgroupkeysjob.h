/*
   SPDX-FileCopyrightText: 2025 Andor Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"
#include <QString>
#include <QVector>

namespace RocketChatRestApi
{

/**
 * QString userId;
 *
 *QString encryptedKey;
 */
struct SuggestedGroupKey {
    QString userId;
    QString encryptedKey; // base64 ????
};

class ProvideUsersWithSuggestedGroupKeysJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ProvideUsersWithSuggestedGroupKeysJob(QObject *parent = nullptr);
    ~ProvideUsersWithSuggestedGroupKeysJob() override;

    void setRoomId(const QString &roomId);
    void setKeys(const QVector<SuggestedGroupKey> &keys);

    QString roomId() const;
    QVector<SuggestedGroupKey> keys() const;

    bool start() override;
    QNetworkRequest request() const override;
    QJsonDocument json() const;

    bool canStart() const override;

    bool requireHttpAuthentication() const override;

protected:
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

Q_SIGNALS:
    void provideUsersWithSuggestedGroupKeysDone(const QJsonObject &result);

private:
    QString mRoomId;
    QVector<SuggestedGroupKey> mKeys;
};

}