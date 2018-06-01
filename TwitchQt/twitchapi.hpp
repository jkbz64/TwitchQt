#ifndef TWITCHAPI_HPP
#define TWITCHAPI_HPP

#include <QNetworkAccessManager>

#include "twitchemotereply.hpp"
#include "twitchgamereply.hpp"
#include "twitchstreamreply.hpp"
#include "twitchuserreply.hpp"

namespace Twitch {
namespace Detail {
    class TwitchApi : public QObject {
        Q_OBJECT
    public:
        TwitchApi(QObject* = nullptr);
        TwitchApi(const QString&);
        TwitchApi(const QString&, QObject*);
        virtual ~TwitchApi();

        // Client ID
        const QString& clientID() const;
        void setClientID(const QString&);

        // Main endpoints
        virtual QString api() const = 0;
        virtual QString emotesApi() const;
        virtual QString ffzApi() const;
        virtual QString bttvApi() const;

        // Games
        virtual GamesReply* getTopGames(int) = 0;
        virtual GameReply* getGameById(const QString&) = 0;
        virtual GamesReply* getGameByIds(const QStringList&) = 0;
        virtual GameReply* getGameByName(const QString&) = 0;
        virtual GamesReply* getGameByNames(const QStringList&) = 0;
        virtual BoxArtReply* getBoxArtByUrl(const QString&, int, int) = 0;

        // Streams
        virtual StreamReply* getStreamByUserId(const QString&) = 0;
        virtual StreamReply* getStreamByName(const QString&) = 0;

        virtual StreamsReply* getStreamsByNames(const QStringList&, int, const QString& = "") = 0;
        virtual StreamsReply* getStreamsByUserIds(const QStringList&, int, const QString& = "") = 0;
        virtual StreamsReply* getStreamsByGameId(const QString&, int, const QString& = "") = 0;
        virtual StreamsReply* getStreamsByGameIds(const QStringList&, int, const QString& = "") = 0;
        virtual StreamsReply* getStreamsByLanguage(const QString&, int, const QString& = "") = 0;
        virtual StreamsReply* getStreamsByLanguages(const QStringList&, int, const QString& = "") = 0;

        // Users
        virtual UserReply* getUserById(const QString& ID) = 0;
        virtual UserReply* getUserByName(const QString&) = 0;
        virtual UsersReply* getUserByIds(const QStringList&, const QString& = "") = 0;
        virtual UsersReply* getUserByNames(const QStringList&, const QString& = "") = 0;

        // Emotes
        virtual EmotesReply* getGlobalEmotes() = 0;
        virtual EmotesReply* getChannelEmotes(const QString&) = 0;
        virtual EmotesReply* getEmotesByEmoteSet(const QString&) = 0;
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

        // Image getter by Url
        virtual ImageReply* getImage(const QString&);

        // Rate limiting
        int rateLimit() const;
        int remainingRequests() const;
        const QDateTime& resetDate() const;

    protected:
        QNetworkAccessManager* m_http;
        QString m_clientID;
        int m_rateLimit{ 0 };
        int m_rateRemaining{ 0 };
        QDateTime m_rateResetDate;

        // Returns request with already included ClientID or not
        virtual QNetworkRequest buildRequest(QUrl, bool = true) = 0;
        // Repeated parameters format for requests
        // For example in Helix it's ampersand (&) and in v5 it's comma (,)
        virtual QString repeatDelimeter(const QString& = "") const = 0;

        template <class T>
        T* createReply(const QNetworkRequest&, bool = true);

        void updateLimits(QNetworkReply*);
    };
}

class Helix : public Detail::TwitchApi {
    Q_OBJECT
public:
    Helix(QObject* = nullptr);
    Helix(const QString&);
    Helix(const QString&, QObject*);
    virtual ~Helix();

    virtual QString api() const override;

    // Games
    virtual GamesReply* getTopGames(int = 20) override;
    virtual GameReply* getGameById(const QString& ID) override;
    virtual GamesReply* getGameByIds(const QStringList&) override;
    virtual GameReply* getGameByName(const QString&) override;
    virtual GamesReply* getGameByNames(const QStringList&) override;
    virtual BoxArtReply* getBoxArtByUrl(const QString&, int, int) override;

    // Streams
    virtual StreamReply* getStreamByUserId(const QString& ID) override;
    virtual StreamReply* getStreamByName(const QString&) override;

    virtual StreamsReply* getStreamsByNames(const QStringList&, int = 20, const QString& = "") override;
    virtual StreamsReply* getStreamsByUserIds(const QStringList&, int = 20, const QString& = "") override;
    virtual StreamsReply* getStreamsByGameId(const QString& ID, int = 20, const QString& = "") override;
    virtual StreamsReply* getStreamsByGameIds(const QStringList&, int = 20, const QString& = "") override;
    virtual StreamsReply* getStreamsByLanguage(const QString&, int = 20, const QString& = "") override;
    virtual StreamsReply* getStreamsByLanguages(const QStringList&, int = 20, const QString& = "") override;

    // Users
    virtual UserReply* getUserById(const QString&) override;
    virtual UserReply* getUserByName(const QString&) override;

    virtual UsersReply* getUserByIds(const QStringList&, const QString& = "") override;
    virtual UsersReply* getUserByNames(const QStringList&, const QString& = "") override;

    // Emotes
    // Well, Helix does not have endpoints for emotes, sooooo.... these do not work, they throw std::runtime_error
    // You need to use TwitchEmotes backend which is included in TwitchApi and handle caching yourself
    virtual EmotesReply* getGlobalEmotes() override;
    virtual EmotesReply* getChannelEmotes(const QString&) override;
    virtual EmotesReply* getEmotesByEmoteSet(const QString&) override;

protected:
    virtual QNetworkRequest buildRequest(QUrl, bool = true) override;
    virtual QString repeatDelimeter(const QString&) const override;
};

#include "twitchapi.inl"

using Api = Helix;
}

#endif // TWITCHAPI_HPP
