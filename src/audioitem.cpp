#include <QDebug>

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <sys/ioctl.h>
# include <sys/fcntl.h>
# include <sys/types.h>
# include <sys/soundcard.h>

#include <audioitem.h>

int m_audioFd = -1;

AudioItem::AudioItem(QObject *parent)
    :QObject(parent), m_audio(""), m_mmapAddr(NULL)
{
    if (!openAudioDevice()) return;
}

AudioItem::~AudioItem()
{
    closeAudioDevice();
}

bool AudioItem::openAudioDevice()
{
    if ((m_audioFd = open(AUDIO_DEVICE, O_WRONLY)) < 0) {
        qDebug() << "Can't open audio device!";
        return false;
    }

    return true;
}

void AudioItem::closeAudioDevice()
{
    if (m_audioFd > -1) {
        ioctl(m_audioFd, SNDCTL_DSP_RESET, 0);
        close(m_audioFd);
    }
}

void AudioItem::setAudio(const QString &audio)
{
    int fd;
    if ((fd = open(audio.toStdString().c_str(), O_RDONLY)) < 0) {
        qDebug() << "Can't open audio file!";
        return ;
    }

    memset(&m_mmapStat, 0, sizeof(m_mmapStat));
    if (fstat(fd, &m_mmapStat) == -1 || m_mmapStat.st_size == 0) {
        qDebug() << "Audio file empty or error!";
        return ;
    }

    if ((m_mmapAddr = mmap(0, m_mmapStat.st_size, PROT_READ, MAP_SHARED, fd, 0)) < (void *)0) {
        qDebug() << "Audio file mmap error!";
        return ;
    }

    close(fd);
}

enum mad_flow input(void *data, struct mad_stream *stream)
{
    struct buffer *buffer = (struct buffer *)data;
    if (!buffer->length)
        return MAD_FLOW_STOP;
    mad_stream_buffer(stream, buffer->start, buffer->length);
    buffer->length = 0;
    return MAD_FLOW_CONTINUE;
}

signed int scale(mad_fixed_t sample)
{
    sample += (1L << (MAD_F_FRACBITS - 16));
    if (sample >= MAD_F_ONE)
        sample = MAD_F_ONE - 1;
    else if (sample < -MAD_F_ONE)
        sample = -MAD_F_ONE;
    return sample >> (MAD_F_FRACBITS + 1 - 16);
}

enum mad_flow output(void *data,
                     struct mad_header const *header, struct mad_pcm *pcm)
{
    unsigned int nchannels, nsamples, n;
    mad_fixed_t const *left_ch, *right_ch;
    unsigned char Output[6912], *OutputPtr;
    int fmt, wrote, speed;

    nchannels = pcm->channels;
    n = nsamples = pcm->length;
    left_ch = pcm->samples[0];
    right_ch = pcm->samples[1];

    fmt = AFMT_S16_LE;
    speed = pcm->samplerate * 2; /*播放速度是采样率的两倍 */
    ioctl(m_audioFd, SNDCTL_DSP_SPEED, &(speed));
    ioctl(m_audioFd, SNDCTL_DSP_SETFMT, &fmt);
    ioctl(m_audioFd, SNDCTL_DSP_CHANNELS, &(pcm->channels));
    OutputPtr = Output;
    while (nsamples--) {
        signed int sample;
        sample = scale(*left_ch++);
        *(OutputPtr++) = sample >> 0;
        *(OutputPtr++) = sample >> 8;
        if (nchannels == 2) {
            sample = scale(*right_ch++);
            *(OutputPtr++) = sample >> 0;
            *(OutputPtr++) = sample >> 8;
        }
    }
    n *= 4;         /*数据长度为pcm音频采样的4倍 */
    OutputPtr = Output;
    while (n) {
        wrote = write(m_audioFd, OutputPtr, n);
        OutputPtr += wrote;
        n -= wrote;
    }
    OutputPtr = Output;
    return MAD_FLOW_CONTINUE;
}

enum mad_flow error(void *data,
                    struct mad_stream *stream, struct mad_frame *frame)
{
    return MAD_FLOW_CONTINUE;
}

void AudioItem::play()
{
    struct buffer buffer;
    struct mad_decoder decoder;
    int result;
    buffer.start = (unsigned char const *)m_mmapAddr;
    buffer.length = m_mmapStat.st_size;
    mad_decoder_init(&decoder, &buffer, input, 0, 0, output, error, 0);
    mad_decoder_options(&decoder, 0);
    result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);
    mad_decoder_finish(&decoder);
}
