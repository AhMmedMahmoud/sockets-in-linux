#include "../include/network_socket.h"

namespace POSIXNetworkSocketLib
{
    /***************** useful for constructor of my child **********************/

    NetworkSocket::NetworkSocket(std::string ipAddress, uint16_t port) : IpAddress{ipAddress},
                                                                         Port{port},
                                                                         FileDescriptor{-1}
    {}



    /********************************** getters ******************************/

    int NetworkSocket::Descriptor() const noexcept
    {
        return FileDescriptor;
    }



    /**************************** fundemental functions *******************/

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


    
    /***********************  virtual deconstructor **************************/

    NetworkSocket::~NetworkSocket() noexcept
    {
        TryClose();
    }
}