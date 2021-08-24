inline Api::Api(QObject* parent)
    : QObject(parent)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID()
{
    resetRateLimit();
    auto diskCache = new QNetworkDiskCache(m_http);
    diskCache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/TCache");
    m_http->setCache(diskCache);
}

inline Api::Api(QNetworkAccessManager* http, QObject* parent)
    : Api(parent)
{
    m_http = http;
}

inline Api::Api(const QString& clientID, QObject* parent)
    : Api(parent)
{
    m_clientID = clientID;
}

inline Api::Api(const QString& clientID, QNetworkAccessManager* http, QObject* parent)
    : Api(parent)
{
    m_http = http;
    m_clientID = clientID;
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

inline const QString& Api::bearerToken() const
{
    return m_bearerToken;
}

inline void Api::setBearerToken(const QString& bearerToken)
{
    m_bearerToken = bearerToken;
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

inline QString Api::repeatDelimeter(const QString& parameter, const QChar& delimeter) const
{
    return QString(QString(delimeter) + "{parameter}=").replace("{parameter}", parameter);
}

inline QNetworkRequest Api::buildRequest(QUrl url, bool includeID, const CacheFlag cacheFlag)
{
    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    if (includeID)
        request.setRawHeader("Client-ID", m_clientID.toUtf8());

    request.setRawHeader("Authorization", QString("Bearer %1").arg(bearerToken()).toUtf8());

    switch (cacheFlag) {
    case CacheFlag::UseNetworkDoNotCache:
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
        request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);
        break;
    case CacheFlag::PreferCache:
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
        request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, true);
        break;
    }

    request.setUrl(url);
    return request;
}

#include <type_traits>

