#include "echoclient.h"
#include <QtCore/QDebug>

EchoClient::EchoClient(const QUrl &url, QObject* parent) :
  QObject(parent),
  m_url(url)
{
    qDebug() << "WebSocket server:" << url;
  connect(&m_webSocket, &QWebSocket::connected, this, &EchoClient::onConnected);
  connect(&m_webSocket, &QWebSocket::disconnected, this, &EchoClient::closed);
  m_webSocket.open(QUrl(url));
}

void EchoClient::onConnected()
{
    qDebug() << "WebSocket connected";
  connect(&m_webSocket, &QWebSocket::textMessageReceived,
          this, &EchoClient::onTextMessageReceived);
  m_webSocket.sendTextMessage(QStringLiteral("Dimka - Kartinka!!!"));
}

void EchoClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;
  m_webSocket.close();
}

