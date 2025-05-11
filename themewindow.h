#ifndef THEMEWINDOW_H
#define THEMEWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>

class ThemeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ThemeWindow(QWidget *parent = nullptr);
    static QString currentTheme();

signals:
    void backToStart();
    void themeSelected(const QString& theme);

private slots:
    void handleForestTheme();
    void handleWaterTheme();
    void handleJapaneseTheme();
    void handleBackButton();

private:
    QPushButton *forestButton;
    QPushButton *waterButton;
    QPushButton *japaneseButton;
    QPushButton *backButton;
    static QString m_currentTheme;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // THEMEWINDOW_H
