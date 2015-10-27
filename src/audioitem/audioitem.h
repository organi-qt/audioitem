#ifndef AUDIOITEM_H
#define AUDIOITEM_H

#include <QObject>
#include <QThread>
#include <QtPlugin>
#include <QMetaType>
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
    Q_ENUMS(State)

public:
    explicit AudioItem(QObject *parent = 0);
    ~AudioItem();

    void play();
    void stop();
    void pause();

    void setAudio(const QString &audioPath);
    QString audio() {return m_audio;}

    void setVolume(int volume);
    unsigned volume() {return m_volume;}

    enum State {    // play state
        StoppedState,
        PlayingState,
        PausedState
    };
    State state() {return m_state;}

signals:
    void playChanged();
    void stopChanged();
    void pauseChanged();
    void audioChanged(const QString &);
    void volumeChanged(int);
    void stateChanged(State);

public slots:
    void setState(State state);

private:

    int m_volume;
    State m_state;

    QThread *m_decoderThread;

    QString m_audio;
    MpgDecoder *m_mpgDecoder;
};

#endif
