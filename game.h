#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QPainter>
#include <QList>
#include <QKeyEvent>

#include "themewindow.h"

class GameField;
class FieldItem;

class FieldItem {
public:
    FieldItem(int x, int y, int type);
    int getType() const;
    bool isOpen() const;
    void setType(int type);

private:
    int m_x;
    int m_y;
    int m_type;
    bool m_isOpen;
};

class GameField : public QWidget {
    Q_OBJECT
public:
    GameField(int f_width, int f_height);
    void keyPressEvent(QKeyEvent *event) override;
    int getHeight() const { return m_height; }
    int getWidth() const { return m_width; }
    int getFieldItemSize() const { return m_fieldItemSize; }
    QList<QList<FieldItem>>& getField() { return m_field; }
    void setPlayerPosition(int x, int y) { m_playerX = x; m_playerY = y; }
    int getCellType(int x, int y) const { return m_field[y][x].getType(); }
    void setCellType(int x, int y, int type) { m_field[y][x].setType(type); }

signals:
    void gameWon();
    void returnToMenuRequested();

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    void generateField();
    void movePlayer(int dx, int dy);
    bool isValidMove(int x, int y) const;
    QList<QList<FieldItem>> m_field;
    int m_width;
    int m_height;
    int m_fieldItemSize;
    int m_playerX;
    int m_playerY;
    void showGameEndWindow(const QString& title, const QString& message);
};


#endif // GAME_H
