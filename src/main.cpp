#include <audioitem.h>

int main()
{
    AudioItem audioItem;

    audioItem.setAudio("/home/root/music.mp3");

    audioItem.play();
    return 0;
}
