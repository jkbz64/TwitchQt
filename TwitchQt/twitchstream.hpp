#ifndef TWITCHSTREAM_HPP
#define TWITCHSTREAM_HPP

#include <QDateTime>
#include <QString>
#include <QVector>

namespace Twitch {
/// Models
struct Stream {
    enum class StreamType {
        No,
        Live,
        Vodcast
    };
    QString m_id;
    QString m_userId;
    QString m_gameId;
    QVector<QString> m_communityIds;
    StreamType m_type;
    QString m_title;
    int m_viewerCount;
    QDateTime m_startedAt;
    QString m_language;
    QString m_thumbnailUrl;
};

using Streams = QVector<Twitch::Stream>;
}

Q_DECLARE_METATYPE(Twitch::Stream);
Q_DECLARE_METATYPE(Twitch::Streams);

#endif // TWITCHSTREAM_HPP
