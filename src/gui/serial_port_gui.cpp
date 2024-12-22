#include <gui/serial_port_gui.h>

#include <QSerialPortInfo>
#include <QDebug>

#include <algorithm>

namespace nvg{

static void _sortInfoList(QList<QSerialPortInfo>& list){
    std::sort(list.begin(), list.end(),
                [](const QSerialPortInfo &a, const QSerialPortInfo & b) {
                    return a.portName() < b.portName();
                });
}

static QList<QSerialPortInfo> _getInfoList(){
    auto infoList = QSerialPortInfo::availablePorts();
    _sortInfoList(infoList);
    return infoList;
}

void SerialPortGUI::_OnTimerEvent(){
    auto info = _getInfoList();
    bool equal = std::equal( info.begin(), info.end(),
                             _info.begin(), _info.end(),
                             []( const QSerialPortInfo &a, const QSerialPortInfo & b ) {
                                return a.portName() == b.portName();
                             } );

    if (equal) return;
    beginResetModel();
    _info = info;
    endResetModel();
    emit countChanged(_info.count());
}

SerialPortGUI::SerialPortGUI(QObject *parent,
                             QString defaultBaudrate)
    : QAbstractListModel(parent)
{
    _info = _getInfoList();

    // Default configuration
    this->_args.baud = defaultBaudrate.toUInt(nullptr);
    this->_args.data = QSerialPort::DataBits::Data8;
    this->_args.parity = QSerialPort::Parity::NoParity;
    this->_args.stop = QSerialPort::StopBits::OneStop;

    QObject::connect(&this->_timer, &QTimer::timeout,
                     this, &SerialPortGUI::_OnTimerEvent);
    this->_timer.start(1000);
}

uint SerialPortGUI::getBaud (void) {
    return _args.baud;
}
void SerialPortGUI::setBaud (uint var) {
    _args.baud = var;
//        qDebug() << "setBaud " << var;
    emit argsChanged(var);
}

uint SerialPortGUI::getData (void) {
    return (uint)(_args.data - QSerialPort::Data5);
}
void SerialPortGUI::setData (uint var) {
    if (var > QSerialPort::Data8 - QSerialPort::Data5)
        return;
//        qDebug() << "setData " << var;
    _args.data = (QSerialPort::DataBits)(var + (uint)QSerialPort::Data5);
}

uint SerialPortGUI::getParity (void) {
    switch (_args.parity) {
    case QSerialPort::NoParity:     return 0;
    case QSerialPort::EvenParity:   return 1;
    case QSerialPort::OddParity:    return 2;
    case QSerialPort::SpaceParity:  return 3;
    case QSerialPort::MarkParity:   return 4;
    default: return 0;
    }
}
void SerialPortGUI::setParity (uint var) {
    switch (var) {
    case 0: _args.parity = QSerialPort::NoParity; break;
    case 1: _args.parity = QSerialPort::EvenParity; break;
    case 2: _args.parity = QSerialPort::OddParity; break;
    case 3: _args.parity = QSerialPort::SpaceParity; break;
    case 4: _args.parity = QSerialPort::MarkParity; break;
    default: return;
    }
//        qDebug() << "setParity " << var;
    emit argsChanged(var);
}

uint SerialPortGUI::getStop (void) {
    switch (_args.stop) {
    case QSerialPort::OneStop:          return 0;
    case QSerialPort::OneAndHalfStop:   return 1;
    case QSerialPort::TwoStop:          return 2;
    default: return 0;
    }
}
void SerialPortGUI::setStop (uint var) {
    switch (var) {
    case 0: _args.stop = QSerialPort::OneStop; break;
    case 1: _args.stop = QSerialPort::OneAndHalfStop; break;
    case 2: _args.stop = QSerialPort::TwoStop; break;
    default: return;
    }
//        qDebug() << "setStop " << var;
    emit argsChanged(var);
}

int SerialPortGUI::getCount(){
    return _info.count();
}

int SerialPortGUI::getIndex(){
    qDebug() << "SerialPortSelectorGUI::getIndex, v =" << _index;
    return _index;
}
void SerialPortGUI::setIndex(int v){
    if(_connected){
        return;
    }
    qDebug() << "SerialPortSelectorGUI::setIndex, v =" << v;
    if(v >= 0){
        _index = v;
        emit locationChanged(_info[_index].systemLocation());
        emit descriptionChanged(_info[_index].description());
    }
}

QString SerialPortGUI::getLocation(){
    if(_index >= 0){
        if(_index < _info.count())
            return _info[_index].systemLocation();
    }
    return QString();
}

QString SerialPortGUI::getDescription(){
    if(_index >= 0 && _index < _info.count())
            return _info[_index].description();
    return QString();
}

bool SerialPortGUI::getConnected(){
    return _connected;
}
void SerialPortGUI::setConnected(bool v){
    _connected = v;
    emit connectedChanged(_connected);
}

int SerialPortGUI::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return _info.count();
}

QVariant SerialPortGUI::data(const QModelIndex& index, int role) const
{
    int i = index.row();
    if(i<0 || i>_info.count()){
        return QVariant();
    }
    switch(role){
        case Qt::DisplayRole:
            return _info.value(i).portName();
    }
    return QVariant();
}

void SerialPortGUI::ConnectionChanged(bool connected){
    qDebug() << "SerialPortGUI::ConnectionChanged, connected = " << connected;
    this->setConnected(connected);
}

void SerialPortGUI::onConnectionButtonClicked(){
    if(!this->_connected){
        if(this->_index >=0 && this->_index < this->_info.size()){
            QString device = this->_info[this->_index].systemLocation();
            _args.name = device;
            emit this->SerialConnectReq(_args);
            qDebug() << "SerialInfoGUI::connect(), device=" << device << " index=" << this->_index;
        }
    }else{
        emit this->SerialDisconnectReq();
    }
}

}
