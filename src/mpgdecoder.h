#ifndef _MPGDECODER_H
#define _MPGDECODER_H

#include <QObject>
#include <QString>

#include <mpg123.h>
#include <alsa/asoundlib.h>

class MpgDecoder: public QObject
{
    Q_OBJECT

public:
    MpgDecoder(const int &nBuffers = 4, QObject *parent = 0);
    ~MpgDecoder();

public slots:
    void stopDecode();
    void startDecode();
    void setAudio(const QString &audioPath);

private:
    long m_rate;
    int m_channels, m_encoding;
    mpg123_handle *m_hMPG123;

    int m_nBuffers;
    unsigned char** m_buffer;

    snd_pcm_t *m_hPCM;
    int m_outputBufferSize;

    bool m_decode;

    bool initPcmDevice();
    void closePcmDevice();
    int writePcmDevice(unsigned char *buf, int bytes);
};

#endif
