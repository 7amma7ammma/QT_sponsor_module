#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sponsor.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>
#include <QIntValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QKeyEvent>

#include <QString>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
//using namespace QtCharts;
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>
#include <QUrl>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "geminiai.h"

#include <qtranslator.h>
#include <QApplication>
#include <QFile>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), sponsor(new Sponsor())
{
    ui->setupUi(this);




    ui->id->installEventFilter(this);
    ui->name->installEventFilter(this);
    ui->organization->installEventFilter(this);
    ui->email->installEventFilter(this);
    ui->phone->installEventFilter(this);
    ui->contributionAmount->installEventFilter(this);
    ui->contractDetails->installEventFilter(this);
    ui->label->installEventFilter(this);

    geminiHandler = new GeminiHandler(this);

    connect(geminiHandler, &GeminiHandler::responseReceived, this, [=](const QString &reply) {
        ui->chatResponseTextEdit->append(reply);
    });

    // Connect UI elements to slots
    connect(ui->pb_afficher, &QPushButton::clicked, this, &MainWindow::displayTable);
    connect(ui->trier_id, &QPushButton::clicked, this, &MainWindow::on_pushButton_trier_id_clicked);
    connect(ui->trier_mount, &QPushButton::clicked, this, &MainWindow::on_pushButton_trier_mount_clicked);
    connect(ui->trier_name, &QPushButton::clicked, this, &MainWindow::on_pushButton_trier_name_clicked);
    connect(ui->recherche, &QPushButton::clicked, this, &MainWindow::on_pushButton_recherche_clicked);
    connect(ui->statistique_organisation, &QPushButton::clicked, this, &MainWindow::on_pushButton_statistique_organisation_clicked);
    connect(ui->exporter_p, &QPushButton::clicked, this, &MainWindow::on_pushButton_exporter_p_clicked);
    connect(ui->btnDarkMode, &QPushButton::clicked, this, &MainWindow::on_btnDarkMode_clicked);



    // Initialize the table view
    displayTable();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sponsor;
}



void MainWindow::on_pb_ajouter_clicked() {
    // Validate ID
    bool idValid;
    int id = ui->id->text().toInt(&idValid);
    /*if (!idValid || id <= 0) {
        QMessageBox::warning(this, tr("ID invalid"), tr("ID Must be a positive integer."), QMessageBox::Ok);

        return;
    }*/

    // Validate Name
    QString name = ui->name->text();
    QRegularExpression nameRegex("^[A-Za-z ]+$");
    if (name.isEmpty() || !nameRegex.match(name).hasMatch()) {
        QMessageBox::warning(this, tr("Invalid Name"), tr("Le nom ne doit contenir que des lettres et des espaces."), QMessageBox::Ok);
        return;
    }

    // Validate organization
    QString organization = ui->organization->text();
    QRegularExpression organizationRegex("^[A-Za-z ]+$");
    if (organization.isEmpty() || !organizationRegex.match(organization).hasMatch()) {
        QMessageBox::warning(this, tr("Type invalide"), tr("Le organization ne doit contenir que des lettres et des espaces."), QMessageBox::Ok);
        return;
    }

    // Validate email
    QString email = ui->email->text();
    QRegularExpression emailRegex("^[\\w\\.-]+@[\\w\\.-]+\\.[a-zA-Z]{2,4}$");
    if (email.isEmpty() || !emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, tr("Type invalide"), tr("L'email n'est pas valide."), QMessageBox::Ok);
        return;
    }

    // Validate phone number
    QString phone = ui->phone->text();
    QRegularExpression phoneRegex("^\\d{8,15}$");
    if (phone.isEmpty() || !phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, tr("Type invalide"), tr("Le numéro de téléphone doit être valide."), QMessageBox::Ok);
        return;
    }

    // Validate contributionAmount
    bool contribution_amountValid;
    double contribution_amount = ui->contributionAmount->text().toDouble(&contribution_amountValid);
    if (!contribution_amountValid || contribution_amount <= 0) {
        QMessageBox::warning(this, tr("Contribution Amount invalide"), tr("Le montant de la contribution doit être un nombre valide."), QMessageBox::Ok);
        return;
    }

    // Validate contract details (if required)
    QString contract_details = ui->contractDetails->text();
    if (contract_details.isEmpty()) {
        QMessageBox::warning(this, tr("Détails du contrat invalides"), tr("Les détails du contrat ne peuvent pas être vides."), QMessageBox::Ok);
        return;
    }

    // Validate renewal date
    QString renewal_date = ui->label->text();
    QRegularExpression renewal_dateRegex("^\\d{4}-\\d{2}-\\d{2}$");  // Date format yyyy-mm-dd
    if (renewal_date.isEmpty() /*|| !renewal_dateRegex.match(renewal_date).hasMatch()*/) {
        QMessageBox::warning(this, tr("Date de renouvellement invalide"), tr("La date de renouvellement doit être au format yyyy-mm-dd."), QMessageBox::Ok);
        return;
    }

    // Create a new Sponsor object and try to add it to the database
    Sponsor newSponsor(id, name, organization , email, phone, contribution_amount, contract_details , renewal_date);
    bool test = newSponsor.ajouter();

    if (test) {
        QMessageBox::information(this, tr("Success"), tr("Sponsor ajouté avec succès."), QMessageBox::Ok);
        displayTable();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("L'ajout du sponsor a échoué."), QMessageBox::Ok);
    }
}


