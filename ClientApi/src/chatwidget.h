#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QFont>
#include <QList>

#include "../../ServerApplication/src/userlist.h"
#include "../../ServerApplication/src/userdata.h"
#include "../../ServerApplication/src/message.h"
#include "styledialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ChatWidget;
}
QT_END_NAMESPACE

//! Основной класс для графического интерфейса окна Чата.
class ChatWidget : public QMainWindow
{
    Q_OBJECT

public:
    //! Конструктор Qt.
    explicit ChatWidget(QWidget *parent = nullptr);

    //! Деструктор Qt.
    ~ChatWidget();

    //!Сеттим основную информацию о пользователе.
    void setInfoAboutUser(const UserData &userinfo);

    //!Создаем конфигурационный файл.
    void setupWindowSettings();


    //! Добавляет имя пользователя в графический интерфейс чата, находящихся в данный момент онлайн.
    void addParticipant(const UserData &username, QColor color);

    //! Удаляет пользователя, если он больше не подключен к серверу.
    void removeParticipant(const QString &username);

    //! Метод, в который передается список всех подключенных к серверу в данный момент пользователей. Из него, в дальнейшем, вызывается addParticipant,
    //! чтобы добавлять пользователей по одному.
    void addParticipants(const UserList &lst);

    /*!
     * \brief addMessage Метод, который добавляет сообщение (и всю информацию о пользователе, который его отправил (см. описание Message.h)) в графический интерфейс чата.
     * \param msg Содержит в себе всю информацию о пользователе и сообщение, которое он отправляет.
     */
    void addMessage(const Message &msg);


signals:
    //! Отправляет сигнал серверу, содержащий сообщение.
    void messageSent(Message &msg);
    //! Отправляет сигнал серверу о том, что пользователь отключился.
    void Disconnectbutton();

private slots:
    //!Событие при нажатии кнопки отправки сообщения.
    void on_SendButton_clicked();

    //!Событие при нажатии кнопки загрузки фото.
    void on_loadImageButton_clicked();

    //!Событие при нажатии кнопки отключения от сервера.
    void on_pushButton_clicked();

    //!Событие для изменения стиля шрифта и цвета шрифта.
    void onStyleChanged(QFont &font, QColor &color);

private:
    //! Интерфейс объекта класса окна чата ChatWidget.
    Ui::ChatWidget *ui;
    //! Объект, который хранит информацию о пользователе.
    UserData userData;
    //! Переменная, хранящая загруженное фото.
    QPixmap bufferedImage;
    //! Объект класса изменений шрифта и/или цвета шрифта.
    StyleDialog dlg;

    QFont userFont;
    QColor userFontColor;
    QColor myPaletteColor[19] = {Qt::black, Qt::red, Qt::darkRed, Qt::green, Qt::darkGreen, Qt::blue, Qt::darkBlue, Qt::cyan, Qt::darkCyan, Qt::magenta, Qt::darkMagenta, Qt::yellow, Qt::darkYellow, Qt::gray, Qt::darkGray};
};

#endif // CHATWIDGET_H
