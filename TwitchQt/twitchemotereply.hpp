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

        virtual Twitch::Emotes toEmotes() = 0;
    };

    class TwitchEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;
        virtual ~TwitchEmotesReply() = default;
        virtual Twitch::Emotes toEmotes() override final;
    };

    class BTTVEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;
        virtual ~BTTVEmotesReply() = default;
        virtual Twitch::Emotes toEmotes() override final;
    };

    class FFZEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;
        virtual ~FFZEmotesReply() = default;
        virtual Twitch::Emotes toEmotes() override final;
    };
}

namespace TwitchEmotes {
    class GlobalEmotesReply : public Detail::TwitchEmotesReply {
        Q_OBJECT
    public:
        using Detail::TwitchEmotesReply::TwitchEmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class SubscriberEmotesReply : public Detail::TwitchEmotesReply {
        Q_OBJECT
    public:
        using Detail::TwitchEmotesReply::TwitchEmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };
}

namespace BTTV {
    class GlobalEmotesReply : public Detail::BTTVEmotesReply {
        Q_OBJECT
    public:
        using Detail::BTTVEmotesReply::BTTVEmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class SubscriberEmotesReply : public Detail::BTTVEmotesReply {
        Q_OBJECT
    public:
        using Detail::BTTVEmotesReply::BTTVEmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };
}

namespace FFZ {
    class GlobalEmotesReply : public Detail::FFZEmotesReply {
        Q_OBJECT
    public:
        using Detail::FFZEmotesReply::FFZEmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class SubscriberEmotesReply : public Detail::FFZEmotesReply {
        Q_OBJECT
    public:
        using Detail::FFZEmotesReply::FFZEmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };
}

#include "twitchemotereply.inl"
}

#endif // TWITCHEMOTEREPLY_HPP
