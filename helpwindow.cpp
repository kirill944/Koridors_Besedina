#include "helpwindow.h"
#include "startwindow.h"

HelpWindow::HelpWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Инструкция");
    setFixedSize(900, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(50, 30, 50, 30);

    QWidget *textContainer = new QWidget(this);
    textContainer->setStyleSheet("background-color: rgba(255, 255, 255, 150); border-radius: 10px;");

    QVBoxLayout *textLayout = new QVBoxLayout(textContainer);
    textLayout->setContentsMargins(30, 20, 30, 20);

    QLabel *instructionLabel = new QLabel(
        "<h2 style='text-align: center; margin-bottom: 20px; font-size: 20px;'>Инструкция к игре:</h2>"
        "<p style='text-align: center; margin-bottom: 15px; font-size: 16px;'>Управление персонажем:</p>"
        "<ul style='margin-left: 30px; margin-right: 30px; margin-bottom: 20px; font-size: 14px;'>"
        "<li style='margin-bottom: 8px;'>Стрелка <b>ВВЕРХ</b> или клавиша <b>W</b> - движение <b>ВВЕРХ</b></li>"
        "<li style='margin-bottom: 8px;'>Стрелка <b>ВНИЗ</b> или <b>S</b> - движение <b>ВНИЗ</b></li>"
        "<li style='margin-bottom: 8px;'>Стрелка <b>ВЛЕВО</b> или <b>A</b> - движение <b>ВЛЕВО</b></li>"
        "<li style='margin-bottom: 8px;'>Стрелка <b>ВПРАВО</b> или <b>D</b> - движение <b>ВПРАВО</b></li>"
        "</ul>"
        "<h2 style='text-align: center; margin-bottom: 20px; font-size: 20px;'>Инструкция к созданию собственного уровня:</h2>"
        "<ul style='margin-left: 30px; margin-right: 30px; margin-bottom: 20px; font-size: 14px;'>"
        "<li style='margin-bottom: 8px;'>Одноразовое нажатие на <b>ЛКМ</b> рисует 1 клетку стенки</li>"
        "<li style='margin-bottom: 8px;'>Зажатие <b>ЛКМ</b> позволяет рисовать непрерывно</li>"
        "<li style='margin-bottom: 8px;'>Нажатие на <b>ПКМ</b> удаляет стенку (можно как одним кликом, так и с зажимом)</li>"
        "<li style='margin-bottom: 8px;'>В случае неудачно нарисованного Вами коридора можно очистить поле</li>"
        "<li style='margin-bottom: 8px;'>Нажатие на кнопку начала прохождения позволит приступить к обходу стенок с таймером</li>"
        "</ul>"
        "<p style='text-align: center; font-size: 18px; margin-top: 15px;'>"
        "<b>ЦЕЛЬ:</b> пройти все коридоры и добраться до выхода</p>"
        "<p style='text-align: center; font-size: 16px; margin-top: 5px;'>"
        "* Выходом является цель справа снизу в конце коридора</p>"
        );
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet("QLabel { color: black; }");

    textLayout->addWidget(instructionLabel);
    layout->addWidget(textContainer, 1);

    backButton = new QPushButton("Вернуться в меню", this);
    backButton->setFixedSize(200, 45);
    backButton->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(255, 255, 255, 150);"
        "border-radius: 5px;"
        "padding: 10px;"
        "font-size: 16px;"
        "min-width: 150px;"
        "}"
        );

    layout->addWidget(backButton, 0, Qt::AlignHCenter);
    layout->setSpacing(25);

    connect(backButton, &QPushButton::clicked, this, &HelpWindow::handleBackButton);
}

void HelpWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QString bgPath = QString(":/resources/%1_bg/help_bg.jpg").arg(ThemeWindow::currentTheme());
    QPixmap background(bgPath);
    if (!background.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), background);
    }
}

void HelpWindow::handleBackButton()
{
    emit backToStart();
    this->close();
}
