#include "echoserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

EchoServer::EchoServer(quint16 port, QObject* parent) :
  QObject(parent),
  m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Echo Server"),
                                          QWebSocketServer::NonSecureMode, this))
{
  if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
      qDebug() << "Echoserver listening on port" << port;


    connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
            this, &EchoServer::onNewConnection);
    connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &EchoServer::closed);

  }
}

EchoServer::~EchoServer()
{
  m_pWebSocketServer->close();
  qDeleteAll(m_clients.begin(), m_clients.end());
}

void EchoServer::onNewConnection()
{
  QWebSocket* pSocket = m_pWebSocketServer->nextPendingConnection();

  connect(pSocket, &QWebSocket::textMessageReceived, this, &EchoServer::processTextMessage);
  connect(pSocket, &QWebSocket::binaryMessageReceived, this, &EchoServer::processBinaryMessage);
  connect(pSocket, &QWebSocket::disconnected, this, &EchoServer::socketDisconnected);

  m_clients << pSocket;
}

void EchoServer::processTextMessage(QString message)
{
  QWebSocket* pClient = qobject_cast<QWebSocket*>(sender());

    qDebug() << "Message received:" << message;

  if (pClient) {
    pClient->sendTextMessage(message);
  }
}

void EchoServer::processBinaryMessage(QByteArray message)
{
  QWebSocket* pClient = qobject_cast<QWebSocket*>(sender());

    qDebug() << "Binary Message received:" << message;

  if (pClient) {
    pClient->sendBinaryMessage(message);
  }
}

void EchoServer::socketDisconnected()
{
  QWebSocket* pClient = qobject_cast<QWebSocket*>(sender());
    qDebug() << "socketDisconnected:" << pClient;

  if (pClient) {
    m_clients.removeAll(pClient);
    pClient->deleteLater();
  }
}

