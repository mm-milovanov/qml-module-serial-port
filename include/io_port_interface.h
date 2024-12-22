#ifndef IIOPORT_H
#define IIOPORT_H

#include <QObject>

namespace nvg{

class IIOPort{
public:
    enum class ConnectionState{
        CONNECTED,
        DISCONNECTED
    };

public:
    virtual ~IIOPort() {};

public:
    virtual bool open() = 0;
    virtual void close() = 0;

    virtual qint64 sendData(const void* buf, const qint64 len) = 0;

signals:
    virtual void dataReceived(QByteArray& data) = 0;
    virtual void dataSended(qint64 bytes) = 0;
    virtual void connectionChanged(nvg::IIOPort::ConnectionState state) = 0;
};

}

Q_DECLARE_INTERFACE(nvg::IIOPort, "nvg::IIOPort")

#endif // IIOPORT_H
