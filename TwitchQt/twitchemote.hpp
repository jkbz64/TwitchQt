#ifndef TWITCHEMOTE_HPP
#define TWITCHEMOTE_HPP

#include <QString>
#include <QVector>

namespace Twitch {
namespace TwitchEmotes {
    struct Emote {
        int m_id;
        QString m_code;
        int m_emoticonSet;
        QString m_description;
        static QString urlTemplate() { return "https://static-cdn.jtvnw.net/emoticons/v1/{{id}}/{{size}}.0"; }
    };

    using Emotes = QVector<TwitchEmotes::Emote>;
}

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
        static QString urlTemplate() { return "https://cdn.betterttv.net/emote/{{id}}/{{size}}x"; }
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
        int m_id;
        int m_margins;
        bool m_modifier;
        QString m_name;
        int m_offset;
        Owner m_owner;
        bool m_public;
        QVector<QString> m_urls;
        int m_width;
        static QString urlTemplate() { return "https://cdn.frankerfacez.com/emoticon/{{id}}/{{size}}"; }
    };

    using Emotes = QVector<FFZ::Emote>;
}

struct Emote {
    enum class EmoteType {
        TwitchEmotes,
        BTTV,
        FFZ
    } m_type;
    QString m_id;
    QString m_code;
    enum class ImageType {
        PNG,
        GIF
    } m_imageType;
    QString m_url;

    static Emote fromEmote(const TwitchEmotes::Emote& emote)
    {
        const auto id = QString::number(emote.m_id);
        return Emote{
            EmoteType::TwitchEmotes,
            id,
            emote.m_code,
            ImageType::PNG,
            QString(TwitchEmotes::Emote::urlTemplate()).replace("{{id}}", id)
        };
    }

    static Emote fromEmote(const BTTV::Emote& emote)
    {
        return Emote{
            EmoteType::BTTV,
            emote.m_id,
            emote.m_code,
            emote.m_imageType.toUpper() == "PNG" ? ImageType::PNG : ImageType::GIF,
            QString(BTTV::Emote::urlTemplate()).replace("{{id}}", emote.m_id)
        };
    }

    static Emote fromEmote(const FFZ::Emote& emote)
    {
        const auto id = QString::number(emote.m_id);
        return Emote{
            EmoteType::FFZ,
            id,
            emote.m_name,
            ImageType::PNG,
            QString(FFZ::Emote::urlTemplate()).replace("{{id}}", id)
        };
    }
};

using Emotes = QVector<Emote>;
}

Q_DECLARE_METATYPE(Twitch::TwitchEmotes::Emote);
Q_DECLARE_METATYPE(Twitch::BTTV::Emote);
Q_DECLARE_METATYPE(Twitch::FFZ::Emote);
Q_DECLARE_METATYPE(Twitch::TwitchEmotes::Emotes);
Q_DECLARE_METATYPE(Twitch::BTTV::Emotes);
Q_DECLARE_METATYPE(Twitch::FFZ::Emotes);
Q_DECLARE_METATYPE(Twitch::Emote);
Q_DECLARE_METATYPE(Twitch::Emotes);

#endif // TWITCHEMOTE_HPP
