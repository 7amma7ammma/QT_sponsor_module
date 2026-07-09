#ifndef SPONSOR_H
#define SPONSOR_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Sponsor {
private:
    int id;
    QString name;
    QString organization;
    QString email;
    QString phone;
    double contribution_amount;
    QString sponsored_projects;
    QString contract_details;
    QString renewal_date;

public:
    // Constructors
    Sponsor();
    Sponsor(int id, QString name, QString organization, QString email,
            QString phone, double contribution_amount, QString sponsored_projects,
            QString contract_details, QString renewal_date);

    // Getters
    int getId() const;
    QString getName() const;
    QString getOrganization() const;
    QString getEmail() const;
    QString getPhone() const;
    double getContributionAmount() const;
    QString getSponsoredProjects() const;
    QString getContractDetails() const;
    QString getRenewalDate() const;

    // Setters
    void setId(int id);
    void setName(QString name);
    void setOrganization(QString organization);
    void setEmail(QString email);
    void setPhone(QString phone);
    void setContributionAmount(double contribution_amount);
    void setSponsoredProjects(QString sponsored_projects);
    void setContractDetails(QString contract_details);
    void setRenewalDate(QString renewal_date);

    // CRUD operations
    bool add();
    QSqlQueryModel* read();
    bool update();
    bool remove(int id);
};

#endif // SPONSOR_H
