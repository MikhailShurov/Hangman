#include "mainwindow.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QDebug>
#include <QGroupBox>

MainWindow::MainWindow() : QMainWindow(nullptr) {
    setupMainWindow();
}

void MainWindow::setupMainWindow() {
    QWidget *widget = new QWidget();

    QGridLayout *mainGridLayout = new QGridLayout();

    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // ToDo write game over panel
    finishPanel = new QLabel();
    finishPanel->setStyleSheet("background-color: yellow; font-size: 40px;");
    finishPanel->setAlignment(Qt::AlignCenter);
    finishPanel->setMinimumSize(QSize(500, 500));
    finishPanel->setSizePolicy(policy);

    QLabel *leftLable = new QLabel();
    leftLable->setStyleSheet("background-color: red; border: 2px solid black;");
    leftLable->setMinimumSize(QSize(500, 500));
    leftLable->setSizePolicy(policy);


    QGroupBox *wordContainer = new QGroupBox();
    wordContainer->setSizePolicy(policy);

    QGridLayout *lettersContainer = new QGridLayout();

    QPushButton **letterButtons = new QPushButton *[26];
    char currentChar = 'A';
    int size = 50;
    for (int i = 0; i < 26; ++i) {
        QPushButton *letter = new QPushButton(QString(currentChar + i));
        letter->setFixedSize(QSize(size, size));
        if (i == 24) {
            lettersContainer->addWidget(letter, 3, 3);
        } else if (i == 25) {
            lettersContainer->addWidget(letter, 3, 4);
        } else {
            lettersContainer->addWidget(letter, i / 8, i % 8);
        }
        letterButtons[i] = letter;
        alphabet_.push_back(letter);
        connect(letter, SIGNAL(clicked()), this, SLOT(checkLetter()));
//        connect(letter, &QPushButton::clicked, this, [this, letter](){checkLetter(letter);}); // That works
    }

    mainGridLayout->addWidget(leftLable, 0, 0, 0, 1);
    mainGridLayout->addWidget(finishPanel, 0, 0, 0, 1);
    finishPanel->hide();
    mainGridLayout->addWidget(wordContainer, 0, 1);
    mainGridLayout->addLayout(lettersContainer, 1, 1);


    widget->setLayout(mainGridLayout);
    setCentralWidget(widget);

    setupWord(wordContainer);
}

QString MainWindow::generateWord() {
    QFile file("words.txt");
    QVector<QString> words;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QString str = file.readLine();
            str.resize(str.size() - 1);
            words.push_back(str);
        }
    }
    QRandomGenerator *rg = QRandomGenerator().global();
    QString task = words[rg->bounded(2998)];
    qDebug() << task;
    return task;
}

void MainWindow::setupWord(QGroupBox *wordLayout) {
    word_ = generateWord();
    int letterAmount = word_.size();

    QHBoxLayout *qhBoxLayout = new QHBoxLayout();


    for (int i = 0; i < letterAmount; ++i) {
        QLabel *label = new QLabel();
        label->setFixedSize(50, 50);
        label->setStyleSheet("background-color: orange; border: 2px solid black; font-size: 20px");
        label->setAlignment(Qt::AlignCenter);
        qhBoxLayout->addWidget(label);
        letters_.push_back(label);
    }
    wordLayout->setLayout(qhBoxLayout);
}

void MainWindow::checkLetter() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    button->setEnabled(false);
    bool rightLetter = false;
    for (int i = 0; i < word_.size(); ++i) {
        if (word_[i] == button->text().toLower() || word_[i] == button->text()) {
            letters_[i]->setText(QString(word_[i]));
            rightLetter = true;
            guessed_letters_ += 1;
        }
    }
    if (guessed_letters_ == word_.size()) {
        finishPanel->setText("You won!");
        finishPanel->raise();
        finishPanel->show();
        deactivateLetters();
    }
    if (!rightLetter) {
        --lifes_;
        if (lifes_ == 0) {
            qDebug() << "Lose";
            finishPanel->setText("Game Over:(");
            finishPanel->raise();
            finishPanel->show();
            deactivateLetters();
        }
    }
}

MainWindow::~MainWindow() noexcept {
    for (int i = 0; i < letters_.size(); ++i) {
        delete letters_[i];
    }
    for (int i = 0; i < alphabet_.size(); ++i) {
        delete alphabet_[i];
    }
}

void MainWindow::deactivateLetters() {
    for (int i = 0; i < alphabet_.size(); ++i) {
        alphabet_[i]->setEnabled(false);
    }
}