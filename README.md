# TwitchQt

A simple Header-only Qt wrapper for the new Twitch.tv api(Helix) written using Qt5.

## Requirements
#### Qt5
- Core
- Network
#### C++11 compatible compiler
- [JSON for Modern C++ (already included)](https://github.com/nlohmann/json) 

You might ask - why external JSON while there's Qt's JSON library included in Qt Core? Well... it simply sucks, but the main problem is that it cannot parse documents bigger than 2^27 bytes (~135MB).

## Usage
Clone this library wherever you want and just `include(path/to/TwitchQt.pri)` in your .pro file. 

There will be (hopefully) a single .hpp version of this library which will make things easier in the future.

## Example
```cpp
#include <QCoreApplication>
#include <TwitchQt/Twitch>

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

## Features
Look at the [twitchapi.hpp](https://github.com/jkbz64/TwitchQt/blob/master/TwitchQt/twitchapi.hpp) file :)

## TODO:
- GET Requests:
  **Clips**
  **Game analytics**
  **Bits**
  **Channel emoticons**
  **Streams metadata**
  **?**
* Every request other than GET OMEGA
* Unit tests
* Maybe webhooks(?)
* Single header version (entire library in one file) 
