#ifndef TCP_LISTENER_H
#define TCP_LISTENER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <array>
#include "network_socket.h"

namespace POSIXNetworkSocketLib
{
    class TcpListener : public NetworkSocket
    {
        /*
        struct sockaddr_in 
           -  used to store socket info
           -  APIs of sockets (bind, accept) knows socket info 
                like ip address for IPv4, ip address for IPv6, port number
           -  has three variables
               - one for store family address
               - one for store ip address
               - one for store port nuumber
        */

    private:
        /******************************* extra attributes *************************/
        
        int mConnection;    //  -1         :  not accepted any client
                            //  value > 0  :  FD of new socket opened to serve the client

    protected:    
        /****************************** for me and my child ***********************/

        struct sockaddr_in mAddress;

    public:
        /************************ disable empty constructor ***********************/

        TcpListener() = delete;



        /******************************* constructor ******************************/

        /// @brief Constructor
        /// @param ipAddress Listen IP address
        /// @param port Listen port number
        TcpListener(std::string ipAddress, uint16_t port);



        /*********************** override functions inherited from parent *********/

        bool TrySetup() noexcept override;
        int Connection() const noexcept override;        



        /**************************** fundemental functions *************************/

        /// @brief Try to accept a client to form a connection
        /// @returns True if the client is successfully accepted; otherwise false
        bool TryAccept() noexcept;


        /// @brief Send a byte array to the connected client
        /// @tparam N Send buffer size
        /// @param buffer Send buffer byte array
        /// @returns Size of sent bytes (-1 in case of sending failed)
        template <std::size_t N>
        ssize_t Send(const std::array<uint8_t, N> &buffer) const noexcept
        {
            ssize_t _result = send(mConnection, buffer.data(), N, MSG_NOSIGNAL);
            return _result;
        }

        /// @brief Receive a byte array from the connected client
        /// @tparam N Receive buffer size
        /// @param buffer Receive buffer byte array
        /// @returns Size of received bytes (-1 in case of receiving failed)
        /// @warning Due to edge-triggered polling, starvation can occur
        template <std::size_t N>
        ssize_t Receive(std::array<uint8_t, N> &buffer) const noexcept
        {
            ssize_t _result = recv(mConnection, buffer.data(), N, 0);

            return _result;
        }

        /// @brief Try to make the current connection (if exists) non-block
        /// @returns True if the non-blocking flag is set successfully; otherwise false
        bool TryMakeConnectionNonblock() noexcept;
    };
}

#endif