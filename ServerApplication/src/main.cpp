#include <iostream>
#include <QCoreApplication>
#include "connectionhandler.h"
#include <QNetworkInterface>


int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);

    ConnectionHandler netmanager;
    netmanager.startServer();

    std::cout << "This is Qt-Chat server console.\n"
                 "Server started at localhost.\n";

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
             qDebug() << address.toString();
             qDebug() << localhost.toString();
    }

    return app.exec();
}
