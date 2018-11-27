#ifndef TWITCHSTREAMREPLY_HPP
#define TWITCHSTREAMREPLY_HPP

#include "twitchreply.hpp"
#include "twitchstream.hpp"

namespace Twitch {
class StreamReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Stream stream();

protected:
    virtual void parseData(const JSON&) override;
};

class StreamsReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;
    int combinedViewerCount() const;

    Twitch::Streams streams();

protected:
    int m_combinedViewerCount{ 0 };
    virtual void parseData(const JSON&) override;
};

#include "twitchstreamreply.inl"
}

#endif // TWITCHSTREAMREPLY_HPP
