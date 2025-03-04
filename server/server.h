#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDataStream>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

private:
    QTcpServer *server;
    QVector <QTcpSocket*> clientsList;
    QByteArray data;

    void sendData(QString str);

private slots:
    void newConnection();

public slots:

    // void incomingConnection(int socketDescriptor);
    void readData();
    void onDisconnect();
};

#endif // SERVER_H