template <class T>
inline T* Api::createReply(const QNetworkRequest& request, bool shouldUpdate)
{
    static_assert(std::is_base_of<Reply, T>::value, " must derive from Twitch::Reply");

    QNetworkReply* requestReply = m_http->get(QNetworkRequest(request));
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

inline void Twitch::Api::resetRateLimit()
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

// Games
inline GamesReply* Api::getTopGames(int first, const QString& after)
{
    QString url = api() + QString("/games/top") + QString("?first=") + QString::number(first);
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(url);
    return createReply<GamesReply>(request);
}

inline GameReply* Api::getGameById(const QString& id)
{
    const QUrl url = api() + QString("/games") + QString("?id=") + id;
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

inline GamesReply* Api::getGameByIds(const QStringList& ids)
{
    const QUrl url = api() + QString("/games") + QString("?id=") + ids.join(repeatDelimeter("id"));
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

inline GameReply* Api::getGameByName(const QString& name)
{
    const QUrl url = api() + QString("/games") + QString("?name=") + name;
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

inline BoxArtReply* Api::getBoxArtByUrl(const QString& url, int width, int height)
{
    QString targetUrl = url;
    targetUrl = targetUrl.replace("{width}x{height}", QString::number(width) + "x" + QString::number(height));
    auto request = buildRequest(QUrl(targetUrl), false, CacheFlag::PreferCache);
    return createReply<BoxArtReply>(request);
}

inline GamesReply* Api::getGameByNames(const QStringList& names)
{
    const QUrl url = api() + QString("/games") + QString("?name=") + names.join(repeatDelimeter("name"));
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

// Streams

inline StreamReply* Api::getStreamByUserId(const QString& userId)
{
    const QUrl url = api() + QString("/streams") + QString("?user_id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

inline StreamReply* Api::getStreamByName(const QString& userName)
{
    const QUrl url = api() + QString("/streams") + QString("?user_login=") + userName;

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

// Videos
inline VideoReply* Api::getVideoById(const QString& id)
{
    const QUrl url = api() + QString("/videos") + QString("?id=") + id;

    auto request = buildRequest(QUrl(url));
    return createReply<VideoReply>(request);
}

inline VideosReply* Api::getVideosByUserId(const QString& id, int first, const QString& after)
{
    QString url = api() + QString("/videos") + QString("?first=") + QString::number(first) + QString("&user_id=") + id;
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<VideosReply>(request);
}

// Users Follows
inline UserFollowsReply* Api::getUserFollowsFromId(const QString& userId)
{
    const QUrl url = api() + QString("/users/follows") + QString("?from_id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<UserFollowsReply>(request);
}

inline UserFollowsReply* Api::getUserFollowsToId(const QString& userId)
{
    const QUrl url = api() + QString("/users/follows") + QString("?to_id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<UserFollowsReply>(request);
}

// User
inline UserReply* Api::getUserById(const QString& userId)
{
    const QUrl url = api() + QString("/users") + QString("?id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

inline UserReply* Api::getUserByName(const QString& name)
{
    const QUrl url = api() + QString("/users") + QString("?login=") + name;

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
    const QUrl url = emotesApi() + QString("/global.json");
    auto request = buildRequest(QUrl(url), false);
    return createReply<TwitchEmotes::GlobalEmotesReply>(request, false);
}

inline BTTV::GlobalEmotesReply* Api::getBTTVGlobalEmotes()
{
    const QUrl url = bttvApi() + QString("/emotes");
    auto request = buildRequest(QUrl(url), false);
    return createReply<BTTV::GlobalEmotesReply>(request, false);
}

inline FFZ::GlobalEmotesReply* Api::getFFZGlobalEmotes()
{
    const QUrl url = ffzApi() + QString("/set/global");
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
    const QUrl url = TwitchEmotes::Emote::urlTemplate().replace("{{id}}", id).replace("{{size}}", QString::number(static_cast<int>(size)));
    auto request = buildRequest(QUrl(url), false);
    return createReply<ImageReply>(request, false);
}

inline ImageReply* Api::getBTTVEmoteImage(const QString& id, EmoteSize size)
{
    const QUrl url = BTTV::Emote::urlTemplate().replace("{{id}}", id).replace("{{size}}", QString::number(static_cast<int>(size)));
    auto request = buildRequest(QUrl(url), false);
    return createReply<ImageReply>(request, false);
}

inline ImageReply* Api::getFFZEmoteImage(const QString& id, EmoteSize size)
{
    const QUrl url = FFZ::Emote::urlTemplate().replace("{{id}}", id).replace("{{size}}", QString::number(static_cast<int>(size)));
    auto request = buildRequest(QUrl(url), false);
    return createReply<ImageReply>(request, false);
}

inline TwitchEmotes::SubscriberEmotesReply* Api::getTwitchEmotesSubscriberEmotes()
{
    const QUrl url = emotesApi() + QString("/subscriber.json");
    auto request = buildRequest(QUrl(url), false);
    return createReply<TwitchEmotes::SubscriberEmotesReply>(request, false);
}

inline BTTV::SubscriberEmotesReply* Api::getBTTVSubscriberEmotesByChannel(const QString& channel)
{
    const QUrl url = bttvApi() + "/channels/" + channel;
    auto request = buildRequest(QUrl(url), false);
    return createReply<BTTV::SubscriberEmotesReply>(request, false);
}

inline FFZ::SubscriberEmotesReply* Api::getFFZSubscriberEmotesByChannel(const QString& channel)
{
    const QUrl url = ffzApi() + "/room/" + channel;
    auto request = buildRequest(QUrl(url), false);
    return createReply<FFZ::SubscriberEmotesReply>(request, false);
}

inline JSONReply* Twitch::Api::getTwitchEmotesEmoteSets()
{
    const QUrl url = emotesApi() + QString("/sets.json");
    auto request = buildRequest(QUrl(url), false);
    return createReply<JSONReply>(request, false);
}

inline EmotesReply* Api::getGlobalEmotes()
{
    // Fallback to twitch emotes
    return getTwitchEmotesGlobalEmotes();
}

inline EmoteSetsReply* Api::getEmotesBySet(const QString& set)
{
    // Fallback to v5
    const QUrl url = QString("https://api.twitch.tv/kraken/chat/emoticon_images?emotesets=" + set);
    auto request = buildRequest(QUrl(url));
    request.setRawHeader("Accept", "application/vnd.twitchtv.v5+json");
    return createReply<EmoteSetsReply>(request, false);
}

inline EmoteSetsReply* Api::getEmotesBySets(const QStringList& sets)
{
    const QUrl url = QString("https://api.twitch.tv/kraken/chat/emoticon_images?emotesets=") + sets.join(",");
    auto request = buildRequest(QUrl(url));
    request.setRawHeader("Accept", "application/vnd.twitchtv.v5+json");
    return createReply<EmoteSetsReply>(request, false);
}

inline Twitch::GlobalBadgesReply* Twitch::Api::getGlobalBadges()
{
    const QUrl url = QString("https://badges.twitch.tv/v1/badges/global/display");
    auto request = buildRequest(url, false);
    return createReply<GlobalBadgesReply>(request, false);
}

inline Twitch::ChannelBadgesReply* Twitch::Api::getChannelBadges(const QString& id)
{
    const QUrl url = QString("https://badges.twitch.tv/v1/badges/channels/") + id + QString("/display");
    auto request = buildRequest(url, false);
    return createReply<ChannelBadgesReply>(request, false);
}
