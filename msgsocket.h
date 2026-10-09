#ifndef MSGSOCKET_H
#define MSGSOCKET_H

#include <pthread.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include "utility.h"

#define SERVER_PORT 10688
#define CLIENT_PORT 10689
#define CAMERA_PORT 10687

#define DEBUG_UDP_PORT  (2888)
#define DEBUG_UDP_IP    ("192.168.28.95")

typedef unsigned char   byte;
typedef unsigned short   ushort;

enum FY_RD_MSG_TYPE_
{
    FY_RD_MSG_TYPE_CMD = 1,
    FY_RD_MSG_TYPE_TRANS = 2,
    FY_RD_MSG_TYPE_VISION = 3,
    FY_RD_MSG_TYPE_UPGRADE = 4,

    FY_RD_MSG_TYPE_MAX
};

int makeMsgPack(byte type, byte cmd, byte subCmd, byte *pData, int len, byte* pMsg);

class MsgSocket
{
private:
    MsgSocket();

    int recvSock;
    int sendSock;

    int serverPort;

    struct sockaddr_in addr;

protected:
    static int run(MsgSocket *p);

public:
    static MsgSocket* getInstance();

    void setDebugAddr(const char* ip, unsigned short port = DEBUG_UDP_PORT);

    int start(int port = SERVER_PORT);

    int response(byte seq, byte result, sockaddr_in &addr);
    int sendMsg(byte* pData, int len);
};

#endif // MSGSOCKET_H
