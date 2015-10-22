#ifndef AUDIOITEM_H
#define AUDIOITEM_H

#include <QObject>

#include <mad.h>
#include <sys/stat.h>
#include <sys/types.h>

#define AUDIO_DEVICE    "/dev/dsp"
//#define AUDIO_DEVICE    "/home/chenzilin/dsp"

struct buffer {
    unsigned char const *start;
    unsigned long length;
};

class AudioItem : public QObject
{
    Q_OBJECT

public:
    explicit AudioItem(QObject *parent = 0);
    ~AudioItem();

    void setAudio(const QString &audio);
    QString audio() { return m_audio; }

    void play();

private:
    bool openAudioDevice();
    void closeAudioDevice();

    QString m_audio;

    void *m_mmapAddr;
    struct stat m_mmapStat;
};


#endif
