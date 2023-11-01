#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include <string>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>


namespace POSIXNetworkSocketLib
{
    /// @brief TCP/IP network socket
    class NetworkSocket
    {
    protected:
        /// @brief File descriptor
        int FileDescriptor;

        /// @brief Listening/Connecting IP address for server/client
        const std::string IpAddress;

        /// @brief Listening/Connecting port number for server/client
        const uint16_t Port;

        /// @brief Constructor
        /// @param ipAddress Binding IPv4 address
        /// @param port Binding port number
        NetworkSocket(std::string ipAddress, uint16_t port);

    public:
        virtual ~NetworkSocket() noexcept ;

        /// @brief Connection descriptor for sending and receiving
        /// @returns A non-negative value if there is a valid connection; otherwise -1
        virtual int Connection() const noexcept = 0;

        /// @brief Try to setup the NetworkSocket
        /// @returns True if the NetworkSocket is successfully set up; otherwise false
        virtual bool TrySetup() noexcept = 0;

        /// @brief Try to close the NetworkSocket
        /// @returns True if the NetworkSocket closed successfully; otherwise false
        virtual bool TryClose() noexcept;
    };
}

#endif