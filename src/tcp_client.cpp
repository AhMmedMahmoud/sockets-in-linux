#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/tcp_client.h"
// for print 
#include <iostream>


namespace AsyncBsdSocketLib
{
    TcpClient::TcpClient(std::string ipAddress, uint16_t port) : NetworkSocket(ipAddress, port),
                                                                 mIsConnected{false}
    {}

    bool TcpClient::TrySetup() noexcept
    {
        FileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        /*
            SOCK_STREAM
               Provides sequenced, reliable, two-way, connection-based
               byte streams (TCP).

            SOCK_DGRAM
               Supports datagrams (connectionless, unreliable messages of
               a fixed maximum length)  (UDP).
        */
        /*
            AF_INET      IPv4 Internet protocols
            AF_INET6     IPv6 Internet protocols
        */

        bool _result = (FileDescriptor >= 0);
        if (_result)
        {
            mAddress.sin_addr.s_addr = inet_addr(IpAddress.c_str());
            mAddress.sin_family = AF_INET;      
            mAddress.sin_port = htons(Port);

            std::cout << "-----------------Socket info--------------\n";
            if(mAddress.sin_family == AF_INET) 
                std::cout << "IPv4 Address" << std::endl;
            else if (mAddress.sin_family == AF_INET6) 
                std::cout << "IPv6 Address" << std::endl;
            else 
                std::cout << "Unknown Address Type" << std::endl;
            std::cout << "IP Address: " << inet_ntoa(mAddress.sin_addr) << std::endl;
            std::cout << "Port: " << ntohs(mAddress.sin_port) << std::endl;
            std::cout << "------------------------------------------\n";
        }
        return _result;
    }

    bool TcpClient::TryConnect() noexcept
    {
        mIsConnected = (  connect(FileDescriptor, (struct sockaddr *)&mAddress, sizeof(mAddress))   == 0);
        return mIsConnected;
    }
    
    int TcpClient::Connection() const noexcept
    {
        return FileDescriptor;
    }

    bool TcpClient::IsConnected() const noexcept
    {
        return mIsConnected;
    }
}