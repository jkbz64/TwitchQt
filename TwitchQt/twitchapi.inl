inline Detail::Api::Api(const QString& clientID)
    : QObject(nullptr)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID(clientID)
{
}

inline Detail::Api::Api(const QString& clientID, QObject* parent)
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

inline GamesReply* Detail::Api::getTopGames()
{
    const QString url = api() + QString("/games/top");
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

inline GameReply* Detail::Api::getGameById(ID id)
{
    const QString url = api() + QString("/games") + QString("?id=") + QString::number(id);
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

inline GamesReply* Detail::Api::getGameByIds(const QStringList& ids)
{
    const QString url = api() + QString("/games") + QString("?id=") + ids.join(repeatDelimeter("id"));
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

inline GameReply* Detail::Api::getGameByName(const QString& name)
{
    const QString url = api() + QString("/games") + QString("?name=") + name;
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

inline GamesReply* Detail::Api::getGameByNames(const QStringList& names)
{
    const QString url = api() + QString("/games") + QString("?name=") + names.join(repeatDelimeter("name"));
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

inline BoxArtReply* Detail::Api::getBoxArtByUrl(const QString& url, int width, int height)
{
    QString targetUrl = url;
    targetUrl = targetUrl.replace("{width}x{height}", QString::number(width) + "x" + QString::number(height));
    auto request = buildRequest(QUrl(targetUrl));
    return createReply<BoxArtReply>(request);
}

inline StreamsReply* Detail::Api::getStreamsByGameId(ID gameId, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?game_id=") + QString::number(gameId);
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Detail::Api::getStreamsByLanguage(const QString& language, const QString& after)
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

inline StreamsReply* Detail::Api::getStreamsByIds(const QStringList& ids, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?user_id=") + ids.join("&user_id=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamReply* Detail::Api::getStreamByName(const QString& userName)
{
    const QString url = api() + QString("/streams") + QString("?user_login=") + userName;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

inline StreamsReply* Detail::Api::getStreamsByNames(const QStringList& names, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?user_login=") + names.join("&user_login=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline UserReply* Detail::Api::getUserById(ID userId)
{
    const QString url = api() + QString("/users") + QString("?id=") + QString::number(userId);

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

inline UserReply* Detail::Api::getUserByName(const QString& name)
{
    const QString url = api() + QString("/users") + QString("?login=") + name;

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

inline UsersReply* Detail::Api::getUserByIds(const QStringList& ids, const QString& after)
{
    QString url = api() + QString("/users") + QString("?id=") + ids.join(repeatDelimeter("id"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<UsersReply>(request);
}

inline UsersReply* Detail::Api::getUserByNames(const QStringList& names, const QString& after)
{
    QString url = api() + QString("/users") + QString("?login=") + names.join(repeatDelimeter("login"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<UsersReply>(request);
}

inline Helix::Helix(const QString& clientID)
    : Api(clientID)
{
}

inline Helix::Helix(const QString& clientID, QObject* parent)
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

inline QString Helix::repeatDelimeter(const QString& parameter) const
{
    return QString("&{parameter}=").replace("{parameter}", parameter);
}
