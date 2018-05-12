#ifndef TWITCHREPLY_HPP
#define TWITCHREPLY_HPP

#include "json.hpp"
#include <QString>
#include <string>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>
#include <QVariant>
#include <QImage>

namespace Twitch {
enum class ReplyState {
    Pending,
    Success,
    Error
};

class Reply : public QObject {
    Q_OBJECT
public:
    Reply(QNetworkReply*);
    virtual ~Reply();

    const ReplyState& currentState() const;
    operator bool() const;

    QVariant& data();

    const QString& cursor() const;
signals:
    void downloadProgress(qint64, qint64);
    void finished();

protected:
    QNetworkReply* m_reply;
    ReplyState m_currentState;
    QVariant m_data;
    QString m_cursor;
};

class RawReply : public Reply {
    Q_OBJECT
public:
    RawReply(QNetworkReply*);
    virtual ~RawReply();

protected:
    virtual void parseData(const QByteArray&) = 0;
};

class JSONReply : public Reply {
    Q_OBJECT
public:
    JSONReply(QNetworkReply*);
    virtual ~JSONReply();

    const JSON& json() const;

protected:
    virtual void parseData(const JSON&) = 0;
    JSON m_json;
};

class ImageReply : public RawReply {
    Q_OBJECT
public:
    using RawReply::RawReply;

protected:
    virtual void parseData(const QByteArray&) override;
};

#include "twitchreply.inl"
}

#endif // TWITCHREPLY_HPP
