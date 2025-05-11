#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include "game.h"
#include "themewindow.h"
#include <QRegularExpressionValidator>

class EditorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = nullptr);

signals:
    void backToStart();

private slots:
    void handleBackButton();
    void handleClearButton();
    void handleStartButton();

private:
    void setupField();
    void setupUI();
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void toggleCell(const QPoint &pos);
    void updateTimer();
    bool isPathAvailable();
    void showResult(bool isWin);
    void setWallAtPosition(const QPoint& pos, bool isWall);
    bool isProtectedCell(int cellX, int cellY) const;
    bool validateTimeInput(const QString &input);

    GameField *m_editorField;
    QPushButton *backButton;
    QTimer *m_gameTimer;
    QLabel *m_timerLabel;
    QFrame *m_timerFrame;
    int m_timeLeft;
    bool m_isDrawing;
    bool m_isPlayingMode;
    QLineEdit *timeInput;
};

#endif // EDITORWINDOW_H
