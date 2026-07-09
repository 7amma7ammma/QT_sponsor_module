#include "connection.h"

Connection::Connection()
{

}

#include <QDebug>
#include <QSqlError>

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("hama"); // Assure-toi que ce DSN existe bien
    db.setUserName("hama");
    db.setPassword("root");

    if (db.open()) {
        test = true;
        qDebug() << "Connexion réussie !";
    } else {
        qDebug() << "Échec de connexion :" << db.lastError().text();
    }

    return test;
}
