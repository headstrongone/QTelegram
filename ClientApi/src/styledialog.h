#ifndef STYLEDIALOG_H
#define STYLEDIALOG_H

#include <QDialog>
#include <QColorDialog>
#include <QFontDialog>


QT_BEGIN_NAMESPACE
namespace Ui {
class StyleDialog;
}
QT_END_NAMESPACE

//!  Основной класс окна изменения стилей шрифта.
class StyleDialog : public QDialog
{

    Q_OBJECT

public:
    //!  Конструктор Qt.
    explicit StyleDialog(QWidget *parent = nullptr);

    //!  Деструктор Qt.
    ~StyleDialog();
    void setupWindowSettings();

signals:
    //!Сигнал, сообщающий об изменении стиля шрифта.
    void styleChanged(QFont&, QColor&);

private slots:
    //!  При нажатии кпонки Select Font открывает меню выбора шрифта.
    void on_selectFont_clicked();

    //!  При нажатии кпонки Select Color открывает меню выбора цвета шрифта.
    void on_selectColor_clicked();

    //!  При нажатии кнопки OK открывает меню выбора шрифта, принимает изменения и закрывает окно.
    void on_pushButton_clicked();

private:
    //! Интерфейс объекта класса окна смены стиля StyleDialog.
    Ui::StyleDialog *ui;

    //! Шрифт пользователя.
    QFont userFont;

    //! Цвет шрифта пользователя.
    QColor userColor;
};

#endif // STYLEDIALOG_H
