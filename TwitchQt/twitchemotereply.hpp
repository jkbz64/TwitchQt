#ifndef TWITCHEMOTEREPLY_HPP
#define TWITCHEMOTEREPLY_HPP

#include "twitchreply.hpp"
#include "twitchemote.hpp"

namespace Twitch {
class EmotesReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;
    virtual ~EmotesReply() = default;
    Twitch::Emotes emotes();

protected:
    virtual void parseData(const JSON&) = 0;
};

namespace TwitchEmotes {
    class GlobalEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class SubscriberEmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;
        Twitch::EmotesMap emotes();

    protected:
        virtual void parseData(const JSON&) override;
    };
}

namespace BTTV {
    class GlobalEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class SubscriberEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };
}

namespace FFZ {
    class GlobalEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class SubscriberEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };
}

class EmoteSetsReply : public EmotesReply {
    Q_OBJECT
public:
    using EmotesReply::EmotesReply;

protected:
    virtual void parseData(const JSON&) override;
};

#include "twitchemotereply.inl"
}

#endif // TWITCHEMOTEREPLY_HPP
