#include "editorwindow.h"
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>
#include <QQueue>
#include <QTimer>
#include <QRegularExpressionValidator>

EditorWindow::EditorWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Создание собственного уровеня");
    setFixedSize(1000, 800);

    setupField();
    setupUI();

    m_isDrawing = false;
    m_isPlayingMode = false;
    m_timeLeft = 60;
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &EditorWindow::updateTimer);
    connect(m_editorField, &GameField::gameWon, this, [this]() {
        showResult(true);
    });
}

void EditorWindow::setupField()
{
    m_editorField = new GameField(20, 20);
    m_editorField->setFixedSize(800, 800);

    for (int i = 0; i < m_editorField->getHeight(); i++) {
        for (int j = 0; j < m_editorField->getWidth(); j++) {
            if (i == 0 || j == 0 || i == m_editorField->getHeight()-1 || j == m_editorField->getWidth()-1) {
                m_editorField->getField()[i][j].setType(1);
            } else {
                m_editorField->getField()[i][j].setType(0);
            }
        }
    }

    m_editorField->getField()[1][1].setType(0);
    m_editorField->setPlayerPosition(1, 1);

    m_editorField->getField()[m_editorField->getHeight()-2][m_editorField->getWidth()-2].setType(2);
}


void EditorWindow::setupUI() {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QVBoxLayout *rightLayout = new QVBoxLayout();

    QLabel *timeLabel = new QLabel("Введите время (MM:SS):", this);
    timeLabel->setStyleSheet("QLabel { color: white; font-size: 14px; }");

    timeInput = new QLineEdit(this);
    timeInput->setPlaceholderText("Например: 01:30");
    timeInput->setMaxLength(5);
    timeInput->setStyleSheet("QLineEdit { background-color: rgba(255, 255, 255, 150); border-radius: 5px; padding: 5px; }");

    QRegularExpressionValidator *timeValidator = new QRegularExpressionValidator(QRegularExpression("^([0-5]?[0-9]):([0-5][0-9])$"), this);
    timeInput->setValidator(timeValidator);

    QPushButton *startButton = new QPushButton("Начать прохождение", this);
    QPushButton *clearButton = new QPushButton("Очистить поле", this);
    backButton = new QPushButton("Вернуться в меню", this);

    m_timerFrame = new QFrame(this);
    m_timerFrame->setStyleSheet("background-color: rgba(255, 255, 255, 150); border-radius: 10px;");
    m_timerFrame->setFixedSize(150, 60);
    m_timerFrame->hide();

    m_timerLabel = new QLabel("Время: 01:00", m_timerFrame);
    m_timerLabel->setAlignment(Qt::AlignCenter);
    m_timerLabel->setStyleSheet("QLabel { color: black; font-size: 18px; font-weight: bold; }");

    QVBoxLayout *frameLayout = new QVBoxLayout(m_timerFrame);
    frameLayout->addWidget(m_timerLabel);
    m_timerFrame->setLayout(frameLayout);

    startButton->setFixedSize(150, 50);
    clearButton->setFixedSize(150, 50);
    backButton->setFixedSize(150, 50);

    QString buttonStyle = "QPushButton {"
                          "background-color: rgba(255, 255, 255, 150);"
                          "border-radius: 5px;"
                          "padding: 8px;"
                          "font-size: 14px;"
                          "min-width: 150px;"
                          "}";
    startButton->setStyleSheet(buttonStyle);
    clearButton->setStyleSheet(buttonStyle);
    backButton->setStyleSheet(buttonStyle);

    rightLayout->addWidget(timeLabel);
    rightLayout->addWidget(timeInput);
    rightLayout->addWidget(startButton, 0, Qt::AlignHCenter);
    rightLayout->addWidget(clearButton, 0, Qt::AlignHCenter);
    rightLayout->addWidget(m_timerFrame, 0, Qt::AlignHCenter);
    rightLayout->addStretch();
    rightLayout->addWidget(backButton, 0, Qt::AlignRight);

    mainLayout->addWidget(m_editorField);
    mainLayout->addLayout(rightLayout);

    connect(backButton, &QPushButton::clicked, this, &EditorWindow::handleBackButton);
    connect(clearButton, &QPushButton::clicked, this, &EditorWindow::handleClearButton);
    connect(startButton, &QPushButton::clicked, this, &EditorWindow::handleStartButton);
}

