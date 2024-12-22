#include <serial_port.h>

#include <QDebug>

namespace nvg{

SerialPort::SerialPort(const SerialPortArgs& args,
                       QObject* parent) :
    QObject{parent}
{
    _port.setPortName(args.name);
    _port.setBaudRate(args.baud);
    _port.setDataBits(args.data);
    _port.setStopBits(args.stop);
    _port.setParity(args.parity);
    _port.setFlowControl(QSerialPort::NoFlowControl);

    QObject::connect(&_port, &QSerialPort::readyRead,
                     this, &SerialPort::onReadyRead);
    QObject::connect(&_port, &QSerialPort::bytesWritten,
                     this, &SerialPort::dataSended);
    QObject::connect(&_port, &QSerialPort::errorOccurred,
                     this, &SerialPort::onErrorOccured);
}

SerialPort::~SerialPort(){
    QObject::disconnect(&_port, &QSerialPort::readyRead,
                        this, &SerialPort::onReadyRead);
    QObject::disconnect(&_port, &QSerialPort::bytesWritten,
                     this, &SerialPort::dataSended);
    QObject::disconnect(&_port, &QSerialPort::errorOccurred,
                        this, &SerialPort::onErrorOccured);

    if(this->_connectionState == ConnectionState::CONNECTED){
        this->close();
    }
}

bool SerialPort::open(){
    if(!_port.open(QIODevice::ReadWrite)){
        qDebug() << "SerialPort::Open: failed to open port"
                 << _port.portName()
                 << _port.errorString();
        return false;
    }
    qDebug() << "SerialPort::Open: serial port opened, name:" << _port.portName();

    this->_connectionState = ConnectionState::CONNECTED;
    emit this->connectionChanged(this->_connectionState);
    return true;
}

void SerialPort::close(){
    if(this->_connectionState == ConnectionState::CONNECTED){
        _port.close();
        this->_connectionState = ConnectionState::DISCONNECTED;
        qDebug() << "SerialPort::Close()";
        emit this->connectionChanged(this->_connectionState);
    }
}

qint64 SerialPort::sendData(const void* buf, const qint64 len){
    return _port.write((const char*)buf, len);
}

void SerialPort::onReadyRead(){
    QByteArray data = _port.readAll();
    //qDebug() << "data.size()=" << data.size();
    emit this->dataReceived(data);
}

void SerialPort::onBytesWritten(qint64 bytes){
    //qDebug() << "send bytes size = " << bytes;
    emit this->dataSended(bytes);
}

void SerialPort::onErrorOccured(QSerialPort::SerialPortError error){
    qDebug() << "SerialPort::onError: error =" << error;
    switch(error) {
        case QSerialPort::ResourceError:
            this->close(); break;
        default:
            break;
    }
}

}
