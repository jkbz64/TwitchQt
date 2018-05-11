#ifndef TWITCHEMOTEREPLY_HPP
#define TWITCHEMOTEREPLY_HPP

#include "twitchreply.hpp"
#include "twitchemote.hpp"

namespace Twitch {

namespace Detail {
    class EmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;
        virtual ~EmotesReply() = default;
        Twitch::Emotes emotes();
    };
}

namespace TwitchEmotes {
    class GlobalEmotesReply : public Detail::EmotesReply {
        Q_OBJECT
    public:
        using Detail::EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class SubscriberEmotesReply : public Detail::EmotesReply {
        Q_OBJECT
    public:
        using Detail::EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };
}

namespace BTTV {
    class GlobalEmotesReply : public Detail::EmotesReply {
        Q_OBJECT
    public:
        using Detail::EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class SubscriberEmotesReply : public Detail::EmotesReply {
        Q_OBJECT
    public:
        using Detail::EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };
}

namespace FFZ {
    class GlobalEmotesReply : public Detail::EmotesReply {
        Q_OBJECT
    public:
        using Detail::EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class SubscriberEmotesReply : public Detail::EmotesReply {
        Q_OBJECT
    public:
        using Detail::EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };
}

#include "twitchemotereply.inl"
}

#endif // TWITCHEMOTEREPLY_HPP
