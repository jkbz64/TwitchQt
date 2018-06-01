    inline Detail::TwitchApi::TwitchApi(QObject* parent)
    : QObject(parent)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID()
{
}

inline Detail::TwitchApi::TwitchApi(const QString& clientID)
    : QObject(nullptr)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID(clientID)
{
}

inline Detail::TwitchApi::TwitchApi(const QString& clientID, QObject* parent)
    : QObject(parent)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID(clientID)
{
}

inline Detail::TwitchApi::~TwitchApi() = default;

inline QString Detail::TwitchApi::emotesApi() const
{
    return QString("https://twitchemotes.com/api_cache/v3");
}

inline QString Detail::TwitchApi::ffzApi() const
{
    return QString("https://api.frankerfacez.com/v1");
}

inline QString Detail::TwitchApi::bttvApi() const
{
    return QString("https://api.betterttv.net/2");
}

inline const QString& Detail::TwitchApi::clientID() const
{
    return m_clientID;
}

inline void Detail::TwitchApi::setClientID(const QString& id)
{
    m_clientID = id;
}

inline int Detail::TwitchApi::rateLimit() const
{
    return m_rateLimit;
}

inline int Detail::TwitchApi::remainingRequests() const
{
    return m_rateRemaining;
}

inline const QDateTime& Detail::TwitchApi::resetDate() const
{
    return m_rateResetDate;
}

#include <type_traits>

template <class T>
inline T* Detail::TwitchApi::createReply(const QNetworkRequest& request, bool shouldUpdate)
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

inline void Detail::TwitchApi::updateLimits(QNetworkReply* reply)
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

// Helix

// Games

