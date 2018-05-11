#ifndef TWITCHEMOTE_HPP
#define TWITCHEMOTE_HPP

#include <QString>
#include <QVector>
#include <QSharedPointer>

namespace Twitch {

struct EmoteData {
    virtual ~EmoteData() = default;
};

namespace TwitchEmotes {
    struct Emote : public EmoteData {
        Emote() = default;
        Emote(int id, QString code, int emoticonSet, QString description)
            : m_id(id)
            , m_code(code)
            , m_emoticonSet(emoticonSet)
            , m_description(description)
        {
        }
        int m_id;
        QString m_code;
        int m_emoticonSet;
        QString m_description;
        static QString urlTemplate() { return "https://static-cdn.jtvnw.net/emoticons/v1/{{id}}/{{size}}.0"; }
    };
}

namespace BTTV {
    struct Restrictions {
        QVector<QString> m_channels;
        QVector<QString> m_games;
    };

    struct Emote : public EmoteData {
        Emote() = default;
        Emote(QString id, QString code, QString channel, Restrictions restrictions, QString imageType)
            : m_id(id)
            , m_code(code)
            , m_channel(channel)
            , m_restrictions(restrictions)
            , m_imageType(imageType)
        {
        }
        QString m_id;
        QString m_code;
        QString m_channel;
        Restrictions m_restrictions;
        QString m_imageType;
        static QString urlTemplate() { return "https://cdn.betterttv.net/emote/{{id}}/{{size}}x"; }
    };
}

namespace FFZ {
    struct Owner {
        qulonglong m_id;
        QString m_displayName;
        QString m_name;
    };

    struct Emote : public EmoteData {
        Emote() = default;
        Emote(QString css, int height, bool hidden, int id, int margins, bool modifier, QString name, int offset, Owner owner, bool isPublic, QVector<QString> urls, int width)
            : m_css(css)
            , m_height(height)
            , m_hidden(hidden)
            , m_id(id)
            , m_margins(margins)
            , m_modifier(modifier)
            , m_name(name)
            , m_offset(offset)
            , m_owner(owner)
            , m_public(isPublic)
            , m_urls(urls)
            , m_width(width)
        {
        }
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
}

enum class EmoteType {
    TwitchEmotes,
    BTTV,
    FFZ
};

class Emote {
public:
    enum class ImageType {
        PNG,
        GIF
    };

private:
    template <typename>
    struct EmoteTag {
    };

    template <typename... Args>
    static Emote createEmoteImpl(EmoteTag<TwitchEmotes::Emote>, Args&&... args)
    {
        Emote emote;
        auto emoteData = QSharedPointer<TwitchEmotes::Emote>::create(std::forward<Args>(args)...);
        emote.m_data = emoteData;
        emote.m_type = EmoteType::TwitchEmotes;
        emote.m_id = QString::number(emoteData->m_id);
        emote.m_code = emoteData->m_code;
        emote.m_url = TwitchEmotes::Emote::urlTemplate().replace("{{id}}", emote.m_id);
        emote.m_imageType = ImageType::PNG;
        return emote;
    }

    template <typename... Args>
    static Emote createEmoteImpl(EmoteTag<BTTV::Emote>, Args&&... args)
    {
        Emote emote;
        auto emoteData = QSharedPointer<BTTV::Emote>::create(std::forward<Args>(args)...);
        emote.m_data = emoteData;
        emote.m_type = EmoteType::BTTV;
        emote.m_id = emoteData->m_id;
        emote.m_code = emoteData->m_code;
        emote.m_url = BTTV::Emote::urlTemplate().replace("{{id}}", emote.m_id);
        emote.m_imageType = emoteData->m_imageType.toUpper() == "PNG" ? ImageType::PNG : ImageType::GIF;
        return emote;
    }

    template <typename... Args>
    static Emote createEmoteImpl(EmoteTag<FFZ::Emote>, Args&&... args)
    {
        Emote emote;
        auto emoteData = QSharedPointer<FFZ::Emote>::create(std::forward<Args>(args)...);
        emote.m_data = emoteData;
        emote.m_type = EmoteType::FFZ;
        emote.m_id = QString::number(emoteData->m_id);
        emote.m_code = emoteData->m_name;
        emote.m_url = FFZ::Emote::urlTemplate().replace("{{id}}", emote.m_id);
        emote.m_imageType = ImageType::PNG;
        return emote;
    }

public:
    template <class EmoteClass, typename... Args>
    static Emote createEmote(Args&&... args)
    {
        return createEmoteImpl(EmoteTag<EmoteClass>{}, std::forward<Args>(args)...);
    }

    Emote()
        : m_data(nullptr)
    {
    }

    Emote(EmoteData* data)
        : m_data(data)
    {
    }

    Emote(const Emote& other)
        : m_type(other.m_type)
        , m_data(other.m_data)
        , m_id(other.m_id)
        , m_code(other.m_code)
        , m_url(other.m_url)
        , m_imageType(other.m_imageType)
    {
    }

    const EmoteType& emoteType() const
    {
        return m_type;
    }

    const QString& id() const
    {
        return m_id;
    }

    const QString& code() const
    {
        return m_code;
    }

    const QString& url() const
    {
        return m_url;
    }

    const ImageType& imageType() const
    {
        return m_imageType;
    }

    template <class EmoteClass>
    const EmoteClass& toEmote() const
    {
        // Maybe some kind of const cast?
        return *dynamic_cast<EmoteClass*>(m_data.data());
    }

private:
    EmoteType m_type;
    QSharedPointer<EmoteData> m_data;

    // Base members
    QString m_id;
    QString m_code;
    QString m_url;
    ImageType m_imageType;
};

using Emotes = QVector<Emote>;
}

Q_DECLARE_METATYPE(Twitch::Emote);
Q_DECLARE_METATYPE(Twitch::Emotes);

#endif // TWITCHEMOTE_HPP
