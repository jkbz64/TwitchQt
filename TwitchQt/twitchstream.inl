
inline void StreamsReply::parseData(const QJsonDocument& json)
{
    Streams streams;
    auto&& root = json.object();
    if(root.contains("data"))
    {
        auto&& data = root.value("data").toArray();
        for(const auto& streamElement : data)
        {
            auto&& stream = streamElement.toObject();
            QString id = stream.value("id").toString();
            QString userId = stream.value("user_id").toString();
            QString gameId = stream.value("game_id").toString();

            QVector<QString> communityIds;
            for(const auto& communityId : stream.value("community_ids").toArray())
                communityIds.push_back(communityId.toString());

            QString typeStr = stream.value("type").toString();
            Stream::StreamType type = Stream::StreamType::No;
            if(typeStr == "live")
                type = Stream::StreamType::Live;
            else if(typeStr == "vodcast")
                type = Stream::StreamType::Vodcast;

            QString title = stream.value("title").toString();
            QString viewerCount = stream.value("viewer_count").toString();

            QString startedAt = stream.value("started_at").toString();
            QString language = stream.value("language").toString();
            QString thumbnailUrl = stream.value("thumbnail_url").toString();

            streams.push_back({
                id.toULongLong(),
                userId.toULongLong(),
                gameId.toULongLong(),
                communityIds,
                type,
                title,
                viewerCount.toULongLong(),
                QDateTime::fromString(startedAt, "yyyy-MM-ddTHH:mm:ssZ"),
                language,
                thumbnailUrl
            });
        }
    }
    m_data.setValue(streams);
}
