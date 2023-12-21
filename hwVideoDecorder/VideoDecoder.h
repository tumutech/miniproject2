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
#include <thread>
#include <mutex>
#include <condition_variable>

class VideoDecoder {
public:
    VideoDecoder(const std::string& path);
    ~VideoDecoder();

    void Initialize();
    void Play();

private:
    std::string videoPath;
    void OpenInputFile();
    void FindVideoStream();
    void OpenCodec();
    void CreateSDLWindow();
    void CreateSDLRenderer();
    void CreateSDLTexture();
    void DecodeAndRenderFrames();

    AVFormatContext* formatContext;
    AVCodec* codec;
    AVCodecContext* codecContext;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int videoStreamIndex;
};