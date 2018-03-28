#ifndef TWITCH_H
#define TWITCH_H

#include <QNetworkAccessManager>
#include <QPointer>
#include <QString>

#include "twitchgame.h"
#include "twitchstream.h"
#include "twitchuser.h"

namespace Twitch {
    Q_GLOBAL_STATIC(QNetworkAccessManager, http);
    Q_GLOBAL_STATIC(QString, api);
    Q_GLOBAL_STATIC(QString, clientID);

    static void setClientID(const QString&);

    // Games
    static QPointer<TopGamesReply> getTopGames();

    // Streams
    static QPointer<StreamReply> getStreamById(qulonglong);
    static QPointer<StreamReply> getStreamByName(QString);

    static QPointer<StreamsReply> getStreamsByGameId(qulonglong, QString = "");
    static QPointer<StreamsReply> getStreamsByLanguage(QString, QString = "");

    // Users
    static QPointer<UserReply> getUserById(qulonglong);
    static QPointer<UserReply> getUserByName(QString);


    #include "twitch.inl"
}

#endif // TWITCH_H
