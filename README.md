# TwitchQt

A simple Header-only Qt wrapper for the new Twitch.tv api(Helix) written using Qt5.

## Requirements
Qt5 and at least C++11 compatible compiler

## Usage
The library is header-only, but it still uses Q_OBJECT macro and other Qt magic so(for now) you'll have to add every header to target headers.
Copy TwitchQt directory to your project directory/whatever place suits you and `include(path/to/TwitchQt.pri)` in your .pro script, you should be fine with this.
In the future there will be a single .hpp version of this library which will eliminate including thingy.

# Example
```cpp
#include <QCoreApplication>
#include <TwitchQt/twitch.hpp>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
	// Twitch::Api is aliased to New Twitch Api (Helix)
	// TODO Twitch::v5 
    Twitch::Api api("CLIENT-ID HERE");

    auto reply = api.getStreamByName("forsen");
    a.connect(reply, &Twitch::Reply::finished, [&a, reply]() {
        if (reply->currentState() == Twitch::ReplyState::Success) {
            // Convert reply data to Stream struct
            auto stream = reply->data().value<Twitch::Stream>();
            qDebug() << stream.m_title; // Print title
            qDebug() << stream.m_viewerCount; // Print viewer count as int
            qDebug() << stream.m_startedAt.date(); // Print date
        }
        // Quit program after receiving reply
        a.quit();
    });

    a.exec();

    return 0;
}

```

## TODO:
- GET Requests:
  **Clips**
  **Game analytics**
  **Bits**
  **Channel emoticons**
  **Streams metadata**
  **?**
* Every request other than GET
* Unit tests
* Maybe webhooks(?)
* Single header version (entire library in one file) 