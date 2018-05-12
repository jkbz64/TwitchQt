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

public:
    template <class EmoteClass, typename... Args>
    static Emote createEmote(Args&&... args);

    Emote();
    Emote(EmoteData* data);
    Emote(const Emote& other);

    const EmoteType& emoteType() const;
    const QString& id() const;
    const QString& code() const;
    const QString& url() const;
    const ImageType& imageType() const;

    template <class EmoteClass>
    const EmoteClass& toEmote() const;

private:
    EmoteType m_type;
    QSharedPointer<EmoteData> m_data;

    // Base members
    QString m_id;
    QString m_code;
    QString m_url;
    ImageType m_imageType;

    template <typename>
    struct EmoteTag {
    };

    template <typename... Args>
    static Emote createEmoteImpl(EmoteTag<TwitchEmotes::Emote>, Args&&... args);
    template <typename... Args>
    static Emote createEmoteImpl(EmoteTag<BTTV::Emote>, Args&&... args);
    template <typename... Args>
    static Emote createEmoteImpl(EmoteTag<FFZ::Emote>, Args&&... args);
};

class Emotes : public QVector<Emote> {
public:
    static Emotes fromTwitchEmotes(const JSON&);
    static Emotes fromBTTV(const JSON&);
    static Emotes fromFFZ(const JSON&);
};
}

Q_DECLARE_METATYPE(Twitch::Emote);
Q_DECLARE_METATYPE(Twitch::Emotes);

namespace Twitch {
#include "twitchemote.inl"
}

#endif // TWITCHEMOTE_HPP
