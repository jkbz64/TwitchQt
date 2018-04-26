
inline void UserReply::parseData(const QJsonDocument& json)
{
    auto&& root = json.object();
    if (root.contains("data")) {
        auto&& data = root.value("data").toArray();
        if (!data.isEmpty()) {
            auto&& user = data.first().toObject();

            QString broadcasterTypeStr = user.value("broadcaster_type").toString();
            User::BroadcasterType broadcasterType = User::BroadcasterType::No;
            if (broadcasterTypeStr == "partner")
                broadcasterType = User::BroadcasterType::Partner;
            else if (broadcasterTypeStr == "affiliate")
                broadcasterType = User::BroadcasterType::Affiliate;

            QString description = user.value("description").toString();
            QString displayName = user.value("display_name").toString();
            //QString email = user.value("email").toString();
            QString id = user.value("id").toString();
            QString login = user.value("login").toString();
            QString offlineImageUrl = user.value("offline_image_url").toString();
            QString profileImageUrl = user.value("profile_image_url").toString();

            QString userTypeStr = user.value("type").toString();
            User::UserType userType = User::UserType::No;
            if (userTypeStr == "global_mod")
                userType = User::UserType::GlobalMod;
            else if (userTypeStr == "admin")
                userType = User::UserType::Admin;
            else if (userTypeStr == "staff")
                userType = User::UserType::Staff;

            QString viewCount = user.value("view_count").toString();

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

inline void UsersReply::parseData(const QJsonDocument& json)
{
    Users users;
    auto&& root = json.object();
    if (root.contains("data")) {
        auto&& data = root.value("data").toArray();
        if (!data.isEmpty()) {
            for (const auto& userElement : data) {
                auto&& user = userElement.toObject();
                QString broadcasterTypeStr = user.value("broadcaster_type").toString();
                User::BroadcasterType broadcasterType = User::BroadcasterType::No;
                if (broadcasterTypeStr == "partner")
                    broadcasterType = User::BroadcasterType::Partner;
                else if (broadcasterTypeStr == "affiliate")
                    broadcasterType = User::BroadcasterType::Affiliate;

                QString description = user.value("description").toString();
                QString displayName = user.value("display_name").toString();
                //QString email = user.value("email").toString();
                QString id = user.value("id").toString();
                QString login = user.value("login").toString();
                QString offlineImageUrl = user.value("offline_image_url").toString();
                QString profileImageUrl = user.value("profile_image_url").toString();

                QString userTypeStr = user.value("type").toString();
                User::UserType userType = User::UserType::No;
                if (userTypeStr == "global_mod")
                    userType = User::UserType::GlobalMod;
                else if (userTypeStr == "admin")
                    userType = User::UserType::Admin;
                else if (userTypeStr == "staff")
                    userType = User::UserType::Staff;

                QString viewCount = user.value("view_count").toString();

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
