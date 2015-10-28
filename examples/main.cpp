#include <QtDebug>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView viewer;
    viewer.setSource(QUrl("qrc:/main.qml"));
    app.connect(viewer.engine(),&QQmlEngine::quit, &QGuiApplication::quit);
    viewer.show();

    return app.exec();
}
