
inline void GameReply::parseData(const JSON& json)
{
    if (json.find("data") != json.end()) {
        auto&& data = json["data"].array();
        if (!data.empty()) {
            auto&& game = data.front();
            QString id = game["id"];
            QString name = game["name"];
            QString boxArtUrl = game["box_art_url"];
            m_data.setValue(Game{
                id.toULongLong(),
                name,
                boxArtUrl
            });
        } else {
            // ??
        }
    }
}

inline void GamesReply::parseData(const JSON& json)
{
    Games games;
    if (json.find("data") != json.end()) {
        auto&& data = json["data"];
        for (const auto& game : data) {
            QString id = game["id"];
            QString name = game["name"];
            QString boxArtUrl = game["box_art_url"];
            games.push_back({ id.toULongLong(),
                name,
                boxArtUrl
            });
        }
    }
    m_data.setValue(games);
}

inline void BoxArtReply::parseData(const QByteArray& data)
{
    m_data.setValue(QImage::fromData(data));
}
