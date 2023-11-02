#include <iostream>
#include "../include/UDPClientServer.h"


using namespace POSIXNetworkSocketLib;

const std::string cAnyIpAddress{"0.0.0.0"};
const std::string cNicIpAddress{"127.0.0.1"};
const std::string cMulticastGroup{"239.0.0.1"};
const uint16_t cPort{5555};


int main()
{

    UdpClient *client;
    client = new UdpClient(cAnyIpAddress, cPort, cNicIpAddress, cMulticastGroup, true);

    bool _succeed = client->TrySetup();
    if (!_succeed)
    {
        std::cout << "Client setup failed.\n";
        return -1;
    }
    std::cout << "Socket setups were successful.\n";
    
    while(1)
    {
        const std::size_t cBufferSize = 5;
        std::array<uint8_t, cBufferSize> _receiveBuffer;

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
    }


    delete client;
    return 0;
}