bool EditorWindow::validateTimeInput(const QString &input) {
    QRegularExpression timeFormat("^(\\d{1,2}):([0-5]\\d)$");
    QRegularExpressionMatch match = timeFormat.match(input);
    if (match.hasMatch()) {
        int minutes = match.captured(1).toInt();
        int seconds = match.captured(2).toInt();
        return (minutes < 60) && (seconds < 60);
    }
    return false;
}

void EditorWindow::handleStartButton() {
    if (!isPathAvailable()) {
        QMessageBox::warning(this, "Ошибка", "Невозможно пройти ваш путь");
        return;
    }

    QString timeStr = timeInput->text();
    if (!validateTimeInput(timeStr)) {
        QMessageBox::warning(this, "Ошибка", "Введите время в формате MM:SS (например, 01:30)");
        return;
    }

    QStringList parts = timeStr.split(':');
    int minutes = parts[0].toInt();
    int seconds = parts[1].toInt();
    m_timeLeft = minutes * 60 + seconds;

    if (m_timeLeft <= 0) {
        QMessageBox::warning(this, "Ошибка", "Время должно быть больше 00:00");
        return;
    }

    m_isPlayingMode = true;
    m_timerLabel->setText(QString("Время: %1:%2")
                              .arg(minutes, 2, 10, QChar('0'))
                              .arg(seconds, 2, 10, QChar('0')));
    m_timerFrame->show();
    m_gameTimer->start(1000);

    m_editorField->setPlayerPosition(1, 1);
    m_editorField->setFocus();
}

void EditorWindow::handleClearButton()
{
    if (m_isPlayingMode) return;

    for (int i = 1; i < m_editorField->getHeight()-1; i++) {
        for (int j = 1; j < m_editorField->getWidth()-1; j++) {
            if (!(i == 1 && j == 1) && !(i == m_editorField->getHeight()-2 && j == m_editorField->getWidth()-2)) {
                m_editorField->getField()[i][j].setType(0);
            }
        }
    }
    m_editorField->update();
}

void EditorWindow::handleBackButton()
{
    emit backToStart();
    this->close();
}

void EditorWindow::updateTimer()
{
    m_timeLeft--;
    if (m_timeLeft <= 0) {
        m_gameTimer->stop();
        showResult(false);
        return;
    }

    int minutes = m_timeLeft / 60;
    int seconds = m_timeLeft % 60;
    m_timerLabel->setText(QString("Время: %1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
}

bool EditorWindow::isPathAvailable()
{
    int height = m_editorField->getHeight();
    int width = m_editorField->getWidth();

    QVector<QVector<bool>> visited(height, QVector<bool>(width, false));
    QQueue<QPair<int, int>> queue;

    queue.enqueue(qMakePair(1, 1));
    visited[1][1] = true;

    while (!queue.isEmpty()) {
        auto current = queue.dequeue();
        int x = current.first;
        int y = current.second;

        if (x == height-2 && y == width-2) {
            return true;
        }

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && ny >= 0 && nx < height && ny < width &&
                !visited[nx][ny] && m_editorField->getField()[nx][ny].getType() != 1) {
                visited[nx][ny] = true;
                queue.enqueue(qMakePair(nx, ny));
            }
        }
    }

    return false;
}

