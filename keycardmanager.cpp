#include "keycardmanager.h"
#include <QSerialPortInfo>
#include <QDebug>

KeyCardManager::KeyCardManager(QObject *parent) : QObject(parent), serial(new QSerialPort(this))
{
    connect(serial, &QSerialPort::readyRead, this, &KeyCardManager::readSerialData);
}

KeyCardManager::~KeyCardManager()
{
    disconnectSerial();
}

bool KeyCardManager::connectSerial(const QString &portName, int baudRate)
{
    serial->setPortName(portName);
    serial->setBaudRate(baudRate);
    if (serial->open(QIODevice::ReadOnly)) {
        qDebug() << "Serial connected to" << portName;
        return true;
    } else {
        qDebug() << "Failed to open serial port!";
        return false;
    }
}

void KeyCardManager::disconnectSerial()
{
    if (serial->isOpen())
        serial->close();
}

void KeyCardManager::readSerialData()
{
    while (serial->canReadLine()) {
        QByteArray line = serial->readLine().trimmed();
        QString uid = QString::fromUtf8(line).toUpper();
        emit uidReceived(uid);
    }
}
