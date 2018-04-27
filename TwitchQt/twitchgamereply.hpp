#ifndef TWITCHGAMEREPLY_HPP
#define TWITCHGAMEREPLY_HPP

#include "twitchgame.hpp"
#include "twitchreply.hpp"
#include <QImage>

Q_DECLARE_METATYPE(Twitch::Game);
Q_DECLARE_METATYPE(Twitch::Games);

namespace Twitch {
class GameReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

protected:
    virtual void parseData(const QJsonDocument&) override;
};

class GamesReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

protected:
    virtual void parseData(const QJsonDocument&) override;
};

class BoxArtReply : public RawReply {
public:
    using RawReply::RawReply;

protected:
    virtual void parseData(const QByteArray&) override;
};

#include "twitchgamereply.inl"
}

#endif // TWITCHGAMEREPLY_HPP
