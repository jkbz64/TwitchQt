#ifndef TWITCHREPLY_H
#define TWITCHREPLY_H

#include <QObject>
#include <QPointer>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

namespace Twitch {
    class Reply : public QObject {
        Q_OBJECT
    public:
        enum class ReplyState {
            Pending,
            Success,
            Error
        };
        Reply(QNetworkReply*);
        QVariant& data();
        const ReplyState& currentState() const;
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
