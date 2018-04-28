#ifndef TWITCHGAME_HPP
#define TWITCHGAME_HPP

#include <QString>
#include <QVector>

namespace Twitch {
// Models
struct Game {
    Game() = default;
    qulonglong m_id;
    QString m_name;
    QString m_boxArtUrl;
};

using Games = QVector<Twitch::Game>;
}

Q_DECLARE_METATYPE(Twitch::Game);
Q_DECLARE_METATYPE(Twitch::Games);

#endif // TWITCHGAME_HPP
