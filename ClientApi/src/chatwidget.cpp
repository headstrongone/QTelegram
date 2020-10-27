#include "chatwidget.h"
#include "ui_chatwidget.h"

#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QBuffer>
#include <SettingsWrapper.cpp>

ChatWidget::ChatWidget(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChatWidget) {
    ui->setupUi(this);
    QPixmap pixmap(":/res/img/attach.png");
    QIcon ButtonIcon(pixmap);

    setWindowTitle(tr("QTelegram"));

    ui->loadImageButton->setIcon(ButtonIcon);
    ui->loadImageButton->setIconSize(pixmap.rect().size());

    connect(ui->actionStyle, SIGNAL(triggered(bool)), &dlg, SLOT(open()));
    connect(&dlg, SIGNAL(styleChanged(QFont&, QColor&)), this, SLOT(onStyleChanged(QFont&, QColor&)));

    setupWindowSettings();
}

ChatWidget::~ChatWidget() {
    delete ui;
}


void ChatWidget::setInfoAboutUser(const UserData &userinfo){
    userData = userinfo;
    auto participant = new QListWidgetItem;
    QFont temp("Gadugi", 12);
    QColor tempcolor("red");
    participant->setFont(temp);
    participant->setForeground(tempcolor);
    participant->setText("You logged as: " + userData.getUsername());
    participant->setFlags(participant->flags() & ~Qt::ItemIsSelectable);
    ui->usernameWidget->addItem(participant);
}

void ChatWidget::addParticipant(const UserData &username, QColor color) {
   auto participant = new QListWidgetItem;

   participant->setFont(userFont);
   participant->setForeground(color);
   participant->setText(username.getUsername());
   participant->setFlags(participant->flags() & ~Qt::ItemIsSelectable);
   ui->Users->addItem(participant);
}


void ChatWidget::addMessage(const Message &msg) {
    auto item = new QListWidgetItem;
    auto image = new QListWidgetItem;

    QPixmap p;
    p.loadFromData(msg.getImage());

    item->setForeground(userFontColor);
    item->setFont(userFont);

    item->setText(msg.getSender() + ": " + msg.getMessage());
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    ui->Dialog_2->addItem(item);

    if (!p.isNull()){
        image->setIcon(p);
        image->setFlags(image->flags() & ~Qt::ItemIsSelectable);
        ui->Dialog_2->addItem(image);
        ui->Dialog_2->setIconSize(QSize(600, 400));
    }
}

void ChatWidget::addParticipants(const UserList &lst) {
     ui->Users->clear();
     int cnt = 0;
     auto users = lst.getUsers();
     for (auto &user : users){
         if (userData.getUsername() != user.getUsername()){
            if (cnt == 18){
                cnt = 0;
            }
            this->addParticipant(user, myPaletteColor[cnt]);
            cnt++;
         }
     }

}


void ChatWidget::removeParticipant(const QString &username) {
    if (!username.isEmpty()){
        auto itemToRemove = ui->Users->findItems(username, Qt::MatchFixedString).at(0);
        delete itemToRemove;
    }
}

void ChatWidget::on_SendButton_clicked() {
    auto text = ui->MessageLine->toPlainText();
    if (text.isEmpty()) return;

    Message msg(userData.getUsername(), text);
    addMessage(msg);

    emit messageSent(msg);
    ui->MessageLine->clear();
}

void ChatWidget::on_loadImageButton_clicked() {
     auto path = QFileDialog::getOpenFileName(this, tr("Open image"), "", tr("*.jpeg *.jpg *.png"));
     auto imageName = path.split("/").last();
     bufferedImage.load(path);

     QByteArray bytes;
     QBuffer buffer(&bytes);
     buffer.open(QIODevice::ReadWrite);
     bufferedImage.save(&buffer, "PNG");
         if (!bufferedImage.isNull()){
         Message msg(userData.getUsername());
         msg.setMessage("");
         msg.setImage(bytes);
         addMessage(msg);
         emit messageSent(msg);
         ui->MessageLine->clear();
         bufferedImage = QPixmap();
     }

}

void ChatWidget::onStyleChanged(QFont &font, QColor &color) {
    userFont = font;
    userFontColor = color;
}

void ChatWidget::on_pushButton_clicked() {
    emit Disconnectbutton();
    ui->Dialog_2->clear();
}


void ChatWidget::setupWindowSettings(){
    SettingsWrapper settings(QDir(":/res"));
    const int MIN_WINDOW_WIDTH = settings.getOrSetValue("ChatWindow/min-width", 950).toInt();
    const int MIN_WINDOW_HEIGHT = settings.getOrSetValue("ChatWindow/min-height", 720).toInt();
    const int x = MIN_WINDOW_WIDTH / 2;
    const int y = MIN_WINDOW_HEIGHT / 2;
    setMinimumSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    resize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    move( x, y );

    QString fontcolour = settings.getOrSetValue("Font/color", "red").toString();
    const int fontsize = settings.getOrSetValue("Font/size", 14).toInt();

    QFont test(":/res/fonts/FuturaPT-Medium.ttf", fontsize);

    userFontColor = fontcolour;
    userFont = test;

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
