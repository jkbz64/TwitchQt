#ifndef TWITCHAPI_HPP
#define TWITCHAPI_HPP

#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "twitchgame.hpp"
#include "twitchreply.hpp"
#include "twitchstream.hpp"
#include "twitchuser.hpp"

namespace Twitch {
using ID = qulonglong;

namespace Detail {
    class Api : public QObject {
        Q_OBJECT
    public:
        Api(const QString&);
        Api(const QString&, QObject*);

        virtual ~Api();
        virtual QString api() const = 0;

        // Games
        GamesReply* getTopGames();
        GameReply* getGameById(ID);
        GamesReply* getGameByIds(const QStringList&);
        GameReply* getGameByName(const QString&);
        GamesReply* getGameByNames(const QStringList&);
        BoxArtReply* getBoxArtByUrl(const QString&, int, int);

        // Streams
        StreamReply* getStreamById(ID);
        StreamReply* getStreamByName(const QString&);

        StreamsReply* getStreamsByNames(const QStringList&, const QString& = "");
        StreamsReply* getStreamsByIds(const QStringList&, const QString& = "");
        StreamsReply* getStreamsByGameId(ID, const QString& = "");
        StreamsReply* getStreamsByGameIds(const QStringList&, const QString& = "");
        StreamsReply* getStreamsByLanguage(const QString&, const QString& = "");
        StreamsReply* getStreamsByLanguages(const QStringList&, const QString& = "");

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
