#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

//We can use this .h file to handle general game info i.e. scores, piece info etc...

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
class QLCDNumber;
class QLabel;
class QPushButton;

QT_END_NAMESPACE

class TetrisBoard;

//class MainWindow : public QMainWindow
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    //~MainWindow();

private:
    //Ui::MainWindow *ui;

    //layout widgets
    TetrisBoard *board;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *quitButton;
    QLCDNumber *scoreLCD;
    QLCDNumber *levelLCD;
    QLCDNumber *linesLCD;
    QLabel *nextPieceLabel;
    QImage *background;
};
#endif // MAINWINDOW_H
