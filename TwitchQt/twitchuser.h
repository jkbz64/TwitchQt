#ifndef TWITCHUSER_H
#define TWITCHUSER_H

#include "twitchreply.h"

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
        // QString m_email;
        qulonglong m_id;
        QString m_login;
        QString m_offlineImageUrl;
        QString m_profileImageUrl;
        UserType m_type;
        qulonglong m_viewCount;
    };

    // Replies

    class UserReply : public Reply {
        Q_OBJECT
    public:
        using Reply::Reply;
    protected:
        virtual void parseData(const QJsonDocument&) override;
    };

    #include "twitchuser.inl"
}

Q_DECLARE_METATYPE(Twitch::User);

#endif // TWITCHUSER_H
