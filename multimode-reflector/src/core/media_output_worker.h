#pragma once

class MediaOutputWorker {
public:

    static void start(
        int idleTimeoutMs,
        int maxTxMs);

    static void stop();

private:

    static void run();

    static bool m_running;
    static int m_idleTimeoutMs;
    static int m_maxTxMs;
};
