#ifndef TWITCHEMOTEREPLY_HPP
#define TWITCHEMOTEREPLY_HPP

#include "twitchreply.hpp"
#include "twitchemote.hpp"

namespace Twitch {
class EmotesReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

protected:
    virtual void parseData(const QJsonDocument&) override;
};

namespace BTTV {
    class EmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;

    protected:
        virtual void parseData(const QJsonDocument&) override;
    };
}

namespace FFZ {
    class EmotesReply : public JSONReply {
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
