
inline void StreamReply::parseData(const JSON& json)
{
    if (json.find("data") != json.end()) {
        auto&& data = json["data"];
        if (!data.empty()) {
            auto&& stream = data.front();
            QString id = stream["id"];
            QString userId = stream["user_id"];
            QString gameId = stream["game_id"];

            QVector<QString> communityIds;
            for (QString communityId : stream["community_ids"].array())
                communityIds.push_back(communityId);

            QString typeStr = stream["type"];
            Stream::StreamType type = Stream::StreamType::No;
            if (typeStr == "live")
                type = Stream::StreamType::Live;
            else if (typeStr == "vodcast")
                type = Stream::StreamType::Vodcast;

            QString title = stream["title"];
            qulonglong viewerCount = stream["viewer_count"];

            QString startedAt = stream["started_at"];
            QString language = stream["language"];
            QString thumbnailUrl = stream["thumbnail_url"];

            m_data.setValue(Stream{
                id.toULongLong(),
                userId.toULongLong(),
                gameId.toULongLong(),
                communityIds,
                type,
                title,
                viewerCount,
                QDateTime::fromString(startedAt, "yyyy-MM-ddTHH:mm:ssZ"),
                language,
                thumbnailUrl });
        } else {
            // ???
        }
    }
}

inline void StreamsReply::parseData(const JSON& json)
{
    Streams streams;
    if (json.find("data") != json.end()) {
        auto&& data = json["data"];
        for (const auto& stream : data) {
            QString id = stream["id"];
            QString userId = stream["user_id"];
            QString gameId = stream["game_id"];

            QVector<QString> communityIds;
            for (QString communityId : stream["community_ids"].array())
                communityIds.push_back(communityId);

            QString typeStr = stream["type"];
            Stream::StreamType type = Stream::StreamType::No;
            if (typeStr == "live")
                type = Stream::StreamType::Live;
            else if (typeStr == "vodcast")
                type = Stream::StreamType::Vodcast;

            QString title = stream["title"];
            qulonglong viewerCount = stream["viewer_count"];

            QString startedAt = stream["started_at"];
            QString language = stream["language"];
            QString thumbnailUrl = stream["thumbnail_url"];

            streams.push_back({ id.toULongLong(),
                userId.toULongLong(),
                gameId.toULongLong(),
                communityIds,
                type,
                title,
                viewerCount,
                QDateTime::fromString(startedAt, "yyyy-MM-ddTHH:mm:ssZ"),
                language,
                thumbnailUrl });

            m_combinedViewerCount += streams.back().m_viewerCount;
        }
    }
    m_data.setValue(streams);
}

inline const QString& StreamsReply::cursor() const
{
    return m_cursor;
}

inline qulonglong StreamsReply::combinedViewerCount() const
{
    return m_combinedViewerCount;
}
