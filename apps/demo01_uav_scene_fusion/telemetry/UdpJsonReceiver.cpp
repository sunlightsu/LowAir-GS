#include "UdpJsonReceiver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

UdpJsonReceiver::UdpJsonReceiver(QObject *parent)
    : QObject(parent), m_udpSocket(new QUdpSocket(this))
{
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpJsonReceiver::processPendingDatagrams);
}

UdpJsonReceiver::~UdpJsonReceiver()
{
    stopListening();
}

void UdpJsonReceiver::startListening(quint16 port)
{
    if (m_udpSocket->bind(QHostAddress::Any, port)) {
        emit started();
    } else {
        qWarning() << "Failed to bind UDP port" << port;
    }
}

void UdpJsonReceiver::stopListening()
{
    m_udpSocket->close();
    emit stopped();
}

void UdpJsonReceiver::processPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(datagram, &parseError);
        
        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "JSON parse error:" << parseError.errorString();
            continue;
        }

        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            
            QString uavId = jsonObj["uav_id"].toString();
            double x = jsonObj["x"].toDouble();
            double y = jsonObj["y"].toDouble();
            double z = jsonObj["z"].toDouble();
            double roll = jsonObj["roll"].toDouble();
            double pitch = jsonObj["pitch"].toDouble();
            double yaw = jsonObj["yaw"].toDouble();
            double battery = jsonObj["battery"].toDouble();

            emit dataReceived(uavId, x, y, z, roll, pitch, yaw, battery);
        }
    }
}
