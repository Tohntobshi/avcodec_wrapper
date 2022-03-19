#include <tuple>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

class VideoDecoder
{
public:
    VideoDecoder(int width, int  height);
    std::tuple<uint8_t *, int> decode(uint8_t * data, int size);
    ~VideoDecoder();
private:
    AVCodecID codecId = AV_CODEC_ID_MPEG4;
    SwsContext * swsDecCont = nullptr;
    AVCodecContext *deC = nullptr;
    int width = 0;
    int height = 0;
};