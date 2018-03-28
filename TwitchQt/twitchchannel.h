#ifndef TWITCHCHANNEL_H
#define TWITCHCHANNEL_H

#include <QString>

namespace Twitch {
    class Channel {
    public:
        Channel();
        const QString& name() const;
    private:
        QString m_name;
    };
}

#endif // TWITCHCHANNEL_H
