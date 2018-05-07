#ifndef TWITCHEMOTEREPLY_HPP
#define TWITCHEMOTEREPLY_HPP

#include "twitchreply.hpp"
#include "twitchemote.hpp"

namespace Twitch {
namespace TwitchEmotes {
    class GlobalEmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;
        Twitch::Emotes toEmotes();

    protected:
        virtual void parseData(const QJsonDocument&) override;
    };

    class SubscriberEmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;

    protected:
        virtual void parseData(const QJsonDocument&) override;
    };
}

namespace BTTV {
    class GlobalEmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;
        Twitch::Emotes toEmotes();

    protected:
        virtual void parseData(const QJsonDocument&) override;
    };

    class SubscriberEmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;

    protected:
        virtual void parseData(const QJsonDocument&) override;
    };
}

namespace FFZ {
    class GlobalEmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;
        Twitch::Emotes toEmotes();

    protected:
        virtual void parseData(const QJsonDocument&) override;
    };

    class SubscriberEmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;

    protected:
        virtual void parseData(const QJsonDocument&) override;
    };
}

#include "twitchemotereply.inl"
}

#endif // TWITCHEMOTEREPLY_HPP
