
inline void setClientID(const QString &id)
{
    *clientID = id;
}

inline static QPointer<TopGamesReply> getTopGames()
{
    QString url = *api + QString("/games/top");
    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", clientID->toUtf8());
    request.setUrl(QUrl(url));

    QNetworkReply* reply = http->get(request);
    return QPointer<TopGamesReply>(new TopGamesReply(reply));
}


inline static QPointer<StreamsReply> getStreamsByGameId(qulonglong gameId)
{
    QString url = *api + QString("/streams");
    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", clientID->toUtf8());
    request.setRawHeader("game_id", QString::number(gameId).toUtf8());
    request.setUrl(QUrl(url));

    QNetworkReply* reply = http->get(request);
    return QPointer<StreamsReply>(new StreamsReply(reply));
}
