#include "geminiai.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProcessEnvironment>
#include <QDebug>

GeminiHandler::GeminiHandler(QObject *parent,QTextEdit *responseTextEdit)
    : QObject(parent),
    manager(new QNetworkAccessManager(this))
    ,
    chatResponseTextEdit(responseTextEdit)
{
    connect(manager, &QNetworkAccessManager::finished, this, &GeminiHandler::onReplyFinished);
}

void GeminiHandler::sendMessageToGemini(const QString &userMessage)
{
    const QString apiKey = qEnvironmentVariable("GEMINI_API_KEY");
    if (apiKey.isEmpty()) {
        emit errorOccurred("GEMINI_API_KEY is not set.");
        return;
    }

    QUrl url(QString("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=%1").arg(apiKey));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Build request JSON based on curl example
    QJsonObject part;
    part["text"] = userMessage;

    QJsonObject message;
    message["parts"] = QJsonArray{ part };

    QJsonObject payload;
    payload["contents"] = QJsonArray{ message };

    QJsonDocument doc(payload);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = manager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        this->onReplyFinished(reply);
    });
}

void GeminiHandler::onReplyFinished(QNetworkReply *reply)
{
    if (!reply) {
        qDebug() << "Reply object is null!";
        emit errorOccurred("Reply object is null.");
        return;
    }

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();

    qDebug() << "Status Code:" << statusCode;
    qDebug() << "Content-Type:" << contentType;

    if (statusCode != 200 || !contentType.contains("application/json")) {
        qDebug() << "Non-JSON or error response received.";
        emit errorOccurred("Unexpected response from Gemini.");
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();

    if (responseData.isEmpty()) {
        qDebug() << "Empty response received";
        emit errorOccurred("Empty response received from Gemini.");
        reply->deleteLater();
        return;
    }

    qDebug() << "Raw response:" << responseData;

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Invalid JSON response:" << parseError.errorString();
        emit errorOccurred("Invalid JSON format.");
        reply->deleteLater();
        return;
    }

    QJsonObject rootObj = jsonDoc.object();
    QJsonArray candidates = rootObj.value("candidates").toArray();

    if (candidates.isEmpty()) {
        qDebug() << "No candidates found in response";
        emit errorOccurred("No response candidates from Gemini.");
        reply->deleteLater();
        return;
    }

    QJsonObject content = candidates[0].toObject().value("content").toObject();
    QJsonArray parts = content.value("parts").toArray();

    if (parts.isEmpty()) {
        qDebug() << "No parts in content";
        emit errorOccurred("Empty response content.");
        reply->deleteLater();
        return;
    }

    QString responseText = parts[0].toObject().value("text").toString().trimmed();
    qDebug() << "Response text:" << responseText;

    emit responseReceived(responseText);

    reply->deleteLater();
}

void GeminiHandler::sendMessage(const QString &message)
{
    sendMessageToGemini(message);
}
