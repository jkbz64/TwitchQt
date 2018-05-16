#ifndef TWITCHAPI_HPP
#define TWITCHAPI_HPP

#include <QNetworkAccessManager>

#include "twitchemotereply.hpp"
#include "twitchgamereply.hpp"
#include "twitchstreamreply.hpp"
#include "twitchuserreply.hpp"

namespace Twitch {
namespace Detail {
    class Api : public QObject {
        Q_OBJECT
    public:
        Api(QObject* = nullptr);
        Api(const QString&);
        Api(const QString&, QObject*);
        virtual ~Api();

        const QString& clientID() const;
        void setClientID(const QString&);

        virtual QString api() const = 0;
        virtual QString emotesApi() const;
        virtual QString ffzApi() const;
        virtual QString bttvApi() const;

        // Games
        virtual GamesReply* getTopGames(int = 20);
        virtual GameReply* getGameById(const QStringID);
        virtual GamesReply* getGameByIds(const QStringList&);
        virtual GameReply* getGameByName(const QString&);
        virtual GamesReply* getGameByNames(const QStringList&);
        virtual BoxArtReply* getBoxArtByUrl(const QString&, int, int);

        // Streams
        virtual StreamReply* getStreamByUserId(const QStringID);
        virtual StreamReply* getStreamByName(const QString&);

        virtual StreamsReply* getStreamsByNames(const QStringList&, int = 20, const QString& = "");
        virtual StreamsReply* getStreamsByUserIds(const QStringList&, int = 20, const QString& = "");
        virtual StreamsReply* getStreamsByGameId(const QStringID, int = 20, const QString& = "");
        virtual StreamsReply* getStreamsByGameIds(const QStringList&, int = 20, const QString& = "");
        virtual StreamsReply* getStreamsByLanguage(const QString&, int = 20, const QString& = "");
        virtual StreamsReply* getStreamsByLanguages(const QStringList&, int = 20, const QString& = "");

        // Users
        virtual UserReply* getUserById(const QStringID);
        virtual UserReply* getUserByName(const QString&);

        virtual UsersReply* getUserByIds(const QStringList&, const QString& = "");
        virtual UsersReply* getUserByNames(const QStringList&, const QString& = "");

        // TwitchEmotes aka official global emotes
        virtual TwitchEmotes::GlobalEmotesReply* getGlobalEmotes();
        virtual TwitchEmotes::SubscriberEmotesReply* getTwitchSubscriberEmotes();
        virtual ImageReply* getTwitchEmoteImage(const QString&);

        // BetterTTV
        virtual BTTV::GlobalEmotesReply* getBTTVGlobalEmotes();
        virtual BTTV::SubscriberEmotesReply* getBTTVSubscriberEmotesByChannel(const QString&);
        virtual ImageReply* getBTTVEmoteImage(const QString&);

        // FFZ
        virtual FFZ::GlobalEmotesReply* getFFZGlobalEmotes();
        virtual FFZ::SubscriberEmotesReply* getFFZSubscriberEmotesByChannel(const QString&);
        virtual ImageReply* getFFZEmoteImage(const QString&);

        // Image getter by Url
        virtual ImageReply* getImage(const QString&);

        int rateLimit() const;
        int remainingRequests() const;
        const QDateTime& resetDate() const;

    protected:
        QNetworkAccessManager* m_http;
        QString m_clientID;
        int m_rateLimit{ 0 };
        int m_rateRemaining{ 0 };
        QDateTime m_rateResetDate;

        virtual QNetworkRequest buildRequest(QUrl, bool = true) = 0;
        // Basically repeated parameters format
        // For example in Helix it's &par= and in v5 it's comma (,)
        virtual QString repeatDelimeter(const QString& = "") const = 0;

        template <class T>
        T* createReply(const QNetworkRequest&, bool = true);

        void updateLimits(QNetworkReply*);
    };
}

class Helix : public Detail::Api {
    Q_OBJECT
public:
    Helix(QObject* = nullptr);
    Helix(const QString&);
    Helix(const QString&, QObject*);
    virtual ~Helix();

    virtual QString api() const override;

protected:
    virtual QNetworkRequest buildRequest(QUrl, bool) override;
    virtual QString repeatDelimeter(const QString&) const override;
};

#include "twitchapi.inl"

using Api = Helix;
}

#endif // TWITCHAPI_HPP
