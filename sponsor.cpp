#include "sponsor.h"

// Default Constructor
Sponsor::Sponsor() : id(0), contribution_amount(0.0) {}

// Parameterized Constructor
Sponsor::Sponsor(int id, QString name, QString organization, QString email,
                 QString phone, double contribution_amount, QString sponsored_projects,
                 QString contract_details, QString renewal_date)
    : id(id), name(name), organization(organization), email(email), phone(phone),
      contribution_amount(contribution_amount), sponsored_projects(sponsored_projects),
      contract_details(contract_details), renewal_date(renewal_date) {}

// Getters
int Sponsor::getId() const { return id; }
QString Sponsor::getName() const { return name; }
QString Sponsor::getOrganization() const { return organization; }
QString Sponsor::getEmail() const { return email; }
QString Sponsor::getPhone() const { return phone; }
double Sponsor::getContributionAmount() const { return contribution_amount; }
QString Sponsor::getSponsoredProjects() const { return sponsored_projects; }
QString Sponsor::getContractDetails() const { return contract_details; }
QString Sponsor::getRenewalDate() const { return renewal_date; }

// Setters
void Sponsor::setId(int id) { this->id = id; }
void Sponsor::setName(QString name) { this->name = name; }
void Sponsor::setOrganization(QString organization) { this->organization = organization; }
void Sponsor::setEmail(QString email) { this->email = email; }
void Sponsor::setPhone(QString phone) { this->phone = phone; }
void Sponsor::setContributionAmount(double contribution_amount) { this->contribution_amount = contribution_amount; }
void Sponsor::setSponsoredProjects(QString sponsored_projects) { this->sponsored_projects = sponsored_projects; }
void Sponsor::setContractDetails(QString contract_details) { this->contract_details = contract_details; }
void Sponsor::setRenewalDate(QString renewal_date) { this->renewal_date = renewal_date; }

#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>

// Create (Add a sponsor)
bool Sponsor::add() {
    QSqlQuery query;
    query.prepare("INSERT INTO sponsors (id, name, organization, email, phone, "
                  "contribution_amount, sponsored_projects, contract_details, renewal_date) "
                  "VALUES (:id, :name, :organization, :email, :phone, :contribution_amount, "
                  ":sponsored_projects, :contract_details, :renewal_date)");

    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":organization", organization);
    query.bindValue(":email", email);
    query.bindValue(":phone", phone);
    query.bindValue(":contribution_amount", contribution_amount);
    query.bindValue(":sponsored_projects", sponsored_projects);
    query.bindValue(":contract_details", contract_details);
    query.bindValue(":renewal_date", renewal_date);

    return query.exec();
}

// Read (Retrieve all sponsors)
QSqlQueryModel* Sponsor::read() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM sponsors");
    return model;
}

// Update (Modify a sponsor)
bool Sponsor::update() {
    QSqlQuery query;
    query.prepare("UPDATE sponsors SET name=:name, organization=:organization, email=:email, "
                  "phone=:phone, contribution_amount=:contribution_amount, "
                  "sponsored_projects=:sponsored_projects, contract_details=:contract_details, "
                  "renewal_date=:renewal_date WHERE id=:id");

    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":organization", organization);
    query.bindValue(":email", email);
    query.bindValue(":phone", phone);
    query.bindValue(":contribution_amount", contribution_amount);
    query.bindValue(":sponsored_projects", sponsored_projects);
    query.bindValue(":contract_details", contract_details);
    query.bindValue(":renewal_date", renewal_date);

    return query.exec();
}

// Delete (Remove a sponsor)
bool Sponsor::remove(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM sponsors WHERE id=:id");
    query.bindValue(":id", id);
    return query.exec();
}

