#include "authmenu.h"
#include "ui_authmenu.h"

#include <QMovie>
#include <QtDebug>
#include <QMessageBox>

AuthMenu::AuthMenu(QWidget *parent) : QMainWindow(parent), ui(new Ui::AuthMenu) {
    ui->setupUi(this);
    ui->Password->setEchoMode(QLineEdit::Password);
    setWindowTitle(tr("Authentication Menu"));
    QPixmap pixmap(":/res/img/logo2.jpg");
    QIcon ButtonIcon(pixmap);
    ui->Logo->setPixmap(pixmap.scaled(100, 100));
    setupWindowSetting();
}

AuthMenu::~AuthMenu() {
    delete ui;
}

void AuthMenu::on_SignButton_clicked() {
     auto login = ui->Login->text();
     auto password = ui->Password->text();
     auto forbiddenCharacters = new QRegExp("[^a-zA-Z0-9]");

     if (login.isEmpty()) {
         this->setStatus("Login is empty.");
         return;
     }

     if(password == ""){
         this->setStatus("Password is empty.");
         return;
     }

     if (login.contains(*forbiddenCharacters)) {
         this->setStatus("Login contains forbidden characters.");
         return;
     }
     ui->SignButton->setEnabled(false);
     ui->Login->setEnabled(false);
     ui->Password->setEnabled(false);
     auto preloader = new QMovie(":/res/img/46.gif");
     preloader->start();
     ui->movieStatus->setMovie(preloader);

     auto tmp = ports;
     emit signIn(login, password, tmp);
}

void AuthMenu::setStatus(const QString &status) {
    ui->Login->clear();
    ui->Password->clear();
    ui->Status->setText(status);
    ui->movieStatus->clear();
    ui->SignButton->setEnabled(true);
    ui->Login->setEnabled(true);
    ui->Password->setEnabled(true);
}

void AuthMenu::on_Login_returnPressed() {
    on_SignButton_clicked();
}

void AuthMenu::setupWindowSetting() {
    SettingsWrapper settings(QDir(":/res"));

    const int MIN_WINDOW_WIDTH = settings.getOrSetValue("AuthWindow/min-width", 300).toInt();
    const int MIN_WINDOW_HEIGHT = settings.getOrSetValue("AuthWindow/min-height", 400).toInt();
    const int x = MIN_WINDOW_WIDTH / 2;
    const int y = MIN_WINDOW_HEIGHT / 2;
    setMinimumSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    resize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    move( x, y );

    const int por = settings.getOrSetValue("Connection/Port", 1232).toInt();
    ports = por;

    QString style = settings.getOrSetValue("Style/theme", "light").toString();

    QFile * stylesFile;
    if (style == "light") {
        stylesFile = new QFile(":/res/style/lightauth.qss");
    } else {
        stylesFile = new QFile(":/res/style/nightauth.qss");
    }

    stylesFile->open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(stylesFile->readAll());
    setStyleSheet(stylesheet);
    stylesFile->close();
    delete stylesFile;
}
