#include <QDebug>
#include <QString>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>

#include "audioitem.h"

void AudioItemQmlPlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri);
    Q_ASSERT(uri == QLatin1String("AudioItem"));
    qmlRegisterType<AudioItem>("AudioItem", 1, 1, "AudioItem");
}


AudioItem::AudioItem(QQuickItem *parent)
    :QQuickItem(parent),m_state(StoppedState),m_mpgDecoder(0)
{
    setVolume(100);
    qRegisterMetaType<State>("State");

    m_decoderThread = new QThread(this);

    m_mpgDecoder = new MpgDecoder(4, 0); // it's parent must be 0
    m_mpgDecoder->moveToThread(m_decoderThread);
    connect(this, SIGNAL(playChanged()), m_mpgDecoder, SLOT(startDecode()));
    connect(this, SIGNAL(stopChanged()), m_mpgDecoder, SLOT(stopDecode()));
    connect(this, SIGNAL(pauseChanged()), m_mpgDecoder, SLOT(pauseDecode()));

    connect(m_mpgDecoder, SIGNAL(decStateChanged(DecState)), this, SLOT(setDecState(DecState)));
    connect(this, SIGNAL(audioChanged(const QString &)), m_mpgDecoder, SLOT(setAudio(const QString &)));
    m_decoderThread->start();
}

AudioItem::~AudioItem()
{
    if (!m_mpgDecoder) {
        delete m_mpgDecoder;
        m_mpgDecoder = 0;
    }
}

void AudioItem::setDecState(DecState decState)
{
    switch (decState) {
    case DecStoppedState:
        setState(StoppedState);
        break;
    case DecPlayingState:
        setState(PlayingState);
        break;
    case DecPausedState:
        setState(PausedState);
        break;
    default:
        break;
    }
}

void AudioItem::setState(State state)
{
    if (m_state != state) {
        m_state = state;
        emit stateChanged(m_state);
    }
}

void AudioItem::play()
{
    emit playChanged();
    setState(PlayingState);
}

void AudioItem::stop()
{
    emit stopChanged();
    setState(StoppedState);
}

void AudioItem::pause()
{
    emit pauseChanged();
    setState(PausedState);
}

void AudioItem::play(const QString &audioPath)
{
    stop();
    setAudio(audioPath);
    play();
}

void AudioItem::setAudio(const QString &audioPath)
{
    if (m_audio != audioPath) {
        m_audio = audioPath;
        emit audioChanged(m_audio);
    }
}

void AudioItem::setVolume(int volume)
{
    int fd;
    int devmask, stereodevs;
    int tmpVolume;

    if (volume < 0)
        tmpVolume = 0;
    else if (volume > 100)
        tmpVolume = 100;
    else
        tmpVolume = volume;

    /* 以只读方式打开混音设备 */
    fd = open("/dev/mixer", O_RDONLY);
    if (fd == -1) {
        qDebug() << "unable to open /dev/mixer!";
        return ;
    }

    if (ioctl(fd, SOUND_MIXER_READ_DEVMASK, &devmask) == -1) {
        qDebug() << "SOUND_MIXER_READ_DEVMASK ioctl failed!";
        return ;
    }

    if (ioctl(fd, SOUND_MIXER_READ_STEREODEVS, &stereodevs) == -1) {
        qDebug() << "SOUND_MIXER_READ_STEREODEVS ioctl failed!";
        return ;
    }

    qDebug() << "List all available devices.";

    const char *sound_device_names[] = SOUND_DEVICE_NAMES;
    for (int i = 0 ; i < SOUND_MIXER_NRDEVICES ; i++) {
        if ((1 << i) & devmask) qDebug() << i << ". " << sound_device_names[i] << "\n";
    }

    // 将两个声道的值合到同一变量
    int level = tmpVolume;
    level = (level << 8) + tmpVolume;
    if (ioctl(fd, MIXER_WRITE(13), &level) == -1) {
        qDebug() << "MIXER_WRITE ioctl failed!";
        return ;
    }

    m_volume = tmpVolume;
    emit volumeChanged(m_volume);

    close(fd);
}
