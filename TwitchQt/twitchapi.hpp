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
    using Cursor = QString;
    using ObjectCount = int;

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
    virtual GamesReply* getTopGames(ObjectCount = 30);
    virtual GameReply* getGameById(const QString&);
    virtual GamesReply* getGameByIds(const QStringList&);
    virtual GameReply* getGameByName(const QString&);
    virtual GamesReply* getGameByNames(const QStringList&);
    virtual BoxArtReply* getBoxArtByUrl(const QString&, int, int);

    // Streams
    virtual StreamReply* getStreamByUserId(const QString&);
    virtual StreamReply* getStreamByName(const QString&);

    virtual StreamsReply* getStreamsByNames(const QStringList&, ObjectCount = 30, const Cursor& = Cursor());
    virtual StreamsReply* getStreamsByUserIds(const QStringList&, ObjectCount = 30, const Cursor& = Cursor());
    virtual StreamsReply* getStreamsByGameId(const QString&, ObjectCount = 30, const Cursor& = Cursor());
    virtual StreamsReply* getStreamsByGameIds(const QStringList&, ObjectCount = 30, const Cursor& = Cursor());
    virtual StreamsReply* getStreamsByLanguage(const QString&, ObjectCount = 30, const Cursor& = Cursor());
    virtual StreamsReply* getStreamsByLanguages(const QStringList&, ObjectCount = 30, const Cursor& = Cursor());

    // Users
    virtual UserReply* getUserById(const QString& ID);
    virtual UserReply* getUserByName(const QString&);
    virtual UsersReply* getUserByIds(const QStringList&, const Cursor& = Cursor());
    virtual UsersReply* getUserByNames(const QStringList&, const Cursor& = Cursor());

    // Emotes // These curently fallback to v5 api
    virtual EmotesReply* getGlobalEmotes();
    virtual EmoteSetsReply* getEmotesBySet(const QString&);
    virtual EmoteSetsReply* getEmotesBySets(const QStringList&);
    virtual ImageReply* getEmoteImage(const QString&, EmoteSize = EmoteSize::Original);

    // Optional backends
    // TwitchEmotes
    virtual TwitchEmotes::GlobalEmotesReply* getTwitchEmotesGlobalEmotes();
    virtual TwitchEmotes::SubscriberEmotesReply* getTwitchEmotesSubscriberEmotes();
    virtual JSONReply* getTwitchEmotesEmoteSets();

    // BetterTTV
    virtual BTTV::GlobalEmotesReply* getBTTVGlobalEmotes();
    virtual BTTV::SubscriberEmotesReply* getBTTVSubscriberEmotesByChannel(const QString&);
    virtual ImageReply* getBTTVEmoteImage(const QString&, EmoteSize = EmoteSize::Original);

    // FFZ
    virtual FFZ::GlobalEmotesReply* getFFZGlobalEmotes();
    virtual FFZ::SubscriberEmotesReply* getFFZSubscriberEmotesByChannel(const QString&);
    virtual ImageReply* getFFZEmoteImage(const QString&, EmoteSize = EmoteSize::Original);

    // Utility
    // Image getter by Url
    virtual ImageReply* getImage(const QString&);

    // Rate limiting
    int rateLimit() const;
    int remainingRequests() const;
    const QDateTime& resetDate() const;

protected:
    QNetworkAccessManager* m_http;
    QString m_clientID;

    void resetRateLimit();
    int m_rateLimit;
    int m_rateRemaining;
    QDateTime m_rateResetDate;

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