void EditorWindow::showResult(bool isWin)
{
    m_gameTimer->stop();
    m_isPlayingMode = false;
    m_timerFrame->hide();

    class ResultWindow : public QWidget {
    public:
        ResultWindow(const QString& title, const QString& message, QWidget* parent = nullptr)
            : QWidget(parent) {
            setWindowTitle(title);
            setFixedSize(400, 200);

            QVBoxLayout *mainLayout = new QVBoxLayout(this);
            mainLayout->setContentsMargins(30, 30, 30, 30);

            QWidget *textContainer = new QWidget(this);
            textContainer->setStyleSheet("background-color: rgba(255, 255, 255, 150); border-radius: 10px;");

            QVBoxLayout *textLayout = new QVBoxLayout(textContainer);
            textLayout->setContentsMargins(20, 20, 20, 20);

            QLabel *msgLabel = new QLabel(message, textContainer);
            msgLabel->setWordWrap(true);
            msgLabel->setAlignment(Qt::AlignCenter);
            msgLabel->setStyleSheet("QLabel { color: black; font-size: 16px; }");

            textLayout->addWidget(msgLabel);

            backButton = new QPushButton("Вернуться в меню", this);
            backButton->setFixedSize(150, 40);
            backButton->setStyleSheet(
                "QPushButton {"
                "background-color: rgba(255, 255, 255, 150);"
                "border-radius: 5px;"
                "color: black;"
                "}"
                );

            mainLayout->addWidget(textContainer);
            mainLayout->addWidget(backButton, 0, Qt::AlignCenter);
        }

        void paintEvent(QPaintEvent* event) override {
            QPainter painter(this);
            QString theme = ThemeWindow::currentTheme();
            QString bgPath = isWin ?
                                 QString(":/resources/%1_bg/victory_bg.jpg").arg(theme) :
                                 QString(":/resources/%1_bg/loose_bg.jpg").arg(theme);
            QPixmap background(bgPath);

            if (!background.isNull()) {
                painter.drawPixmap(0, 0, width(), height(), background);
            }
            QWidget::paintEvent(event);
        }

        QPushButton* backButton;
        bool isWin;
    };

    QString title = isWin ? "Победа!" : "Поражение";
    QString message = isWin ? "Ура! Вы справились с коридорами!"
                            : "Увы! Время вышло, вы не успели добраться до цели :(";

    ResultWindow *resultWindow = new ResultWindow(title, message);
    resultWindow->isWin = isWin;

    connect(resultWindow->backButton, &QPushButton::clicked, this, [this, resultWindow]() {
        resultWindow->close();
        emit backToStart();
        this->close();
    });

    resultWindow->show();
}

void EditorWindow::mousePressEvent(QMouseEvent *event)
{
    if (m_isPlayingMode) return;

    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        setWallAtPosition(event->pos(), true);
    }
    else if (event->button() == Qt::RightButton) {
        m_isDrawing = true;
        setWallAtPosition(event->pos(), false);
    }
}

void EditorWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPlayingMode) return;

    if (m_isDrawing) {
        if (event->buttons() & Qt::LeftButton) {
            setWallAtPosition(event->pos(), true);
        }
        else if (event->buttons() & Qt::RightButton) {
            setWallAtPosition(event->pos(), false);
        }
    }
}

void EditorWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        m_isDrawing = false;
    }
}

void EditorWindow::setWallAtPosition(const QPoint& pos, bool isWall)
{
    int x = pos.x();
    int y = pos.y();

    if (x >= 0 && y >= 0 && x < m_editorField->width() && y < m_editorField->height()) {
        int cellCol = y / m_editorField->getFieldItemSize();
        int cellRow = x / m_editorField->getFieldItemSize();

        if (isProtectedCell(cellRow, cellCol)) {
            return;
        }

        m_editorField->getField()[cellRow][cellCol].setType(isWall ? 1 : 0);
        m_editorField->update();
    }
}

bool EditorWindow::isProtectedCell(int cellX, int cellY) const
{
    return (cellX <= 0 || cellY <= 0 ||
            cellX >= m_editorField->getHeight()-1 ||
            cellY >= m_editorField->getWidth()-1 ||
            (cellX == 1 && cellY == 1) ||
            (cellX == m_editorField->getHeight()-2 && cellY == m_editorField->getWidth()-2));
}





void EditorWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QString bgPath = QString(":/resources/%1_bg/game_bg.jpg").arg(ThemeWindow::currentTheme());
    QPixmap background(bgPath);
    if (!background.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), background);
    }
}

