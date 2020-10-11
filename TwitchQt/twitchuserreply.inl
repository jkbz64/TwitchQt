
inline void UserReply::parseData(const JSON& json)
{
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        if (!data.empty()) {
            const auto& user = data.front();
            QString broadcasterTypeStr = user.value("broadcaster_type", QString(""));
            User::BroadcasterType broadcasterType = User::BroadcasterType::No;
            if (broadcasterTypeStr == "partner")
                broadcasterType = User::BroadcasterType::Partner;
            else if (broadcasterTypeStr == "affiliate")
                broadcasterType = User::BroadcasterType::Affiliate;

            QString userTypeStr = user.value("type", QString(""));
            User::UserType userType = User::UserType::No;
            if (userTypeStr == "global_mod")
                userType = User::UserType::GlobalMod;
            else if (userTypeStr == "admin")
                userType = User::UserType::Admin;
            else if (userTypeStr == "staff")
                userType = User::UserType::Staff;

            m_data.setValue(User{
                broadcasterType,
                user.value("description", QString("")),
                user.value("display_name", QString("")),
                user.value("email", QString("")),
                user.value("id", QString("-1")),
                user.value("login", QString("")),
                user.value("offline_image_url", QString("")),
                user.value("profile_image_url", QString("")),
                userType,
                user.value("view_count", -1) });
        }
    }
}

inline void UsersReply::parseData(const JSON& json)
{
    Users users;
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        for (const auto& user : data) {
            QString broadcasterTypeStr = user.value("broadcaster_type", QString(""));
            User::BroadcasterType broadcasterType = User::BroadcasterType::No;
            if (broadcasterTypeStr == "partner")
                broadcasterType = User::BroadcasterType::Partner;
            else if (broadcasterTypeStr == "affiliate")
                broadcasterType = User::BroadcasterType::Affiliate;

            QString userTypeStr = user.value("type", QString(""));
            User::UserType userType = User::UserType::No;
            if (userTypeStr == "global_mod")
                userType = User::UserType::GlobalMod;
            else if (userTypeStr == "admin")
                userType = User::UserType::Admin;
            else if (userTypeStr == "staff")
                userType = User::UserType::Staff;

            users.push_back({ broadcasterType,
                user.value("description", QString("")),
                user.value("display_name", QString("")),
                user.value("email", QString("")),
                user.value("id", QString("-1")),
                user.value("login", QString("")),
                user.value("offline_image_url", QString("")),
                user.value("profile_image_url", QString("")),
                userType,
                user.value("view_count", -1) });
        }
    }
    m_data.setValue(users);
}

inline void UserFollowsReply::parseData(const JSON& json)
{
    UserFollows userFollows;
    userFollows.m_total = json["total"].get<int>();
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        for (const auto& follow : data) {
            userFollows.m_follows.push_back({
                follow.value("from_id", QString("")),
                follow.value("from_name", QString("")),
                follow.value("to_id", QString("")),
                follow.value("to_name", QString("")),
                follow.value("followed_at", QString("")),
            });
        }
    }
    m_data.setValue(userFollows);
}

inline Twitch::User Twitch::UserReply::user()
{
    return m_data.value<Twitch::User>();
}

inline Twitch::Users Twitch::UsersReply::users()
{
    return m_data.value<Twitch::Users>();
}

inline Twitch::UserFollows Twitch::UserFollowsReply::userFollows()
{
    return m_data.value<Twitch::UserFollows>();
}
