#include <QDebug>

#include "audio.h"

Audio::Audio(QObject *parent)
    : QObject(parent)
{
    m_audioItem = new AudioItem;

    m_audioItem->setAudio("/home/root/music.mp3");
    m_audioItem->play();
}


