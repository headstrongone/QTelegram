#ifndef NETSERVICE_H
#define NETSERVICE_H

#include "authmenu.h"
#include "chatwidget.h"

#include "../../ServerApplication/src/authanswer.h"
#include "../../ServerApplication/src/userdata.h"
#include "../../ServerApplication/src/userlist.h"
#include "../../ServerApplication/src/connectionhandler.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QEventLoop>

//!  Основной класс для передачи/приема информации между клиентом и сервером.
class NetService : QObject
{
    Q_OBJECT
public:
    //! Конструктор Qt.
    explicit NetService(QObject *parent = nullptr);

    //! Деструктор Qt.
    virtual ~NetService();

    //! Запись и отправка на сервер данных, передаваемых клиентом.
    void write(QTcpSocket *socket, const BaseData &data);

private slots:

    //! Метод, вызываемый сразу же после нажатия кпопки подключения. Подключается к серверу.
    void onSignIn(const QString &username, const QString &password, const int &_port);

    //! Метод, вызываемый после нажатия кнопки отключения от сервера. Закрывает сокет, отключает от сервера.
    void onDisconnectbutton();

    //! Если от сервера пришло сообщение об успешном подключение, отправляет серверу данные о пользователе.
    void onSocketConnected();

    //! Если от сервера пришло сообщение об отключении пользователя, закрывает сокет.
    void onSocketDisconnected();

    //! Если не удалось передать пакеты серверу, выдает сообщение "Connection Timeout. " в меню индетификации и закрывает сокет.
    void onConnectionTimeOut();

    //! Принимает сообщение от сервера, определяя какой тип данных от сервера получен. В следствии, вызывает один из описанных методов, для записи данных
    //! в окно чата для пользователя.
    void onSocketReadyRead();

    //! Передает сообщение, которое пользователь отправляет, в метод write(), описанный раннее.
    void onMessageSent(Message &msg);

private:

    //! Содержит сигналы, передаваемые через сокет от сервера к пользователю.
    void connectSocketSignals();


    //! Принимает поток данных от сервера, содержащий в себе список всех пользователей, подключенный к серверу.
    void readUserList(QDataStream &readStream);

    //! Принимает поток данных от сервера, содержащий в себе ответ разрешение на подключение от сервера.
    void readAuthAnswer(QDataStream &readStream);

    //! Принимает поток данных от сервера, содержащий в себе информацию о новом подключившимся пользователе.
    void readNewConnection(QDataStream &readStream);

    //! Принимает поток данных от сервера, содержащий в себе информацию об отключившимся пользователе.
    void readDisconnection(QDataStream &readStream);

    //! Принимает поток данных от сервера, содержащий в себе сообщение, отправленное любым из подключившихся пользователей.
    void readMessage(QDataStream &readStream);

private:

    //! Объект класса окна идентификации AuthMenu.
    AuthMenu *menu;

    //! Объекта класса окна чата ChatWidget.
    ChatWidget *chat;

    //! Переменная содержит размер сообщения, переваемого серверу.
    qint64 sizeRead = 0;

    //!Переменная содержит тип данных, которые поступили в данных момент от сервера.
    qint8 type = -1;

    //!Флаг на сообщение.
    bool flagmessage = false;

    //!Массив для принятия пакетов от сервера. Собирает кусочки пакетов в одно целое.
    QByteArray tmparray;

    //! Сообщение, принятое от сервера. Содержит в себе фотографии больше 50 кб.
    Message tmpmessage;

    //! Сокет для обмена пакетами между клиентом и сервером.
    QTcpSocket *socket;

    //! Таймер подключения.
    QTimer *ctimer; // connection timer

    //! Информация о пользователе.
    UserData identifier;
};

#endif // NETSERVICE_H