void MainWindow::on_pb_supprimer_clicked() {
    int id = ui->id->text().toInt();


    if (!sponsor->checkIfExists(id)) {
        QMessageBox::critical(this, tr("Erreur"), tr("sponsor not found."), QMessageBox::Ok);
        return;
    }


    bool test = sponsor->supprimer(id);

    if (test) {
        QMessageBox::information(this, tr("Success"), tr("sponsor Deleted Successfully."), QMessageBox::Ok);
        displayTable();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("sponsor Couldn't Be Deleted."), QMessageBox::Ok);
    }
}

void MainWindow::on_pb_modifier_clicked() {
    // Validate ID
    bool idValid;
    int id = ui->id->text().toInt(&idValid);
   /* if (!idValid || id <= 0) {
        QMessageBox::warning(this, tr("ID invalide"), tr("L'ID doit être un nombre entier positif."), QMessageBox::Ok);
        return;
    }*/

    // Validate Name
    QString name = ui->name->text();
    QRegularExpression nameRegex("^[A-Za-z ]+$");  // Use QRegularExpression
    if (name.isEmpty() || !nameRegex.match(name).hasMatch()) {
        QMessageBox::warning(this, tr("Nom invalide"), tr("Le nom ne doit contenir que des lettres et des espaces."), QMessageBox::Ok);
        return;
    }


    // Validate organization
    QString organization = ui->organization->text();
    QRegularExpression organizationRegex("^[A-Za-z ]+$");  // Use QRegularExpression
    if (organization.isEmpty() || !organizationRegex.match(organization).hasMatch()) {
        QMessageBox::warning(this, tr("Type invalide"), tr("Le organization ne doit contenir que des lettres et des espaces."), QMessageBox::Ok);
        return;
    }

    QString email = ui->email->text();
    QRegularExpression emailRegex("^[\\w\\.-]+@[\\w\\.-]+\\.[a-zA-Z]{2,4}$");
      // Use QRegularExpression
    if (email.isEmpty() || !emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, tr("Type invalide"), tr("Le email ne doit contenir que des lettres et des espaces."), QMessageBox::Ok);
        return;
    }

    QString phone = ui->phone->text();
    QRegularExpression phoneRegex("^\\d{8,15}$");  // Allows 8-15 digits
      // Use QRegularExpression
    if (phone.isEmpty() || !phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, tr("Type invalide"), tr("Le phone ne doit contenir que des lettres et des espaces."), QMessageBox::Ok);
        return;
    }

    // Validate contributionAmount
    bool contribution_amountValid;
    double contribution_amount = ui->contributionAmount->text().toInt(&contribution_amountValid);
    if (!contribution_amountValid || id <= 0) {
        QMessageBox::warning(this, tr(" contributionAmount invalide"), tr("L'contributionAmount doit être un nombre entier positif."), QMessageBox::Ok);
        return;
    }

    QString contract_details = ui->contractDetails->text();
    QRegularExpression contract_detailsRegex("^[A-Za-z ]+$");  // Use QRegularExpression
    if (contract_details.isEmpty() /*|| !contract_detailsRegex.match(contract_details).hasMatch()*/) {
        QMessageBox::warning(this, tr("Type invalide"), tr("Le contract_details ne doit contenir que des lettres et des espaces."), QMessageBox::Ok);
        return;
    }
    QString renewal_date = ui->label->text();
    QRegularExpression renewal_dateRegex("^[A-Za-z ]+$");  // Use QRegularExpression
    if (renewal_date.isEmpty() /*||!renewal_dateRegex.match(renewal_date).hasMatch()*/) {
        QMessageBox::warning(this, tr("Type invalide"), tr("Le renewal_date ne doit contenir que des lettres et des espaces."), QMessageBox::Ok);
        return;
    }


    // Update the sponsor
    sponsor->setId(id);
    sponsor->setName(name);
    sponsor->setOrganization(organization);
    sponsor->setEmail(email);
    sponsor->setPhone(phone);
    sponsor->setPhone(phone);
    sponsor->setContributionAmount(contribution_amount);
    sponsor->setContractDetails(contract_details);
    sponsor->setRenewalDate(renewal_date);

    bool test = sponsor->modifier(id);

    if (test) {
        QMessageBox::information(this, tr("Success"), tr("modification Was Successful."), QMessageBox::Ok);
        displayTable();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Modification Error."), QMessageBox::Ok);
    }
}

