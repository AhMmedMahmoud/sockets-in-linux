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
TcpClient *client;


void onReceive()
{
    std::cout << "............. on Receive ............\n";
    const std::size_t cBufferSize = 5; 
    std::array<uint8_t, cBufferSize> cReceiveBuffer;

    bool _received = client->Receive(cReceiveBuffer) > 0;
    if (_received)
    {
        std::cout << "The server received data from the client.\n\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    else
    {
        std::cout << "receive from OS that socket is closed by server\n";
        std::cout << "put running = false\n";
        running = false;
    }
}

void onSend()
{
    std::cout << "............. on Send ............\n";
    const std::size_t cBufferSize = 5;
    const std::array<uint8_t, cBufferSize> cSendBuffer = {0x48, 0x65, 0x6c, 0x6c, 0x6f};

    bool _sent = client->Send(cSendBuffer) > 0;
    if (_sent)
    {
        std::cout << "The client sent 'Hello' to the server.\n\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    else
    {
        std::cout << "put running = false\n";
        running = false;
    }
}



void tester(int cTimeoutMs)
{
    while (running)
    {
        poller->TryPoll(cTimeoutMs);
           
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::this_thread::yield();
    }
}

int main()
{
    const int cTimeoutMs = 1; 
    const std::string cIpAddress = "127.0.0.1";
    const uint16_t cPort = 9900;

    poller = new Poller();
    client = new TcpClient(cIpAddress, cPort);
    running = true;


    bool _succeed = client->TrySetup();
    if (_succeed)
    {
        /******** waitting if server hasnot up yet **********/
        _succeed = client->TryConnect();
        while(!_succeed)
        {
            _succeed = client->TryConnect();
        }
        std::cout << "client side ---> The client connected to the server.\n\n";

        poller->TryAddReceiver(client, &onReceive);
        poller->TryAddSender(client, &onSend);

        // execute tester function in seperated thread and block until it finishes
        std::async(tester, cTimeoutMs);
    }
    else
    {
        std::cout << "Socket setups failed.\n";
    }



    delete client;
    delete poller;
    std::cout << "end of main\n";
    return 0;
}
