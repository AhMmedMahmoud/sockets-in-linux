#include <iostream>
#include <future>
#include <csignal>
#include "../include/poller.h"
#include "../include/UDPClientServer.h"

using namespace POSIXNetworkSocketLib;

const int cTimeoutMs = 1;
const std::string cAnyIpAddress{"0.0.0.0"};
const std::string cMulticastGroup{"239.0.0.1"};
const std::string cNicIpAddress{"127.0.0.1"};
const uint16_t cPort{5555};

bool running;
Poller *poller;
UdpClientServer *server;


void onReceive(void)
{
    std::cout << "\n............. on receive ............\n";

    const std::size_t cBufferSize = 5;

    std::array<uint8_t, cBufferSize> _receiveBuffer;

    std::string _ipAddress;
    uint16_t _port;
    bool _received = server->Receive(_receiveBuffer, _ipAddress, _port) > 0;

    if (_received && _ipAddress == cNicIpAddress && _port == cPort)
    {
        std::cout << "The server received data from the client.\n";
    }
}


bool trySetup(void)
{
    bool _succeed = server->TrySetup();
    if (!_succeed)
    {
        std::cout << "Server setup failed.\n";
        return false;
    }

    _succeed = poller->TryAddReceiver(server, &onReceive);
    if (!_succeed)
    {
        std::cout << "Add server to poller failed.\n";
        return false;
    }

    std::cout << "Socket setups were successful.\n";
    return true;
}

void tester(void)
{
    poller = new Poller();
    server = new UdpClientServer(cAnyIpAddress, cPort, cNicIpAddress, cMulticastGroup, true);

    running = trySetup();

    while (running)
    {
        poller->TryPoll(cTimeoutMs);
        std::this_thread::yield();
    }

    delete server;
    delete poller;
}

int main()
{
    running = true;
    std::async(tester);
    return 0;
}
