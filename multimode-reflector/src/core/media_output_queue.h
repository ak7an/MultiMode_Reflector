#pragma once

#include "media_frame.h"

#include <mutex>
#include <queue>

class MediaOutputQueue {
public:

    static void push(
        const MediaFrame& frame);

    static bool pop(
        MediaFrame& frame);

    static size_t size();

private:

    static std::queue<MediaFrame> m_queue;
    static std::mutex m_mutex;
};
