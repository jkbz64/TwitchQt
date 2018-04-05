
inline void setClientID(const QString& id)
{
    *clientID = id;
}

inline static QPointer<TopGamesReply> getTopGames()
{
    const QString url = *api + QString("/games/top");

    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", clientID->toUtf8());
    request.setUrl(QUrl(url));

    QNetworkReply* reply = http->get(request);
    return QPointer<TopGamesReply>(new TopGamesReply(reply));
}

inline static QPointer<StreamsReply> getStreamsByGameId(ID gameId, QString after)
{
    QString url = *api + QString("/streams") + QString("?game_id=") + QString::number(gameId);
    if (!after.isEmpty())
        url += QString("&after=") + after;

    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", clientID->toUtf8());
    request.setUrl(QUrl(url));

    QNetworkReply* reply = http->get(request);
    return QPointer<StreamsReply>(new StreamsReply(reply));
}

inline static QPointer<StreamsReply> getStreamsByLanguage(QString language, QString after)
{
    QString url = *api + QString("/streams") + QString("?language=") + language;
    if (!after.isEmpty())
        url += QString("&after=") + after;

    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", clientID->toUtf8());
    request.setUrl(QUrl(url));

    QNetworkReply* reply = http->get(request);
    return QPointer<StreamsReply>(new StreamsReply(reply));
}

inline static QPointer<StreamReply> getStreamById(ID userId)
{
    const QString url = *api + QString("/streams") + QString("?user_id=") + QString::number(userId);

    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", clientID->toUtf8());
    request.setUrl(QUrl(url));

    QNetworkReply* reply = http->get(request);
    return QPointer<StreamReply>(new StreamReply(reply));
}

inline static QPointer<StreamReply> getStreamByName(const QString& userName)
{
    const QString url = *api + QString("/streams") + QString("?user_login=") + userName;

    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", clientID->toUtf8());
    request.setUrl(QUrl(url));

    QNetworkReply* reply = http->get(request);
    return QPointer<StreamReply>(new StreamReply(reply));
}

inline static QPointer<UserReply> getUserById(ID userId)
{
    const QString url = *api + QString("/users") + QString("?id=") + QString::number(userId);

    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", clientID->toUtf8());
    request.setUrl(QUrl(url));

    QNetworkReply* reply = http->get(request);
    return QPointer<UserReply>(new UserReply(reply));
}

inline static QPointer<UserReply> getUserByName(QString name)
{
    const QString url = *api + QString("/users") + QString("?login=") + name;

    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", clientID->toUtf8());
    request.setUrl(QUrl(url));

    QNetworkReply* reply = http->get(request);
    return QPointer<UserReply>(new UserReply(reply));
}
