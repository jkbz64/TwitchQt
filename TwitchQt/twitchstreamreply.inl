
inline void StreamReply::parseData(const JSON& json)
{
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        if (!data.empty()) {
            const auto& stream = data.front();

            QString typeStr = stream["type"];
            Stream::StreamType type = Stream::StreamType::No;
            if (typeStr == "live")
                type = Stream::StreamType::Live;
            else if (typeStr == "vodcast")
                type = Stream::StreamType::Vodcast;
            QString startedAt = stream["started_at"];

            m_data.setValue(Stream{
                stream.value("id", QString("-1")),
                stream.value("user_id", QString("-1")),
                stream.value("user_name", QString("")),
                stream.value("game_id", QString("-1")),
                stream.value("community_ids", QVector<QString>()),
                type,
                stream.value("title", QString("")),
                stream.value("viewer_count", -1),
                QDateTime::fromString(startedAt, "yyyy-MM-ddTHH:mm:ssZ"),
                stream.value("language", QString("")),
                stream.value("thumbnail_url", QString("")) });
        } else {
            // ???
        }
    }
}

inline void StreamsReply::parseData(const JSON& json)
{
    Streams streams;
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        for (const auto& stream : data) {
            QString typeStr = stream["type"];

            Stream::StreamType type = Stream::StreamType::No;
            if (typeStr == "live")
                type = Stream::StreamType::Live;
            else if (typeStr == "vodcast")
                type = Stream::StreamType::Vodcast;
            QString startedAt = stream["started_at"];

            streams.push_back({ stream.value("id", QString("-1")),
                stream.value("user_id", QString("-1")),
                stream.value("user_name", QString("")),
                stream.value("game_id", QString("-1")),
                stream.value("community_ids", QVector<QString>()),
                type,
                stream.value("title", QString("")),
                stream.value("viewer_count", -1),
                QDateTime::fromString(startedAt, "yyyy-MM-ddTHH:mm:ssZ"),
                stream.value("language", QString("")),
                stream.value("thumbnail_url", QString("")) });

            m_combinedViewerCount += streams.back().m_viewerCount;
        }
    }
    m_data.setValue(streams);
}

inline int StreamsReply::combinedViewerCount() const
{
    return m_combinedViewerCount;
}

inline Twitch::Stream Twitch::StreamReply::stream()
{
    return m_data.value<Twitch::Stream>();
}

inline Twitch::Streams Twitch::StreamsReply::streams()
{
    return m_data.value<Twitch::Streams>();
}
