#ifndef AUDIOITEMQMLPLUGIN_H
#define AUDIOITEMQMLPLUGIN_H

#include <QQmlExtensionPlugin>

class AudioItemQmlPlugin: public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char *uri);
};

#endif
