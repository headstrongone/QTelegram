#ifndef BASEDATA_H
#define BASEDATA_H

#include <QDataStream>

//!Перечисление значений qint8. Используются как флаги для данных (какие данные в данный момент передаются) при работе между клиентом и сервером.
enum DataType : qint8
{
    None,
    Ping,
    AuthRequest,
    AuthResponse,
    UserListResponse,
    NewConnection,
    Disconnection,
    Msg // 'Message' reduction
};

//!Класс для передачи перечислений.
class BaseData
{
public:
    //!Конструктор
    explicit BaseData() = default;

    //! Перегрузка оператора >>.
    friend QDataStream &operator >> (QDataStream &stream, BaseData &data);

    //! Перегрузка оператора <<.
    friend QDataStream &operator << (QDataStream &stream, const BaseData &data);

    //! Переопределение метода toStream.
    virtual QDataStream& toStream(QDataStream &stream) const = 0;

    //! Переопределение метода fromStream.
    virtual QDataStream& fromStream(QDataStream &stream) = 0;

    //! Переопределение метода type.
    virtual DataType type() const  = 0;

    //!Деструктор.
    virtual ~BaseData() = default;
};

QDataStream &operator >> (QDataStream &stream, BaseData &data);
QDataStream &operator << (QDataStream &stream, BaseData &data);

#endif // BASEDATA_H
