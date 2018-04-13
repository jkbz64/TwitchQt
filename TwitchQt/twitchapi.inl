inline Detail::Api::Api(QString clientID)
    : QObject(nullptr)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID(clientID)
{
}

inline Detail::Api::Api(QString clientID, QObject* parent)
    : QObject(parent)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID(clientID)
{
}

inline Detail::Api::~Api()
{
}

#include <type_traits>

template <class T>
inline T* Detail::Api::createReply(const QNetworkRequest& request)
{
    static_assert(std::is_base_of<Reply, T>::value, " must derive from Twitch::Reply");

    QNetworkReply* requestReply = m_http->get(request);
    auto reply = new T(requestReply);
    reply->setParent(this);

    return reply;
}

inline TopGamesReply* Detail::Api::getTopGames()
{
    const QString url = api() + QString("/games/top");
    auto request = buildRequest(QUrl(url));
    return createReply<TopGamesReply>(request);
}

inline GameReply* Detail::Api::getGameById(ID id)
{
    const QString url = api() + QString("/games") + QString("?id=") + QString::number(id);
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

inline GameReply* Detail::Api::getGameByName(const QString& name)
{
    const QString url = api() + QString("/games") + QString("?name=") + name;
    qDebug() << url;
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

inline BoxArtReply* Detail::Api::getBoxArtByUrl(const QString& url, int width, int height)
{
    QString targetUrl = url;
    targetUrl = targetUrl.replace("{width}x{height}", QString::number(width) + "x" + QString::number(height));
    auto request = buildRequest(QUrl(targetUrl));
    return createReply<BoxArtReply>(request);
}

inline StreamsReply* Detail::Api::getStreamsByGameId(ID gameId, QString after)
{
    QString url = api() + QString("/streams") + QString("?game_id=") + QString::number(gameId);
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Detail::Api::getStreamsByLanguage(QString language, QString after)
{
    QString url = api() + QString("/streams") + QString("?language=") + language;
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamReply* Detail::Api::getStreamById(ID userId)
{
    const QString url = api() + QString("/streams") + QString("?user_id=") + QString::number(userId);

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

inline StreamReply* Detail::Api::getStreamByName(const QString& userName)
{
    const QString url = api() + QString("/streams") + QString("?user_login=") + userName;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

inline UserReply* Detail::Api::getUserById(ID userId)
{
    const QString url = api() + QString("/users") + QString("?id=") + QString::number(userId);

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

inline UserReply* Detail::Api::getUserByName(QString name)
{
    const QString url = api() + QString("/users") + QString("?login=") + name;

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

inline Helix::Helix(QString clientID)
    : Api(clientID)
{
}

inline Helix::Helix(QString clientID, QObject* parent)
    : Api(clientID, parent)
{
}

inline Helix::~Helix() = default;

inline QString Helix::api() const
{
    return QString("https://api.twitch.tv/helix");
}

inline QNetworkRequest Helix::buildRequest(QUrl url)
{
    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    request.setRawHeader("Client-ID", m_clientID.toUtf8());
    request.setUrl(url);

    return request;
}