inline GamesReply* Helix::getTopGames(int first)
{
    const QString url = api() + QString("/games/top") + QString("?first=") + QString::number(first);
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

inline GameReply* Helix::getGameById(const QString& id)
{
    const QString url = api() + QString("/games") + QString("?id=") + id;
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

inline GamesReply* Helix::getGameByIds(const QStringList& ids)
{
    const QString url = api() + QString("/games") + QString("?id=") + ids.join(repeatDelimeter("id"));
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

inline GameReply* Helix::getGameByName(const QString& name)
{
    const QString url = api() + QString("/games") + QString("?name=") + name;
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

inline BoxArtReply* Helix::getBoxArtByUrl(const QString& url, int width, int height)
{
    QString targetUrl = url;
    targetUrl = targetUrl.replace("{width}x{height}", QString::number(width) + "x" + QString::number(height));
    auto request = buildRequest(QUrl(targetUrl));
    return createReply<BoxArtReply>(request);
}

inline GamesReply* Helix::getGameByNames(const QStringList& names)
{
    const QString url = api() + QString("/games") + QString("?name=") + names.join(repeatDelimeter("name"));
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

// Streams

inline StreamReply* Helix::getStreamByUserId(const QString& userId)
{
    const QString url = api() + QString("/streams") + QString("?user_id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

inline StreamReply* Helix::getStreamByName(const QString& userName)
{
    const QString url = api() + QString("/streams") + QString("?user_login=") + userName;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

inline StreamsReply* Helix::getStreamsByNames(const QStringList& names, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&user_login=") + names.join("&user_login=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Helix::getStreamsByUserIds(const QStringList& ids, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&user_id=") + ids.join("&user_id=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Helix::getStreamsByGameId(const QString& gameId, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&game_id=") + gameId;
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Helix::getStreamsByGameIds(const QStringList& ids, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&game_id=") + ids.join("&game_id=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Helix::getStreamsByLanguage(const QString& language, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("?language=") + language;
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

inline StreamsReply* Helix::getStreamsByLanguages(const QStringList& languages, int first, const QString& after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("?language=") + languages.join(repeatDelimeter("language"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

// User
inline UserReply* Helix::getUserById(const QString& userId)
{
    const QString url = api() + QString("/users") + QString("?id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

inline UserReply* Helix::getUserByName(const QString& name)
{
    const QString url = api() + QString("/users") + QString("?login=") + name;

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

inline UsersReply* Helix::getUserByIds(const QStringList& ids, const QString& after)
{
    QString url = api() + QString("/users") + QString("?id=") + ids.join(repeatDelimeter("id"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<UsersReply>(request);
}

inline UsersReply* Helix::getUserByNames(const QStringList& names, const QString& after)
{
    QString url = api() + QString("/users") + QString("?login=") + names.join(repeatDelimeter("login"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<UsersReply>(request);
}

// Emotes

inline TwitchEmotes::GlobalEmotesReply* Detail::TwitchApi::getTwitchEmotesGlobalEmotes()
{
    const QString url = emotesApi() + QString("/global.json");
    auto request = buildRequest(QUrl(url), false);
    return createReply<TwitchEmotes::GlobalEmotesReply>(request, false);
}

inline BTTV::GlobalEmotesReply* Detail::TwitchApi::getBTTVGlobalEmotes()
{
    const QString url = bttvApi() + QString("/emotes");
    auto request = buildRequest(QUrl(url), false);
    return createReply<BTTV::GlobalEmotesReply>(request, false);
}

inline FFZ::GlobalEmotesReply* Detail::TwitchApi::getFFZGlobalEmotes()
{
    const QString url = ffzApi() + QString("/set/global");
    auto request = buildRequest(QUrl(url), false);
    return createReply<FFZ::GlobalEmotesReply>(request, false);
}

inline ImageReply* Detail::TwitchApi::getImage(const QString& url)
{
    auto request = buildRequest(QUrl(url), false);
    return createReply<ImageReply>(request, false);
}

inline ImageReply* Detail::TwitchApi::getEmoteImage(const QString& id, EmoteSize size)
{
    const QString url = TwitchEmotes::Emote::urlTemplate().replace("{{id}}", id).replace("{{size}}", QString::number(static_cast<int>(size)));
    auto request = buildRequest(QUrl(url), false);
    return createReply<ImageReply>(request, false);
}

inline ImageReply* Detail::TwitchApi::getBTTVEmoteImage(const QString& id, EmoteSize size)
{
    const QString url = BTTV::Emote::urlTemplate().replace("{{id}}", id).replace("{{size}}", QString::number(static_cast<int>(size)));
    auto request = buildRequest(QUrl(url), false);
    return createReply<ImageReply>(request, false);
}

inline ImageReply* Detail::TwitchApi::getFFZEmoteImage(const QString& id, EmoteSize size)
{
    const QString url = FFZ::Emote::urlTemplate().replace("{{id}}", id).replace("{{size}}", QString::number(static_cast<int>(size)));
    auto request = buildRequest(QUrl(url), false);
    return createReply<ImageReply>(request, false);
}

inline TwitchEmotes::SubscriberEmotesReply* Detail::TwitchApi::getTwitchEmotesSubscriberEmotes()
{
    const QString url = emotesApi() + QString("/subscriber.json");
    auto request = buildRequest(QUrl(url), false);
    return createReply<TwitchEmotes::SubscriberEmotesReply>(request, false);
}

inline BTTV::SubscriberEmotesReply* Detail::TwitchApi::getBTTVSubscriberEmotesByChannel(const QString& channel)
{
    const QString url = bttvApi() + "/channels/" + channel;
    auto request = buildRequest(QUrl(url), false);
    return createReply<BTTV::SubscriberEmotesReply>(request, false);
}

inline FFZ::SubscriberEmotesReply* Detail::TwitchApi::getFFZSubscriberEmotesByChannel(const QString& channel)
{
    const QString url = ffzApi() + "/room/" + channel;
    auto request = buildRequest(QUrl(url), false);
    return createReply<FFZ::SubscriberEmotesReply>(request, false);
}

inline JSONReply* Twitch::Detail::TwitchApi::getTwitchEmotesEmoteSets()
{
    const QString url = emotesApi() + QString("/sets.json");
    auto request = buildRequest(QUrl(url), false);
    return createReply<JSONReply>(request, false);
}

// Helix

inline Helix::Helix(QObject* parent)
    : TwitchApi(parent)
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

inline Helix::Helix(const QString& clientID)
    : TwitchApi(clientID)
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

inline Helix::Helix(const QString& clientID, QObject* parent)
    : TwitchApi(clientID, parent)
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
    if (includeID)
        request.setRawHeader("Client-ID", m_clientID.toUtf8());
    request.setUrl(url);

    return request;
}

inline QString Helix::repeatDelimeter(const QString& parameter) const
{
    return QString("&{parameter}=").replace("{parameter}", parameter);
}

inline EmotesReply* Twitch::Helix::getGlobalEmotes()
{
    throw std::runtime_error("Helix does not have emotes endpoints yet, use the TwitchEmotes backend");
}

inline EmotesReply* Twitch::Helix::getChannelEmotes(const QString&)
{
    throw std::runtime_error("Helix does not have emotes endpoints yet, use the TwitchEmotes backend");
}

inline EmotesReply* Twitch::Helix::getEmotesByEmoteSet(const QString&)
{
    throw std::runtime_error("Helix does not have emotes endpoints yet, use the TwitchEmotes backend");
}
