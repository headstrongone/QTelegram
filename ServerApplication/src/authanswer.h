#ifndef AUTHANSWER_H
#define AUTHANSWER_H

#include "basedata.h"

//! Класс для хранения ответов, получаемых от сервера.
class AuthAnswer : public BaseData
{

public:

    //! Конструктор с параметром ответа
    AuthAnswer(bool answer);

    //!Конструктор по умолчанию
    AuthAnswer() = default;

    //! Переопределение метода toStream
    virtual QDataStream& toStream(QDataStream &stream) const override;

    //! Переопределение метода fromStream
    virtual QDataStream& fromStream(QDataStream &stream) override;

    //! Переопределение метода type
    virtual DataType type() const override;

    //! Возвращает true при успешной аутентификации, false - при неуспешной.
    bool isSigned() const;

    //! Устанавливает значение answer в true при успешной аутентификации, false - при неуспешной.
    void setSigned(bool answer);

private:
    //! Ответ, передаваемый классом.
    bool answer;

};

#endif // AUTHANSWER_H
