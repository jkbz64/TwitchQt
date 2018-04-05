# TwitchQt

A simple Header-only Qt wrapper for the new Twitch.tv api(Helix) written using Qt5.

## Requirements
Qt5 and at least C++11 compatible compiler

## Usage
It's header only, just include the twitch.hpp header.

# Example
```cpp
#include <QCoreApplication>
#include <TwitchQt/twitch.hpp>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    Twitch::setClientID(QString("INSERT CLIENT-ID HERE"));

    auto reply = Twitch::getStreamByName(QString("sodapoppin"));
    a.connect(reply, &Twitch::Reply::finished, [&reply]() {
        if (reply->currentState() == Twitch::Reply::ReplyState::Success) {
            auto stream = reply->data().value<Twitch::Stream>();
            qDebug() << stream.m_title;
            qDebug() << stream.m_viewerCount;
            qDebug() << stream.m_startedAt.date();
        }
    });
    a.exec();

    return 0;
}
```

## TODO:
- Get Requests 
  **Clips**
  **Game analytics**
  **Bits**
  **Channel emoticons**
  **Streams metadata**
  **?**
* "Set requests" < only Get requests implemented so far
* Unit tests
* Maybe webhooks(?)
* Single header version (entire library in one file) 