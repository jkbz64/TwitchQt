
inline void GlobalBadgesReply::parseData(const JSON& json)
{
    Twitch::Badges badges;
    if (json.find("badge_sets") != json.end()) {
        for (const auto& badge : json["badge_sets"].get<JSON::object_t>()) {
            QMap<QString, Badge::Version> versions;
            for (const auto& versionObject : badge.second["versions"].get<JSON::object_t>()) {
                const auto& version = versionObject.second;
                versions[QString::fromStdString(versionObject.first)] = Badge::Version{
                    version.value("image_url_1x", QString()),
                    version.value("description", QString()),
                    version.value("title", QString())
                };
            }
            badges.push_back(Twitch::Badge{
                QString::fromStdString(badge.first),
                versions });
        }
    }
    m_data.setValue(badges);
}

inline Twitch::Badges Twitch::GlobalBadgesReply::badges()
{
    return m_data.value<Twitch::Badges>();
}
