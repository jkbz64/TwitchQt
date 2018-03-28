#include <QCoreApplication>

#include <twitch.h>
#include <thread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    *Twitch::api = "https://api.twitch.tv/helix/";
    Twitch::setClientID(QString("hqq61vsp32mvxsfhy6a9o7eemxdv5e"));

    auto reply = Twitch::getTopGames();

    a.exec();

    return 0;
}
