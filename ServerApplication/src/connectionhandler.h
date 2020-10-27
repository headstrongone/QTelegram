#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QTcpServer>
#include <QTcpSocket>

#include <QMap>
#include <QString>
#include <iostream>
#include <fstream>

#include "authanswer.h"
#include "userlist.h"
#include "userdata.h"
#include "message.h"

//! Основной класс сервера
class ConnectionHandler : public QObject
{
    Q_OBJECT
public:
    //! Конструктор
    explicit ConnectionHandler(QObject *parent = nullptr);

    //! Запуск сервера
    void startServer();

    //! Остановка сервера
    void stopServer();

    //! Проверка готовности сервера
    bool isListening() const;

    /*!
     * \brief Запись и отправка клиенту данных, передаваемых сервером.
     * \param socket Дейтаграммный сокет, для осуществления обмена пакетами данных.
     * \param from Данные, отправляемые сервером пользователю.
     */
    void write(QTcpSocket *socket, const BaseData &from);

    //! Деструктор
    ~ConnectionHandler();

private slots:
    //! Принимает сигнал попытки подключения нового пользователя.
    void on_New_Connection();
    //! Удаляет данные отключившегося пользователя из базы, после чего передает информацию об отключении всем пользователям в сети.
    void on_Client_Disconnection();
    //!  Ловим ошибки сокета
    void on_Socket_Error(QAbstractSocket::SocketError);
    //!  Сервер принимает данные, отправленные клиентом. В зависимости от их типа, данные обрабатываются и уходят дальше.
    void on_Socket_Ready_Read();

private:
    //! Проверка на валидность пользователя. Если логин занят - сокет будет закрыт, доступ не откроется.
    void readAuthRequest(QTcpSocket *socket);

    //! Занесение нового пользователя в базу. Если проверка на валидность прошла  успешно - даем добро на подключение. Сообщаем клиентам о новом подключении.
    void writeAuthRequest(QTcpSocket *socket, UserData read);

    //! Отправляем клиентам данные о новом пользователе.
    void writeAuthAnswer(QTcpSocket *socket, bool answer);

    //! Считываем сообщение, которое отправил клиент.
    void readMessage(QTcpSocket *socket);

    //! Высылаем сообщение от одного клиента всем остальным, кроме отправителя.
    void sendOutMessage(QTcpSocket *sender, const Message &msg);

    //! Метод для отправки базы юзеров всем клиентам.
    void writeUserList(QTcpSocket *socket, const QList<UserData> &lst);

    //! Метод для уведомления всех юзеров о том, что один из пользователей отключился от сервера.
    void writeAboutUserDisconnection(const UserData &disconnectedUser);

    //! Переменна хранения IPv4 адрессов.
    QString getIPv4AddrString(QTcpSocket *socket);

    //! Сигналы для сокета.
    void connectSocketSignals(QTcpSocket *socket);


private:
    //! База клиентов.
    QMap<UserData, QTcpSocket*> clients;

    //! Флаг на сообщение.
    bool flag = false;

    //! Сообщение, принятое клиентом. Содержит в себе фотографии больше 50 кб.
    Message tmpmessage;

    //!Массив для принятия пакетов от клиента. Собирает кусочки пакетов в одно целое.
    QByteArray tmparray;

    //! Переменная содержит размер сообщения, переваемого клиентом.
    qint64 sizeRead = 0;

    //!Переменная содержит тип данных, которые поступили в данных момент от клиента.
    qint8 type = -1;

    //! Сервер.
    QTcpServer *serv;
};

#endif // CONNECTIONHANDLER_H
