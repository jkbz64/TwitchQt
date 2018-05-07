
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

inline void TwitchEmotes::GlobalEmotesReply::parseData(const QJsonDocument& json)
{
    QVector<TwitchEmotes::Emote> emotes;
    auto&& root = json.object();
    for (const auto& emoteObject : root) {
        auto&& emoteData = emoteObject.toObject();
        QString code = emoteData.value("code").toString();
        QString description = emoteData.value("description").toString();
        int emoticonSet = emoteData.value("emoticon_set").toInt();
        int id = emoteData.value("id").toInt();
        emotes.push_back({
            id,
            code,
            emoticonSet,
            description
        });
    }
    m_data.setValue(emotes);
}

inline void TwitchEmotes::SubscriberEmotesReply::parseData(const QJsonDocument &json)
{
    Q_UNUSED(json);
    // TODO endpoint...
}

inline void BTTV::GlobalEmotesReply::parseData(const QJsonDocument& json)
{
    auto&& root = json.object();
    if(root.contains("status") && root.value("status").toInt() != 200)
        return;

    QVector<BTTV::Emote> emotes;
    auto&& emotesObject = root.value("emotes").toArray();
    for(const auto& emoteValue : emotesObject)
    {
        auto&& emote = emoteValue.toObject();
        QString id = emote.value("id").toString();
        QString code = emote.value("code").toString();
        QString channel = emote.value("channel").toString();
        // TODO restrictions
        Restrictions restrictions;
        QString imageType = emote.value("imageType").toString();

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
inline void BTTV::SubscriberEmotesReply::parseData(const QJsonDocument &json)
{
    BTTV::Emotes emotes;

    auto&& root = json.object();
    if(root.contains("status") && root.value("status").toInt() != 200)
        return;

    auto&& emotesObject = root.value("emotes").toArray();
    for(const auto& emoteValue : emotesObject)
    {
        auto&& emote = emoteValue.toObject();
        QString id = emote.value("id").toString();
        QString code = emote.value("code").toString();
        QString channel = emote.value("channel").toString();
        // TODO restrictions
        Restrictions restrictions;
        QString imageType = emote.value("imageType").toString();

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

inline void FFZ::GlobalEmotesReply::parseData(const QJsonDocument& json)
{
    auto&& root = json.object();
    QVector<int> defaultSets;
    auto&& defaultSetsObject = root.value("default_sets").toArray();
    for(const auto& defaultSet : defaultSetsObject)
        defaultSets.push_back(defaultSet.toInt());

    FFZ::Emotes emotes;
    auto&& sets = root.value("sets").toObject();
    for(const auto& setValue : defaultSets)
    {
        auto&& set = sets.value(QString::number(setValue)).toObject();
        auto&& emoticons = set.value("emoticons").toArray();
        for(const auto& emoteObject : emoticons)
        {
            auto&& emote = emoteObject.toObject();
            QString css = emote.value("css").toString();
            int height = emote.value("height").toInt();
            bool hidden = emote.value("hidden").toBool();
            int id = emote.value("id").toInt();
            int margins = emote.value("margins").toInt();
            bool modifier = emote.value("modifier").toBool();
            QString name = emote.value("name").toString();
            int offset = emote.value("offset").toInt();
            auto&& ownerObject = emote.value("owner").toObject();
            qulonglong ownerID = ownerObject.value("_id").toString().toULongLong();
            QString ownerDisplayName = ownerObject.value("display_name").toString();
            QString ownerName = ownerObject.value("name").toString();
            Owner owner{
                ownerID,
                ownerDisplayName,
                ownerName
            };
            bool isPublic = emote.value("public").toBool();
            QVector<QString> urls;
            auto&& urlObject = emote.value("urls").toObject();
            for(const auto& url : urlObject)
                urls.push_back(url.toString());
            int width = emote.value("width").toInt();

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

inline void FFZ::SubscriberEmotesReply::parseData(const QJsonDocument &json)
{
    auto&& root = json.object();
    FFZ::Emotes emotes;
    auto&& sets = root.value("sets").toObject();
    for(const auto& setValue : sets)
    {
        auto&& set = setValue.toObject();
        auto&& emoticons = set.value("emoticons").toArray();
        for(const auto& emoteObject : emoticons)
        {
            auto&& emote = emoteObject.toObject();
            QString css = emote.value("css").toString();
            int height = emote.value("height").toInt();
            bool hidden = emote.value("hidden").toBool();
            int id = emote.value("id").toInt();
            int margins = emote.value("margins").toInt();
            bool modifier = emote.value("modifier").toBool();
            QString name = emote.value("name").toString();
            int offset = emote.value("offset").toInt();
            auto&& ownerObject = emote.value("owner").toObject();
            qulonglong ownerID = ownerObject.value("_id").toString().toULongLong();
            QString ownerDisplayName = ownerObject.value("display_name").toString();
            QString ownerName = ownerObject.value("name").toString();
            Owner owner{
                ownerID,
                ownerDisplayName,
                ownerName
            };
            bool isPublic = emote.value("public").toBool();
            QVector<QString> urls;
            auto&& urlObject = emote.value("urls").toObject();
            for(const auto& url : urlObject)
                urls.push_back(url.toString());
            int width = emote.value("width").toInt();

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
