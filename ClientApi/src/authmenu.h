#ifndef AUTHMENU_H
#define AUTHMENU_H

#include <QMainWindow>
#include <SettingsWrapper.cpp>

QT_BEGIN_NAMESPACE
namespace Ui { class AuthMenu; }
QT_END_NAMESPACE

//!  Основной класс для графического интерфейса окна идентификации.
class AuthMenu : public QMainWindow
{
    Q_OBJECT

public:

    //! Конструктор Qt.
    AuthMenu(QWidget *parent = nullptr);

    //! Деструктор Qt.
    ~AuthMenu();

    /*!
     * \brief При подключении и/или ошибке подключения передает QString, которой содержит описание проблемы подключения. Так же очищает поля логина/пароля.
     * \param status Сообщение об ошибке подключения.
     */
    void setStatus(const QString &status);

    //!Создание конфигурационного файла
    void setupWindowSetting();
signals:

    //! Отправляет сигнал запроса на подключение пользователя, содержащий логин и пароль серверу.
    void signIn(const QString &username, const QString &password, const int &_port);

private slots:
    void on_SignButton_clicked(); //!< Событие при нажатии кнопки Sign In.
    void on_Login_returnPressed(); //!< Событие при нажатии на поле Login.

private:
    Ui::AuthMenu *ui;  //!< Интерфейс объекта класса окна идентификации AuthMenu.
    int ports;
};
#endif // AUTHMENU_H
