#include <iostream>
#include "../include/tcp_listener.h"

using namespace AsyncBsdSocketLib;


int main()
{
    const std::string cIpAddress = "127.0.0.1";
    const uint16_t cPort = 9900;

    TcpListener *server;

    server = new TcpListener(cIpAddress, cPort);

    bool _succeed = server->TrySetup();
    if (!_succeed)
    {
        std::cout << "server side ---> Socket setups failed.\n";
        return -1;
    }
    std::cout << "server side ---> Socket setups were successful.\n";


    bool _accepted = server->TryAccept();
    if (_accepted)
    {
        std::cout << "server side ---> The server accepted the client.\n";
        
        const std::size_t cBufferSize = 5;

        while(1)
        {
            std::array<uint8_t, cBufferSize> cReceiveBuffer;
            bool _received = server->Receive(cReceiveBuffer) > 0;

            if (_received)
            {
                std::cout << "server side ---> The server received '";
                for(int i = 0; i < cBufferSize; i++)
                {
                    std::cout <<  static_cast<char>(cReceiveBuffer[i]);
                }
                std::cout << "' from the client \n";
            }
        }
    }
    else
    {
        std::cout << "server side ---> The server declined the client.\n";
        return -1;
    }


    delete server;    
    return 0;
}
