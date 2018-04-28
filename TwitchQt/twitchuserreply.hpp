#ifndef TWITCHUSERREPLY_HPP
#define TWITCHUSERREPLY_HPP

#include "twitchreply.hpp"
#include "twitchuser.hpp"

namespace Twitch {
class UserReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

protected:
    virtual void parseData(const QJsonDocument&) override;
};

class UsersReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

protected:
    virtual void parseData(const QJsonDocument&) override;
};

#include "twitchuserreply.inl"
}

#endif // TWITCHUSERREPLY_HPP
