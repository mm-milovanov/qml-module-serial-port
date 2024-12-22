#include <utils/ping.h>

#include <winsock2.h>
#include <QDebug>
#include <stdint.h>

namespace nvg{

#define ICMP_ECHO                   8
#define DEFAULT_PAYLOAD_LENGTH      32

#pragma pack(push, 1)

struct icmphdr
{
    uint8_t type;                       /* message type */
    uint8_t code;                       /* type sub-code */
    uint16_t checksum;
    union
    {
        struct
        {
            uint16_t        id;
            uint16_t        sequence;
        } echo;                         /* echo datagram */
        uint32_t        gateway;        /* gateway address */
        struct
        {
            uint16_t        __unused;
            uint16_t        mtu;
        } frag;                        /* path mtu discovery */
    } un;
};

struct icmp_message_t{
    struct icmphdr  hdr;
    uint8_t         pad[DEFAULT_PAYLOAD_LENGTH];
};
#pragma pack(pop)

static uint16_t ICMPChecksum(uint16_t *icmph, int len)
{
    assert(len >= 0);

    uint16_t ret = 0;
    uint32_t sum = 0;
    uint16_t odd_byte;

    while (len > 1) {
        sum += *icmph++;
        len -= 2;
    }

    if (len == 1) {
        *(uint8_t*)(&odd_byte) = * (uint8_t*)icmph;
        sum += odd_byte;
    }

    sum =  (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    ret =  ~sum;

    return ret;
}

NvgQPing::NvgQPing(QObject *parent)
    : QObject{parent},
    _seq{0}
{
    // init WSA
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0){
        throw std::system_error();
    }

    // init socket
    this->_nativeFd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(!this->_nativeFd){
        WSACleanup();
        throw std::system_error();
    }

    this->_notifier = new QSocketNotifier(this->_nativeFd,
                                          QSocketNotifier::Read,
                                          this);

    QObject::connect(this->_notifier, &QSocketNotifier::activated,
                     this, &NvgQPing::onActivated);
}

NvgQPing::~NvgQPing(){
    WSACleanup();
}

void NvgQPing::ping(const char* dstAddr){
    int bcast = 0;
    QString qsAddr{dstAddr};
//    if(qsAddr.endsWith("255")){
//        bcast = 1;
//    }

    /*
     * Turning 192.168.7.255 to 192.168.7.
     */
    QStringList addrBytes = qsAddr.split('.');
    QString qsSubnet;
    for (uint i=0;i<3;i++)
        qsSubnet += addrBytes[i] + '.';
    std::string subnet = qsSubnet.toStdString();

    int ret = setsockopt(this->_nativeFd, SOL_SOCKET,
                         SO_BROADCAST, (char*)&bcast, sizeof(bcast));
    if(ret != 0){
        qDebug() << "NvgQPing::ping,setsockopt, ret:" << ret;
    }

    // Initialize the destination address
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    char sAddr[32];
    for (byte i=0;i<255;i++) {
        sprintf(sAddr, "%s%u", subnet.data(), i);
        addr.sin_addr.S_un.S_addr = inet_addr(sAddr);

        // Initialize the ICMP header
        icmp_message_t msg;
        this->formatICMP(&msg, sizeof(msg));

        // Send the packet
        if(sendto(this->_nativeFd, (char*)&msg, sizeof(msg),
                   0, (struct sockaddr*) &addr, sizeof(addr)) < 0)
        {
            qDebug() << "sendto() errno: " << errno;
        }
//        qDebug() << "sendto() send ping request to addr: " <<  buf;
        this->_seq ++;
    }

//    qDebug() << "sendto: ok.";
}

void NvgQPing::onActivated(QSocketDescriptor socket,
                     QSocketNotifier::Type type)
{
    char buf[1024];
    struct sockaddr from;
    int len = sizeof(struct sockaddr_in);
    int rc = recvfrom(this->_nativeFd,
                      buf, sizeof(buf), 0,
                      &from, &len);

    QHostAddress qAddr(&from);
    emit this->responseReceived(qAddr);

//    qDebug() << rc << qAddr.toString();
}

void NvgQPing::formatICMP(void* buf, int len){
    icmp_message_t* msg =
        (icmp_message_t*)buf;
    memset(msg, 0, sizeof(icmp_message_t));

    msg->hdr.type = ICMP_ECHO;
    msg->hdr.code = 0;
    msg->hdr.checksum = 0;
    msg->hdr.un.echo.id = htons((getpid() & 0xFFFF));
    msg->hdr.un.echo.sequence = htons(this->_seq);

    uint8_t fill = 0x61;
    for(int i=0; i!=DEFAULT_PAYLOAD_LENGTH; i++){
        msg->pad[i] = fill;

        fill += 1;

        if(fill==0x78){
            fill = 0x61;
        }
    }

    msg->hdr.checksum = ICMPChecksum((uint16_t*)msg, sizeof(icmp_message_t));;
}

} // namespace nvg
