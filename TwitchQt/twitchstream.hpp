#ifndef TWITCHSTREAM_H
#define TWITCHSTREAM_H

#include "twitchreply.hpp"

namespace Twitch {
    /// Models
    struct Stream {
        enum class StreamType {
            No,
            Live,
            Vodcast
        };
        qulonglong m_id;
        qulonglong m_userId;
        qulonglong m_gameId;
        QVector<QString> m_communityIds;
        StreamType m_type;
        QString m_title;
        qulonglong m_viewerCount;
        QDateTime m_startedAt;
        QString m_language;
        QString m_thumbnailUrl;
    };

    using Streams = QVector<Twitch::Stream>;

    /// Replies
    class StreamReply : public Reply {
        Q_OBJECT
    public:
        using Reply::Reply;
    protected:
        virtual void parseData(const QJsonDocument&) override;
    };

    class StreamsReply : public Reply {
        Q_OBJECT
    public:
        using Reply::Reply;
        qulonglong combinedViewerCount() const;
        const QString& cursor() const;
    protected:
        qulonglong m_combinedViewerCount{0u};
        QString m_cursor;
        virtual void parseData(const QJsonDocument&) override;
    };

    #include "twitchstream.inl"
}

Q_DECLARE_METATYPE(Twitch::Stream);
Q_DECLARE_METATYPE(Twitch::Streams);

#endif // TWITCHSTREAM_H
