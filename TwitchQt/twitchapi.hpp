#ifndef TWITCHAPI_HPP
#define TWITCHAPI_HPP

#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QStandardPaths>

#include "twitchemotereply.hpp"
#include "twitchgamereply.hpp"
#include "twitchstreamreply.hpp"
#include "twitchuserreply.hpp"

namespace Twitch {
class Api : public QObject {
    Q_OBJECT
public:
    Api(QObject* = nullptr);
    Api(QNetworkAccessManager*, QObject* = nullptr);
    Api(const QString&, QObject* = nullptr);
    Api(const QString&, QNetworkAccessManager*, QObject* = nullptr);
    virtual ~Api();

    // Client ID
    const QString& clientID() const;
    void setClientID(const QString&);

    // Main endpoints
    QString api() const;
    QString emotesApi() const;
    QString ffzApi() const;
    QString bttvApi() const;

    // Games
    GamesReply* getTopGames(int);
    GameReply* getGameById(const QString&);
    GamesReply* getGameByIds(const QStringList&);
    GameReply* getGameByName(const QString&);
    GamesReply* getGameByNames(const QStringList&);
    BoxArtReply* getBoxArtByUrl(const QString&, int, int);

    // Streams
    StreamReply* getStreamByUserId(const QString&);
    StreamReply* getStreamByName(const QString&);

    StreamsReply* getStreamsByNames(const QStringList&, int = 30, const QString& = "");
    StreamsReply* getStreamsByUserIds(const QStringList&, int = 30, const QString& = "");
    StreamsReply* getStreamsByGameId(const QString&, int = 30, const QString& = "");
    StreamsReply* getStreamsByGameIds(const QStringList&, int = 30, const QString& = "");
    StreamsReply* getStreamsByLanguage(const QString&, int = 30, const QString& = "");
    StreamsReply* getStreamsByLanguages(const QStringList&, int = 30, const QString& = "");

    // Users
    UserReply* getUserById(const QString& ID);
    UserReply* getUserByName(const QString&);
    UsersReply* getUserByIds(const QStringList&, const QString& = "");
    UsersReply* getUserByNames(const QStringList&, const QString& = "");

    // Emotes // These curently fallback to v5 api
    EmotesReply* getGlobalEmotes();
    EmoteSetsReply* getEmotesBySet(const QString&);
    EmoteSetsReply* getEmotesBySets(const QStringList&);
    ImageReply* getEmoteImage(const QString&, EmoteSize = EmoteSize::Original);

    // Optional backends
    // TwitchEmotes
    TwitchEmotes::GlobalEmotesReply* getTwitchEmotesGlobalEmotes();
    TwitchEmotes::SubscriberEmotesReply* getTwitchEmotesSubscriberEmotes();
    JSONReply* getTwitchEmotesEmoteSets();

    // BetterTTV
    BTTV::GlobalEmotesReply* getBTTVGlobalEmotes();
    BTTV::SubscriberEmotesReply* getBTTVSubscriberEmotesByChannel(const QString&);
    ImageReply* getBTTVEmoteImage(const QString&, EmoteSize = EmoteSize::Original);

    // FFZ
    FFZ::GlobalEmotesReply* getFFZGlobalEmotes();
    FFZ::SubscriberEmotesReply* getFFZSubscriberEmotesByChannel(const QString&);
    ImageReply* getFFZEmoteImage(const QString&, EmoteSize = EmoteSize::Original);

    // Utility
    // Image getter by Url
    ImageReply* getImage(const QString&);

    // Rate limiting
    int rateLimit() const;
    int remainingRequests() const;
    const QDateTime& resetDate() const;

    // Cache settings
    struct CacheSettings {
        qreal m_topGamesExpireTime = 5.0;
        qreal m_topStreamsExpireTime = 0.0;

    private:
        friend class Api;
        QDateTime m_lastTopGamesFetch = QDateTime::currentDateTime().addDays(-1);
        QMap<QString, QDateTime> m_topStreamsFetches;

        bool shouldFetchTopGames() const
        {
            return m_lastTopGamesFetch.secsTo(QDateTime::currentDateTime()) >= m_topGamesExpireTime;
        }

        bool shouldFetchTopStreams(const QString& game) const
        {
            return m_topStreamsFetches[game].secsTo(QDateTime::currentDateTime()) >= m_topStreamsExpireTime;
        }
    };

    CacheSettings cacheSettings() const;
    void setCacheSettings(const CacheSettings& cacheSettings);

protected:
    QNetworkAccessManager* m_http;
    QString m_clientID;

    void resetRateLimit();
    int m_rateLimit;
    int m_rateRemaining;
    QDateTime m_rateResetDate;

    CacheSettings m_cacheSettings;

    using IncludeID = bool;
    enum class CacheFlag {
        UseNetworkDoNotCache,
        PreferCache
    };
    QNetworkRequest buildRequest(QUrl, IncludeID = true, const CacheFlag = CacheFlag::UseNetworkDoNotCache);
    // Repeated parameters format for requests
    // For example in Helix it's ampersand (&) and in v5 it's comma (,)
    QString repeatDelimeter(const QString& = "", const QChar& = '&') const;

    template <class T>
    T* createReply(const QNetworkRequest&, bool = true);

    void updateLimits(QNetworkReply*);
};
#include "twitchapi.inl"
}

#endif // TWITCHAPI_HPP
