// VideoDecoder.h
#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/hwcontext.h>
#include <libavutil/log.h>
#include <libswscale/swscale.h>
}
#include <SDL.h>
#include <SDL_main.h>

class VideoDecoder {
public:
    VideoDecoder(const char* videoPath);
    ~VideoDecoder();

    void Initialize();
    void Play();

private:
    const char* videoPath;
    AVFormatContext* formatContext;
    AVCodec* codec;
    AVCodecContext* codecContext;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int videoStreamIndex;

    void OpenInputFile();
    void FindVideoStream();
    void OpenCodec();
    void CreateSDLWindow();
    void CreateSDLRenderer();
    void CreateSDLTexture();
    void DecodeAndRenderFrames();
};
