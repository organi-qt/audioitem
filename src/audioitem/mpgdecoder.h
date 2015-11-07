#ifndef MPGDECODER_H
#define MPGDECODER_H

#include <QObject>
#include <QString>
#include <QMetaType>

#include <mpg123.h>
#include <alsa/asoundlib.h>

enum DecState {
    DecStoppedState,
    DecPlayingState,
    DecPausedState
};

static void qRegisterMediaPlayerMetaTypes()
{
    qRegisterMetaType<DecState>("DecState");
}
Q_CONSTRUCTOR_FUNCTION(qRegisterMediaPlayerMetaTypes)


class MpgDecoder: public QObject
{
    Q_OBJECT

public:
    MpgDecoder(const int &nBuffers = 4, QObject *parent = 0);
    ~MpgDecoder();

public slots:
    void stopDecode();
    void startDecode();
    void pauseDecode();
    void setAudio(const QString &audio);

signals:
    void audioChanged(QString);
    void decStateChanged(DecState);

private:
    // this function must be private
    void decode();

    // this function must be private
    void setDecState(DecState state);
    DecState m_DecState;

    long m_rate;
    int m_channels, m_encoding;
    mpg123_handle *m_hMPG123;

    int m_nBuffers;
    unsigned char** m_buffer;

    snd_pcm_t *m_hPCM;
    int m_outputBufferSize;

    QString m_audio;

    void initMpg123();
    void closeMpg123();

    void initPcmDevice();
    void closePcmDevice();
    int writePcmDevice(unsigned char *buf, int bytes);
};

#endif
