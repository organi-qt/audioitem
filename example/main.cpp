#include <QtDebug>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "audio.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Audio audio;
    Q_UNUSED(audio);

    QQuickView viewer;
    viewer.setSource(QUrl("qrc:/main.qml"));
    app.connect(viewer.engine(),&QQmlEngine::quit, &QGuiApplication::quit);
    viewer.show();

    return app.exec();
}
