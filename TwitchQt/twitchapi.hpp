#ifndef TWITCHAPI_HPP
#define TWITCHAPI_HPP

#include <QNetworkAccessManager>

#include "twitchemotereply.hpp"
#include "twitchgamereply.hpp"
#include "twitchstreamreply.hpp"
#include "twitchuserreply.hpp"

namespace Twitch {
class Api : public QObject {
    Q_OBJECT
public:
    Api(QObject* = nullptr);
    Api(QNetworkAccessManager*);
    Api(QNetworkAccessManager*, QObject*);
    Api(const QString&);
    Api(const QString&, QNetworkAccessManager*);
    Api(const QString&, QObject*);
    Api(const QString&, QNetworkAccessManager*, QObject*);
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
    EmotesReply* getChannelEmotes(const QString&);
    EmotesReply* getEmotesByEmoteSet(const QString&);
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

protected:
    QNetworkAccessManager* m_http;
    QString m_clientID;
    int m_rateLimit;
    int m_rateRemaining;
    QDateTime m_rateResetDate;

    // Returns request with already included ClientID or not
    QNetworkRequest buildRequest(QUrl, bool = true);
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
