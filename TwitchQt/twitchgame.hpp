#ifndef TWITCHGAME_H
#define TWITCHGAME_H

#include "twitchreply.hpp"
#include <QImage>

namespace Twitch {
// Models
struct Game {
    qulonglong m_id;
    QString m_name;
    QString m_boxArtUrl;
};

using Games = QVector<Twitch::Game>;

// Game-related replies
class GameReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

protected:
    virtual void parseData(const QJsonDocument&) override;
};

class TopGamesReply : public JSONReply {
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

#include "twitchgame.inl"
}

Q_DECLARE_METATYPE(Twitch::Game);
Q_DECLARE_METATYPE(Twitch::Games);

#endif // TWITCHGAME_H
