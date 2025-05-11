#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QFrame>
#include "themewindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void handleBackButton();
    void updateTimer();
    void handleGameLost();
    void showThemeWindow();

private:
    GameField *m_gameField;
    QWidget *m_centralWidget;
    QGridLayout *m_gridLayout;
    QPushButton *backButton;
    QLabel *timerLabel;
    QFrame *timerFrame;
    QTimer *gameTimer;
    int timeLeft;
    ThemeWindow* m_themeWindow;
};
#endif // MAINWINDOW_H
