
template <typename... Args>
inline Emote Emote::createEmoteImpl(EmoteTag<TwitchEmotes::Emote>, Args&&... args)
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
inline Emote Emote::createEmoteImpl(EmoteTag<BTTV::Emote>, Args&&... args)
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
inline Emote Emote::createEmoteImpl(EmoteTag<FFZ::Emote>, Args&&... args)
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

template <class EmoteClass, typename... Args>
inline Emote Emote::createEmote(Args&&... args)
{
    return createEmoteImpl(EmoteTag<EmoteClass>{}, std::forward<Args>(args)...);
}

inline Emote::Emote()
    : m_data(nullptr)
{
}

inline Emote::Emote(EmoteData* data)
    : m_data(data)
{
}

inline Emote::Emote(const Emote& other)
    : m_type(other.m_type)
    , m_data(other.m_data)
    , m_id(other.m_id)
    , m_code(other.m_code)
    , m_url(other.m_url)
    , m_imageType(other.m_imageType)
{
}

inline const EmoteType& Emote::emoteType() const
{
    return m_type;
}

inline const QString& Emote::id() const
{
    return m_id;
}

inline const QString& Emote::code() const
{
    return m_code;
}

inline const QString& Emote::url() const
{
    return m_url;
}

inline const Emote::ImageType& Emote::imageType() const
{
    return m_imageType;
}

template <class EmoteClass>
inline const EmoteClass& Emote::toEmote() const
{
    // Maybe some kind of const cast?
    return *dynamic_cast<EmoteClass*>(m_data.data());
}


inline Twitch::Emotes Twitch::Emotes::fromTwitchEmotes(const JSON &json)
{
    Twitch::Emotes emotes;
    for(const auto& emote : json)
    {
        emotes.push_back(Twitch::Emote::createEmote<TwitchEmotes::Emote>(
            emote["id"].get<int>(),
            emote["code"].get<QString>(),
            emote["emoticon_set"].get<int>(),
            emote["description"].get<QString>()
        ));
    }
    return emotes;
}

inline Twitch::Emotes Twitch::Emotes::fromBTTV(const JSON &json)
{
    Twitch::Emotes emotes;
    for(const auto& emote : json)
    {
        emotes.push_back(Twitch::Emote::createEmote<BTTV::Emote>(
            emote["id"].get<QString>(),
            emote["code"].get<QString>(),
            emote["channel"].get<QString>(),
            BTTV::Restrictions{},
            emote["imageType"].get<QString>()
        ));
    }
    return emotes;
}

inline Twitch::Emotes Twitch::Emotes::fromFFZ(const JSON &json)
{
    Twitch::Emotes emotes;
    for(const auto& emote : json)
    {
        int margins = 0;
        if(!emote["margins"].is_null() && emote["margins"].is_number())
            margins = emote["margins"];

        int offset = 0;
        if(!emote["offset"].is_null() && emote["offset"].is_number())
            margins = emote["offset"];

        auto&& ownerObject = emote["owner"];
        qulonglong ownerID = ownerObject["_id"];
        QString ownerDisplayName = ownerObject["display_name"];
        QString ownerName = ownerObject["name"];
        FFZ::Owner owner{
            ownerID,
            ownerDisplayName,
            ownerName
        };
        QVector<QString> urls;
        auto&& urlObject = emote["urls"].object();
        for(const auto& url : urlObject)
            urls.push_back(url);

        emotes.push_back(Twitch::Emote::createEmote<FFZ::Emote>(
            emote["css"].get<QString>(),
            emote["height"].get<int>(),
            emote["hidden"].get<bool>(),
            emote["id"].get<int>(),
            margins,
            emote["modifier"].get<bool>(),
            emote["name"].get<QString>(),
            offset,
            owner,
            emote["public"].get<bool>(),
            urls,
            emote["width"].get<int>()
        ));
    }
    return emotes;
}
