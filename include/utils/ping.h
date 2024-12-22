#ifndef NVGQPING_H
#define NVGQPING_H

#include <QObject>
#include <QSocketNotifier>
#include <QHostAddress>

namespace nvg{

class NvgQPing : public QObject
{
    Q_OBJECT
public:
    explicit NvgQPing(QObject *parent = nullptr);
    ~NvgQPing();

    void ping(const char* dstAddr);

signals:
    void responseReceived(QHostAddress);

private slots:
    void onActivated(QSocketDescriptor socket,
                     QSocketNotifier::Type type);

private:
    void formatICMP(void* buf, int len);

private:
    QSocketNotifier* _notifier;
    int              _nativeFd;
    int              _seq;
};

}

#endif // NVGQPING_H
