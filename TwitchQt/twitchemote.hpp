#ifndef TWITCHEMOTE_HPP
#define TWITCHEMOTE_HPP

#include <QString>
#include <QVector>

namespace Twitch {
struct Emote {
    qulonglong m_id;
    QString m_code;
    int m_emoticonSet;
    QString m_description;
};

namespace BTTV {
    struct Restrictions {
        QVector<QString> m_channels;
        QVector<QString> m_games;
    };

    struct Emote {
        QString m_id;
        QString m_code;
        QString m_channel;
        Restrictions m_restrictions;
        QString m_imageType;
    };
    using Emotes = QVector<BTTV::Emote>;
}

namespace FFZ {
    struct Owner {
        qulonglong m_id;
        QString m_displayName;
        QString m_name;
    };

    struct Emote {
        QString m_css;
        int m_height;
        bool m_hidden;
        QString m_id;
        int m_margins;
        bool m_modifier;
        QString m_name;
        int m_offset;
        Owner m_owner;
        bool m_public;
        QVector<QString> m_urls;
        int m_width;
    };
    using Emotes = QVector<FFZ::Emote>;
}

using Emotes = QVector<Emote>;
}

Q_DECLARE_METATYPE(Twitch::Emote);
Q_DECLARE_METATYPE(Twitch::BTTV::Emote);
Q_DECLARE_METATYPE(Twitch::FFZ::Emote);
Q_DECLARE_METATYPE(Twitch::Emotes);
Q_DECLARE_METATYPE(Twitch::BTTV::Emotes);
Q_DECLARE_METATYPE(Twitch::FFZ::Emotes);

#endif // TWITCHEMOTE_HPP
