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

struct Follow {
    QString m_fromId;
    QString m_fromName;
    QString m_toId;
    QString m_toName;
    QString m_followedAt;
};

struct UserFollows {
    int m_total;
    QVector<Twitch::Follow> m_follows;
};

using Users = QVector<User>;
}

Q_DECLARE_METATYPE(Twitch::User);
Q_DECLARE_METATYPE(Twitch::Users);
Q_DECLARE_METATYPE(Twitch::UserFollows);

#endif // TWITCHUSER_HPP
