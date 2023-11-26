#include <arpa/inet.h>
#include <fcntl.h>
#include "../include/TCPServerMultipleClient.h"
// for print 
#include <iostream>


namespace POSIXNetworkSocketLib
{
    /******************************* constructor ******************************/

    TcpListenerMultipleClients::TcpListenerMultipleClients(std::string ipAddress, uint16_t port) : TcpListener(ipAddress, port)
    {}


    
    /*********************** overload functions inherited from parent *********/
    
    int counter = 0;
    int TcpListenerMultipleClients::TryAccept() noexcept
    {
        socklen_t _addressLength = sizeof(mAddress);
        int client_fd = accept(FileDescriptor, (struct sockaddr *)&mAddress, &_addressLength);
   
        // // Generate a unique ID for the client
        // clients[counter+1].id = counter+1;
        // clients[counter+1].client_fd = client_fd;

        return client_fd;
    }


    bool TcpListenerMultipleClients::TryClose(int clientFD) noexcept
    {
        // Check the descriptor validity
        if (clientFD == -1)
        {
            return false;
        }

        int _returnCode = close(clientFD);

        bool _result = _returnCode > -1;
        return _result;
    }
}