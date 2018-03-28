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
    static QPointer<StreamsReply> getStreamsByGameId(qulonglong);
    static QPointer<StreamsReply> getStreamsByLanguage(QString);

    #include "twitch.inl"
}

#endif // TWITCH_H
