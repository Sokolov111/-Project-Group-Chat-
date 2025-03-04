#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class TCPClient : public QMainWindow
{
    Q_OBJECT

public:
    TCPClient(QWidget *parent = nullptr);
    ~TCPClient();

    QTcpSocket *clientSocket;
    QByteArray data;

private slots:
    void on_sendTx_clicked();
    void onClientError(QAbstractSocket::SocketError socketError);

private:
    Ui::MainWindow *ui;
    void readData();
    void SendMessageToServer(QString val);
    bool connected = false;
};
#endif // MAINWINDOW_H
