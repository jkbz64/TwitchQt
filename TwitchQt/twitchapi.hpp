#ifndef TWITCHAPI_HPP
#define TWITCHAPI_HPP

#include <QNetworkAccessManager>

#include "twitchgamereply.hpp"
#include "twitchstreamreply.hpp"
#include "twitchuserreply.hpp"

namespace Twitch {
using ID = qulonglong;

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

        // Games
        GamesReply* getTopGames(int = 20);
        GameReply* getGameById(ID);
        GamesReply* getGameByIds(const QStringList&);
        GameReply* getGameByName(const QString&);
        GamesReply* getGameByNames(const QStringList&);
        BoxArtReply* getBoxArtByUrl(const QString&, int, int);

        // Streams
        StreamReply* getStreamByUserId(ID);
        StreamReply* getStreamByName(const QString&);

        StreamsReply* getStreamsByNames(const QStringList&, int = 20, const QString& = "");
        StreamsReply* getStreamsByUserIds(const QStringList&, int = 20, const QString& = "");
        StreamsReply* getStreamsByGameId(ID, int = 20, const QString& = "");
        StreamsReply* getStreamsByGameIds(const QStringList&, int = 20, const QString& = "");
        StreamsReply* getStreamsByLanguage(const QString&, int = 20, const QString& = "");
        StreamsReply* getStreamsByLanguages(const QStringList&, int = 20, const QString& = "");

        // Users
        UserReply* getUserById(ID);
        UserReply* getUserByName(const QString&);

        UsersReply* getUserByIds(const QStringList&, const QString& = "");
        UsersReply* getUserByNames(const QStringList&, const QString& = "");

        int rateLimit() const;
        int remainingRequests() const;
        const QDateTime& resetDate() const;

    protected:
        QNetworkAccessManager* m_http;
        QString m_clientID;
        int m_rateLimit{ 0 };
        int m_rateRemaining{ 0 };
        QDateTime m_rateResetDate;

        virtual QNetworkRequest buildRequest(QUrl) = 0;
        // Basically repeated parameters format
        // For example in Helix it's &par= and in v5 it's comma (,)
        virtual QString repeatDelimeter(const QString& = "") const = 0;

        template <class T>
        T* createReply(const QNetworkRequest&);

        void updateLimits(QNetworkReply*);
    };
}

class Helix : public Detail::Api {
    Q_OBJECT
public:
    Helix(const QString&);
    Helix(const QString&, QObject*);
    ~Helix();

    virtual QString api() const override;

protected:
    virtual QNetworkRequest buildRequest(QUrl) override;
    virtual QString repeatDelimeter(const QString&) const override;
};

#include "twitchapi.inl"

using Api = Helix;
}

#endif // TWITCHAPI_HPP
