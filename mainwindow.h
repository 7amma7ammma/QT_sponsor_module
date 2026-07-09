#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDate>
#include "qtranslator.h"
#include "sponsor.h"
#include "geminiai.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_ajouter_clicked();
    void on_pb_supprimer_clicked();
    void on_pb_modifier_clicked();
    void on_tab_equipement_clicked(const QModelIndex &index);
    void displayTable();
    bool eventFilter(QObject *obj, QEvent *event);
    void on_pushButton_trier_name_clicked();
    void on_pushButton_trier_mount_clicked();
    void on_pushButton_trier_id_clicked();
    void on_pushButton_recherche_clicked();
    void on_pushButton_statistique_organisation_clicked();
    void on_pushButton_exporter_p_clicked();
    void onSendMessage();
    void on_sendMessageButton_clicked();
    void onResponseReceived( const QString &reply);
    void on_FR_clicked();
    void on_AR_clicked();
    void on_ENG_clicked();
    void on_btnDarkMode_clicked();
    void on_btnLightMode_clicked();

private:
    Ui::MainWindow *ui;
    Sponsor *sponsor;
    GeminiHandler *geminiHandler;
    QTranslator translator;
    QString loadStyleSheet(const QString &filePath);



};

#endif // MAINWINDOW_H
