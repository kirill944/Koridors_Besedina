#include "startwindow.h"
#include <QPainter>
#include <QLabel>
#include <QApplication>

StartWindow::StartWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Коридоры");
    setFixedSize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Добро пожаловать в Коридоры!", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "font-size: 30px;"
        "font-weight: bold;"
        "color: white;"
        "margin-bottom: 50px;"
        "}"
        );

    QWidget *buttonsContainer = new QWidget(this);
    QVBoxLayout *buttonsLayout = new QVBoxLayout(buttonsContainer);
    buttonsLayout->setContentsMargins(50, 0, 50, 0);

    startButton = new QPushButton("Начать игру", buttonsContainer);
    helpButton = new QPushButton("Инструкция", buttonsContainer);
    editorButton = new QPushButton("Создать собственный уровень", buttonsContainer);
    themeButton = new QPushButton("Выбрать тематику коридоров", buttonsContainer);
    QPushButton *exitButton = new QPushButton("Выход", buttonsContainer);

    QString buttonStyle = "QPushButton {"
                          "background-color: rgba(255, 255, 255, 150);"
                          "border-radius: 5px;"
                          "padding: 8px;"
                          "font-size: 14px;"
                          "min-width: 200px;"
                          "margin: 5px;"
                          "}";

    startButton->setStyleSheet(buttonStyle);
    helpButton->setStyleSheet(buttonStyle);
    editorButton->setStyleSheet(buttonStyle);
    themeButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(helpButton);
    buttonsLayout->addWidget(editorButton);
    buttonsLayout->addWidget(themeButton);
    buttonsLayout->addWidget(exitButton);
    buttonsLayout->addStretch();

    mainLayout->addStretch(1);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(buttonsContainer);
    mainLayout->addStretch(2);

    connect(startButton, &QPushButton::clicked, this, &StartWindow::startGame);
    connect(helpButton, &QPushButton::clicked, this, &StartWindow::showHelp);
    connect(editorButton, &QPushButton::clicked, this, &StartWindow::showEditor);
    connect(themeButton, &QPushButton::clicked, this, &StartWindow::showTheme);
    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
}

void StartWindow::showTheme()
{
    ThemeWindow *themeWindow = new ThemeWindow();
    connect(themeWindow, &ThemeWindow::backToStart, this, &QWidget::show);
    connect(themeWindow, &ThemeWindow::themeSelected, this, [this](const QString& theme) {
        update();
    });
    this->hide();
    themeWindow->show();
}

void StartWindow::startGame()
{
    mainWindow = new MainWindow();
    connect(mainWindow, &MainWindow::destroyed, this, [this]() {
        this->deleteLater();
    });
    mainWindow->show();
    this->close();
}

void StartWindow::showHelp()
{
    helpWindow = new HelpWindow();
    connect(helpWindow, &HelpWindow::backToStart, this, &QWidget::show);
    this->hide();
    helpWindow->show();
}


void StartWindow::showEditor()
{
    EditorWindow *editorWindow = new EditorWindow();
    connect(editorWindow, &EditorWindow::backToStart, this, &QWidget::show);
    this->hide();
    editorWindow->show();
}

void StartWindow::handleThemeSelected(const QString& theme)
{
    qDebug() << "Selected theme:" << theme;
}
void StartWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QString bgPath = QString(":/resources/%1_bg/start_bg.jpg").arg(ThemeWindow::currentTheme());
    QPixmap background(bgPath);
    if (!background.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), background);
    }
}
