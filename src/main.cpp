#include <QGuiApplication>

#include <audioitem.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    AudioItem audioItem;

    audioItem.setAudio("/home/root/music.wav");
    audioItem.play();

    return app.exec();
}
