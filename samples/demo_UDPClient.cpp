#include <iostream>
#include "../include/UDPClientServer.h"


using namespace POSIXNetworkSocketLib;

const std::string cAnyIpAddress{"0.0.0.0"};
const std::string cNicIpAddress{"127.0.0.1"};
const std::string cMulticastGroup{"239.0.0.1"};
const uint16_t cPort{5555};

/*
int main()
{

    UdpClientServer *server;
    server = new UdpClientServer(cAnyIpAddress, cPort, cNicIpAddress, cMulticastGroup, true);

    bool _succeed = server->TrySetup();
    if (!_succeed)
    {
        std::cout << "Server setup failed.\n";
        return -1;
    }
    std::cout << "Socket setups were successful.\n";
    

    while(1)
    {
        const std::size_t cBufferSize = 5;
        const std::array<uint8_t, cBufferSize> cSendBuffer = {0x48, 0x65, 0x6c, 0x6c, 0x6f};

        bool _sent = server->Send(cSendBuffer, cMulticastGroup, cPort) > 0;
        if (_sent)
        {
            std::cout << "The server sent 'Hello' to the client.\n";
        }
        else
        {
            std::cout << "server side ---> The server declined the client.\n";
            return -1;
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    delete server;
    return 0;
}
*/
int main()
{
    UdpClientServer *client;
    client = new UdpClientServer(cAnyIpAddress, cPort, cNicIpAddress, cMulticastGroup, true);

    bool _succeed = client->TrySetup();
    if (!_succeed)
    {
        std::cout << "Client setup failed.\n";
        return -1;
    }
    std::cout << "Socket setups were successful.\n";
    

    const std::size_t cBufferSize = 5;
    const std::array<uint8_t, cBufferSize> cSendBuffer = {0x48, 0x65, 0x6c, 0x6c, 0x6f};
    std::array<uint8_t, cBufferSize> _receiveBuffer;

    // sending
    bool _sent = client->Send(cSendBuffer, cMulticastGroup, cPort) > 0;
    if (_sent)
    {
        std::cout << "client side ---> sent 'Hello' to the server.\n";
    }
    else
    {
        std::cout << "client side ---> The server declined the client.\n";
        return -1;
    }

    // receiving
    std::string _ipAddress;
    uint16_t _port;
    bool _received = client->Receive(_receiveBuffer, _ipAddress, _port) > 0;
    if (_received && _ipAddress == cNicIpAddress && _port == cPort)
    {
        std::cout << "The client received data from the client.\n";
    }
    else
    {
        std::cout << "client side ---> The server terminates.\n";
        return -1;
    }


    delete client;
    return 0;
}
