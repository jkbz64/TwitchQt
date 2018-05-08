
inline void UserReply::parseData(const JSON& json)
{
    if (json.find("data") != json.end()) {
        auto&& data = json["data"];
        if (!data.empty()) {
            auto&& user = data.front();

            QString broadcasterTypeStr = user["broadcaster_type"];
            User::BroadcasterType broadcasterType = User::BroadcasterType::No;
            if (broadcasterTypeStr == "partner")
                broadcasterType = User::BroadcasterType::Partner;
            else if (broadcasterTypeStr == "affiliate")
                broadcasterType = User::BroadcasterType::Affiliate;

            QString description = user["description"];
            QString displayName = user["display_name"];
            //QString email = user["email"];
            QString id = user["id"];
            QString login = user["login"];
            QString offlineImageUrl = user["offline_image_url"];
            QString profileImageUrl = user["profile_image_url"];

            QString userTypeStr = user["type"];
            User::UserType userType = User::UserType::No;
            if (userTypeStr == "global_mod")
                userType = User::UserType::GlobalMod;
            else if (userTypeStr == "admin")
                userType = User::UserType::Admin;
            else if (userTypeStr == "staff")
                userType = User::UserType::Staff;

            QString viewCount = user["view_count"];

            m_data.setValue(User{
                broadcasterType,
                description,
                displayName,
                //email,
                id.toULongLong(),
                login,
                offlineImageUrl,
                profileImageUrl,
                userType,
                viewCount.toULongLong() });
        } else {
            // ??
        }
    }
}

inline void UsersReply::parseData(const JSON& json)
{
    Users users;
    if (json.find("data") != json.end()) {
        auto&& data = json["data"];
        if (!data.empty()) {
            for (const auto& user : data) {
                QString broadcasterTypeStr = user["broadcaster_type"];
                User::BroadcasterType broadcasterType = User::BroadcasterType::No;
                if (broadcasterTypeStr == "partner")
                    broadcasterType = User::BroadcasterType::Partner;
                else if (broadcasterTypeStr == "affiliate")
                    broadcasterType = User::BroadcasterType::Affiliate;

                QString description = user["description"];
                QString displayName = user["display_name"];
                //QString email = user["email"];
                QString id = user["id"];
                QString login = user["login"];
                QString offlineImageUrl = user["offline_image_url"];
                QString profileImageUrl = user["profile_image_url"];

                QString userTypeStr = user["type"];
                User::UserType userType = User::UserType::No;
                if (userTypeStr == "global_mod")
                    userType = User::UserType::GlobalMod;
                else if (userTypeStr == "admin")
                    userType = User::UserType::Admin;
                else if (userTypeStr == "staff")
                    userType = User::UserType::Staff;

                QString viewCount = user["view_count"];

                users.push_back({ broadcasterType,
                    description,
                    displayName,
                    //email,
                    id.toULongLong(),
                    login,
                    offlineImageUrl,
                    profileImageUrl,
                    userType,
                    viewCount.toULongLong() });
            }
        } else {
            // ??
        }
    }
}
