#include "userlist.h"

#include <QDebug>
UserList::UserList(const QList<UserData> &users) :
    users(users)
{ }


QDataStream& UserList::toStream(QDataStream &stream) const {
    stream << users;
    return stream;
}

QDataStream& UserList::fromStream(QDataStream &stream) {
    stream >> users;
    return stream;
}

void UserList::setUser(const UserData &user){
    users.push_back(user);
}

DataType UserList::type() const {
    return UserListResponse;
}

QList<UserData> UserList::getUsers() const {
  return users;
}
