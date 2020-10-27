#include "styledialog.h"
#include "ui_styledialog.h"
#include <SettingsWrapper.cpp>

#include <QDebug>


StyleDialog::StyleDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::StyleDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    setWindowTitle(tr("Font Settings"));

    QString pattern = tr("<font color='%1'>%2</font>");
    ui->Pattern->setText(pattern.arg(userColor.name(), "Pattern"));
    QFont temp("red");
    ui->Pattern->setFont(temp);

    ui->currentColor->setText("Current color: " + userColor.name());
    ui->currentFont->setText("Current font: " + userFont.family());
    setupWindowSettings();
}

StyleDialog::~StyleDialog() {
    delete ui;
}

void StyleDialog::on_selectFont_clicked() {
    bool ok;
    userFont = QFontDialog::getFont(&ok, this);
    ui->Pattern->setFont(userFont);
    ui->currentFont->setText("Current font: " + userFont.family());

}

void StyleDialog::on_selectColor_clicked() {
    userColor = QColorDialog::getColor(Qt::red, this);
    QString pattern = tr("<font color='%1'>%2</font>");
    ui->Pattern->setText(pattern.arg(userColor.name(), "Pattern"));
    ui->currentColor->setText("Current color: " + userColor.name());
}

void StyleDialog::on_pushButton_clicked() {
    emit styleChanged(userFont, userColor);
    this->close();
}

void StyleDialog::setupWindowSettings(){
    SettingsWrapper settings(QDir(":/res"));

    QString style = settings.getOrSetValue("Style/theme", "light").toString();

    QFile * stylesFile;

    if (style == "light") {
        stylesFile = new QFile(":/res/style/lightchat.qss");
    } else {
        stylesFile = new QFile(":/res/style/nightchat.qss");
    }

    stylesFile->open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(stylesFile->readAll());
    setStyleSheet(stylesheet);
    stylesFile->close();
    delete stylesFile;
}
