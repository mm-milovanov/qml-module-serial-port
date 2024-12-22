#include <udp_port.h>
#include "qnetworkdatagram.h"

namespace nvg{
    
UdpPort::UdpPort(const UdpPortArgs& args,
                 QObject* parent) :
    QObject{parent},
    _defaultDestAddr(args.dstAddr),
    _defaultDestPort(args.dstPort)
{
    if(args.srcPort != 0){
        QHostAddress bindAddr = QHostAddress::Any;
        if(args.any){
            bindAddr.setAddress(args.srcAddr);
        }
        _socket.bind(bindAddr, args.srcPort);
    }

    QObject::connect(&_socket, &QUdpSocket::readyRead,
                     this, &UdpPort::onReadyRead);
}

UdpPort::UdpPort(uint16_t dstPort, uint16_t srcPort,
                 QObject* parent) :
    QObject{parent},
    _defaultDestAddr(QHostAddress::Broadcast),
    _defaultDestPort(dstPort)
{
    QHostAddress bindAddr = QHostAddress::Any;
    bindAddr.setAddress(QHostAddress::Broadcast);
    _socket.bind(bindAddr, srcPort);

    QObject::connect(&_socket, &QUdpSocket::readyRead,
                     this, &UdpPort::onReadyRead);
}

UdpPort::~UdpPort(){
    _socket.close();
}

bool UdpPort::open(){
    return true;
}

void UdpPort::close(){
    _socket.close();
}

qint64 UdpPort::sendData(const void* buf, const qint64 len){
    qint64 ret = _socket.writeDatagram((const char*)buf,
                                       len,
                                       this->_defaultDestAddr,
                                       this->_defaultDestPort);
    return ret;
}

void UdpPort::onReadyRead(){
    QNetworkDatagram dgram = _socket.receiveDatagram();
    if(dgram.isValid()){
        const QByteArray& data = dgram.data();
        emit this->dataReceived(const_cast<QByteArray&>(data));
    }
}

}
