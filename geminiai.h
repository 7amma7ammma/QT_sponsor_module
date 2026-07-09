#ifndef GEMINIHANDLER_H
#define GEMINIHANDLER_H

#include "QtNetwork/qnetworkreply.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextEdit>

class GeminiHandler : public QObject
{
    Q_OBJECT
public:
    explicit GeminiHandler(QObject *parent = nullptr, QTextEdit *responseTextEdit = nullptr); // Declare constructor correctly
    void sendMessage(const QString &message);

    void sendMessageToGemini(const QString &userMessage);
    void processMessage(const QString &message) ;
signals:
    void responseReceived(const QString &response);
    void errorOccurred(const QString &error);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QTextEdit *chatResponseTextEdit;  // This is the member variable
    QNetworkAccessManager *manager;
};

#endif // GEMINAI_H
