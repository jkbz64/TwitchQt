
inline void GameReply::parseData(const QJsonDocument &json)
{
    auto&& root = json.object();
    if(root.contains("data"))
    {
        auto&& data = root.value("data").toArray();
        if(!data.isEmpty())
        {
            auto&& game = data.first().toObject();
            QString id = game.value("id").toString();
            QString name = game.value("name").toString();
            QString boxArtUrl = game.value("box_art_url").toString();
            m_data.setValue(Game{
                id.toULongLong(),
                name,
                boxArtUrl
            });
        }
        else
        {
            // ??
        }
    }
}

inline void TopGamesReply::parseData(const QJsonDocument& json)
{
    Games topGames;
    auto&& root = json.object();
    if(root.contains("data"))
    {
        auto&& data = root.value("data").toArray();
        for(const auto& gameElement : data)
        {
            auto&& game = gameElement.toObject();
            QString id = game.value("id").toString();
            QString name = game.value("name").toString();
            QString boxArtUrl = game.value("box_art_url").toString();
            topGames.push_back({
                id.toULongLong(),
                name,
                boxArtUrl
            });
        }
    }
    m_data.setValue(topGames);
}



