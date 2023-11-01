#include "../include/network_socket.h"

namespace POSIXNetworkSocketLib
{
    NetworkSocket::NetworkSocket(std::string ipAddress, uint16_t port) : IpAddress{ipAddress},
                                                                         Port{port},
                                                                         FileDescriptor{-1}
    {}


    bool NetworkSocket::TryClose() noexcept
    {
        // Check the descriptor validity
        if (FileDescriptor == -1)
        {
            return false;
        }

        int _returnCode = close(FileDescriptor);
        bool _result = _returnCode > -1;

        return _result;
    }

    NetworkSocket::~NetworkSocket() noexcept
    {
        TryClose();
    }
}