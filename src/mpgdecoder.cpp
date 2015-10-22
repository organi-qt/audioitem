#include <QDebug>

#include "mpgdecoder.h"

static const struct {
    snd_pcm_format_t alsa;
    int mpg123;
} format_map[] = {
    { SND_PCM_FORMAT_S16,    MPG123_ENC_SIGNED_16   },
    { SND_PCM_FORMAT_U16,    MPG123_ENC_UNSIGNED_16 },
    { SND_PCM_FORMAT_U8,     MPG123_ENC_UNSIGNED_8  },
    { SND_PCM_FORMAT_S8,     MPG123_ENC_SIGNED_8    },
    { SND_PCM_FORMAT_A_LAW,  MPG123_ENC_ALAW_8      },
    { SND_PCM_FORMAT_MU_LAW, MPG123_ENC_ULAW_8      },
    { SND_PCM_FORMAT_S32,    MPG123_ENC_SIGNED_32   },
    { SND_PCM_FORMAT_U32,    MPG123_ENC_UNSIGNED_32 },
    { SND_PCM_FORMAT_FLOAT,  MPG123_ENC_FLOAT_32    },
    { SND_PCM_FORMAT_FLOAT64, MPG123_ENC_FLOAT_64   }
};
#define NUM_FORMATS (sizeof format_map / sizeof format_map[0])


MpgDecoder::MpgDecoder(const int &nBuffers, QObject *parent)
    :QObject(parent), m_nBuffers(nBuffers)
{
    m_hMPG123 = 0;

    m_outputBufferSize = 0;
    m_rate = m_channels = m_encoding = 0;

    m_buffer = new unsigned char*[nBuffers];
}

MpgDecoder::~MpgDecoder()
{
    if (m_hMPG123) {
        mpg123_delete(m_hMPG123);

        m_hMPG123 = 0;
    }

    mpg123_exit();

    delete [] m_buffer;
}

void MpgDecoder::setAudio(const QString &audioPath)
{
    if (mpg123_init() != MPG123_OK) {
        qDebug() << "mpg123 init error!\n";
        return ;
    }

    if ((m_hMPG123 = mpg123_new("NEON", NULL)) == NULL) {
        qDebug() << "mpg123 new error!\n";
        return ;
    }

    m_outputBufferSize = mpg123_safe_buffer();
    mpg123_param(m_hMPG123, MPG123_FORCE_RATE, 44100, 0);
    mpg123_param(m_hMPG123, MPG123_FLAGS, MPG123_FORCE_STEREO, 0);

    if (mpg123_open(m_hMPG123, audioPath.toStdString().c_str()) != MPG123_OK) {
        qDebug() << "mpg123 open error!\n";
        return ;
    }

    mpg123_getformat(m_hMPG123, &m_rate, &m_channels, &m_encoding);
    qDebug() << "\nMP3 stream's rate: " << m_rate
         << "HZ, Channels: " << m_channels
         << ", Encoding: " << m_encoding;

    initPcmDevice();
}

void MpgDecoder::startDecode()
{
    if (!m_hMPG123) return ;

    m_decode = true;

    for (int i = 0; i < m_nBuffers; ++i)
        m_buffer[i] = new unsigned char[m_outputBufferSize];

    int bytes, buffer_size;
    size_t decoded_bytes;
    for (int index = 0; m_decode; /*empty*/)
    {
        snd_pcm_wait(m_hPCM, 1000);

        bytes = snd_pcm_frames_to_bytes(m_hPCM, snd_pcm_avail_update(m_hPCM));
        buffer_size = bytes > m_outputBufferSize ? m_outputBufferSize : bytes;

        mpg123_read(m_hMPG123, m_buffer[index], buffer_size, &decoded_bytes);

        if(decoded_bytes == 0){
            m_decode = false;
            return ;
        }

        writePcmDevice(m_buffer[index], decoded_bytes);

        if (++index >= m_nBuffers) index = 0;
    }
}

void MpgDecoder::stopDecode()
{
    m_decode = false;

    closePcmDevice();

    for (int i = 0; i < m_nBuffers; ++i)
        delete [] m_buffer[i];
}

