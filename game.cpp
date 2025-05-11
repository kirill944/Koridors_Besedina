#include "game.h"
#include <QSet>
#include <random>
#include <QDebug>
#include <algorithm>
#include <random>

GameField::GameField(int f_width, int f_height)
{
    setFixedSize(800, 800);
    m_fieldItemSize = std::min(height() / f_height, width() / f_width);
    m_height = f_height;
    m_width = f_width;
    generateField();
    m_playerX = 1;
    m_playerY = 1;

    setFocusPolicy(Qt::StrongFocus);
    generateField();
}
void GameField::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter;
    painter.begin(this);

    QLinearGradient wallGradient(0, 0, width(), height());
    wallGradient.setColorAt(0.0, QColor("#FFFACD"));
    wallGradient.setColorAt(0.5, QColor("#98FB98"));
    wallGradient.setColorAt(1.0, QColor("#ADD8E6"));
    QString theme = ThemeWindow::currentTheme();
    QPixmap targetIcon;
    QPixmap playerIcon;

    if (theme == "forest") {
        targetIcon.load(":/resources/forest_bg/frog_icon.png");
        playerIcon.load(":/resources/forest_bg/frog_icon.png");
    } else if (theme == "japanese") {
        targetIcon.load(":/resources/japanese_bg/cat_icon.png");
        playerIcon.load(":/resources/japanese_bg/cat_icon.png");
    } else if (theme == "water") {
        targetIcon.load(":/resources/water_bg/fish_icon.png");
        playerIcon.load(":/resources/water_bg/fish_icon.png");
    }

    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            int type = m_field[i][j].getType();
            if (type == 1) {
                painter.fillRect(
                    m_fieldItemSize * i,
                    m_fieldItemSize * j,
                    m_fieldItemSize,
                    m_fieldItemSize,
                    QBrush(wallGradient)
                    );
            } else if (type == 2) {
                if (!targetIcon.isNull()) {
                    QPixmap scaledIcon = targetIcon.scaled(
                        m_fieldItemSize * 2,
                        m_fieldItemSize * 2,
                        Qt::KeepAspectRatio
                        );
                    painter.drawPixmap(
                        m_fieldItemSize * i - m_fieldItemSize/2,
                        m_fieldItemSize * j - m_fieldItemSize/2,
                        scaledIcon
                        );
                } else {
                    painter.fillRect(
                        m_fieldItemSize * i,
                        m_fieldItemSize * j,
                        m_fieldItemSize,
                        m_fieldItemSize,
                        Qt::green
                        );
                }
            }
        }
    }

    if (!playerIcon.isNull()) {
        QPixmap scaledPlayerIcon = playerIcon.scaled(
            m_fieldItemSize * 2,
            m_fieldItemSize * 2,
            Qt::KeepAspectRatio
            );
        painter.drawPixmap(
            m_playerY * m_fieldItemSize - m_fieldItemSize/2,
            m_playerX * m_fieldItemSize - m_fieldItemSize/2,
            scaledPlayerIcon
            );
    } else {
        painter.fillRect(
            m_playerY * m_fieldItemSize,
            m_playerX * m_fieldItemSize,
            m_fieldItemSize,
            m_fieldItemSize,
            Qt::red
            );
    }

    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            if (m_field[i][j].getType() == 1) {
                bool drawTop = (j == 0 || m_field[i][j - 1].getType() != 1);
                bool drawBottom = (j == m_height - 1 || m_field[i][j + 1].getType() != 1);
                bool drawLeft = (i == 0 || m_field[i - 1][j].getType() != 1);
                bool drawRight = (i == m_width - 1 || m_field[i + 1][j].getType() != 1);

                int x = m_fieldItemSize * i;
                int y = m_fieldItemSize * j;

                if (drawTop) {
                    painter.drawLine(x, y, x + m_fieldItemSize, y);
                }
                if (drawBottom) {
                    painter.drawLine(x, y + m_fieldItemSize, x + m_fieldItemSize, y + m_fieldItemSize);
                }
                if (drawLeft) {
                    painter.drawLine(x, y, x, y + m_fieldItemSize);
                }
                if (drawRight) {
                    painter.drawLine(x + m_fieldItemSize, y, x + m_fieldItemSize, y + m_fieldItemSize);
                }
            }
        }
    }

    painter.end();
}

void GameField::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
        movePlayer(-1, 0);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        movePlayer(1, 0);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        movePlayer(0, -1);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        movePlayer(0, 1);
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void GameField::movePlayer(int dx, int dy)
{
    int newX = m_playerX + dx;
    int newY = m_playerY + dy;

    if (newX >= 0 && newY >= 0 &&
        newX < m_height && newY < m_width &&
        m_field[newY][newX].getType() != 1)
    {
        m_playerX = newX;
        m_playerY = newY;
        update();

        if (m_field[newY][newX].getType() == 2) {
            emit gameWon();
        }
    }
}

FieldItem::FieldItem(int x, int y, int type)
{
    m_x = x;
    m_y = y;
    m_type = type;
}

QList<QPair<int, int>> getNeigbours(int heigh, int width, int x, int y) {
    QList<QPair<int, int>> neigbours;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (x + i >= 0 && y + j >= 0 && x + i < width && x + y < heigh)
                neigbours.append(qMakePair(x + i, y + j));
    return neigbours;
}


void generateMaze(QList<QList<int>>& maze, int x1, int y1, int x2, int y2) {
    int WALL = 1;
    int PATH = 0;
    if (x2 - x1 < 2 || y2 - y1 < 2) {
        return;
    }

    int wallX = x1 + 1 + rand() % ((x2 - x1) / 2) * 2;
    int wallY = y1 + 1 + rand() % ((y2 - y1) / 2) * 2;

    for (int i = x1; i <= x2; i++) maze[i][wallY] = WALL;
    for (int j = y1; j <= y2; j++) maze[wallX][j] = WALL;

    QList<QPair<int, int>> openings = {
        {wallX, y1},
        {wallX, y2},
        {x1, wallY},
        {x2, wallY}
    };

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(openings.begin(), openings.end(), rng);

    openings.pop_back();

    for (auto& opening : openings) {
        maze[opening.first][opening.second] = PATH;
    }

    generateMaze(maze, x1, y1, wallX - 1, wallY - 1);
    generateMaze(maze, x1, wallY + 1, wallX - 1, y2);
    generateMaze(maze, wallX + 1, y1, x2, wallY - 1);
    generateMaze(maze, wallX + 1, wallY + 1, x2, y2);
}

void GameField::generateField()
{
    srand (time(NULL));
    QList<QList<int>> maze;
    for (int i = 0; i < m_height; i++) {
        QList<int> str;
        for (int j = 0; j < m_width; j++) {
            str.append(0);
        }
        maze.append(str);
    }
    generateMaze(maze, 1, 1, m_height-2, m_width-2);
    for (int i = 0; i < m_height; i++) {
        maze[i][0] = 1;
        maze[i][m_width - 1] = 1;
    }
    for (int j = 0; j < m_width; j++) {
        maze[0][j] = 1;
        maze[m_height - 1][j] = 1;
    }
    maze[m_height - 2][m_width - 2] = 2;
    for (int i = 0; i < m_height; i++) {
        QList<FieldItem> str;
        for (int j = 0; j < m_width; j++) {
            str.append(FieldItem(i, j, maze[i][j]));
        }
        m_field.append(str);
    }
}

int FieldItem::getType() const
{
    return this->m_type;
}

bool FieldItem::isOpen() const
{
    return this->m_isOpen;
}

void FieldItem::setType(int type)
{
    this->m_type = type;
}
