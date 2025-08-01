#pragma once

#include <QObject>

class UploadDownloadRsaKeyPairTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void uploadDownloadCompare();
};