#ifndef TWITCHGAME_H
#define TWITCHGAME_H

#include <QString>
#include "twitchreply.h"

namespace Twitch {
    // Models
    struct Game {
        qulonglong m_id;
        QString m_name;
        QString m_boxArtUrl;
    };

    using Games = QVector<Twitch::Game>;

    // Game-related replies
    class GameReply : public Reply {
        Q_OBJECT
    public:
        using Reply::Reply;
    protected:
        virtual void parseData(const QJsonDocument&) override;
    };

    class TopGamesReply : public Reply {
        Q_OBJECT
    public:
        using Reply::Reply;
    protected:
        virtual void parseData(const QJsonDocument&) override;
    };

    #include "twitchgame.inl"
}

Q_DECLARE_METATYPE(Twitch::Game);
Q_DECLARE_METATYPE(Twitch::Games);

#endif // TWITCHGAME_H
