#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>

#include <AudioItem/audioitem.h>

class Audio: public QObject
{
    Q_OBJECT

public:
    Audio(QObject *parent = 0);
    ~Audio(){}

private:

    AudioItem *m_audioItem;
};

#endif
