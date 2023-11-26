#include <iostream>
#include "../include/TCPClient.h"


using namespace POSIXNetworkSocketLib;

#define clientAndServerInSameMachine 1

int main()
{
    /*********** determine ip address and port number ************/

    #if clientAndServerInSameMachine
        const std::string IpAddress = "127.0.0.1";
    #else
        const std::string cIpAddress = "192.168.36.116";   // ip address of server  (using "hostname -I" command in terminal to get it)
    #endif

    const uint16_t Port = 9900;

    

    /****** define object of class TcpClient *********/

    TcpClient *client;
    client = new TcpClient(IpAddress, Port);



    /*********** try to setup socket ************/

    bool _succeed = client->TrySetup();
    if (!_succeed)
    {
        std::cout << "client side ---> Socket setups failed.\n";
        return -1;
    }
	std::cout << "client side ---> Socket setups were successful.\n";



    /********** waitting if server hasnot up yet ************/

	_succeed = client->TryConnect();
    while(!_succeed)
    {
        _succeed = client->TryConnect();
    }
    std::cout << "client side ---> The client connected to the server\n\n";



    /******* prepare buffer for receiving and buffer to hold data to send ********/

    const std::size_t cBufferSizeforSending = 5;
    const std::size_t cBufferSizeforReceiving = 12;

    const std::array<uint8_t, cBufferSizeforSending> cSendBuffer = {0x48, 0x65, 0x6c, 0x6c, 0x6f};
    std::array<uint8_t, cBufferSizeforReceiving> cReceiveBuffer;
    


    /********** sending message as request **********/

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



    /********** receiving message as reply **********/

    bool _received = client->Receive(cReceiveBuffer) > 0;
    if (_received)
    {
        // print received data and from which client
        std::cout << "client side ---> The client received ";
        for(int i = 0; i < cBufferSizeforReceiving; i++)
        {
            std::cout <<  static_cast<char>(cReceiveBuffer[i]);
        }
        std::cout << " from the server" << std::endl;
    }
    else
    {
        std::cout << "client side ---> The server declined the client.\n";
        return -1;
    }

    delete client;    
    return 0;
}
