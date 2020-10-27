#include "connectionhandler.h"

#include <QDebug>
#include <QList>
#include <QEventLoop>
#include <QTimer>
#include <settingswrapper.cpp>

ConnectionHandler::ConnectionHandler(QObject *parent) :
    QObject(parent),
    serv(new QTcpServer(this))
{
    connect(serv, SIGNAL(newConnection()),
            this, SLOT(on_New_Connection()));
}

ConnectionHandler::~ConnectionHandler() {
    delete serv;
}


void ConnectionHandler::startServer() {
    SettingsWrapper settings(QDir(":/res"));
    const int port = settings.getOrSetValue("Connection/Port", 1232).toInt();
    if (serv->listen(QHostAddress::Any, port)){
        qDebug() << "Server listening";
    } else {
        qDebug() << "Server is not listening";
    }
}

void ConnectionHandler::stopServer() {
    serv->close();
    clients.clear();
}



void ConnectionHandler::on_New_Connection() {
    auto client = serv->nextPendingConnection();
    connectSocketSignals(client);
    qDebug() << "New connection try from" << getIPv4AddrString(client) << "...";
}



void ConnectionHandler::connectSocketSignals(QTcpSocket *socket) {
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(on_Socket_Ready_Read()));

    connect(socket, SIGNAL(disconnected()),
            this, SLOT(on_Client_Disconnection()));

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(on_Socket_Error(QAbstractSocket::SocketError)));
}



void ConnectionHandler::on_Client_Disconnection() {
    auto disconnectedSocket = static_cast<QTcpSocket *>(sender());
    auto username = clients.key(disconnectedSocket);
    clients.remove(username);
    username.setType(Disconnection);
    writeAboutUserDisconnection(username);
}



bool ConnectionHandler::isListening() const {
    return serv->isListening();
}



void
ConnectionHandler::on_Socket_Error(QAbstractSocket::SocketError) {
    auto socket = static_cast<QTcpSocket *>(sender());
    qDebug() << getIPv4AddrString(socket) << socket->errorString();
}



QString ConnectionHandler::getIPv4AddrString(QTcpSocket *socket) {
    QString IPv4IPv6 = socket->peerAddress().toString();
    QString IPv4 = IPv4IPv6.right(IPv4IPv6.size() - 7);
    return IPv4;
}



void ConnectionHandler::write(QTcpSocket *socket, const BaseData &data) {
        QByteArray datagram;
        QDataStream writeStream(&datagram, QIODevice::WriteOnly);
        writeStream << qint64(0) << (qint8)data.type();
        writeStream << data;
        writeStream.device()->seek(qint64(0));
        writeStream << qint64(datagram.size() - sizeof(qint64)); // writing data size
        socket->write(datagram);
        socket->readAll();
}


void ConnectionHandler::on_Socket_Ready_Read() {
    auto socket = static_cast<QTcpSocket *>(sender());
    QDataStream readStream(socket);

    QList<UserData> tmpqz;
    QList<QTcpSocket*> tmpq;

    for (auto e: clients.toStdMap()){
        tmpqz.push_back(e.first);
        tmpq.push_back(e.second);
   }

    if (flag == false){
        readStream >> sizeRead;
        readStream >> type;
        flag = true;
    }

    if (socket->bytesAvailable() < sizeRead - 1){
        auto socket2 = socket;
        tmparray.append(socket2->readAll());
    }

    if (type == DataType::Msg && tmparray.isEmpty()){
        readMessage(socket);
        sizeRead = 0;
        type = -1;
        flag = false;
    }

    if (type == DataType::Msg && tmparray.size() == sizeRead - 1){
                QDataStream messageStream(&tmparray, QIODevice::ReadWrite);
                messageStream >> tmpmessage;
                sendOutMessage(socket, tmpmessage);
                tmparray.clear();
                Message tmp2;
                tmpmessage = tmp2;
                sizeRead = 0;
                type = -1;
                flag = false;
    }


    /* waiting till we get full data */
    if (type == DataType::UserListResponse){
        for (int e = 0; e < tmpq.size(); e++){
            writeUserList(tmpq[e], clients.keys());
        }
        flag = false;
    }
    if (type == DataType::AuthRequest){
        readAuthRequest(socket);
        flag = false;
    }
}

void ConnectionHandler::readAuthRequest(QTcpSocket *socket) {
    QDataStream readStream(socket);
    UserData read;
    readStream >> read;

    bool flag = false;

    for (auto a : clients.keys()){
        if (a.getUsername() == read.getUsername()){
            flag = true;
        }
    }

    if (flag == false){
        writeAuthRequest(socket, read);
    } else {
        read.setType(DataType::NewConnection);
        writeAuthAnswer(socket, false);
    }
}

void ConnectionHandler::writeAuthRequest(QTcpSocket *socket, UserData read){
   clients.insert(read, socket);
   writeAuthAnswer(socket, true);
   read.setType(DataType::NewConnection);
}



void ConnectionHandler::writeAuthAnswer(QTcpSocket *socket, bool answer) {
    AuthAnswer ans(answer);
    write(socket, ans);
}



void ConnectionHandler::writeUserList(QTcpSocket *socket, const QList<UserData> &lst) {
    UserList participants(lst);
    write(socket, participants);
}


void ConnectionHandler::writeAboutUserDisconnection(const UserData &disconnectedUser) {
    for (auto &user : clients.values()) write(user, disconnectedUser);
}



void ConnectionHandler::sendOutMessage(QTcpSocket *sender, const Message &msg) {
    auto senderUsername = clients.key(sender).getUsername();
    for (auto &user : clients.values())
        if (clients.key(user).getUsername() != senderUsername)
            write(user, msg);
}


void ConnectionHandler::readMessage(QTcpSocket *socket) {
    QDataStream readStream(socket);
    Message read;
    readStream >> read;
    sendOutMessage(socket, read);
}
