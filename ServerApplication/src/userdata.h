#ifndef USERDATA_H
#define USERDATA_H

#include <QFont>
#include <QColor>

#include "basedata.h"
//!Основной класс для хранения данных о пользователе.
class UserData : public BaseData
{

public:
    //! Конструктор по умолчанию.
    UserData();

    //! Конструктор с параметрами.
    UserData(const QString &username);

    //! Установить имя пользователя.
    void setUsername(const QString &username);

    //! Установить пароль пользователя.
    void setPassword(const QString &password);

    //! Установить тип данных пользователя.
    void setType(const DataType type);

    //! Получить логин пользователя.
    QString getUsername() const;

    //! Получить пароль пользователя.
    QString getPassword() const;

    //! Переопределение метода toStream.
    virtual QDataStream& toStream(QDataStream &stream) const override;

    //! Переопределение метода fromStream.
    virtual QDataStream& fromStream(QDataStream &stream) override;

    //! Переопределение метода type.
    virtual DataType type() const override;


private:

    //! Имя пользователя.
    QString username;

    //! Пароль пользователя
    QString password;

    //! Тип данных.
    DataType currentType;
};

//! Перегрузка оператора <.
bool operator <(const UserData &tmp1, const UserData &tmp2);


#endif // USERDATA_H
