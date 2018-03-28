
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
    QString url = *api + QString("/streams") + QString("?game_id=") + QString::number(gameId);
    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", clientID->toUtf8());
    request.setUrl(QUrl(url));

    QNetworkReply* reply = http->get(request);
    return QPointer<StreamsReply>(new StreamsReply(reply));
}


inline static QPointer<Twitch::StreamsReply> getStreamsByLanguage(QString language)
{
    QString url = *api + QString("/streams") + QString("?language=") + language;
    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", clientID->toUtf8());
    request.setUrl(QUrl(url));

    QNetworkReply* reply = http->get(request);
    return QPointer<StreamsReply>(new StreamsReply(reply));
}
