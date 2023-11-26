#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include <string>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>


namespace POSIXNetworkSocketLib
{
    /// @brief abstract class represent network socket
    class NetworkSocket
    {
        /*
           any network socket has
           ----------------------------
           - attributes
              - FileDescriptor
              - ipAddress
              - port number

            - functions
              - TrySetup   --> need to override
              - TryClose
            
            ----------------------------------------------------------
            network socket over TCP   |   network socket over UDP
            ----------------------------------------------------------
              API flow for setup is   |   API flow for setup is
              completely different    |   completely different
              in case of UDP          |   in case of TCP


        */
    protected:
        /********************************** for my child **************************/

        /// @brief File descriptor
        int FileDescriptor;

        /// @brief Listening/Connecting IP address for server/client
        const std::string IpAddress;

        /// @brief Listening/Connecting port number for server/client
        const uint16_t Port;



        /***************** useful for constructor of my child **********************/

        /// @brief Constructor
        /// @param ipAddress Binding IPv4 address
        /// @param port Binding port number
        NetworkSocket(std::string ipAddress, uint16_t port);

    public:
        /***********************  virtual deconstructor **************************/

        virtual ~NetworkSocket() noexcept ;



        /********************************** getters ******************************/

        /// @brief File descriptor
        /// @returns A non-negative number if the communicator has been already set up
        int Descriptor() const noexcept;



        /**************************** pure virtual functions **********************/
        
        /// @brief Connection descriptor for sending and receiving
        /// @returns A non-negative value if there is a valid connection; otherwise -1
        virtual int Connection() const noexcept = 0;

        /// @brief Try to setup the NetworkSocket
        /// @returns True if the NetworkSocket is successfully set up; otherwise false
        virtual bool TrySetup() noexcept = 0;



        /**************************** fundemental functions *********************/

        /// @brief Try to close the NetworkSocket
        /// @returns True if the NetworkSocket closed successfully; otherwise false
        bool TryClose() noexcept;
    };
}

#endif