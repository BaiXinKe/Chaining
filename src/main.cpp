#include <iostream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <unistd.h>

#include "main/EventLoop.hpp"
#include "main/InetAddr.hpp"
#include "main/ListenSocket.hpp"

int main(int argc, char** argv)
{
    Chaining::net::EventLoop loop;

    Chaining::net::ListenSocket socket_ { &loop, AF_INET, SOCK_STREAM, 0 };
    Chaining::net::InetAddr addr { "0.0.0.0", 1234 };

    socket_.bind(addr);
    socket_.listen();

    loop.loop();
}