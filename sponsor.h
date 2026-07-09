#ifndef SPONSOR_H
#define SPONSOR_H

#include <QString>
#include <QSqlQueryModel>

class Sponsor {
public:
    Sponsor();

    Sponsor(int, QString, QString, QString, QString, double, QString, QString);

    int getId() const;
    QString getName() const;
    QString getOrganization() const;
    QString getEmail() const;
    QString getPhone() const;
    double getContributionAmount() const;
    QString getContractDetails() const;
    QString getRenewalDate() const;

    void setId(int);
    void setName(const QString);
    void setOrganization(QString);
    void setEmail(QString);
    void setPhone( QString);
    void setContributionAmount(double);
    void setContractDetails(QString);
    void setRenewalDate( QString);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool checkIfExists(int);
    bool modifier(int id);


    QSqlQueryModel* afficherTrieParNom();
    QSqlQueryModel* afficherTrieParId();
    QSqlQueryModel* afficherTrieParMount();

private:
    int id;
    QString name;
    QString organization;
    QString email;
    QString phone;
    double contribution_amount;
    QString contract_details;
    QString renewal_date;
};


#endif // SPONSOR_H
