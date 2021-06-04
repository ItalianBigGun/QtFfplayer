
#include <iostream>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

class H264Decode {
	
	bool Init();
	int H264_PutVideoStream(char* buffer, int bufferLen);
	int H264_GetNextVideoFrame(char* buffer, int bufferLen, int yuFormate);
	int H264_GetNextVideoFrame_Rgb(char* buffer, int bufferLen, int width, int height);

public:
    struct SwsContext *_img_convert_ctx;
	AVCodecContext* _pCodecContext;
	AVCodec* _pH264VideoDecoder;
	AVFrame* _pFrameYuv;
	uint8_t* _dst_dataTmp[4];
	int _dst_linesize[4];
	int _out_rgb_buffer_len;
	bool _init, _av_register;
};

bool H264Decode::Init()
{
    if (_init)
        return true;

    _init = true;
    if (!_av_register)
    {
        _av_register = true;
        av_register_all();
    }

    _pCodecContext = avcodec_alloc_context3(NULL);
    _pH264VideoDecoder = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (_pH264VideoDecoder == NULL)
    {
        return false;
    }    

    //初始化参数，下面的参数应该由具体的业务决定  AV_PIX_FMT_YUV420P;
    _pCodecContext->time_base.num = 1;
    _pCodecContext->frame_number = 1; //每包一个视频帧  
    _pCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    _pCodecContext->bit_rate = 0;
    _pCodecContext->time_base.den = 25;//帧率  
    _pCodecContext->width = 0;//视频宽  
    _pCodecContext->height = 0;//视频高 
    _pCodecContext->pix_fmt = AV_PIX_FMT_YUVJ420P;
    _pCodecContext->color_range = AVCOL_RANGE_MPEG;

    if (avcodec_open2(_pCodecContext, _pH264VideoDecoder, NULL) >= 0)
        _pFrameYuv = av_frame_alloc();
    else
        return false;

    return true;
}

int H264Decode::H264_PutVideoStream(char* buffer, int bufferLen)
{
    AVPacket packet = { 0 };
    packet.data = (uint8_t*)buffer;    //这里填入一个指向完整H264数据帧的指针  
    packet.size = bufferLen;        //这个填入H264数据帧的大小  

    int ret = avcodec_send_packet(_pCodecContext, &packet);
    return ret;
}

int H264Decode::H264_GetNextVideoFrame(char* buffer, int bufferLen, int yuFormate)
{
    if (avcodec_receive_frame(_pCodecContext, _pFrameYuv) == 0)
    {
        int height = _pCodecContext->height;
        int width = _pCodecContext->width;

        if (yuFormate == 1)
        {
            ////写入数据  
            int yLen = height * width;
            memcpy(buffer, _pFrameYuv->data[0], yLen);

            int uLen = yLen / 4;
            memcpy(buffer + yLen, _pFrameYuv->data[1], uLen);

            int vLen = uLen;
            memcpy(buffer + yLen + uLen, _pFrameYuv->data[2], vLen);
            return 0;
        }
        else
        {
            ////写入数据  
            int yLen = height * width;
            memcpy(buffer, _pFrameYuv->data[0], yLen);

            int uLen = yLen / 4;
            memcpy(buffer + yLen, _pFrameYuv->data[2], uLen);

            int vLen = uLen;
            memcpy(buffer + yLen + uLen, _pFrameYuv->data[1], vLen);
            return 0;
        }
    }
    return -1;
}

int H264Decode::H264_GetNextVideoFrame_Rgb(char* buffer, int bufferLen, int width, int height)
{
    if (avcodec_receive_frame(_pCodecContext, _pFrameYuv) == 0)
    {
        //ResetRgbScale(width, height);

        int n = (_out_rgb_buffer_len == bufferLen);

        uint8_t * data[3];
        data[0] = _pFrameYuv->data[0];
        data[1] = _pFrameYuv->data[2]; //u v 向量互换
        data[2] = _pFrameYuv->data[1];

        _dst_dataTmp[0] = (uint8_t *)buffer; //少一次复制
        int ret = sws_scale(_img_convert_ctx, (const uint8_t* const*)data, _pFrameYuv->linesize, 0, _pCodecContext->height,
            _dst_dataTmp, _dst_linesize);
        return 0;
    }
    return -1;
}
