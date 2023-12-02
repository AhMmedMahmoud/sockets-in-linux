#include <iostream>
#include <future>
#include <csignal>
#include "../include/poller.h"
#include "../include/UDPClientServer.h"
#include "../include/concurrent_queue.h"
#include <algorithm>


using namespace POSIXNetworkSocketLib;

const std::string cMulticastGroup{"239.0.0.5"};
const std::string cNicIpAddress{"127.0.0.1"};
const uint16_t cPort{5555};
const int cTimeoutMs = 1;
const std::string cAnyIpAddress{"0.0.0.0"};
const bool cShared{true};

bool running;
Poller *poller;
UdpClientServer *client;
ConcurrentQueue<std::vector<uint8_t>> mSendingQueue;


void send(const std::vector<uint8_t> &value)
{
    mSendingQueue.TryEnqueue(value);
}

void onReceive(void)
{
    std::cout << "\n............. on Receive ............\n";

    const std::size_t cBufferSize = 5;

    std::array<uint8_t, cBufferSize> _receiveBuffer;

    std::string _ipAddress;
    uint16_t _port;
    bool _received = client->Receive(_receiveBuffer, _ipAddress, _port) > 0;

    if (_received && _ipAddress == cNicIpAddress && _port == cPort)
    {
        std::cout << "The server received data from the client.\n";
    }
}

void onSend(void)
{
    while(!mSendingQueue.Empty())
    {
        std::cout << "\n............. on send ............\n";
        
        const std::size_t cBufferSize = 256;
        std::vector<uint8_t> _payload;
        bool _dequeued{mSendingQueue.TryDequeue(_payload)};
        if (_dequeued)
        {
            std::array<uint8_t, cBufferSize> _buffer;
            std::copy_n(
                std::make_move_iterator(_payload.begin()),
                _payload.size(),
                _buffer.begin());

            bool _sent = client->Send(_buffer, cMulticastGroup, cPort) > 0;
            if (_sent)
            {
                std::cout << "The client sent data to the server.\n";
            }
            else
            {
                std::cout << "failed to send after deque the Sending Queue\n";
            }
        }
    }
}

bool trySetup(void)
{
    bool _succeed ;
    _succeed = client->TrySetup();
    if (!_succeed)
    {
        std::cout << "Client setup failed.\n";
        return false;
    }

    _succeed = poller->TryAddSender(client, &onSend);
    if (!_succeed)
    {
        std::cout << "Add client for sending to poller failed.\n";
        return false;
    }

    _succeed = poller->TryAddReceiver(client, &onReceive);
    if (!_succeed)
    {
        std::cout << "Add client for receiving to poller failed.\n";
        return false;
    }

    std::cout << "Socket setups were successful.\n";
    return true;
}

void tester(void)
{
    poller = new Poller();
    client = new UdpClientServer(cAnyIpAddress, cPort, cNicIpAddress, cMulticastGroup, true);
    
    running = trySetup();

    int counter = 0;
    while (running)
    {
        poller->TryPoll(cTimeoutMs);
        std::this_thread::yield();

        if(counter == 0)
        {
            std::vector<uint8_t> data = {1,2,3,4,5};
            send(data);
            counter = 1;
        }
    }

    delete client;
    delete poller;
}

int main()
{
    running = true;
    std::async(tester);
    return 0;
}
