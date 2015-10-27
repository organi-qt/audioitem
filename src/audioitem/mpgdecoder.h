#ifndef _MPGDECODER_H
#define _MPGDECODER_H

#include <QObject>
#include <QString>
#include <QMetaType>

#include <mpg123.h>
#include <alsa/asoundlib.h>

class MpgDecoder: public QObject
{
    Q_OBJECT

public:
    MpgDecoder(const int &nBuffers = 4, QObject *parent = 0);
    ~MpgDecoder();

    Q_ENUMS(State)

public:
    enum State {    // play state
        StoppedState,
        PlayingState,
        PausedState
    };

public slots:
    void stopDecode();
    void startDecode();
    void pauseDecode(){} // unuseded now
    void setAudio(const QString &audioPath);

signals:
    void stateChanged(State);

private:
    // this function must be private
    void setState(State state);
    State m_state;

    long m_rate;
    int m_channels, m_encoding;
    mpg123_handle *m_hMPG123;

    int m_nBuffers;
    unsigned char** m_buffer;

    snd_pcm_t *m_hPCM;
    int m_outputBufferSize;

    bool initPcmDevice();
    void closePcmDevice();
    int writePcmDevice(unsigned char *buf, int bytes);
};

#endif
