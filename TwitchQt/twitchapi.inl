inline Api::Api(QObject* parent)
    : QObject(parent)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID()
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

inline Api::Api(QNetworkAccessManager* http)
    : QObject(nullptr),
      m_http(http),
      m_clientID()
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

inline Api::Api(QNetworkAccessManager* http, QObject* parent)
    : QObject(parent),
      m_http(http),
      m_clientID()
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

inline Api::Api(const QString& clientID)
    : QObject(nullptr)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID(clientID)
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

inline Api::Api(const QString& clientID, QObject* parent)
    : QObject(parent)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID(clientID)
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

inline Api::Api(const QString& clientID, QNetworkAccessManager* http)
    : QObject(nullptr)
    , m_http(http)
    , m_clientID(clientID)
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

inline Api::Api(const QString& clientID, QNetworkAccessManager* http, QObject* parent)
    : QObject(parent),
      m_http(http),
      m_clientID(clientID)
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

inline Api::~Api() = default;

inline QString Api::api() const
{
    return QString("https://api.twitch.tv/helix");
}

inline QString Api::emotesApi() const
{
    return QString("https://twitchemotes.com/api_cache/v3");
}

inline QString Api::ffzApi() const
{
    return QString("https://api.frankerfacez.com/v1");
}

inline QString Api::bttvApi() const
{
    return QString("https://api.betterttv.net/2");
}

inline const QString& Api::clientID() const
{
    return m_clientID;
}

inline void Api::setClientID(const QString& id)
{
    m_clientID = id;
}

inline int Api::rateLimit() const
{
    return m_rateLimit;
}

inline int Api::remainingRequests() const
{
    return m_rateRemaining;
}

inline const QDateTime& Api::resetDate() const
{
    return m_rateResetDate;
}

#include <type_traits>

template <class T>
inline T* Api::createReply(const QNetworkRequest& request, bool shouldUpdate)
{
    static_assert(std::is_base_of<Reply, T>::value, " must derive from Twitch::Reply");

    QNetworkReply* requestReply = m_http->get(request);
    T* reply = new T(requestReply);
    reply->setParent(this);

    // Check if it should update rate rate limiting
    if (shouldUpdate) {
        connect(requestReply, &QNetworkReply::metaDataChanged, [this, requestReply]() {
            updateLimits(requestReply);
        });
    }

    return reply;
}

