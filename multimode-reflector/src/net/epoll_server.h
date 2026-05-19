#pragma once

class EpollServer {
public:
    void init(int port);
    void tick();
};
