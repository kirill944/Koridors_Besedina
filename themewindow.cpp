#include "themewindow.h"
#include <QPainter>
#include <QMessageBox>

QString ThemeWindow::m_currentTheme = "forest";

QString ThemeWindow::currentTheme()
{
    return m_currentTheme;
}

ThemeWindow::ThemeWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Выбор тематики");
    setFixedSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    forestButton = new QPushButton("Таинственный лес", this);
    waterButton = new QPushButton("Подводный мир", this);
    japaneseButton = new QPushButton("Японская эстетика", this);
    backButton = new QPushButton("Вернуться в меню", this);

    QString buttonStyle = "QPushButton {"
                          "background-color: rgba(255, 255, 255, 150);"
                          "border-radius: 5px;"
                          "padding: 8px;"
                          "font-size: 14px;"
                          "min-width: 150px;"
                          "}";
    forestButton->setStyleSheet(buttonStyle);
    waterButton->setStyleSheet(buttonStyle);
    japaneseButton->setStyleSheet(buttonStyle);
    backButton->setStyleSheet(buttonStyle);

    layout->addStretch();
    layout->addWidget(forestButton);
    layout->addWidget(waterButton);
    layout->addWidget(japaneseButton);
    layout->addStretch();
    layout->addWidget(backButton, 0, Qt::AlignRight);

    connect(forestButton, &QPushButton::clicked, this, &ThemeWindow::handleForestTheme);
    connect(waterButton, &QPushButton::clicked, this, &ThemeWindow::handleWaterTheme);
    connect(japaneseButton, &QPushButton::clicked, this, &ThemeWindow::handleJapaneseTheme);
    connect(backButton, &QPushButton::clicked, this, &ThemeWindow::handleBackButton);
}

void ThemeWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QString bgPath = QString(":/resources/%1_bg/theme_bg.jpg").arg(m_currentTheme);
    QPixmap background(bgPath);
    if (!background.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), background);
    }
    QWidget::paintEvent(event);
}

void ThemeWindow::handleForestTheme()
{
    m_currentTheme = "forest";
    emit themeSelected(m_currentTheme);
    QMessageBox::information(this, "Тема изменена", "Выбрана тематика: Таинственный лес");
    emit backToStart();
    this->close();
}

void ThemeWindow::handleWaterTheme()
{
    m_currentTheme = "water";
    emit themeSelected(m_currentTheme);
    QMessageBox::information(this, "Тема изменена", "Выбрана тематика: Подводный мир");
    emit backToStart();
    this->close();
}

void ThemeWindow::handleJapaneseTheme()
{
    m_currentTheme = "japanese";
    emit themeSelected(m_currentTheme);
    QMessageBox::information(this, "Тема изменена", "Выбрана тематика: Японская эстетика");
    emit backToStart();
    this->close();
}

void ThemeWindow::handleBackButton()
{
    emit backToStart();
    this->close();
}
