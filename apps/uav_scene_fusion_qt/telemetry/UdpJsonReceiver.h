#ifndef UDPJSONRECEIVER_H
#define UDPJSONRECEIVER_H

#include <QObject>
#include <QUdpSocket>

class UdpJsonReceiver : public QObject
{
    Q_OBJECT

public:
    explicit UdpJsonReceiver(QObject *parent = nullptr);
    ~UdpJsonReceiver();

    void startListening(quint16 port);
    void stopListening();

signals:
    void started();
    void stopped();
    void dataReceived(const QString &uavId, double x, double y, double z, double roll, double pitch, double yaw, double battery);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *m_udpSocket;
};

#endif // UDPJSONRECEIVER_H
