#ifndef UDPPORTGUI_H
#define UDPPORTGUI_H

#include <udp_port.h>
#include <QObject>

namespace nvg{

class UdpPortGUI : public QObject
{
    Q_OBJECT

public:

    explicit UdpPortGUI(QObject *parent = nullptr);
    ~UdpPortGUI() {};

public:

    Q_PROPERTY(bool connected READ getConnected WRITE setConnected NOTIFY connectedChanged)
    bool getConnected (void) { return _connected; }
    void setConnected (bool v) { _connected = v; emit connectedChanged(_connected); }

    Q_PROPERTY(QString srcAdrs READ getSrcAdrs WRITE setSrcAdrs NOTIFY argsChanged)
    QString getSrcAdrs (void) { return _args.srcAddr; }
    void setSrcAdrs (QString v) { _args.srcAddr = v; emit argsChanged(); }

    Q_PROPERTY(uint srcPort READ getSrcPort WRITE setSrcPort NOTIFY argsChanged)
    uint getSrcPort (void) { return _args.srcPort; }
    void setSrcPort (uint v) { _args.srcPort = v; emit argsChanged(); }

    Q_PROPERTY(QString dstAdrs READ getDstAdrs WRITE setDstAdrs NOTIFY argsChanged)
    QString getDstAdrs (void) { return _args.dstAddr; }
    void setDstAdrs (QString v) { _args.dstAddr = v; emit argsChanged(); }

    Q_PROPERTY(uint dstPort READ getDstPort WRITE setDstPort NOTIFY argsChanged)
    uint getDstPort (void) { return _args.dstPort; }
    void setDstPort (uint v) { _args.dstPort = v; emit argsChanged(); }

    Q_PROPERTY(bool lock READ getGUILock NOTIFY stateChanged)
    bool getGUILock (void) { return _GUILock;  }
    void setGUILock (bool lock) { _GUILock = lock; emit stateChanged(lock);}


signals:
    void connectedChanged(bool);
    void UdpConnectReq(const UdpPortArgs& args);
    void UdpDisconnectReq(void);
    void argsChanged(void);
    void stateChanged(bool);

public:
    Q_INVOKABLE void onConnectionButtonClicked();

public slots:
    void ConnectionChanged(bool connected);

private:
    bool _connected = false;
    bool _GUILock = false;
    UdpPortArgs _args;
};

}

#endif // UDPPORTGUI_H
