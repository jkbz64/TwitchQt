#ifndef TWITCHSTREAM_HPP
#define TWITCHSTREAM_HPP

#include <QDateTime>
#include <QString>

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
}

#endif // TWITCHSTREAM_HPP
