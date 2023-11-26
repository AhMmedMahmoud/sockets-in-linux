#include <iostream>
#include "../include/TCPServer.h"

using namespace POSIXNetworkSocketLib;

#define clientAndServerInSameMachine 1


int main()
{
    /*********** determine ip address and port number ************/

    #ifdef clientAndServerInSameMachine
        const std::string IpAddress = "127.0.0.1"; 
    #else
        const std::string cIpAddress = "0.0.0.0";
    #endif

    const uint16_t Port = 9900;



    /****** define object of class TcpListener *********/

    TcpListener *server;
    server = new TcpListener(IpAddress, Port);



    /****** try to setup the server *********/

    bool _succeed = server->TrySetup();
    if (!_succeed)
    {
        std::cout << "server side ---> Socket setups failed.\n";
        return -1;
    }
    std::cout << "server side ---> Socket setups were successful.\n";



    /******* prepare buffer for receiving and buffer to hold data to send ********/

    const std::size_t cBufferSizeforSending = 12;
    const std::size_t cBufferSizeforReceiving = 5;
    const std::array<uint8_t, cBufferSizeforSending> cSendBuffer = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74};
    std::array<uint8_t, cBufferSizeforReceiving> cReceiveBuffer;
    


    /********** wait until a client connect ***********/

    bool _accepted = server->TryAccept();
    if (_accepted)
    {
        std::cout << "server side ---> The server accepted the client.\n";
          
        bool _received = server->Receive(cReceiveBuffer) > 0;
        if (_received)
        {
            // print received data and from which client
            std::cout << "server side ---> The server received ";
            for(int i = 0; i < cBufferSizeforReceiving; i++)
            {
                std::cout <<  static_cast<char>(cReceiveBuffer[i]);
            }
            std::cout << " from the client" << std::endl;


            // send data to client
            bool _sent = server->Send(cSendBuffer) > 0;
            if (_sent)
            {
                std::cout << "server side ---> response to the client" << std::endl;
            }
            else
            {
                std::cout << "server side ---> failed to response to the client" << std::endl;
                return -1;
            }
            server->TryClose();
        }
        else
        {
            std::cout << "server side ---> The client terminates.\n";
            return -1;
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

