#ifndef TWITCHEMOTEREPLY_HPP
#define TWITCHEMOTEREPLY_HPP

#include "twitchreply.hpp"
#include "twitchemote.hpp"
#include <QImage>

namespace Twitch {
namespace TwitchEmotes {
    class EmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;
        Twitch::Emotes toEmotes();

    protected:
        virtual void parseData(const QJsonDocument&) override;
    };
}

namespace BTTV {
    class EmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;
        Twitch::Emotes toEmotes();

    protected:
        virtual void parseData(const QJsonDocument&) override;
    };
}

namespace FFZ {
    class EmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;
        Twitch::Emotes toEmotes();

    protected:
        virtual void parseData(const QJsonDocument&) override;
    };
}

class EmoteImageReply : public RawReply {
    Q_OBJECT
public:
    using RawReply::RawReply;

protected:
    virtual void parseData(const QByteArray&) override;
};

#include "twitchemotereply.inl"
}

#endif // TWITCHEMOTEREPLY_HPP
