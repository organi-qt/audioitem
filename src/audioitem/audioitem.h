#ifndef AUDIOITEM_H
#define AUDIOITEM_H

#include <QThread>
#include <QMetaType>
#include <QQuickItem>
#include <QQmlExtensionPlugin>

#include "mpgdecoder.h"

class AudioItemQmlPlugin: public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

class AudioItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString audio READ audio WRITE setAudio NOTIFY audioChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)

    Q_ENUMS(State)

public:
    explicit AudioItem(QQuickItem *parent = 0);
    ~AudioItem();

    Q_INVOKABLE void play();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void pause();

    void setAudio(const QString &audioPath);
    QString audio() {return m_audio;}

    void setVolume(int volume);
    unsigned volume() {return m_volume;}

    enum State {
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
    void setDecState(DecState decState);

private:
    void setState(State state);

    int m_volume;
    State m_state;

    QThread *m_decoderThread;

    QString m_audio;
    MpgDecoder *m_mpgDecoder;
};

#endif
