#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    timerId = startTimer(figure_speed);
    ui->setupUi(this);
    ui->LabelGameOver->setText("");
    ui->LabelGameOver->setGeometry(offset_x, 30, block_width * number_width, 20);

    ui->pauseButton->setGeometry(offset_x + block_width * 7 / 2, offset_y - ui->pauseButton->height() - 15,
                                 ui->pauseButton->width(), ui->pauseButton->height());
    ui->pauseButton->setIcon(QIcon(":/res/res/pause.png"));
    ui->leftButton->setIcon(QIcon(":/res/res/left.png"));
    ui->downButton->setIcon(QIcon(":/res/res/down.png"));
    ui->rightButton->setIcon(QIcon(":/res/res/right.png"));
    ui->rotationButton->setIcon(QIcon(":/res/res/plus.png"));
    ui->upButton->setIcon(QIcon(":/res/res/plus.png"));
}

MainWindow::~MainWindow() {
    killTimer(timerId);
    delete ui;
}

void MainWindow::paintGrid(QPainter &qp) {
    qp.setPen(QColor(199, 199, 199));

    for (int i = 0; i <= number_width; ++i) {
        qp.drawLine(offset_x + i * block_width, offset_y, offset_x + i * block_width,
                    offset_y + number_height * block_width);
    }

    for (int i = 0; i <= number_height; ++i) {
        qp.drawLine(offset_x, offset_y + i * block_width, offset_x + number_width * block_width,
                    offset_y + i * block_width);
    }
}

void MainWindow::paintBlocks(QPainter &qp) {
    auto coord = *(field.getFiled());
    for (int y = 0; y < number_height; ++y) {
        for (int x = 0; x < number_width; ++x) {
            qp.fillRect(offset_x + x * block_width, offset_y + y * block_width, block_width, block_width,
                        coord[y]->arr[x]);
        }
    }
}

void MainWindow::paintCurrent(QPainter &qp) {
    auto coord = *(current.getCoordinates());
    for (size_t i = 0; i < coord.size(); i += 2) {
        qp.fillRect(offset_x + (coord[i] + current.offsetx) * block_width,
                    offset_y + (coord[i + 1] + current.offsety) * block_width,
                    block_width, block_width, current.color);
    }
}

void MainWindow::paintRows(QPainter &qp) {
    qp.setPen(Qt::GlobalColor::yellow);
    for (int &y: to_remove) {
        qp.drawLine(offset_x, offset_y + y * block_width, offset_x + number_width * block_width,
                    offset_y + y * block_width);
        for (int i = 0; i <= number_width; ++i) {
            qp.drawLine(offset_x + i * block_width, offset_y + y * block_width, offset_x + i * block_width,
                        offset_y + (y + 1) * block_width);
        }
        qp.drawLine(offset_x, offset_y + (y + 1) * block_width, offset_x + number_width * block_width,
                    offset_y + (y + 1) * block_width);
    }
}

void MainWindow::paintFreeCubes(QPainter &qp) {
    auto ready_fly_cubes = *(field.getReadyFlyCubes());
    for (size_t i = 0; i < ready_fly_cubes.size(); ++i) {
        int x = ready_fly_cubes[i].getCoordinates()->at(0);
        int y = ready_fly_cubes[i].getCoordinates()->at(1);
        qp.fillRect(offset_x + x * block_width, offset_y + y * block_width, block_width, block_width,
                    white_cubes ? Qt::GlobalColor::white : ready_fly_cubes[i].color);
    }
}

void MainWindow::paintFlyCubes(QPainter &qp) {
    for (size_t i = 0; i < fly_cubes.size(); ++i) {
        int x = fly_cubes[i].getCoordinates()->at(0) + fly_cubes[i].offsetx;
        int y = fly_cubes[i].getCoordinates()->at(1) + fly_cubes[i].offsety;
        qp.fillRect(offset_x + x * block_width, offset_y + y * block_width, block_width, block_width,
                    fly_cubes[i].color);
    }
}

void MainWindow::paintWhiteBackground(QPainter &qp) {
    qp.fillRect(0, 0, offset_x, this->height(), Qt::GlobalColor::white);
    qp.fillRect(offset_x, 0, this->width(), offset_y, Qt::GlobalColor::white);
    qp.fillRect(offset_x + block_width * number_width, offset_y, offset_x, this->height(), Qt::GlobalColor::white);
    qp.fillRect(offset_x, offset_y + block_width * number_height, width(), height(), Qt::GlobalColor::white);
}