inline void Api::updateLimits(QNetworkReply* reply)
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
inline GamesReply* Api::getTopGames(int first)
{
    const QString url = api() + QString("/games/top") + QString("?first=") + QString::number(first);
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

inline GameReply* Api::getGameById(const QString& id)
{
    const QString url = api() + QString("/games") + QString("?id=") + id;
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

inline GamesReply* Api::getGameByIds(const QStringList& ids)
{
    const QString url = api() + QString("/games") + QString("?id=") + ids.join(repeatDelimeter("id"));
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

inline GameReply* Api::getGameByName(const QString& name)
{
    const QString url = api() + QString("/games") + QString("?name=") + name;
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

inline BoxArtReply* Api::getBoxArtByUrl(const QString& url, int width, int height)
{
    QString targetUrl = url;
    targetUrl = targetUrl.replace("{width}x{height}", QString::number(width) + "x" + QString::number(height));
    auto request = buildRequest(QUrl(targetUrl));
    return createReply<BoxArtReply>(request);
}

inline GamesReply* Api::getGameByNames(const QStringList& names)
{
    const QString url = api() + QString("/games") + QString("?name=") + names.join(repeatDelimeter("name"));
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

// Streams

inline StreamReply* Api::getStreamByUserId(const QString& userId)
{
    const QString url = api() + QString("/streams") + QString("?user_id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

inline StreamReply* Api::getStreamByName(const QString& userName)
{
    const QString url = api() + QString("/streams") + QString("?user_login=") + userName;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

inline StreamsReply* Api::getStreamsByNames(const QStringList& names, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&user_login=") + names.join("&user_login=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Api::getStreamsByUserIds(const QStringList& ids, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&user_id=") + ids.join("&user_id=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Api::getStreamsByGameId(const QString& gameId, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&game_id=") + gameId;
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Api::getStreamsByGameIds(const QStringList& ids, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&game_id=") + ids.join("&game_id=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Api::getStreamsByLanguage(const QString& language, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("?language=") + language;
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Api::getStreamsByLanguages(const QStringList& languages, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("?language=") + languages.join(repeatDelimeter("language"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

// User
inline UserReply* Api::getUserById(const QString& userId)
{
    const QString url = api() + QString("/users") + QString("?id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

inline UserReply* Api::getUserByName(const QString& name)
{
    const QString url = api() + QString("/users") + QString("?login=") + name;

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

inline UsersReply* Api::getUserByIds(const QStringList& ids, const QString& after)
{
    QString url = api() + QString("/users") + QString("?id=") + ids.join(repeatDelimeter("id"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<UsersReply>(request);
}

inline UsersReply* Api::getUserByNames(const QStringList& names, const QString& after)
{
    QString url = api() + QString("/users") + QString("?login=") + names.join(repeatDelimeter("login"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<UsersReply>(request);
}

// Emotes
inline TwitchEmotes::GlobalEmotesReply* Api::getTwitchEmotesGlobalEmotes()
{
    const QString url = emotesApi() + QString("/global.json");
    auto request = buildRequest(QUrl(url), false);
    return createReply<TwitchEmotes::GlobalEmotesReply>(request, false);
}

inline BTTV::GlobalEmotesReply* Api::getBTTVGlobalEmotes()
{
    const QString url = bttvApi() + QString("/emotes");
    auto request = buildRequest(QUrl(url), false);
    return createReply<BTTV::GlobalEmotesReply>(request, false);
}

inline FFZ::GlobalEmotesReply* Api::getFFZGlobalEmotes()
{
    const QString url = ffzApi() + QString("/set/global");
    auto request = buildRequest(QUrl(url), false);
    return createReply<FFZ::GlobalEmotesReply>(request, false);
}

inline ImageReply* Api::getImage(const QString& url)
{
    auto request = buildRequest(QUrl(url), false);
    return createReply<ImageReply>(request, false);
}

inline ImageReply* Api::getEmoteImage(const QString& id, EmoteSize size)
{
    const QString url = TwitchEmotes::Emote::urlTemplate().replace("{{id}}", id).replace("{{size}}", QString::number(static_cast<int>(size)));
    auto request = buildRequest(QUrl(url), false);
    return createReply<ImageReply>(request, false);
}

inline ImageReply* Api::getBTTVEmoteImage(const QString& id, EmoteSize size)
{
    const QString url = BTTV::Emote::urlTemplate().replace("{{id}}", id).replace("{{size}}", QString::number(static_cast<int>(size)));
    auto request = buildRequest(QUrl(url), false);
    return createReply<ImageReply>(request, false);
}

inline ImageReply* Api::getFFZEmoteImage(const QString& id, EmoteSize size)
{
    const QString url = FFZ::Emote::urlTemplate().replace("{{id}}", id).replace("{{size}}", QString::number(static_cast<int>(size)));
    auto request = buildRequest(QUrl(url), false);
    return createReply<ImageReply>(request, false);
}

inline TwitchEmotes::SubscriberEmotesReply* Api::getTwitchEmotesSubscriberEmotes()
{
    const QString url = emotesApi() + QString("/subscriber.json");
    auto request = buildRequest(QUrl(url), false);
    return createReply<TwitchEmotes::SubscriberEmotesReply>(request, false);
}

inline BTTV::SubscriberEmotesReply* Api::getBTTVSubscriberEmotesByChannel(const QString& channel)
{
    const QString url = bttvApi() + "/channels/" + channel;
    auto request = buildRequest(QUrl(url), false);
    return createReply<BTTV::SubscriberEmotesReply>(request, false);
}

inline FFZ::SubscriberEmotesReply* Api::getFFZSubscriberEmotesByChannel(const QString& channel)
{
    const QString url = ffzApi() + "/room/" + channel;
    auto request = buildRequest(QUrl(url), false);
    return createReply<FFZ::SubscriberEmotesReply>(request, false);
}

inline JSONReply* Twitch::Api::getTwitchEmotesEmoteSets()
{
    const QString url = emotesApi() + QString("/sets.json");
    auto request = buildRequest(QUrl(url), false);
    return createReply<JSONReply>(request, false);
}

inline QNetworkRequest Api::buildRequest(QUrl url, bool includeID)
{
    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    if (includeID)
        request.setRawHeader("Client-ID", m_clientID.toUtf8());
    request.setUrl(url);

    return request;
}

inline QString Api::repeatDelimeter(const QString& parameter, const QChar& delimeter) const
{
    return QString(QString(delimeter) + "{parameter}=").replace("{parameter}", parameter);
}

inline EmotesReply* Api::getGlobalEmotes()
{
    // Fallback to twitch emotes
    return getTwitchEmotesGlobalEmotes();
}

inline EmotesReply* Api::getChannelEmotes(const QString& name)
{
    // Fallback to v5
    throw std::runtime_error("Helix does not have emotes endpoints yet, use the TwitchEmotes backend");
}

inline EmotesReply* Api::getEmotesByEmoteSet(const QString& name)
{
    // Fallback to v5
    throw std::runtime_error("Helix does not have emotes endpoints yet, use the TwitchEmotes backend");
}
