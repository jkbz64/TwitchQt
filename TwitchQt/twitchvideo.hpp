#ifndef TWITCHVIDEO_HPP
#define TWITCHVIDEO_HPP

#include <QDateTime>
#include <QString>
#include <QVector>

namespace Twitch {
/// Models

struct MutedSegment {
    int duration;
    int offset;
};

struct Video {
    enum class VideoType {
        Upload,
        Archive,
        Highlight
    };
    QString m_id;
    QString m_streamId;
    QString m_userId;
    QString m_userLogin;
    QString m_userName;
    QString m_title;
    QString m_description;
    QDateTime m_createdAt;
    QDateTime m_publishedAt;
    QString m_url;
    QString m_thumbnailUrl;
    QString m_viewable;
    int m_viewCount;
    QString m_language;
    VideoType m_type;
    QString m_duration;
    QList<MutedSegment> m_mutedSegments;
};

using Videos = QVector<Twitch::Video>;
}

Q_DECLARE_METATYPE(Twitch::MutedSegment);
Q_DECLARE_METATYPE(Twitch::Video);
Q_DECLARE_METATYPE(Twitch::Videos);

#endif // TWITCHVIDEO_HPP
