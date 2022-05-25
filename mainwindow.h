#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include "field.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

const int kBlink = 4;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int block_width = 17;
    const int number_width = 10;
    const int number_height = 20;
    int offset_x = 100;
    int offset_y = 100;
    int figure_speed = 1000 / kBlink;
    Field field = Field(&to_remove, &fly_cubes);
    Figure current;
    bool game_state = true;

    std::vector<int> to_remove;
    std::vector <Figure> fly_cubes;
    int timerId;
    int timer_cycle = 0;
    bool white_cubes = true;

    void paintGrid(QPainter &qp);
    void paintBlocks(QPainter &qp);
    void paintCurrent(QPainter &qp);
    void paintRows(QPainter &qp);
    void paintFreeCubes(QPainter &qp);
    void paintFlyCubes(QPainter &qp);
    void paintWhiteBackground(QPainter &qp);
    void moveLeft();
    void moveRight();
    void moveDown();
    void moveUp();
    void rotateCounter();
    void rotateWise();

protected:
    void paintEvent(QPaintEvent *) override;

private slots:
    void timerEvent(QTimerEvent * event) override;
    void keyPressEvent(QKeyEvent *) override;
    void on_pauseButton_pressed();
    void on_leftButton_pressed();
    void on_downButton_pressed();
    void on_rightButton_pressed();
    void on_rotationButton_pressed();
    void on_upButton_pressed();
};

#endif // MAINWINDOW_H