bool MpgDecoder::initPcmDevice()
{
    snd_pcm_uframes_t buffer_size;
    snd_pcm_uframes_t period_size;

    snd_pcm_hw_params_t *hw = NULL;
    snd_pcm_hw_params_alloca(&hw);

    snd_pcm_sw_params_t *sw = NULL;
    snd_pcm_sw_params_alloca(&sw);

    if (snd_pcm_open(&m_hPCM, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        qDebug() << "initPcmDevice: cannnot open pcm\n";
        return false;
    }

    if (snd_pcm_hw_params_any(m_hPCM, hw) < 0) {
        qDebug() << "initPcmDevice: no configuration available\n";
        return false;
    }

    if (snd_pcm_hw_params_set_access(m_hPCM, hw, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
        qDebug() << "initPcmDevice: device does not support interleaved access\n";
        return false;
    }

    int format = m_encoding;
    for (unsigned i = 0; i < NUM_FORMATS; ++i) {
        if (format == format_map[i].mpg123) {
            format = format_map[i].alsa;
            break;
        }
    }

    if (format == SND_PCM_FORMAT_UNKNOWN) {
        qDebug() << "initPcmDevice: invalid sample format " << format;
        return false;
    }

    if (snd_pcm_hw_params_set_format(m_hPCM, hw, (snd_pcm_format_t)format) < 0) {
        qDebug() << "initPcmDevice: cannot set format " << snd_pcm_format_name((snd_pcm_format_t)format) << "\n";
        return false;
    }
    qDebug() << "SND_PCM format: " << snd_pcm_format_name((snd_pcm_format_t)format);

    if (snd_pcm_hw_params_set_channels(m_hPCM, hw, m_channels) < 0) {
        qDebug() << "initPcmDevice: cannot set " << m_channels << "channels";
        return false;
    }
    qDebug() << "SND_PCM channles: " << m_channels;

    unsigned rate = m_rate;
    if (snd_pcm_hw_params_set_rate_near(m_hPCM, hw, &rate, NULL) < 0) {
        qDebug() << "initPcmDevice: cannot set rate " << rate;
        return false;
    }

    if ((long)rate != m_rate) {
        qDebug() << "initPcmDevice: rate " << m_rate << " not available!";
        return false;
    }
    qDebug() << "SND_PCM sampling rate: " << m_rate;

    buffer_size = m_outputBufferSize;
    if (snd_pcm_hw_params_set_buffer_size_near(m_hPCM, hw, &buffer_size) < 0) {
        qDebug() << "initPcmDevice: cannot set buffer size";
        return false;
    }
    qDebug() << "SND_PCM buffer size: " << buffer_size;

    period_size = buffer_size / 4;
    if (snd_pcm_hw_params_set_period_size_near(m_hPCM, hw, &period_size, NULL) < 0) {
        qDebug() << "initPcmDevice: cannot set period size";
        return false;
    }
    qDebug() << "SND_PCM period size: " << period_size;

    if (snd_pcm_hw_params(m_hPCM, hw) < 0) {
        qDebug() << "initPcmDevice: cannot set hw params";
        return false;
    }

    if (snd_pcm_sw_params_current(m_hPCM, sw) < 0) {
        qDebug() << "initPcmDevice: cannot get sw params";
        return false;
    }

    if (snd_pcm_sw_params_set_start_threshold(m_hPCM, sw, 0) < 0) {
        qDebug() << "initPcmDevice: cannot set start threshold";
        return false;
    }

    if (snd_pcm_sw_params_set_avail_min(m_hPCM, sw, 2048) < 0) {
        qDebug() << "initPcmDevice: cannot set min available";
        return false;
    }

    if (snd_pcm_sw_params(m_hPCM, sw) < 0) {
        qDebug() << "initPcmDevice: cannot set sw params";
        return false;
    }

    return true;
}

int MpgDecoder::writePcmDevice(unsigned char *buf, int bytes)
{
    snd_pcm_uframes_t frames;
    snd_pcm_sframes_t written;

    frames = snd_pcm_bytes_to_frames(m_hPCM, bytes);
    written = snd_pcm_writei(m_hPCM, buf, frames);

    if (written == -EINTR) written = 0;
    else
        if (written == -EPIPE) {
            if (snd_pcm_prepare(m_hPCM) >= 0)
                written = snd_pcm_writei(m_hPCM, buf, frames);
        }

    if (written >= 0)
        return snd_pcm_frames_to_bytes(m_hPCM, written);
    else {
        if (snd_pcm_state(m_hPCM) == SND_PCM_STATE_SUSPENDED) {
            snd_pcm_resume(m_hPCM);
            if (snd_pcm_state(m_hPCM) == SND_PCM_STATE_SUSPENDED)
                return -1;
        }
        return 0;
    }
}

void MpgDecoder::closePcmDevice()
{
    snd_pcm_drop(m_hPCM);
    snd_pcm_prepare(m_hPCM);

    if (m_hPCM != NULL)
    {
        if (snd_pcm_state(m_hPCM) == SND_PCM_STATE_RUNNING)
            snd_pcm_drain(m_hPCM);
        snd_pcm_close(m_hPCM);
    }
}
