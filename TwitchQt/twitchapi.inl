inline Detail::Api::Api(QObject *parent)
  : QObject(parent)
  , m_http(new QNetworkAccessManager(this))
  , m_clientID()
{

}

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

inline Detail::Api::~Api() = default;

inline QString Detail::Api::emotesApi() const
{
    return QString("https://twitchemotes.com/api_cache/v3");
}

inline QString Detail::Api::ffzApi() const
{
    return QString("https://api.frankerfacez.com/v1");
}

inline QString Detail::Api::bttvApi() const
{
    return QString("https://api.betterttv.net/2");
}

inline const QString& Detail::Api::clientID() const
{
    return m_clientID;
}

inline void Detail::Api::setClientID(const QString& id)
{
    m_clientID = id;
}

inline int Detail::Api::rateLimit() const
{
    return m_rateLimit;
}

inline int Detail::Api::remainingRequests() const
{
    return m_rateRemaining;
}

inline const QDateTime& Detail::Api::resetDate() const
{
    return m_rateResetDate;
}

#include <type_traits>

template <class T>
inline T* Detail::Api::createReply(const QNetworkRequest& request, bool shouldUpdate)
{
    static_assert(std::is_base_of<Reply, T>::value, " must derive from Twitch::Reply");

    QNetworkReply* requestReply = m_http->get(request);
    T* reply = new T(requestReply);
    reply->setParent(this);

    if(shouldUpdate)
    {
        // Update rate limiting
        connect(requestReply, &QNetworkReply::metaDataChanged, [this, requestReply]() {
            updateLimits(requestReply);
        });
    }

    return reply;
}

inline void Detail::Api::updateLimits(QNetworkReply* reply)
{
    if (reply->hasRawHeader("RateLimit-Limit"))
        m_rateLimit = reply->rawHeader("RateLimit-Limit").toInt();
    if (reply->hasRawHeader("RateLimit-Remaining"))
        m_rateRemaining = reply->rawHeader("RateLimit-Remaining").toInt();
    if (reply->hasRawHeader("RateLimit-Reset")) {
        auto timestamp = reply->rawHeader("RateLimit-Reset").toUInt();
        m_rateResetDate.setTime_t(timestamp);
    }
}

// Games

inline GamesReply* Detail::Api::getTopGames(int first)
{
    const QString url = api() + QString("/games/top") + QString("?first=") + QString::number(first);
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

inline BoxArtReply* Detail::Api::getBoxArtByUrl(const QString& url, int width, int height)
{
    QString targetUrl = url;
    targetUrl = targetUrl.replace("{width}x{height}", QString::number(width) + "x" + QString::number(height));
    auto request = buildRequest(QUrl(targetUrl));
    return createReply<BoxArtReply>(request);
}

inline GamesReply* Detail::Api::getGameByNames(const QStringList& names)
{
    const QString url = api() + QString("/games") + QString("?name=") + names.join(repeatDelimeter("name"));
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

// Streams

inline StreamReply* Detail::Api::getStreamByUserId(ID userId)
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

inline StreamsReply* Detail::Api::getStreamsByNames(const QStringList& names, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&user_login=") + names.join("&user_login=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Detail::Api::getStreamsByUserIds(const QStringList& ids, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&user_id=") + ids.join("&user_id=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Detail::Api::getStreamsByGameId(ID gameId, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&game_id=") + QString::number(gameId);
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Detail::Api::getStreamsByGameIds(const QStringList& ids, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&game_id=") + ids.join("&game_id=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Detail::Api::getStreamsByLanguage(const QString& language, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("?language=") + language;
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Detail::Api::getStreamsByLanguages(const QStringList& languages, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("?language=") + languages.join(repeatDelimeter("language"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

// User

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

// Emotes

inline EmotesReply* Detail::Api::getGlobalEmotes()
{
    const QString url = emotesApi() + QString("/global.json");
    auto request = buildRequest(QUrl(url), false);
    return createReply<EmotesReply>(request, false);
}

inline BTTV::EmotesReply* Detail::Api::getBTTVGlobalEmotes()
{
    const QString url = bttvApi() + QString("/emotes");
    auto request = buildRequest(QUrl(url), false);
    return createReply<BTTV::EmotesReply>(request, false);
}

inline FFZ::EmotesReply* Detail::Api::getFFZGlobalEmotes()
{
    const QString url = ffzApi() + QString("/set/global");
    auto request = buildRequest(QUrl(url), false);
    return createReply<FFZ::EmotesReply>(request, false);
}

inline EmoteImageReply* Detail::Api::getEmoteImage(ID id)
{
    const QString url = Emote::urlTemplate().replace("{{id}}", QString::number(id)).replace("{{size}}", "1");
    auto request = buildRequest(QUrl(url), false);
    return createReply<EmoteImageReply>(request, false);
}

inline EmoteImageReply* Detail::Api::getBTTVEmoteImage(QString id)
{
    const QString url = BTTV::Emote::urlTemplate().replace("{{id}}", id).replace("{{size}}", "1");
    auto request = buildRequest(QUrl(url), false);
    return createReply<EmoteImageReply>(request, false);
}

inline EmoteImageReply* Detail::Api::getFFZEmoteImage(ID id)
{
    const QString url = FFZ::Emote::urlTemplate().replace("{{id}}", QString::number(id)).replace("{{size}}", "1");
    auto request = buildRequest(QUrl(url), false);
    return createReply<EmoteImageReply>(request, false);
}



// Helix

inline Helix::Helix(const QString& clientID)
    : Api(clientID)
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

inline Helix::Helix(const QString& clientID, QObject* parent)
    : Api(clientID, parent)
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

inline Helix::~Helix() = default;

inline QString Helix::api() const
{
    return QString("https://api.twitch.tv/helix");
}

inline QNetworkRequest Helix::buildRequest(QUrl url, bool includeID)
{
    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    if(includeID)
        request.setRawHeader("Client-ID", m_clientID.toUtf8());
    request.setUrl(url);

    return request;
}

inline QString Helix::repeatDelimeter(const QString& parameter) const
{
    return QString("&{parameter}=").replace("{parameter}", parameter);
}
