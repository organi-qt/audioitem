#include <QDebug>
#include <QString>
#include <QProcess>

#include "audioitem.h"

AudioItem::AudioItem(QObject *parent)
    :QObject(parent),m_mpgDecoder(0)
{
    setVolume(85);

    m_decoderThread = new QThread(this);

    m_mpgDecoder = new MpgDecoder(4);
    m_mpgDecoder->moveToThread(m_decoderThread);

    connect(this, SIGNAL(playChanged()), m_mpgDecoder, SLOT(startDecode()));
    connect(this, SIGNAL(stopChanged()), m_mpgDecoder, SLOT(stopDecode()));
    connect(this, SIGNAL(audioChanged(const QString &)), m_mpgDecoder, SLOT(setAudio(const QString &)));

    m_decoderThread->start();
}

AudioItem::~AudioItem()
{
    delete m_mpgDecoder;
}

void AudioItem::play()
{
    emit playChanged();
}

void AudioItem::stop()
{
    emit stopChanged();
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
    if (volume < 0)
        m_volume = 0;
    else if (volume > 100)
        m_volume = 100;
    else
        m_volume = volume;

    QString cmd = "amixer cset numid=8,iface=MIXER,name='Playback Volume' ";
    cmd += QString::number(m_volume * 255 / 100); // hardware volume 0 ~ 255

    qDebug() << "Set Volume: " << m_volume * 255 / 100
             << "\nCmd: " << cmd;

    system(cmd.toStdString ().c_str());
    emit volumeChanged(m_volume);
}
