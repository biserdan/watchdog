/**
 * @file UDP_Client.h
 * @author Kevin Steiger (steigkev@students.zhaw.ch)
 * @brief UDP_Client header file
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef BA_UDP_CLIENT_H
#define BA_UDP_CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/**
 * @brief Class for using a UDP socket
 * 
 */
class UDP_Client {
public:
    UDP_Client(int server_port, int server_address);
    ~UDP_Client();
    int send(char *message, int length);

private:
    struct sockaddr_in cliAddr;
    struct sockaddr_in remoteServAddr;
    int sockfd;
    int rc;
    int i;

};

#endif //BA_UDP_CLIENT_H
