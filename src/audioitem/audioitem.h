#ifndef AUDIOITEM_H
#define AUDIOITEM_H

#include <QThread>
#include <QMetaType>
#include <QQuickItem>

#include "mpgdecoder.h"

class AudioItem : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(State)

public:
    explicit AudioItem(QQuickItem *parent = 0);
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
