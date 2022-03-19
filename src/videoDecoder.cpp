#include "videoDecoder.h"
#include <iostream>

VideoDecoder::VideoDecoder(int w, int h):
width(w),
height(h)
{
    AVCodec *codec = avcodec_find_decoder(codecId);
    deC = avcodec_alloc_context3(codec);
    swsDecCont = sws_getContext(width, height, AV_PIX_FMT_YUV420P, width, height, AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
    deC->width = width;
    deC->height = height;
    deC->pix_fmt = AV_PIX_FMT_YUV420P;
    if (avcodec_open2(deC, codec, nullptr) < 0)
    {
        std::cout << "cannot open codec\n";
    }
}

VideoDecoder::~VideoDecoder()
{
    sws_freeContext(swsDecCont);
    avcodec_free_context(&deC);
}

std::tuple<uint8_t *, int> VideoDecoder::decode(uint8_t * data, int size)
{
    AVPacket *pak = av_packet_alloc();
    pak->size = size;
    pak->data = data;
    int spRes = avcodec_send_packet(deC, pak);
    if (spRes != 0)
    {
        if (spRes == AVERROR(EAGAIN))
        {
            std::cout << "send packet eagain\n";
        }
        else
        {
            std::cout << "send packet error\n";
        }
    }
    av_packet_free(&pak);

    AVFrame *dcFrame = av_frame_alloc();
    if (avcodec_receive_frame(deC, dcFrame) == 0)
    {
        AVFrame *destDcFrame = av_frame_alloc();
        destDcFrame->width = width;
        destDcFrame->height = height;
        destDcFrame->format = AV_PIX_FMT_BGR24;
        av_image_alloc(destDcFrame->data, destDcFrame->linesize, width, height, AV_PIX_FMT_BGR24, 1);
        sws_scale(swsDecCont, dcFrame->data, dcFrame->linesize, 0, height, destDcFrame->data, destDcFrame->linesize);
        uint32_t bufSize = av_image_get_buffer_size(AV_PIX_FMT_BGR24, width, height, 1);
        uint8_t *decodedData = new uint8_t[bufSize];
        av_image_copy_to_buffer(decodedData, bufSize, destDcFrame->data, destDcFrame->linesize, AV_PIX_FMT_BGR24, width, height, 1);
        av_freep(&destDcFrame->data[0]);
        av_frame_free(&dcFrame);
        av_frame_free(&destDcFrame);
        return { decodedData, bufSize };
    }
    av_frame_free(&dcFrame);
    return { nullptr, 0 };
}