#include "netservice.h"

#include <QtDebug>
#include <QBuffer>

NetService::NetService(QObject *parent)
    : QObject(parent),
    menu(new AuthMenu()),
    chat(new ChatWidget()),
    socket(new QTcpSocket(this)),
    ctimer(new QTimer(this))
{
    menu->show();
    chat->setVisible(false);

    connectSocketSignals();
    connect(menu, SIGNAL(signIn(QString, QString, int)), this, SLOT(onSignIn(QString, QString, int)));
    connect(ctimer, SIGNAL(timeout()), this, SLOT(onConnectionTimeOut()));
    connect(chat, SIGNAL(messageSent(Message&)), this, SLOT(onMessageSent(Message&)));
    connect(chat, SIGNAL(Disconnectbutton()), this, SLOT(onDisconnectbutton()));
}

NetService::~NetService(){
    delete menu;
    delete chat;
}


void NetService::connectSocketSignals() {
    connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

void NetService::onDisconnectbutton(){
     socket->abort();
     menu->setStatus("You are succesfully disconnected");
     menu->show();
     chat->close();
     return;
}


void NetService::onSignIn(const QString &username, const QString &password, const int &_port) {
    identifier.setUsername(username);
    identifier.setPassword(password);
    socket->connectToHost(QHostAddress::LocalHost, _port);
    // Create 10 seconds timer for waiting server response
    ctimer->start(1000);
}


void NetService::onSocketConnected() {
    ctimer->stop();
    identifier.setType(DataType::AuthRequest);
    write(socket, identifier);
}


void NetService::onSocketDisconnected() {
    socket->abort();
}


void NetService::onConnectionTimeOut() {
    ctimer->stop();
    menu->setStatus("Connection timeout.");
    socket->abort();
    return;
}


void NetService::readUserList(QDataStream &readStream) {
    UserList lst;
    readStream >> lst;
    chat->addParticipants(lst);

}

void NetService::readDisconnection(QDataStream &readStream) {
    UserData user;
    readStream >> user;
    chat->removeParticipant(user.getUsername());
}

void NetService::readMessage(QDataStream &readStream) {
    Message msg;
    readStream >> msg;
    chat->addMessage(msg);
}

void NetService::write(QTcpSocket *socket, const BaseData &data) {
    QByteArray datagram;
    QDataStream writeStream(&datagram, QIODevice::WriteOnly);
    writeStream << qint64(0) << (qint8)data.type();
    writeStream << data;
    writeStream.device()->seek(qint64(0));
    writeStream << qint64(datagram.size() - sizeof(qint64));
    socket->write(datagram);
}


void NetService::readAuthAnswer(QDataStream &readStream) {
    AuthAnswer ans;
    readStream >> ans;
    if (!ans.isSigned()) {
        socket->abort();
        menu->setStatus("Authentication error. Username is already in use.");
        return;
    }
    identifier.setType(DataType::UserListResponse);
    write(socket, identifier);;
    chat->setInfoAboutUser(identifier);
    menu->close();
    chat->show();
}


void NetService::onSocketReadyRead() {
    auto socket = static_cast<QTcpSocket *>(sender());
    QDataStream readStream(socket);

    if (flagmessage == false){
        readStream >> sizeRead;
        readStream >> type;
        flagmessage = true;
    }

    if (socket->bytesAvailable() < sizeRead - 1){
        auto socket2 = socket;
        tmparray.append(socket2->readAll());
    }

    if (type == DataType::Msg && tmparray.isEmpty()){
        readMessage(readStream);
        sizeRead = 0;
        type = -1;
        flagmessage = false;
    }

    if (type == DataType::Msg && tmparray.size() == sizeRead - 1){
                QDataStream messageStream(&tmparray, QIODevice::ReadWrite);
                messageStream >> tmpmessage;
                chat->addMessage(tmpmessage);
                tmparray.clear();
                Message tmpmessage2;
                tmpmessage = tmpmessage2;
                sizeRead = 0;
                type = -1;
                flagmessage = false;
    }

    if (type == DataType::AuthResponse) {
        readAuthAnswer(readStream);
        flagmessage = false;
    }
    if (type == DataType::UserListResponse) {
        readUserList(readStream);
        flagmessage = false;
    }
    if (type == DataType::NewConnection) {
        //readNewConnection(readStream);
        flagmessage = false;
    }
    if (type == DataType::Disconnection) {
        readDisconnection(readStream);
        flagmessage = false;
    }
}



void NetService::onMessageSent(Message &msg){
    msg.setSender(identifier.getUsername());
    write(socket, msg);
}
