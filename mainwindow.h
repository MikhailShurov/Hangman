#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    void setupMainWindow();
    QString generateWord();
    QString word_;
    QVector<QPushButton*> alphabet_;
    QVector<QLabel*> letters_;
    void setupWord(QGroupBox*);
    int lifes_ = 12;
    int guessed_letters_ = 0;

    QLabel* finishPanel;
    void deactivateLetters();
public slots:
    void checkLetter();
};
#endif // MAINWINDOW_H
