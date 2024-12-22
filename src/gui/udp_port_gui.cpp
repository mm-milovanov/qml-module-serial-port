#include <gui/udp_port_gui.h>

#include <QDebug>

namespace nvg{

UdpPortGUI::UdpPortGUI(QObject *parent)
    : QObject{parent}
{
    // Default configuration
    this->_args.dstAddr = "127.0.0.1";
    this->_args.dstPort = 1236;
    this->_args.srcAddr = "127.0.0.1";
    this->_args.srcPort = 1234;
}

void UdpPortGUI::ConnectionChanged(bool connected){
    qDebug() << "SerialPortGUI::ConnectionChanged, connected = " << connected;
    this->setConnected(connected);
}

void UdpPortGUI::onConnectionButtonClicked(){
    if (!this->_connected) {
        emit this->UdpConnectReq(_args);
        qDebug() << "UdpInfoGUI::connect() source_address = "
                 << this->_args.srcAddr << ":" << this->_args.srcPort
                 << "; destination_address = "
                 << this->_args.dstAddr << ":" << this->_args.dstPort;
    }
    else {
        emit this->UdpDisconnectReq();
        qDebug() << "UdpInfoGUI::disconnect()";
    }
}

}
