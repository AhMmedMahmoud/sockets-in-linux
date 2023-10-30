#include <iostream>
#include "../include/tcp_client.h"
// for delay
#include <chrono>
#include <thread>

using namespace AsyncBsdSocketLib;



int main()
{
    const std::string cIpAddress = "127.0.0.1";
    const uint16_t cPort = 9900;

    TcpClient *client;

    client = new TcpClient(cIpAddress, cPort);

    /******** try to setup socket **********/
    bool _succeed = client->TrySetup();
    if (!_succeed)
    {
        std::cout << "client side ---> Socket setups failed.\n";
        return -1;
    }
	std::cout << "client side ---> Socket setups were successful.\n";


    /******** waitting if server hasnot up yet **********/
	_succeed = client->TryConnect();
    while(!_succeed)
    {
        _succeed = client->TryConnect();
    }
    std::cout << "client side ---> The client connected to the server.\n";
    
    while(1)
    {
        const std::size_t cBufferSize = 5;
        const std::array<uint8_t, cBufferSize> cSendBuffer = {0x48, 0x65, 0x6c, 0x6c, 0x6f};

        bool _sent = client->Send(cSendBuffer) > 0;
        if (_sent)
        {
            std::cout << "client side ---> The client sent 'Hello' to the server.\n";
        }
        else
        {
            std::cout << "client side ---> The server declined the client.\n";
            return -1;
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    delete client;    
    return 0;
}
