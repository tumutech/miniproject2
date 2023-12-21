// VideoDecoder.cpp
#include "pch.h"
#include "framework.h"
#include "VideoDecoder.h"
#include <stdexcept>
VideoDecoder::VideoDecoder(const std::string& path)
    : videoPath(path), formatContext(nullptr), codec(nullptr),
    codecContext(nullptr), window(nullptr), renderer(nullptr),
    texture(nullptr), videoStreamIndex(-1) {}

VideoDecoder::~VideoDecoder() {
    av_frame_free(nullptr);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);

    if (texture)
        SDL_DestroyTexture(texture);

    if (renderer)
        SDL_DestroyRenderer(renderer);

    if (window)
        SDL_DestroyWindow(window);

    SDL_Quit();
}

void VideoDecoder::Initialize() {
    avformat_network_init();
    OpenInputFile();
    FindVideoStream();
    OpenCodec();
    CreateSDLWindow();
    CreateSDLRenderer();
    CreateSDLTexture();
}

void VideoDecoder::Play() {
    DecodeAndRenderFrames();
}

void VideoDecoder::OpenInputFile() {
    if (avformat_open_input(&formatContext, videoPath.c_str(), nullptr, nullptr) < 0)
        throw std::runtime_error("Could not open the file");
}

void VideoDecoder::FindVideoStream() {
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1)
        throw std::runtime_error("No video stream found in the input file");
}

void VideoDecoder::OpenCodec() {
    codec = avcodec_find_decoder_by_name("h264_qsv");
    if (!codec)
        throw std::runtime_error("Hardware decoder not found");

    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext)
        throw std::runtime_error("Failed to allocate codec context");

    if (avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar) < 0)
        throw std::runtime_error("Failed to set codec parameters");

    if (avcodec_open2(codecContext, codec, nullptr) < 0)
        throw std::runtime_error("Failed to open codec with hardware acceleration");

    av_log(nullptr, AV_LOG_INFO, "Decoder: %s\n", codecContext->codec->name);
}

void VideoDecoder::CreateSDLWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL initialization failed: " + std::string(SDL_GetError()));

    window = SDL_CreateWindow("Decoded Video", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        codecContext->width, codecContext->height, SDL_WINDOW_SHOWN);
    if (!window)
        throw std::runtime_error("SDL window creation failed: " + std::string(SDL_GetError()));
}

void VideoDecoder::CreateSDLRenderer() {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
        throw std::runtime_error("SDL renderer creation failed: " + std::string(SDL_GetError()));

    // Set logical resolution
    SDL_RenderSetLogicalSize(renderer, codecContext->width, codecContext->height);
}


void VideoDecoder::CreateSDLTexture() {
    // Use the pixel format and dimensions of the decoded video frame
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING,
        codecContext->width, codecContext->height);
    if (!texture)
        throw std::runtime_error("SDL texture creation failed: " + std::string(SDL_GetError()));
}


void VideoDecoder::DecodeAndRenderFrames() {
    AVPacket packet;
    av_init_packet(&packet);
    packet.data = nullptr;
    packet.size = 0;

    AVFrame* decodedFrame = av_frame_alloc();

    while (av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == videoStreamIndex) {
            avcodec_send_packet(codecContext, &packet);
            while (avcodec_receive_frame(codecContext, decodedFrame) == 0) {
                // Update SDL texture
                SDL_UpdateTexture(texture, nullptr, decodedFrame->data[0], decodedFrame->linesize[0]);

                // Clear the renderer
                SDL_RenderClear(renderer);

                // Calculate destination rectangle to maintain aspect ratio
                SDL_Rect destRect;
                destRect.x = 0;
                destRect.y = 0;

                float aspectRatio = static_cast<float>(decodedFrame->width) / static_cast<float>(decodedFrame->height);
                destRect.w = static_cast<int>(codecContext->width);
                destRect.h = static_cast<int>(codecContext->width / aspectRatio);

                // Render the texture to the calculated destination rectangle
                SDL_RenderCopy(renderer, texture, nullptr, &destRect);

                // Update the renderer
                SDL_RenderPresent(renderer);

                // Delay to control frame rate (adjust as needed)
                SDL_Delay(16);
            }
        }
        av_packet_unref(&packet);

        // Check for SDL_QUIT events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return;
            }
        }
    }

    av_frame_free(&decodedFrame);
}
