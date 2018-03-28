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

    static QPointer<Twitch::TopGamesReply> getTopGames();
    static QPointer<Twitch::StreamsReply> getStreamsByGameId(qulonglong);

    #include "twitch.inl"
}

#endif // TWITCH_H