void MainWindow::on_tab_equipement_clicked(const QModelIndex &index) {
    int row = index.row();
    ui->id->setText(ui->table_sponsor->model()->index(row, 0).data().toString());
    ui->name->setText(ui->table_sponsor->model()->index(row, 1).data().toString());
    ui->organization->setText(ui->table_sponsor->model()->index(row, 2).data().toString());
    ui->email->setText(ui->table_sponsor->model()->index(row, 3).data().toString());
    ui->phone->setText(ui->table_sponsor->model()->index(row, 4).data().toString());
    ui->contributionAmount->setText(ui->table_sponsor->model()->index(row, 5).data().toString());
    ui->contractDetails->setText(ui->table_sponsor->model()->index(row, 7).data().toString());
    ui->label->setText(ui->table_sponsor->model()->index(row, 8).data().toString());
}

void MainWindow::displayTable() {
    QSqlQueryModel *model = sponsor->afficher();
    ui->table_sponsor->setModel(model);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // Check if the event is for the ID field
        if (obj == ui->id) {
            // Allow only digits and backspace
            if (!keyEvent->text().isEmpty() && !keyEvent->text().at(0).isDigit()) {
                QMessageBox::warning(this, tr("ID invalid"), tr("The ID must only include numbers."), QMessageBox::Ok);
                return true;  // Block the event
            }
        }

        // Check if the event is for the Name field
        if (obj == ui->name) {
            // Allow only alphabetic characters and spaces
            QRegularExpression nameRegex("^[A-Za-z ]*$");  // Use QRegularExpression
            if (!keyEvent->text().isEmpty() && !nameRegex.match(keyEvent->text()).hasMatch()) {
                QMessageBox::warning(this, tr("Invalid Name"), tr("The name must only include letters and spaces."), QMessageBox::Ok);
                return true;  // Block the event
            }
        }

        // Check if the event is for the Type field
        if (obj == ui->organization) {
            // Allow only alphabetic characters and spaces
            QRegularExpression organizationRegex("^[A-Za-z ]*$");  // Use QRegularExpression
            if (!keyEvent->text().isEmpty() && !organizationRegex.match(keyEvent->text()).hasMatch()) {
                QMessageBox::warning(this, tr("Invalid Type"), tr("The organization must only include letters and spaces"), QMessageBox::Ok);
                return true;  // Block the event
            }
        }
    }

    // Pass the event to the base class
    return QMainWindow::eventFilter(obj, event);
}




//beheee
void MainWindow::on_pushButton_trier_name_clicked() {
    Sponsor S ;
    ui->table_sponsor->setModel(S.afficherTrieParNom());
    QMessageBox::information(this, "Succès", "Tri effectué par nom dans l'ordre croissant.");
}
void MainWindow::on_pushButton_trier_id_clicked() {
    Sponsor S ;
    QSqlQueryModel* model = S.afficherTrieParId();
    ui->table_sponsor->setModel(model); // Assurez-vous que tableView est le bon nom de votre vue
    QMessageBox::information(this, "Succès", "Tri effectué par quantite de pièce dans l'ordre croissant.");
}

