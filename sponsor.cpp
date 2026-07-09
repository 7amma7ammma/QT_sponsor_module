#include "sponsor.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include <QSqlError>

Sponsor::Sponsor() {
    id = 0;
    name = "";
    organization = "";
    email = "";
    phone = "";
    contribution_amount = 0.0;
    contract_details = "";
    renewal_date = "";
}

Sponsor::Sponsor(int id, QString name, QString organization , QString email,  QString phone,
                 double contribution_amount, QString contract_details , QString renewal_date) {
    this->id = id;
    this->name = name;
    this->organization = organization;
    this->email = email;
    this->phone = phone;
    this->contribution_amount = contribution_amount;
    this->contract_details = contract_details;
    this->renewal_date = renewal_date;
}

int Sponsor::getId() const { return id; }
QString Sponsor::getName() const { return name; }
QString Sponsor::getOrganization() const { return organization; }
QString Sponsor::getEmail() const { return email; }
QString Sponsor::getPhone() const { return phone; }
double Sponsor::getContributionAmount() const { return contribution_amount; }
QString Sponsor::getContractDetails() const { return contract_details; }
QString Sponsor::getRenewalDate() const { return renewal_date; }

void Sponsor::setId(int id) { this->id = id; }
void Sponsor::setName(QString name) { this->name = name; }
void Sponsor::setOrganization(QString organization) { this->organization = organization; }
void Sponsor::setEmail(QString email) { this->email = email; }
void Sponsor::setPhone(QString phone) { this->phone = phone; }
void Sponsor::setContributionAmount(double amount) { this->contribution_amount = amount; }
void Sponsor::setContractDetails(QString details) { this->contract_details = details; }
void Sponsor::setRenewalDate(QString date) { this->renewal_date = date; }

bool Sponsor::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO SPONSORS (ID, NAME, ORGANIZATION, EMAIL, PHONE, CONTRIBUTION, CONTRACT_DETAILS, RENEWAL_DATE) "
                  "VALUES (:id, :name, :organization, :email, :phone, :contribution, :contract_details, :renewal_date)");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":organization", organization);
    query.bindValue(":email", email);
    query.bindValue(":phone", phone);
    query.bindValue(":contribution", contribution_amount);
    query.bindValue(":contract_details", contract_details);
    query.bindValue(":renewal_date", renewal_date);

    return query.exec();
}

QSqlQueryModel* Sponsor::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SPONSORS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NAME"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ORGANIZATION"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PHONE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CONTRIBUTION"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("CONTRACT DETAILS"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("RENEWAL DATE"));

    return model;
}

bool Sponsor::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM SPONSORS WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Sponsor::checkIfExists(int id) {
    QSqlQuery query;
    query.prepare("SELECT ID FROM SPONSORS WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec() && query.next();
}

bool Sponsor::modifier(int id) {
    QSqlQuery query;
    query.prepare("UPDATE SPONSORS SET NAME = :name, ORGANIZATION = :organization, EMAIL = :email, PHONE = :phone, "
                  "CONTRIBUTION = :contribution, CONTRACT_DETAILS = :contract_details, RENEWAL_DATE = :renewal_date WHERE ID = :id");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":organization", organization);
    query.bindValue(":email", email);
    query.bindValue(":phone", phone);
    query.bindValue(":contribution", contribution_amount);
    query.bindValue(":contract_details", contract_details);
    query.bindValue(":renewal_date", renewal_date);

    return query.exec();
}


//behee

QSqlQueryModel* Sponsor::afficherTrieParNom() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM HAMA.SPONSORS ORDER BY NAME ASC");
    return model;
}

QSqlQueryModel* Sponsor::afficherTrieParId() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM HAMA.SPONSORS ORDER BY ID ASC");
    return model;
}

QSqlQueryModel* Sponsor::afficherTrieParMount() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM HAMA.SPONSORS ORDER BY CONTRIBUTION ASC");
    return model;
}

