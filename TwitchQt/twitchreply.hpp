#ifndef TWITCHREPLY_H
#define TWITCHREPLY_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>
#include <QVariant>

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
    QVariant& data();
    const ReplyState& currentState() const;
    operator bool() const;
signals:
    void finished();

protected:
    QVariant m_data;
    virtual void parseData(const QJsonDocument&) = 0;

private:
    ReplyState m_currentState;
    QNetworkReply* m_reply;
};

#include "twitchreply.inl"
}

#endif // TWITCHREPLY_H
