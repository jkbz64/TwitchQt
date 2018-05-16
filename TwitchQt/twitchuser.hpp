#ifndef TWITCHUSER_HPP
#define TWITCHUSER_HPP

#include <QString>

namespace Twitch {
// Models
struct User {
    enum class UserType {
        No,
        GlobalMod,
        Admin,
        Staff
    };
    enum class BroadcasterType {
        No,
        Partner,
        Affiliate
    };
    BroadcasterType m_broadcasterType;
    QString m_description;
    QString m_displayName;
    QString m_email;
    QString m_id;
    QString m_login;
    QString m_offlineImageUrl;
    QString m_profileImageUrl;
    UserType m_type;
    int m_viewCount;
};

using Users = QVector<User>;
}

Q_DECLARE_METATYPE(Twitch::User);
Q_DECLARE_METATYPE(Twitch::Users);

#endif // TWITCHUSER_HPP
