#ifndef SERIALPORTGUI_H
#define SERIALPORTGUI_H

#include <serial_port.h>

#include <QObject>
#include <QAbstractListModel>
#include <QSerialPortInfo>
#include <QTimer>

namespace nvg{

class SerialPortGUI : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SerialPortGUI(QObject *parent = nullptr,
                           QString defaultBaudrate = "115200");

    Q_PROPERTY(int count READ getCount NOTIFY countChanged)
    int getCount();

    Q_PROPERTY(int index READ getIndex WRITE setIndex NOTIFY indexChanged)
    int getIndex();
    void setIndex(int v);

    Q_PROPERTY(QString location READ getLocation NOTIFY locationChanged)
    QString getLocation();

    Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged)
    QString getDescription();

    Q_PROPERTY(bool connected READ getConnected WRITE setConnected NOTIFY connectedChanged)
    bool getConnected();
    void setConnected(bool v);

    Q_PROPERTY(uint baud    READ getBaud    WRITE setBaud   NOTIFY argsChanged)
    uint getBaud (void);
    void setBaud (uint var);

    Q_PROPERTY(uint data    READ getData    WRITE setData   NOTIFY argsChanged)
    uint getData (void);
    void setData (uint var);

    Q_PROPERTY(uint parity  READ getParity  WRITE setParity NOTIFY argsChanged)
    uint getParity (void);
    void setParity (uint var);

    Q_PROPERTY(uint stop    READ getStop    WRITE setStop   NOTIFY argsChanged)
    uint getStop (void);
    void setStop (uint var);

    Q_INVOKABLE void onConnectionButtonClicked();

    virtual int rowCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

signals:
    void countChanged(int);
    void indexChanged(int);
    void locationChanged(QString);
    void descriptionChanged(QString);
    void connectedChanged(bool);
    void argsChanged(uint);

    void SerialConnectReq(const SerialPortArgs& args);
    void SerialDisconnectReq();

public slots:
    void ConnectionChanged(bool connected);
    void _OnTimerEvent();

private:
    int _index = -1;
    QList<QSerialPortInfo> _info;
    QTimer _timer;
    bool _connected = false;

    SerialPortArgs _args;
};

}

#endif // SERIALPORTGUI_H
