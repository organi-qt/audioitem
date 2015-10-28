#include "audioitem.h"
#include "audioitemqmlplugin.h"

void AudioItemQmlPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("AudioItem"));
    qmlRegisterType<AudioItem>("AudioItem", 1, 1, "AudioItem");
}
