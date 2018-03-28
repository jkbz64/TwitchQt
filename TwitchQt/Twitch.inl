
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

