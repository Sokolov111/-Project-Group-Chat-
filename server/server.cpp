#include "server.h"

Server::Server(QObject *parent) : QObject(parent){
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &Server::newConnection);

    if(server->listen(QHostAddress::Any, 1111)){ // Открывается порт 1111, который слушает любые подлючения ( любые хосты )
        qDebug() << "Server started. ";
    }else{
        qDebug() << "Server startion error !";
    }
}

void Server::newConnection(){ // При новом подключении
    data.clear();
    QTcpSocket *clientSocket = server->nextPendingConnection(); // Сервер слушает когда будет следующее подкл.
    clientsList.append(clientSocket); // Помеятить в массив со всеми клиентами.

    qDebug() << "New client conected !";
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readData); // Когда клиент отправляет данные.
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onDisconnect); // Когда клиент отключается.
}

void Server::readData(){ // Считать полученные данные.
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender()); // Обработчик сигналов.
    if(!clientSocket) return;

    QByteArray data = clientSocket->readAll(); // Считать все полученные данные.
    QDataStream in(&data, QIODevice::ReadOnly); // ?
    in.setVersion(QDataStream::Qt_5_9);

    QString receivedString;
    in >> receivedString;

    sendData(receivedString); // Отправить данные клиентам.
}

void Server::sendData(QString str){
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    out << str;

    QString countClientsText = "\n Количество клиентов: ";
    out << countClientsText;

    QString countClients = QString::number(clientsList.size());
    out << countClients;

    for (int i = 0; i < clientsList.size() ; i++){
        clientsList[i]->write(data); // Отправить каждому клиенту строку.
    }
}

void Server::onDisconnect(){
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if(!clientSocket) return;

    qDebug() << "Client Disconnected !";
    clientsList.removeAll(clientSocket);
    // clientsList.remove(clientSocket);
    clientSocket->deleteLater();
}
