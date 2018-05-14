#ifndef TWITCHEMOTE_HPP
#define TWITCHEMOTE_HPP

#include "json.hpp"
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

namespace nlohmann {

template <>
struct adl_serializer<Twitch::EmoteType> {
    static Twitch::EmoteType from_json(const json& j)
    {
        const QString& type = j;
        if (type == "TwitchEmotes")
            return Twitch::EmoteType::TwitchEmotes;
        else if (type == "BTTV")
            return Twitch::EmoteType::BTTV;
        else if (type == "FFZ")
            return Twitch::EmoteType::FFZ;
    }
    static void to_json(json& j, const Twitch::EmoteType& t)
    {
        switch (t) {
        case Twitch::EmoteType::TwitchEmotes:
            j = QString("TwitchEmotes");
            break;
        case Twitch::EmoteType::BTTV:
            j = QString("BTTV");
            break;
        case Twitch::EmoteType::FFZ:
            j = QString("FFZ");
            break;
        }
    }
};

template <>
struct adl_serializer<Twitch::TwitchEmotes::Emote> {
    static Twitch::TwitchEmotes::Emote from_json(const json& emote)
    {
        return Twitch::TwitchEmotes::Emote(
            emote.value("id", -1),
            emote.value("code", QString("ERROR")),
            emote.value("emoticon_set", -1),
            emote.value("description", QString("")));
    }

    static void to_json(json& j, const Twitch::TwitchEmotes::Emote& emote)
    {
        j["id"] = emote.m_id;
        j["code"] = emote.m_code;
        j["emoticon_set"] = emote.m_emoticonSet;
        j["description"] = emote.m_description;
    }
};

template <>
struct adl_serializer<Twitch::BTTV::Emote> {
    static Twitch::BTTV::Emote from_json(const json& emote)
    {
        return Twitch::BTTV::Emote(
            emote.value("id", QString("-1")),
            emote.value("code", QString("ERROR")),
            emote.value("channel", QString("ERROR")),
            Twitch::BTTV::Restrictions{},
            emote.value<QString>("imageType", QString("PNG")));
    }

    static void to_json(json& j, const Twitch::BTTV::Emote& emote)
    {
        j["id"] = emote.m_id;
        j["code"] = emote.m_code;
        j["channel"] = emote.m_channel;
        // TODO restrictions
        j["imageType"] = emote.m_imageType;
    }
};

template <>
struct adl_serializer<Twitch::FFZ::Emote> {
    static Twitch::FFZ::Emote from_json(const json& emote)
    {
        /* auto&& ownerObject = emote["owner"];
        qulonglong ownerID = ownerObject["_id"];
        QString ownerDisplayName = ownerObject["display_name"];
        QString ownerName = ownerObject["name"];
        Twitch::FFZ::Owner owner{
            ownerID,
            ownerDisplayName,
            ownerName
        };*/

        QVector<QString> urls;
        auto&& urlObject = emote["urls"].object();
        for (const auto& url : urlObject)
            urls.push_back(url);

        return Twitch::FFZ::Emote(
            emote.value("css", QString("")),
            emote.value("height", -1),
            emote.value("hidden", false),
            emote.value("id", -1),
            emote.value("margins", 0),
            emote.value("modifier", false),
            emote.value("name", QString("ERROR")),
            emote.value("offset", 0),
            Twitch::FFZ::Owner{},
            emote.value("public", false),
            urls,
            emote.value("width", -1));
    }

    static void to_json(json& j, const Twitch::FFZ::Emote& emote)
    {
        j["css"] = emote.m_css;
        j["height"] = emote.m_height;
        j["name"] = emote.m_name;
        j["hidden"] = emote.m_hidden;
        j["offset"] = emote.m_offset;
        j["id"] = emote.m_id;
        j["margins"] = emote.m_margins;
        j["modifier"] = emote.m_modifier;
        // TODO j["owner"] = emote.m_owner;
        j["public"] = emote.m_public;
        j["urls"] = emote.m_urls;
        j["width"] = emote.m_width;
    }
};

template <>
struct adl_serializer<Twitch::Emote> {
    static Twitch::Emote from_json(const json& j)
    {
        const Twitch::EmoteType type = j.value("type", Twitch::EmoteType::TwitchEmotes);
        if (type == Twitch::EmoteType::TwitchEmotes) {
            const Twitch::TwitchEmotes::Emote& emoteData = j["data"];
            return Twitch::Emote::createEmote<Twitch::TwitchEmotes::Emote>(emoteData);
        } else if (type == Twitch::EmoteType::BTTV) {
            const Twitch::BTTV::Emote& emoteData = j["data"];
            return Twitch::Emote::createEmote<Twitch::BTTV::Emote>(emoteData);
        } else if (type == Twitch::EmoteType::FFZ) {
            const Twitch::FFZ::Emote& emoteData = j["data"];
            return Twitch::Emote::createEmote<Twitch::FFZ::Emote>(emoteData);
        }

        return Twitch::Emote{};
    }

    static void to_json(json& j, const Twitch::Emote& t)
    {
        j["type"] = t.emoteType();
        j["id"] = t.id();
        j["code"] = t.code();
        j["url"] = t.url();
        switch (t.imageType()) {
        case Twitch::Emote::ImageType::PNG:
            j["imageType"] = QString("PNG");
            break;
        case Twitch::Emote::ImageType::GIF:
            j["imageType"] = QString("GIF");
            break;
        }
        switch (t.emoteType()) {
        case Twitch::EmoteType::TwitchEmotes:
            j["data"] = t.toEmote<Twitch::TwitchEmotes::Emote>();
            break;
        case Twitch::EmoteType::BTTV:
            j["data"] = t.toEmote<Twitch::BTTV::Emote>();
            break;
        case Twitch::EmoteType::FFZ:
            j["data"] = t.toEmote<Twitch::FFZ::Emote>();
            break;
        }
    }
};
}

Q_DECLARE_METATYPE(Twitch::Emote);
Q_DECLARE_METATYPE(Twitch::Emotes);

namespace Twitch {
#include "twitchemote.inl"
}

#endif // TWITCHEMOTE_HPP
