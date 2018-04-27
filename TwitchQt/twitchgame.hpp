#ifndef TWITCHGAME_HPP
#define TWITCHGAME_HPP

#include <QString>
#include <QVector>

namespace Twitch {
// Models
struct Game {
    qulonglong m_id;
    QString m_name;
    QString m_boxArtUrl;
};

using Games = QVector<Twitch::Game>;
}

#endif // TWITCHGAME_HPP
