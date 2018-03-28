#ifndef TWITCH_H
#define TWITCH_H

#include <memory>

#include <QNetworkAccessManager>
#include <QPointer>
#include <QString>

#include "twitchreply.h"
#include "twitchgame.h"
#include "twitchstream.h"

namespace Twitch {
    Q_GLOBAL_STATIC(QNetworkAccessManager, http);
    Q_GLOBAL_STATIC(QString, api);
    Q_GLOBAL_STATIC(QString, clientID);

    static void setClientID(const QString&);

    //Games
    static QPointer<TopGamesReply> getTopGames();

    // Streams
    static QPointer<StreamReply> getStreamById(qulonglong);
    static QPointer<StreamReply> getStreamByName(QString);

    static QPointer<StreamsReply> getStreamsByGameId(qulonglong, QString = "");
    static QPointer<StreamsReply> getStreamsByLanguage(QString, QString = "");


    #include "twitch.inl"
}

#endif // TWITCH_H
