#pragma once

class MediaOutputWorker {
public:

    static void start();

    static void stop();

private:

    static void run();

    static bool m_running;
};