void MainWindow::on_pushButton_trier_mount_clicked() {
    Sponsor S ;
    QSqlQueryModel* model = S.afficherTrieParMount();
    ui->table_sponsor->setModel(model); // Assurez-vous que tableView est le bon nom de votre vue
    QMessageBox::information(this, "Succès", "Tri effectué par reference dans l'ordre croissant.");
}





void MainWindow::on_pushButton_recherche_clicked()
{
    // Récupérer la valeur saisie dans le lineEdit
    int ID = ui->lineEdit_recherche->text().toInt();

    // Construire la requête SQL
    QSqlQuery query;
    query.prepare("SELECT * FROM HAMA.SPONSORS WHERE ID = :id");
    query.bindValue(":id", ID);
    query.exec();

    // Créer un nouveau modèle et l'affecter au QTableView
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);
    ui->table_sponsor->setModel(model);


}


void MainWindow::on_pushButton_statistique_organisation_clicked()
{
    // Préparer la requête pour obtenir les données nécessaires
    QSqlQuery query;
    query.prepare("SELECT ORGANIZATION, COUNT(*) AS count FROM HAMA.SPONSORS GROUP BY ORGANIZATION");

    // Exécuter la requête et vérifier s'il y a des résultats
    if (!query.exec()) {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de l'obtention des statistiques : ") + query.lastError().text());
        return;
    }

    // Stocker les données dans une map pour compter les pièces par categorie
    QMap<QString, int> quantitesParOrganisation;
    while (query.next()) {
        QString organisation = query.value(0).toString();
        int count = query.value(1).toInt();
        quantitesParOrganisation[organisation] = count;
    }

    // Vérifier qu'il y a des données pour générer les statistiques
    if (quantitesParOrganisation.isEmpty()) {
        QMessageBox::information(this, tr("Aucune donnée"), tr("Aucune donnée disponible pour générer les statistiques."));
        return;
    }

    // Créer une série de camembert et remplir avec les données
    QPieSeries *series = new QPieSeries();
    int total = 0;
    for (auto count : quantitesParOrganisation.values()) {
        total += count;
    }

    // Fill the pie series with percentage labels
    for (auto it = quantitesParOrganisation.begin(); it != quantitesParOrganisation.end(); ++it) {
        QString organisation = it.key();
        int count = it.value();
        qreal percent = (static_cast<qreal>(count) / total) * 100.0;

        QString label = QString("%1 (%2%)").arg(organisation).arg(QString::number(percent, 'f', 1));
        series->append(label, count);

    }

    // Rendre les labels visibles pour chaque part de camembert
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
    }

    // Créer un objet QChart et ajouter la série de camembert
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("STAT BY ORGANISATION");
    chart->legend()->setAlignment(Qt::AlignRight);

    // Afficher le graphique dans un QChartView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Supprimer l'ancien layout de widget_stat (si existant)
    if (ui->widget_stat->layout() != nullptr) {
        QLayout *oldLayout = ui->widget_stat->layout();
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();  // Supprimer les widgets du layout
            delete item;
        }
        delete oldLayout;
    }

    // Ajouter un nouveau layout contenant le graphique
    QVBoxLayout *layout = new QVBoxLayout(ui->widget_stat);
    layout->addWidget(chartView);
    ui->widget_stat->setLayout(layout);
}



