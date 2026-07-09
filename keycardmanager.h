#ifndef KEYCARDMANAGER_H
#define KEYCARDMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSqlDatabase>

class KeyCardManager : public QObject
{
    Q_OBJECT
public:
    explicit KeyCardManager(QObject parent = nullptr);
    ~KeyCardManager();

    bool connectSerial(const QString &portName, int baudRate = 9600);
    void disconnectSerial();

signals:
    void uidReceived(const QString &uid);

private slots:
    void readSerialData();

private:
    QSerialPort *serial;
};

#endif // KEYCARDMANAGER_H
