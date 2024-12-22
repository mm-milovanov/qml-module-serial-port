#ifndef UDPPORT_H
#define UDPPORT_H

#include <io_port_interface.h>

#include <QObject>
#include <QUdpSocket>

#include <stdint.h>

namespace nvg{

struct UdpPortArgs{
    bool any = false;
    QString srcAddr;    /* null for IN_ADDR_ANY */
    quint16 srcPort;    /* 0 for bindless socket */
    QString dstAddr;    /* default dest addr */
    quint16 dstPort;    /* default dest port */
};

class UdpPort : public QObject, public IIOPort
{
    Q_OBJECT
    Q_INTERFACES(nvg::IIOPort)
public:
    UdpPort(const UdpPortArgs& args,
            QObject *parent = nullptr);
    UdpPort(uint16_t dstPort, uint16_t srcPort,
            QObject *parent = nullptr);
    ~UdpPort();

public:
    bool open() override;
    void close() override;

    qint64 sendData(const void* buf, const qint64 len) override;

signals:
    void dataReceived(QByteArray& data) override;
    void dataSended(qint64 bytes) override;
    void connectionChanged(nvg::IIOPort::ConnectionState state) override;

private slots:
    void onReadyRead();

private:
    ConnectionState _connectionState;

    QUdpSocket      _socket;
    QHostAddress    _defaultDestAddr;
    quint16         _defaultDestPort;
};

}

#endif
