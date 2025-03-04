#include "mainwindow.h"
#include "ui_mainwindow.h"

TCPClient::TCPClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    clientSocket = new QTcpSocket(this);

    connect(clientSocket, &QTcpSocket::connected, this, [=](){
        qDebug() << "Client connected !";
        connected = true;
    });
    connect(clientSocket, // В случае ошибки при подключении.
            static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this,
            &TCPClient::onClientError);

    connect(clientSocket, // При поступлении данных от сервера.
            &QTcpSocket::readyRead,
            this,
            &TCPClient::readData); // Считывание данных.

    connect(clientSocket, // При отключении сокета.
            &QTcpSocket::disconnected,
            this,
            &TCPClient::deleteLater); // Удаление при отключении клиента.

    clientSocket->connectToHost("127.0.0.1", 1111); // Подключение к локальному хосту на порт 1111.
}

TCPClient::~TCPClient()
{
    delete ui;
}

void TCPClient::onClientError(QAbstractSocket::SocketError socketError){
    qDebug() << "Error: " << socketError; // Отображение ошибки.
}

void TCPClient::SendMessageToServer(QString val){
    data.clear();

    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << val;

    clientSocket->write(data); // Записать строку в сокет.
}

void TCPClient::readData(){
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll();
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_9);

    QString receivedString;
    QString countClients;
    QString countClientsN;

    in >> receivedString;
    in >> countClients;
    in >> countClientsN;

    qDebug() << "Получено сообщение: " << receivedString;
    qDebug() << "Получено сообщение: " << countClients;
    qDebug() << "Получено сообщение: " << countClientsN;

    ui->RXTextBrowser->setPlainText(receivedString + countClients + countClientsN); // Записать строку в окно RX.
}

void TCPClient::on_sendTx_clicked() // Действие при нажатии на кнопку.
{
    SendMessageToServer(ui->TXlineEdit->text()); // Отправить в ф-ю текст из lineEdit
    ui->TXlineEdit->clear(); // Очистить окно RX.
}
