
inline void TwitchEmotes::GlobalEmotesReply::parseData(const JSON& json)
{
    m_data.setValue(Emotes::fromTwitchEmotes(json));
}

inline void TwitchEmotes::SubscriberEmotesReply::parseData(const JSON &json)
{
    Twitch::EmotesMap emotes;
    for(const auto& user : json)
    {
        const QString id = user.value("channel_id", QString(""));
        if(user.find("emotes") != user.end() && !id.isEmpty())
        {
             Twitch::Emotes userEmotes = Emotes::fromTwitchEmotes(user["emotes"]);
             for(const auto& emote : userEmotes)
             {
                 emotes.insert(id.toStdString(), emote);
             }
        }
    }
    m_data.setValue(emotes);
}

inline void BTTV::GlobalEmotesReply::parseData(const JSON& json)
{
    if(json.find("status") != json.end() && json["status"].get<int>() != 200)
        return;

    auto&& emotesArray = json["emotes"];
    m_data.setValue(Emotes::fromBTTV(emotesArray));
}

inline void BTTV::SubscriberEmotesReply::parseData(const JSON &json)
{
    Twitch::Emotes emotes;

    if(json.find("status") != json.end() && json["status"].get<int>() != 200)
    {
        m_currentState = ReplyState::Error;
        return;
    }

    auto&& emotesArray = json["emotes"];
    m_data.setValue(Emotes::fromBTTV(emotesArray));
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
        auto&& emotesArray = set["emoticons"];
        emotes << Emotes::fromFFZ(emotesArray);
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
        auto&& emotesArray = set["emoticons"];
        emotes << Emotes::fromFFZ(emotesArray);
    }

    m_data.setValue(emotes);
}

inline Twitch::Emotes EmotesReply::emotes()
{
    return m_data.value<Twitch::Emotes>();
}

inline Twitch::EmotesMap TwitchEmotes::SubscriberEmotesReply::emotes()
{
    return m_data.value<Twitch::EmotesMap>();
}
