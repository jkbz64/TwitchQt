#ifndef TWITCHGAME_H
#define TWITCHGAME_H

#include <QString>
#include "twitchreply.h"

namespace Twitch {
    // Game model
    struct Game {
        int m_id;
        QString m_name;
        QString m_boxArtUrl;
    };

    // Game-related replies

    class GameReply : public Reply {
        Q_OBJECT
    public:
        GameReply(QNetworkReply*);
        ~GameReply() {}
    protected:
        virtual void parseData(const QJsonDocument&) override;
    };

    using Games = QVector<Twitch::Game>;

    class TopGamesReply : public Reply {
        Q_OBJECT
    public:
        TopGamesReply(QNetworkReply*);
        ~TopGamesReply() {}
    protected:
        virtual void parseData(const QJsonDocument&) override;
    };

    #include "twitchgame.inl"
}

Q_DECLARE_METATYPE(Twitch::Game);
Q_DECLARE_METATYPE(Twitch::Games);

#endif // TWITCHGAME_H
