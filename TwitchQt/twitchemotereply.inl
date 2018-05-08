
inline Twitch::Emotes Detail::TwitchEmotesReply::toEmotes()
{
    Twitch::Emotes emotes;
    for(const auto& emote : m_data.value<TwitchEmotes::Emotes>())
        emotes.push_back(Twitch::Emote::fromEmote(emote));
    return emotes;
}

inline Twitch::Emotes Detail::BTTVEmotesReply::toEmotes()
{
    Twitch::Emotes emotes;
    for(const auto& emote : m_data.value<BTTV::Emotes>())
        emotes.push_back(Twitch::Emote::fromEmote(emote));
    return emotes;
}

inline Twitch::Emotes Detail::FFZEmotesReply::toEmotes()
{
    Twitch::Emotes emotes;
    for(const auto& emote : m_data.value<FFZ::Emotes>())
        emotes.push_back(Twitch::Emote::fromEmote(emote));
    return emotes;
}

inline void TwitchEmotes::GlobalEmotesReply::parseData(const JSON& json)
{
    TwitchEmotes::Emotes emotes;
    for (const auto& emote : json) {
        QString code = emote["code"];
        QString description = emote["description"];
        int emoticonSet = emote["emoticon_set"];
        int id = emote["id"];
        emotes.push_back({
            id,
            code,
            emoticonSet,
            description
        });
    }
    m_data.setValue(emotes);
}

inline void TwitchEmotes::SubscriberEmotesReply::parseData(const JSON &json)
{
    TwitchEmotes::Emotes emotes;
    for(const auto& user : json)
    {
        auto&& emotesArray = user["emotes"];
        for(const auto& emote : emotesArray)
        {
            int id = emote["id"];
            QString code = emote["code"];
            int emoticonSet = emote["emoticon_set"];
            emotes.push_back(TwitchEmotes::Emote{
                id,
                code,
                emoticonSet,
                QString("")
            });
        }
    }
    m_data.setValue(emotes);
}

inline void BTTV::GlobalEmotesReply::parseData(const JSON& json)
{
    if(json.find("status") != json.end() && json["status"].get<int>() != 200)
        return;

    QVector<BTTV::Emote> emotes;
    auto&& emotesArray = json["emotes"];
    for(const auto& emote : emotesArray)
    {
        QString id = emote["id"];
        QString code = emote["code"];
        QString channel = emote["channel"];
        // TODO restrictions
        Restrictions restrictions;
        QString imageType = emote["imageType"];;

        emotes.push_back(BTTV::Emote{
            id,
            code,
            channel,
            restrictions,
            imageType
        });
    }
    m_data.setValue(emotes);
}

// For now it's pretty much like Global one, but later we'll get the bots badges here too.
inline void BTTV::SubscriberEmotesReply::parseData(const JSON &json)
{
    BTTV::Emotes emotes;

    if(json.find("status") != json.end() && json["status"].get<int>() != 200)
    {
        m_currentState = ReplyState::Error;
        return;
    }

    auto&& emotesArray = json["emotes"];
    for(const auto& emote : emotesArray)
    {
        QString id = emote["id"];
        QString code = emote["code"];
        QString channel = emote["channel"];
        // TODO restrictions
        Restrictions restrictions;
        QString imageType = emote["imageType"];

        emotes.push_back(BTTV::Emote{
            id,
            code,
            channel,
            restrictions,
            imageType
        });
    }

    m_data.setValue(emotes);
}

inline void FFZ::GlobalEmotesReply::parseData(const JSON& json)
{
    FFZ::Emotes emotes;

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
            QString css = emote["css"];
            int height = emote["height"];
            bool hidden = emote["hidden"];
            int id = emote["id"];
            int margins = emote["margins"];
            bool modifier = emote["modifier"];
            QString name = emote["name"];
            int offset = emote["offset"];
            auto&& ownerObject = emote["owner"];
            qulonglong ownerID = ownerObject["_id"];
            QString ownerDisplayName = ownerObject["display_name"];
            QString ownerName = ownerObject["name"];
            Owner owner{
                ownerID,
                ownerDisplayName,
                ownerName
            };
            bool isPublic = emote["public"];
            QVector<QString> urls;
            auto&& urlObject = emote["urls"].object();
            for(const auto& url : urlObject)
                urls.push_back(url);
            int width = emote["width"];

            emotes.push_back(FFZ::Emote{
                css,
                height,
                hidden,
                id,
                margins,
                modifier,
                name,
                offset,
                owner,
                isPublic,
                urls,
                width
            });
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

    FFZ::Emotes emotes;
    auto&& sets = json["sets"];
    for(const auto& set : sets)
    {
        auto&& emoticons = set["emoticons"];
        for(const auto& emote : emoticons)
        {
            QString css = emote["css"];
            int height = emote["height"];
            bool hidden = emote["hidden"];
            int id = emote["id"];
            int margins = 0;
            if(!emote["margins"].is_null() && emote["margins"].is_number())
                margins = emote["margins"];
            bool modifier = emote["modifier"];
            QString name = emote["name"];
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
            bool isPublic = emote["public"];
            QVector<QString> urls;
            auto&& urlObject = emote["urls"];
            for(const auto& urlObject : urlObject)
            {
                QString url_ = urlObject;
                urls.push_back(url_);
            }
            int width = emote["width"];

            emotes.push_back(FFZ::Emote{
                css,
                height,
                hidden,
                id,
                margins,
                modifier,
                name,
                offset,
                owner,
                isPublic,
                urls,
                width
            });
        }
    }

    m_data.setValue(emotes);
}
