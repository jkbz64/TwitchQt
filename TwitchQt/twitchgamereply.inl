
inline void GameReply::parseData(const JSON& json)
{
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        if (!data.empty()) {
            const auto& game = data.front();
            m_data.setValue(Game{
                game.value("id", QString("-1")),
                game.value("name", QString("ERROR")),
                game.value("box_art_url", QString("")) });
        } else {
            // ??
        }
    }
}

inline void GamesReply::parseData(const JSON& json)
{
    Games games;
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        for (const auto& game : data) {
            games.push_back({ game.value("id", QString("-1")),
                game.value("name", QString("ERROR")),
                game.value("box_art_url", QString("")) });
        }
    }
    m_data.setValue(games);
}

inline Twitch::Games Twitch::GamesReply::games()
{
    return m_data.value<Twitch::Games>();
}

inline void BoxArtReply::parseData(const QByteArray& data)
{
    m_data.setValue(QImage::fromData(data));
}
