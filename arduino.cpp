#include "arduino.h"

Arduino::Arduino() {
    serial = new QSerialPort;
    arduino_is_available = false;
    arduino_port_name = "";
}

int Arduino::connect_arduino() {
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.hasVendorIdentifier() && info.hasProductIdentifier()) {
            if (info.vendorIdentifier() == arduino_uno_vendor_id && info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = info.portName();
            }
        }
    }

    if (arduino_is_available) {
        serial->setPortName(arduino_port_name);
        if (!serial->open(QIODevice::ReadWrite))
            return -1;
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        return 0;
    }

    return -1;
}

int Arduino::close_arduino() {
    if (serial->isOpen()) {
        serial->close();
        return 0;
    }
    return -1;
}

int Arduino::write_to_arduino(QByteArray data) {
    if (serial->isWritable()) {
        serial->write(data);
        return 0;
    }
    return -1;
}

QByteArray Arduino::read_from_arduino() {
    if (serial->isReadable()) {
        data = serial->readAll();
    }
    return data;
}

QSerialPort* Arduino::getserial() {
    return serial;
}

QString Arduino::getarduino_port_name() {
    return arduino_port_name;
}
