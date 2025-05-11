#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "helpwindow.h"
#include "themewindow.h"
#include "editorwindow.h"

class StartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);

private slots:
    void startGame();
    void showHelp();
    void showEditor();
    void showTheme();
    void handleThemeSelected(const QString& theme);

private:
    QPushButton *startButton;
    QPushButton *helpButton;
    MainWindow *mainWindow;
    HelpWindow *helpWindow;
    QPushButton *editorButton;
    QPushButton *themeButton;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // STARTWINDOW_H
