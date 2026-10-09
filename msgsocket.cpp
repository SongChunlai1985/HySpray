#include <unistd.h>

#include "msgsocket.h"
#include "thread"

#define BUFFER_LEN (1024*4)
#define CMD_LEADER "cmdname:"
#define CMD_ENDIAN ";"

#define MSG_DATA_SYNC_FALG  (0x47)

#pragma pack(push)
#pragma pack(1)

struct MSG_HEAD
{
    byte sync;
    ushort len;
    byte seq;
    byte type;
    byte cmd;
    byte subCmd;
    byte data[0];
};

#pragma pack(pop)

static byte seq = 0;
static byte recv_buf[BUFFER_LEN+1] = {0};

int MsgSocket::run(MsgSocket *p)
{
#if 0
    socklen_t addr_len = sizeof(struct sockaddr_in);
    //pthread_t t_id;
    struct sockaddr_in server;
    struct sockaddr_in client;

    unsigned short port = p->serverPort;

    // client socket, for send message:
    if((p->sendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
            LogError("Server socket could not be created.\n");
            return 0;
    }

    // server socket, for recv message:
    if((p->recvSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
            LogError("Server socket could not be created.\n");
            return 0;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(p->recvSock, (struct sockaddr *) &server, sizeof(server)) < 0){
        LogError("Server bind failed. Server already running? Proper permissions?\n");
        return 2;
    }

    LogDebug("Server started at port:%d", port);

    byte* pAddr = (byte*)(&client.sin_addr.s_addr);
    int total= 0;
    while(true)
    {
        int recvnum;
        if (0 > (recvnum = recvfrom(p->recvSock, recv_buf + total, sizeof(recv_buf) - total,
                                  0, (struct sockaddr *)&client, &addr_len)))
        {
            LogError("\nrecv error\n");
            sleep(1);
            total = 0;
            continue;
        }

        //printf("recv data from %X:", client.sin_addr.s_addr);
        printf("recv data from %d.%d.%d.%d:", pAddr[0], pAddr[1], pAddr[2], pAddr[3]);

        if (recvnum < 16)
            printbyte(recv_buf + total, recvnum);
        else
            printbyte(recv_buf + total, 16);

        recv_buf[total+recvnum] = 0;
        total += recvnum;

        total = p->procData(recv_buf, total, client);

        if ((total >= BUFFER_LEN) || (total < 0))
        {
            total = 0;
        }
    }
    #endif
}

MsgSocket::MsgSocket()
{
    recvSock = -1;
    sendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

int MsgSocket::start(int port)
{
    serverPort = port;
//    thread procThread(MsgSocket::run, this);
//    procThread.detach();

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(DEBUG_UDP_IP);
    addr.sin_port = htons(DEBUG_UDP_PORT);

    return 0;
}

MsgSocket *MsgSocket::getInstance()
{
     static MsgSocket theMsgSocketServer;
     return &theMsgSocketServer;
}

void MsgSocket::setDebugAddr(const char *ip, unsigned short port)
{
    if (ip)
    {
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip);
        addr.sin_port = htons(port);
    }
}

int MsgSocket::response(byte seq, byte result, sockaddr_in &addr)
{
#define RESPONSE_DATA_CRC_LEN    (2)
#define RESPONSE_DATA_LEN    (5)

    byte data[16];
    int offset = 0;

    data[offset++] = MSG_DATA_SYNC_FALG;
    data[offset++] = seq;
    data[offset++] = result;

    unsigned short crc = 0;//getCRC16(data+1, RESPONSE_DATA_CRC_LEN);

    data[offset++] = (crc >> 8) & 0xFF;
    data[offset++] = crc & 0xFF;

    addr.sin_family = AF_INET;
    //addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(CLIENT_PORT);

    return 0;
}

int MsgSocket::sendMsg(byte *pData, int len)
{
    int ret = 0;
    if ((pData == nullptr) || (len <= 0))
    {
        return 0;
    }

    if (sendSock < 0)
    {
        return 0;
    }

    ret = sendto(sendSock, pData, len, 0, (struct sockaddr *)&addr, sizeof(addr));
    return ret;
}
