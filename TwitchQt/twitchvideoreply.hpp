#ifndef TWITCHVIDEOREPLY_HPP
#define TWITCHVIDEOREPLY_HPP

#include "twitchreply.hpp"
#include "twitchvideo.hpp"

namespace Twitch {
class VideoReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Video video();

protected:
    virtual void parseData(const JSON&) override;
};

class VideosReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;
    int combinedViewerCount() const;

    Twitch::Videos videos();

protected:
    int m_combinedViewerCount{ 0 };
    virtual void parseData(const JSON&) override;
};

#include "twitchvideoreply.inl"
}

#endif // TWITCHVIDEOREPLY_HPP
