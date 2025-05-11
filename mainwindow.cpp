#include "mainwindow.h"
#include "startwindow.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QMessageBox>

class ResultWindow : public QWidget {
public:
    ResultWindow(const QString& title, const QString& message, const QString& backgroundPath, QWidget* parent = nullptr)
        : QWidget(parent), backgroundPath(backgroundPath) {
        setWindowTitle(title);
        setFixedSize(400, 250);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(30, 30, 30, 30);
        mainLayout->setSpacing(20);

        QWidget *textContainer = new QWidget(this);
        textContainer->setStyleSheet("background-color: rgba(255, 255, 255, 150); border-radius: 10px;");

        QVBoxLayout *textLayout = new QVBoxLayout(textContainer);
        textLayout->setContentsMargins(20, 20, 20, 20);

        QLabel *msgLabel = new QLabel(message, textContainer);
        msgLabel->setWordWrap(true);
        msgLabel->setAlignment(Qt::AlignCenter);
        msgLabel->setStyleSheet("QLabel { color: black; font-size: 16px; font-weight: bold; }");

        textLayout->addWidget(msgLabel);

        backButton = new QPushButton("Вернуться в меню", this);
        backButton->setFixedSize(200, 50);
        backButton->setStyleSheet(buttonStyle);

        mainLayout->addWidget(textContainer);
        mainLayout->addWidget(backButton, 0, Qt::AlignCenter);
    }

    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        QPixmap background(backgroundPath);

        if (!background.isNull()) {
            painter.drawPixmap(0, 0, width(), height(), background);
        }
        QWidget::paintEvent(event);
    }

    QPushButton* backButton;

private:
    QString backgroundPath;
    QString buttonStyle =
        "QPushButton {"
        "background-color: rgba(255, 255, 255, 150);"
        "border-radius: 5px;"
        "padding: 8px;"
        "font-size: 14px;"
        "min-width: 200px;"
        "margin: 10px;"
        "color: black;"
        "outline: none;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255, 255, 255, 200);"
        "}";
};


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_themeWindow(nullptr)
{
    setWindowTitle("Коридоры");
    setFixedSize(1200, 800);
    m_gameField = new GameField(31, 31);
    m_centralWidget = new QWidget();
    m_gridLayout = new QGridLayout();

    timeLeft = 60;
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateTimer);
    gameTimer->start(1000);

    timerFrame = new QFrame(this);
    timerFrame->setStyleSheet("background-color: rgba(255, 255, 255, 150); border-radius: 10px;");
    timerFrame->setFixedSize(150, 60);

    timerLabel = new QLabel("Время: 1:00", timerFrame);
    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setStyleSheet("QLabel { color: black; font-size: 18px; font-weight: bold; }");

    QVBoxLayout *frameLayout = new QVBoxLayout(timerFrame);
    frameLayout->addWidget(timerLabel);
    timerFrame->setLayout(frameLayout);

    backButton = new QPushButton("Вернуться в меню", this);
    backButton->setFixedSize(150, 50);
    backButton->setStyleSheet("QPushButton {"
                              "background-color: rgba(255, 255, 255, 150);"
                              "border-radius: 5px;"
                              "padding: 8px;"
                              "font-size: 14px;"
                              "min-width: 150px;"
                              "}");

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addStretch();
    rightLayout->addWidget(timerFrame, 0, Qt::AlignHCenter);
    rightLayout->addStretch();
    rightLayout->addWidget(backButton, 0, Qt::AlignRight);
    rightLayout->setSpacing(20);

    m_gridLayout->setContentsMargins(20, 20, 20, 20);
    m_gridLayout->setSpacing(20);

    m_gridLayout->addWidget(m_gameField, 0, 0);
    m_gridLayout->addLayout(rightLayout, 0, 1);

    m_centralWidget->setLayout(m_gridLayout);
    setCentralWidget(m_centralWidget);

    m_gameField->setFocus();

    connect(m_gameField, &GameField::gameWon, this, [this]() {
        gameTimer->stop();
        QString theme = ThemeWindow::currentTheme();
        QString bgPath = QString(":/resources/%1_bg/victory_bg.jpg").arg(theme);

        ResultWindow *resultWindow = new ResultWindow("Победа!", "Ура! Вы справились с коридорами!", bgPath);
        connect(resultWindow->backButton, &QPushButton::clicked, this, [this, resultWindow]() {
            this->close();
            resultWindow->close();
            StartWindow *startWindow = new StartWindow();
            startWindow->show();
        });

        resultWindow->show();
    });

    connect(backButton, &QPushButton::clicked, this, &MainWindow::handleBackButton);
}

void MainWindow::updateTimer()
{
    timeLeft--;
    if (timeLeft <= 0) {
        gameTimer->stop();
        handleGameLost();
        return;
    }

    int minutes = timeLeft / 60;
    int seconds = timeLeft % 60;
    timerLabel->setText(QString("Время: %1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
}

void MainWindow::handleGameLost()
{
    QString theme = ThemeWindow::currentTheme();
    QString bgPath = QString(":/resources/%1_bg/loose_bg.jpg").arg(theme);
    ResultWindow *lostWindow = new ResultWindow("Поражение", "Увы! Время вышло, вы не успели добраться до цели", bgPath);
    connect(lostWindow->backButton, &QPushButton::clicked, this, [this, lostWindow]() {
        this->close();
        lostWindow->close();
        StartWindow *startWindow = new StartWindow();
        startWindow->show();
    });

    lostWindow->show();
}

void MainWindow::handleBackButton()
{
    gameTimer->stop();
    this->close();
    StartWindow *startWindow = new StartWindow();
    startWindow->show();
}

void MainWindow::showThemeWindow()
{
    if (!m_themeWindow) {
        m_themeWindow = new ThemeWindow(this);
        connect(m_themeWindow, &ThemeWindow::backToStart, this, &MainWindow::handleBackButton);
    }
    m_themeWindow->show();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QString bgPath = QString(":/resources/%1_bg/game_bg.jpg").arg(ThemeWindow::currentTheme());
    QPixmap background(bgPath);
    if (!background.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), background);
    }
}

MainWindow::~MainWindow()
{
    delete gameTimer;
    delete m_themeWindow;
}
