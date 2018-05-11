
inline void TwitchEmotes::GlobalEmotesReply::parseData(const JSON& json)
{
    Twitch::Emotes emotes;
    for (const auto& emote : json) {
        emotes.push_back(Twitch::Emote::createEmote<TwitchEmotes::Emote>(
            emote["id"].get<int>(),
            emote["code"].get<QString>(),
            emote["emoticon_set"].get<int>(),
            emote["description"].get<QString>()
        ));
    }
    m_data.setValue(emotes);
}

inline void TwitchEmotes::SubscriberEmotesReply::parseData(const JSON &json)
{
    Twitch::Emotes emotes;
    for(const auto& user : json)
    {
        auto&& emotesArray = user["emotes"];
        for(const auto& emote : emotesArray)
        {
            emotes.push_back(Twitch::Emote::createEmote<TwitchEmotes::Emote>(
                emote["id"].get<int>(),
                emote["code"].get<QString>(),
                emote["emoticon_set"].get<int>(),
                QString("")
            ));
        }
    }
    m_data.setValue(emotes);
}

inline void BTTV::GlobalEmotesReply::parseData(const JSON& json)
{
    if(json.find("status") != json.end() && json["status"].get<int>() != 200)
        return;

    Twitch::Emotes emotes;
    auto&& emotesArray = json["emotes"];
    for(const auto& emote : emotesArray)
    {
        // TODO restrictions
        emotes.push_back(Twitch::Emote::createEmote<BTTV::Emote>(
            emote["id"].get<QString>(),
            emote["code"].get<QString>(),
            emote["channel"].get<QString>(),
            Restrictions{},
            emote["imageType"].get<QString>()
        ));
    }
    m_data.setValue(emotes);
}

// For now it's pretty much like Global one, but later we'll get the bots badges here too.
inline void BTTV::SubscriberEmotesReply::parseData(const JSON &json)
{
    Twitch::Emotes emotes;

    if(json.find("status") != json.end() && json["status"].get<int>() != 200)
    {
        m_currentState = ReplyState::Error;
        return;
    }

    auto&& emotesArray = json["emotes"];
    for(const auto& emote : emotesArray)
    {
        emotes.push_back(Twitch::Emote::createEmote<BTTV::Emote>(
            emote["id"].get<QString>(),
            emote["code"].get<QString>(),
            emote["channel"].get<QString>(),
            Restrictions{},
            emote["imageType"].get<QString>()
        ));
    }

    m_data.setValue(emotes);
}

inline void FFZ::GlobalEmotesReply::parseData(const JSON& json)
{
    Twitch::Emotes emotes;

    QVector<int> defaultSets;
    auto&& setsArray = json["default_sets"];
    for(const auto& set : setsArray)
        defaultSets.push_back(set.get<int>());

    auto&& sets = json["sets"];
    for(const auto& set : sets)
    {
        auto&& emoticons = set["emoticons"].array();
        for(const auto& emote : emoticons)
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
            Owner owner{
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
    }

    m_data.setValue(emotes);
}

inline void FFZ::SubscriberEmotesReply::parseData(const JSON& json)
{
    if(json.find("error") != json.end())
    {
        m_currentState = ReplyState::Error;
        return;
    }

    Twitch::Emotes emotes;
    auto&& sets = json["sets"];
    for(const auto& set : sets)
    {
        auto&& emoticons = set["emoticons"];
        for(const auto& emote : emoticons)
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
            Owner owner{
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
    }

    m_data.setValue(emotes);
}

inline Twitch::Emotes Twitch::Detail::EmotesReply::emotes()
{
    return m_data.value<Twitch::Emotes>();
}
