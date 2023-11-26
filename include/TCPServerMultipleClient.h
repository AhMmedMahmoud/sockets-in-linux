#ifndef TCP_LISTENER_MULTIPLE_CLIENTS_H
#define TCP_LISTENER_MULTIPLE_CLIENTS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <array>
#include "network_socket.h"
#include "TCPServer.h"
#include <iostream>

#define MAX_EVENTS 10
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// typedef struct {
//     int id;
//     int client_fd;
// } Client;

namespace POSIXNetworkSocketLib
{
    
    class TcpListenerMultipleClients : public TcpListener
    {
    private:
        /******************************* extra attributes *************************/
        // Client clients[MAX_CLIENTS];

    public:
        /************************ disable empty constructor ***********************/

        TcpListenerMultipleClients() = delete;



        /******************************* constructor ******************************/

        /// @brief Constructor
        /// @param ipAddress Listen IP address
        /// @param port Listen port number
        TcpListenerMultipleClients(std::string ipAddress, uint16_t port);



        /*********************** overload functions inherited from parent *********/

        bool TryClose(int clientFD) noexcept;

        /// @brief Try to accept a client to form a connection
        /// @returns True if the client is successfully accepted; otherwise false
        int TryAccept() noexcept;

        /// @brief Send a byte array to the connected client
        /// @tparam N Send buffer size
        /// @param buffer Send buffer byte array
        /// @returns Size of sent bytes (-1 in case of sending failed)
        template <std::size_t N>
        ssize_t Send(const std::array<uint8_t, N> &buffer, int clientFD) const noexcept
        {
            ssize_t _result = send(clientFD, buffer.data(), N, MSG_NOSIGNAL);
            return _result;
        }

        /// @brief Receive a byte array from the connected client
        /// @tparam N Receive buffer size
        /// @param buffer Receive buffer byte array
        /// @returns Size of received bytes (-1 in case of receiving failed)
        /// @warning Due to edge-triggered polling, starvation can occur
        template <std::size_t N>
        ssize_t Receive(std::array<uint8_t, N> &buffer, int clientFD) const noexcept
        {
            ssize_t _result = recv(clientFD, buffer.data(), N, 0);
            return _result;
        }
    };
}

#endif