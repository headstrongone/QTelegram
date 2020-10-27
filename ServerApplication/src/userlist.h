#ifndef USERLIST_H
#define USERLIST_H

#include "userdata.h"
//! Класс для хранения данных о всех пользователях, находящийхся в сети.
class UserList : public BaseData
{
public:
    //! Конструктор по умолчанию.
    UserList() = default;

    //! Конструктор с параметром ответа.
    UserList(const QList<UserData> &users);

    //! Переопределение метода toStream.
    virtual QDataStream& toStream(QDataStream &stream) const override;

    //! Переопределение метода fromStream.
    virtual QDataStream& fromStream(QDataStream &stream) override;

    //! Переопределение метода type.
    virtual DataType type() const override;

    //! Добавить клиента в массив.
    void setUser(const UserData &user);

    //! Получение списка клиентов.
    QList<UserData> getUsers() const;

private:

    //! Массив клиентов.
    QList<UserData> users;

};
#endif // USERLIST_H