void MainWindow::moveLeft() {
    current.offsetx -= 1;
    //  If found collision - don't react.
    if (field.checkCollisions(&current)) {
        current.offsetx += 1;
    }

    if (field.checkFutureCollision(&current, 1)) {
        current = Figure();
    }
}

void MainWindow::moveRight() {
    current.offsetx += 1;
    //  If found collision - don't react.
    if (field.checkCollisions(&current)) {
        current.offsetx -= 1;
    }

    if (field.checkFutureCollision(&current, 1)) {
        current = Figure();
    }
}

void MainWindow::moveDown() {
    if (!field.checkFutureCollision(&current, 1)) {
        current.moveDown();
    } else if (field.game_over) {
        ui->LabelGameOver->setText("Game is over!");
        white_cubes = false;
        killTimer(timerId);
        game_state = false;
    } else {
        current = Figure();
    }
}

void MainWindow::moveUp() {
    for (size_t i = 0; i < fly_cubes.size(); ++i) {
        // move up.
        fly_cubes[i].offsety--;
        if (fly_cubes[i].offsety + fly_cubes[i].getCoordinates()->at(1) < 0) {
            fly_cubes.erase(fly_cubes.begin() + i);
            --i;
        }
    }
    field.checkCollisionCubes(&current);
    if (!current.getCoordinates()->size()) {
        current = Figure();
    }
    update();
}

void MainWindow::rotateCounter() {
    current.rotateCounterClock();
    //  If found collision - don't react.
    if (field.checkCollisions(&current)) {
        current.rotateClockWise();
    }

    if (field.checkFutureCollision(&current, 1)) {
        current = Figure();
    }
}

void MainWindow::rotateWise() {
    current.rotateClockWise();
    //  If found collision - don't react.
    if (field.checkCollisions(&current)) {
        current.rotateCounterClock();
    }

    if (field.checkFutureCollision(&current, 1)) {
        current = Figure();
    }
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter qp;
    qp.begin(this);

    //  Painting stuck blocks.
    paintBlocks(qp);
    //  Painting main flying down figure.
    paintCurrent(qp);
    //  Painting feature blinking cubes.
    paintFreeCubes(qp);
    //  Painting flying cube.
    paintFlyCubes(qp);
    //  Painting white background.
    paintWhiteBackground(qp);
    //  Painting grid(just lines).
    paintGrid(qp);
    //  Highlighting full row.
    paintRows(qp);

    qp.end();
}

void MainWindow::timerEvent(QTimerEvent *) {
    white_cubes = !white_cubes;
    if (!timer_cycle) {
        field.removeRows();
        to_remove.clear();
        field.getReadyFlyCubes()->clear();
        field.checkFreeCubes();

        moveUp();
        moveDown();
    }
    //  Refreshig timer data.
    timer_cycle = (timer_cycle + 1) % kBlink;

    this->update();
    this->setFocus();
}

void MainWindow::keyPressEvent(QKeyEvent *key) {
    if (game_state) {
        switch (key->nativeVirtualKey()) {
            case 37:
                moveLeft();
                break;
            case 38:
                field.launchCubes();
                break;
            case 39:
                moveRight();
                break;
            case 40:
                moveDown();
                break;
            case 188:
                rotateCounter();
                break;
            case 190:
                rotateWise();
                break;
            default:
                return;
        }
        field.checkCollisionCubes(&current);
        if (!current.getCoordinates()->size()) {
            current = Figure();
        }
        this->update();
    }
}

void MainWindow::on_pauseButton_pressed() {
    if (game_state && !field.game_over) {
        killTimer(timerId);
        game_state = false;
        this->ui->LabelGameOver->setText("Game is paused.");
    } else if (field.game_over) {
        to_remove.clear();
        field.startOver();
        current = Figure();
        timerId = startTimer(figure_speed);
        game_state = true;
        this->ui->LabelGameOver->setText("");
        this->update();
    } else {
        timerId = startTimer(figure_speed);
        game_state = true;
        this->ui->LabelGameOver->setText("");
    }
}


void MainWindow::on_leftButton_pressed() {
    if (game_state) {
        moveLeft();
        this->update();
    }
}


void MainWindow::on_downButton_pressed() {
    if (game_state) {
        moveDown();
        this->update();
    }
}


void MainWindow::on_rightButton_pressed() {
    if (game_state) {
        moveRight();
        this->update();
    }
}


void MainWindow::on_rotationButton_pressed() {
    rotateWise();
}


void MainWindow::on_upButton_pressed() {
    field.launchCubes();
}
