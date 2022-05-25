#ifndef FIELD_H
#define FIELD_H

#include <QColor>
#include <vector>
#include "figure.h"

static
const int row_number = 20;
static
const int column_number = 10;

struct CoordinatesX {
    int filled = 0;
    QColor arr[column_number];

    CoordinatesX() {
        for (int i = 0; i < column_number; ++i) {
            arr[i] = Qt::GlobalColor::white;
        }
    }
};

class Field {
    std::vector<int> *rows_to_remove;
    std::vector <Figure> *fly_cubes;
    std::vector <Figure> ready_fly_cubes;
    std::vector<CoordinatesX *> field = std::vector<CoordinatesX *>(row_number);
    void saveFigure(Figure *f, int gap);

public:
    bool game_over = false;
    //  Checks if futher down move of the figure will make it touch other figures.
    //  If so, saves the figure information and returns true.
    bool checkFutureCollision(Figure *f, int sec_gap);
    //  Returns true if figure overlaps.
    bool checkCollisions(Figure *f);
    //  Checks if collision happened. If so returns the cube that caused collision.
    //  Otherwise returns null. Removes collapsed cubes from array.
    void checkCollisionCubes(Figure *f);
    void checkFreeCubes();
    void removeFreeCubes();
    void checkFullRow();
    void removeRows();
    void launchCubes();
    void startOver();;
    std::vector <Figure> *getReadyFlyCubes();
    inline std::vector<CoordinatesX *> *getFiled() {
        return &field;
    }
    Field(std::vector<int> *to_remove, std::vector <Figure> *free_cubes);
    ~Field();
};

#endif // FIELD_H
