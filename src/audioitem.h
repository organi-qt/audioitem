#ifndef AUDIOITEM_H
#define AUDIOITEM_H

#include <QObject>
#include <QThread>
#include <QtPlugin>
#include <qglobal.h>
#include "mpgdecoder.h"

#define QtPluginAudioItem_iid "org.qt-project.Qt.PluginAudioItem"

class AudioItemInterface
{
public:
        virtual ~AudioItemInterface(){}
};

Q_DECLARE_INTERFACE (AudioItemInterface, QtPluginAudioItem_iid);

class Q_DECL_EXPORT AudioItem : public QObject, public AudioItemInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QtPluginAudioItem_iid)
    Q_INTERFACES(AudioItemInterface)

public:
    explicit AudioItem(QObject *parent = 0);
    ~AudioItem();

    void play();
    void stop();

    void setAudio(const QString &audioPath);
    QString audio() {return m_audio;}

    void setVolume(int volume);
    unsigned volume() {return m_volume;}

signals:
    void playChanged();
    void stopChanged();
    void audioChanged(const QString &);
    void volumeChanged(int);

private:

    int m_volume;

    QThread *m_decoderThread;

    QString m_audio;
    MpgDecoder *m_mpgDecoder;
};

#endif
