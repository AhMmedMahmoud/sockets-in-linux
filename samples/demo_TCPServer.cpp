#include <iostream>
#include "../include/TCPServer.h"

using namespace POSIXNetworkSocketLib;

#define clientAndServerInSameMachine 1


int main()
{
    #ifdef clientAndServerInSameMachine
        const std::string IpAddress = "127.0.0.1"; 
    #else
        const std::string cIpAddress = "0.0.0.0";
    #endif

    const uint16_t Port = 9900;

    TcpListener *server;

    server = new TcpListener(IpAddress, Port);

    bool _succeed = server->TrySetup();
    if (!_succeed)
    {
        std::cout << "server side ---> Socket setups failed.\n";
        return -1;
    }
    std::cout << "server side ---> Socket setups were successful.\n";


    // block until client connect
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
            else
            {
                std::cout << "server side ---> The client terminates.\n";
                return -1;
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
