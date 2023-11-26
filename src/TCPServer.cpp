#include <arpa/inet.h>
#include <fcntl.h>
#include "../include/TCPServer.h"
// for print 
#include <iostream>


namespace POSIXNetworkSocketLib
{
    /******************************* constructor ******************************/

    TcpListener::TcpListener(std::string ipAddress, uint16_t port) : NetworkSocket(ipAddress, port),
                                                                     mConnection{-1}
    {}

    

    /*********************** override functions inherited from parent *********/

    bool TcpListener::TrySetup() noexcept
    {
        //Creating socket file descriptor
        FileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

        bool _result = (FileDescriptor >= 0);
        if (_result)
        {
            // Option value
            int _opt = 1;
            // Configuring socket option for reusing the port and the address
            // combine two options: SO_REUSEADDR and SO_REUSEPORT. These options allow the reuse of local addresses and ports.
            // setsockopt allows you to set various options for a socket. 
            _result = (  setsockopt(FileDescriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)) == 0  );
            if (_result)
            {
                mAddress.sin_addr.s_addr = inet_addr(IpAddress.c_str());
                mAddress.sin_family = AF_INET;
                mAddress.sin_port = htons(Port);

                std::cout << "-----------------Socket info--------------\n";
                std::cout << "FD: " << FileDescriptor << std::endl;
                if(mAddress.sin_family == AF_INET) 
                    std::cout << "IPv4 Address" << std::endl;
                else if (mAddress.sin_family == AF_INET6) 
                    std::cout << "IPv6 Address" << std::endl;
                else 
                    std::cout << "Unknown Address Type" << std::endl;
                std::cout << "IP Address: " << inet_ntoa(mAddress.sin_addr) << std::endl;
                std::cout << "Port: " << ntohs(mAddress.sin_port) << std::endl;
                std::cout << "------------------------------------------\n";

                // Binding the socket to the port
                _result = (  bind(FileDescriptor, (struct sockaddr *)&mAddress, sizeof(mAddress)) == 0  );
                if (_result)
                {
                    // cBacklog = 3
                    _result = (  listen(FileDescriptor, 3) == 0  );
                }
            }
        }

        return _result;
    }

    int TcpListener::Connection() const noexcept
    {
        return mConnection;
    }



    /**************************** fundemental functions *************************/

    bool TcpListener::TryAccept() noexcept
    {
        int _addressLength = sizeof(mAddress);        
        mConnection = accept(FileDescriptor, (struct sockaddr *)&mAddress, (socklen_t *)&_addressLength);

        bool _result = (mConnection >= 0);
        return _result;
    }


    bool TcpListener::TryMakeConnectionNonblock() noexcept
    {
        // Check the connection validity
        if (mConnection == -1)
        {
            return false;
        }

        // Get the current flags
        int _flags = fcntl(mConnection, F_GETFL, 0);
        if (_flags == -1)
        {
            return false;
        }

        // Add the non-blocking flag
        _flags |= O_NONBLOCK;

        // Set back the flags
        int _returnCode = fcntl(mConnection, F_SETFL, _flags);
        bool _result = (_returnCode != -1);

        return _result;
    }
}