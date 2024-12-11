#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <lib-Qnvg/iioport.h>

#include <QObject>
#include <QSerialPort>

namespace nvg{

struct SerialPortArgs {
    QString name;
    qint32 baud;
    QSerialPort::DataBits data;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stop;
};

class SerialPort : public QObject, public IIOPort
{
    Q_OBJECT
    Q_INTERFACES(nvg::IIOPort)
public:
    SerialPort(const SerialPortArgs& args,
               QObject *parent = nullptr);
    ~SerialPort();

public:
    bool open() override;
    void close() override;

    qint64 sendData(const void* buf, const qint64 len) override;

signals:
    void dataReceived(QByteArray& data) override;
    void dataSended(qint64) override;
    void connectionChanged(nvg::IIOPort::ConnectionState state) override;

private slots:
    void onReadyRead();
    void onBytesWritten(qint64);
    void onErrorOccured(QSerialPort::SerialPortError error);

private:
    QSerialPort _port;
    ConnectionState _connectionState;
};
   
} // namespace nvg

#endif // SERIALPORT_H
