#ifndef TWITCH_H
#define TWITCH_H

#include <QNetworkAccessManager>
#include <QPointer>
#include <QString>

#include "twitchgame.hpp"
#include "twitchreply.hpp"
#include "twitchstream.hpp"
#include "twitchuser.hpp"

namespace Twitch {

using ID = qulonglong;

Q_GLOBAL_STATIC(QNetworkAccessManager, http);
Q_GLOBAL_STATIC_WITH_ARGS(QString, api, ("https://api.twitch.tv/helix/"));
Q_GLOBAL_STATIC(QString, clientID);

static void setClientID(const QString&);

// Games
static QPointer<TopGamesReply> getTopGames();

// Streams
static QPointer<StreamReply> getStreamById(ID);
static QPointer<StreamReply> getStreamByName(const QString&);

static QPointer<StreamsReply> getStreamsByGameId(ID, QString = "");
static QPointer<StreamsReply> getStreamsByLanguage(QString, QString = "");

// Users
static QPointer<UserReply> getUserById(ID);
static QPointer<UserReply> getUserByName(QString);

#include "twitch.inl"
} // namespace Twitch

#endif // TWITCH_H