void MainWindow::on_pushButton_exporter_p_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "save PDF", "", "PDF (*.pdf)");
    if (fileName.isEmpty()) {
        QMessageBox::information(this, "Annulation", " the export has been canceled.");
        return;
    }
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(4, 4, 4, 4));
    QPainter painter(&pdfWriter);
    const QString logoPath = QDir(QApplication::applicationDirPath()).filePath("assets/logo2.png");
    if (QFile::exists(logoPath)) {
        QImage logo(logoPath);
        QRect logoRect(30, 30, 2000, 2000);
        painter.drawImage(logoRect, logo);
    }
    QFont titleFont("Times New Roman",15, QFont::Bold);
    painter.setFont(titleFont);
    QRect titleRect(200, logoRect.bottom()+50, pdfWriter.width() - 300, 80);
    painter.drawText(titleRect, Qt::AlignCenter, "LIST OF SPONSORS");
    QFont font("Times New Roman", 8);
    painter.setFont(font);
    QTableView *tableView = ui->table_sponsor;
    QAbstractItemModel *model = tableView->model();
    int rowCount = model->rowCount();
    int columnCount = model->columnCount();
    int pageWidth = pdfWriter.width() - 60;
    QVector<int> columnWidths(columnCount);
    int totalColumnWidth = pageWidth;
    for (int col = 0; col < columnCount; ++col) {
        columnWidths[col] = totalColumnWidth / columnCount;
    }
    int x = 30;
    int y = titleRect.bottom() + 10;
    for (int col = 0; col < columnCount; ++col) {
        QRect rect(x, y, columnWidths[col], 200);
        painter.drawRect(rect);
        painter.drawText(rect, Qt::AlignCenter, model->headerData(col, Qt::Horizontal).toString());
        x += columnWidths[col];
    }
    y += 700;
    for (int row = 0; row < rowCount; ++row) {
        x = 50;
        for (int col = 0; col < columnCount; ++col) {
            QRect rect(x, y, columnWidths[col], 700);
            painter.drawRect(rect);
            QString cellText = model->data(model->index(row, col)).toString();
            painter.drawText(rect, Qt::AlignCenter, cellText);
            x += columnWidths[col];
        }
        y += 500;
    }

    painter.end();
    QMessageBox::information(this, "Succès", "pdf has been succefully saved.");
}
//
void MainWindow::onSendMessage()
{
    QString userMessage = ui->chatInputLineEdit->text();
    ui->chatInputLineEdit->clear();

    if (userMessage.isEmpty())
        return;

    ui->chatResponseTextEdit->append("You: " + userMessage);

    geminiHandler->sendMessage(userMessage);

}

void MainWindow::on_sendMessageButton_clicked()
{
    QString userMessage = ui->chatInputLineEdit->text();  // Adjust widget name
    if (!userMessage.isEmpty()) {
        geminiHandler->sendMessageToGemini(userMessage);
    }
}

void MainWindow::onResponseReceived(const QString &reply)
{
    ui->chatResponseTextEdit->append("Bot: " + reply);  // Display bot response in chat
}

void MainWindow::on_ENG_clicked()
{
    qApp->removeTranslator(&translator); // remove any existing translator
    bool loaded = translator.load(QDir(QApplication::applicationDirPath()).filePath("translations/Atelier_Connexion_en.qm"));
    qDebug() << "EN loaded:" << loaded;
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
}

void MainWindow::on_FR_clicked()
{
    qApp->removeTranslator(&translator);
    bool loaded = translator.load(QDir(QApplication::applicationDirPath()).filePath("translations/Atelier_Connexion_fr.qm"));
    qDebug() << "FR loaded:" << loaded;
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
}

void MainWindow::on_AR_clicked()
{
    qApp->removeTranslator(&translator);
    bool loaded = translator.load(QDir(QApplication::applicationDirPath()).filePath("translations/Atelier_Connexion_ar.qm"));
    qDebug() << "AR loaded:" << loaded;
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
}


//dark mode
QString MainWindow::loadStyleSheet(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "❌ Failed to open:" << filePath;
        return "";
    }

    QTextStream in(&file);
    return in.readAll();
}

void MainWindow::on_btnDarkMode_clicked()
{
    QString style = loadStyleSheet(QDir(QApplication::applicationDirPath()).filePath("styles/dark.qss"));

    if (style.isEmpty()) {
        qDebug() << "❌ Dark stylesheet not found!";
        return;
    }

    qApp->setStyleSheet("");
    qApp->setStyleSheet(style);
}
void MainWindow::on_btnLightMode_clicked()
{
    QString style = loadStyleSheet(QDir(QApplication::applicationDirPath()).filePath("styles/light.qss"));

    if (style.isEmpty()) {
        qDebug() << "❌ Light stylesheet not found!";
        return;
    }

    qApp->setStyleSheet("");
    qApp->setStyleSheet(style);
}

