
inline void VideoReply::parseData(const JSON& json)
{
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        if (!data.empty()) {
            const auto& video = data.front();

            QList<MutedSegment> mutedSegmentsList;
            if (video.find("muted_segments") != video.end()) {
                const auto& mutedSegments = video["muted_segments"];
                for (const auto& segment : mutedSegments) {
                    if (segment.find("duration") == segment.end()) {
                        continue;
                    }
                    MutedSegment ms;
                    ms.duration = segment.value("duration", -1);
                    ms.offset = segment.value("offset", -1);
                    mutedSegmentsList.append(ms);
                }
            }

            QString typeStr = video["type"];
            Video::VideoType type;
            if (typeStr == "upload")
                type = Video::VideoType::Upload;
            else if (typeStr == "archive")
                type = Video::VideoType::Archive;
            else if (typeStr == "highlight")
                type = Video::VideoType::Highlight;

            QString createdAt = video["created_at"];
            QString publishedAt = video["published_at"];

            m_data.setValue(Video{video.value("id", QString("-1")),
                                  video.value("stream_id", QString("-1")),
                                  video.value("user_id", QString("-1")),
                                  video.value("user_login", QString("")),
                                  video.value("user_name", QString("")),
                                  video.value("title", QString()),
                                  video.value("description", QString("")),
                                  QDateTime::fromString(createdAt, Qt::ISODate),
                                  QDateTime::fromString(publishedAt, Qt::ISODate),
                                  video.value("url", QString("")),
                                  video.value("thumbnail_url", QString("")),
                                  video.value("viewable", QString("")),
                                  video.value("view_count", -1),
                                  video.value("language", QString("")),
                                  type,
                                  video.value("duration", QString("")),
                                  mutedSegmentsList});
        } else {
            // ???
        }
    }
}

inline void VideosReply::parseData(const JSON& json)
{
    Videos videos;
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        for (const auto& video : data) {

            QList<MutedSegment> mutedSegmentsList;
            if (video.find("muted_segments") != video.end()) {
                const auto& mutedSegments = video["muted_segments"];
                for (const auto& segment : mutedSegments) {
                    if (segment.find("duration") == segment.end()) {
                        continue;
                    }
                    MutedSegment ms;
                    ms.duration = segment.value("duration", -1);
                    ms.offset = segment.value("offset", -1);
                    mutedSegmentsList.append(ms);
                }
            }

            QString typeStr = video["type"];
            Video::VideoType type;
            if (typeStr == "upload")
                type = Video::VideoType::Upload;
            else if (typeStr == "archive")
                type = Video::VideoType::Archive;
            else if (typeStr == "highlight")
                type = Video::VideoType::Highlight;

            QString createdAt = video["created_at"];
            QString publishedAt = video["published_at"];

            videos.push_back({video.value("id", QString("-1")),
                              video.value("stream_id", QString("-1")),
                              video.value("user_id", QString("-1")),
                              video.value("user_login", QString("")),
                              video.value("user_name", QString("")),
                              video.value("title", QString()),
                              video.value("description", QString("")),
                              QDateTime::fromString(createdAt, Qt::ISODate),
                              QDateTime::fromString(publishedAt, Qt::ISODate),
                              video.value("url", QString("")),
                              video.value("thumbnail_url", QString("")),
                              video.value("viewable", QString("")),
                              video.value("view_count", -1),
                              video.value("language", QString("")),
                              type,
                              video.value("duration", QString("")),
                              mutedSegmentsList});
        }
    }
    m_data.setValue(videos);
}

inline int VideosReply::combinedViewerCount() const
{
    return m_combinedViewerCount;
}

inline Twitch::Video Twitch::VideoReply::video()
{
    return m_data.value<Twitch::Video>();
}

inline Twitch::Videos Twitch::VideosReply::videos()
{
    return m_data.value<Twitch::Videos>();
}
