#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "themewindow.h"

class HelpWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HelpWindow(QWidget *parent = nullptr);

signals:
    void backToStart();

private slots:
    void handleBackButton();

private:
    QPushButton *backButton;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // HELPWINDOW_H
