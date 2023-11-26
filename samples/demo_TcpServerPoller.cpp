#include <iostream>
#include <future>
#include "../include/poller.h"
#include "../include/TCPClient.h"
// for delay
#include <chrono>
#include <thread>

using namespace POSIXNetworkSocketLib;

bool running;
Poller *poller;
TcpListener *server;

void onReceive()
{
    std::cout << "............. on Receive ............\n";
    const std::size_t cBufferSize = 5; 
    std::array<uint8_t, cBufferSize> cReceiveBuffer;
    
    bool _received = server->Receive(cReceiveBuffer) > 0;
    if (_received)
    {
        std::cout << "The server received data from the client.\n\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    else
    {
        std::cout << "receive from OS that socket is closed by client\n";
        std::cout << "server side ---> waitting until client connects\n\n";
    }
}

void onSend()
{
    std::cout << "............. on Send ............\n";
    const std::size_t cBufferSize = 5;
    const std::array<uint8_t, cBufferSize> cSendBuffer = {0x48, 0x65, 0x6c, 0x6c, 0x6f};

    bool _sent = server->Send(cSendBuffer) > 0;
    if (_sent)
    {
        std::cout << "The client sent 'Hello' to the server.\n\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void onAccept()
{
    std::cout << "............. on Accept ............\n";
 
    bool _accepted = server->TryAccept();
    if (_accepted)
    {
        std::cout << "The server accepted the client.\n\n";
        server->TryMakeConnectionNonblock();
        poller->TryAddReceiver(server, &onReceive);
    }
    else
    {
        std::cout << "The server declined the client.\n\n";
        running = false;
    }
}

void tester(const int cTimeoutMs)
{
    std::cout << "server side ---> waitting until client connects.\n\n";
    while (running)
    {
        poller->TryPoll(cTimeoutMs);
    }
}

int main()
{
    const int cTimeoutMs = 1; 
    const std::string cIpAddress = "127.0.0.1";
    const uint16_t cPort = 9900;

    poller = new Poller();
    server = new TcpListener(cIpAddress, cPort);
    running = true;


    bool _succeed = server->TrySetup();
    if (_succeed)
    {
        // regist callback function that will be called whenever tcp client request a connection
        poller->TryAddListener(server, &onAccept);

        // execute tester function in seperated thread and block until it finishes
        std::async(tester, cTimeoutMs);
    }
    else
    {
        std::cout << "Socket setups failed.\n";
    }


    delete server;
    delete poller;
    std::cout << "end of main\n";
    return 0;
}
