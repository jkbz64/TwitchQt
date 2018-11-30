#ifndef TWITCHBADGEREPLY_HPP
#define TWITCHBADGEREPLY_HPP

#include "twitchreply.hpp"
#include "twitchbadge.hpp"

namespace Twitch {
class GlobalBadgesReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Badges badges();

protected:
    virtual void parseData(const JSON&) override;
};

// Pretty much the same algorithm for parsing the json document
using ChannelBadgesReply = GlobalBadgesReply;

#include "twitchbadgereply.inl"
}

#endif // TWITCHBADGEREPLY_HPP
