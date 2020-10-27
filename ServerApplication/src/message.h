 #ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <QFont>
#include <QColor>

#include "basedata.h"
//! Класс для хранения данных о сообщении, отправленным клиентом на сервер.
class Message : public BaseData
{

public:
    //! Конструктор по умолчанию.
    Message();

    //! Конструктор с параметром ответа.
    Message(const QString &sender,
            const QString &message = "",
            const QByteArray &image = QByteArray());

    //! Переопределение метода toStream.
    virtual QDataStream& toStream(QDataStream &stream) const override;

    //! Переопределение метода fromStream.
    virtual QDataStream& fromStream(QDataStream &stream) override;

    //! Переопределение метода type.
    virtual DataType type() const override;

    //! Добавить картинку.
    void setImage(const QByteArray &image);

    //! Добавить текст сообщения.
    void setMessage(const QString &message);

    //! Добавить именя пользователя.
    void setSender(const QString &sender);

    //! Получить картинку.
    QByteArray getImage() const;

    //! Получить текст сообщения.
    QString getMessage() const;

    //! Получить имя пользователя.
    QString getSender() const;

    void setType(const DataType type);


private:
    //! Отправитель.
    QString sender;

    //! Сообщение.
    QString message;

    //! Картинка.
    QByteArray image;

};

#endif // MESSAGE_H
