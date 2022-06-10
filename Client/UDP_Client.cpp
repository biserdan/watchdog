/**
 * @file UDP_Client.cpp
 * @author Kevin Steiger (steigkev@students.zhaw.ch)
 * @brief UDP_Client source file
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "UDP_Client.h"

/**
 * @brief Construct a new udp client::udp client object
 * 
 * @param server_port Contains the desired port for messages
 * @param server_address Contains the Nvidia Jetson Nano server address
 */
UDP_Client::UDP_Client(int server_port, int server_address) {
    struct hostent *h;

    // check server IP-address
    h = gethostbyname(server_address);

    if (h == NULL)
    {
        printf("Unknown host '%s' \n", server_address);
        exit(EXIT_FAILURE);
    }

    printf("Send data to '%s' (IP : %s) \n", h->h_name,
           inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

    remoteServAddr.sin_family = h->h_addrtype;
    memcpy((char *)&remoteServAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    remoteServAddr.sin_port = htons(server_port);

    // create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        printf("Can't open socket (%s) \n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // bind port
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(0);

    rc = bind(sockfd, (struct sockaddr *)&cliAddr,
              sizeof(cliAddr));

    if (rc < 0)
    {
        printf("Can't bind port (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Destroy the udp client::udp client object
 * 
 */
UDP_Client::~UDP_Client() {
    close(sockfd);
}

/**
 * @brief Sends control messages to the server
 * 
 * @param message Contains the buffer pointer to the control message
 * @param length Contains the buffer size
 * @return int Returns error code from sendto(...)
 */
int UDP_Client::send(char *message, int length) {
    return sendto(sockfd, message, length, 0,
           (struct sockaddr *)&remoteServAddr,
           sizeof(remoteServAddr));
}
