#ifndef TWITCHAPI_HPP
#define TWITCHAPI_HPP

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
        TopGamesReply* getTopGames();
        GameReply* getGameById(ID);
        GameReply* getGameByName(const QString&);
        BoxArtReply* getBoxArtByUrl(const QString&, int, int);

        // Streams
        StreamReply* getStreamById(ID);
        StreamReply* getStreamByName(const QString&);

        StreamsReply* getStreamsByGameId(ID, const QString& = "");
        StreamsReply* getStreamsByLanguage(const QString&, const QString& = "");

        // Users
        UserReply* getUserById(ID);
        UserReply* getUserByName(const QString&);

    protected:
        QNetworkAccessManager* m_http;
        QString m_clientID;

        virtual QNetworkRequest buildRequest(QUrl) = 0;

        template <class T>
        T* createReply(const QNetworkRequest&);
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
};

#include "twitchapi.inl"
}

#endif // TWITCHAPI_HPP
