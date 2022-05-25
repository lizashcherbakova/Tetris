#include "field.h"

Field::Field(std::vector<int> *to_remove, std::vector <Figure> *free_cubes) {
    for (size_t i = 0; i < field.size(); ++i) {
        field[i] = new CoordinatesX();
    }
    rows_to_remove = to_remove;
    fly_cubes = free_cubes;
}

Field::~Field() {
    for (size_t i = 0; i < field.size(); ++i) {
        delete field[i];
    }
}


void Field::saveFigure(Figure *f, int gap) {
    // here we check for the end of the game.
    auto points = *(f->getCoordinates());
    for (size_t i = 0; i < points.size(); i += 2) {
        //  Check if y coordinate is out of range.
        if (points[i + 1] + f->offsety + gap >= 0) {
            field[points[i + 1] + f->offsety + gap]->arr[points[i] + f->offsetx] = f->color;
            field[points[i + 1] + f->offsety + gap]->filled++;
        } else {
            game_over = true;
            break;
        }
    }
    checkFullRow();
    ready_fly_cubes.clear();
    checkFreeCubes();
}

bool Field::checkFutureCollision(Figure *f, int secGap) {
    //  Finding first full row.
    auto points = *(f->getCoordinates());
    for (size_t i = 0; i < points.size(); i += 2) {
        //  Don't check if it is out of the field.
        if (points[i + 1] + f->offsety + secGap < 0) {
            continue;
        }
        //  Check if next secGap steps will cause collision.
        if (points[i + 1] + f->offsety + secGap >= row_number) {
            saveFigure(f, secGap - 1);
            return true;
        }
        if (field[points[i + 1] + f->offsety + secGap]->arr[points[i] + f->offsetx] != Qt::GlobalColor::white) {
            //  Then save figure to the filed.
            saveFigure(f, secGap - 1);
            return true;
        }
    }
    return false;
}

bool Field::checkCollisions(Figure *f) {
    auto points = *(f->getCoordinates());
    for (size_t i = 0; i < points.size(); i += 2) {
        // Check that figure doesn't cross the borders.
        if (points[i] + f->offsetx < 0 || points[i] + f->offsetx >= column_number) {
            return true;
        }

        if (points[i + 1] + f->offsety < 0) {
            continue;
        }

        if (points[i + 1] + f->offsety >= row_number) {
            return true;
        }

        if (field[points[i + 1] + f->offsety]->arr[points[i] + f->offsetx] != Qt::GlobalColor::white) {
            return true;
        }
    }
    return false;
}

void Field::checkCollisionCubes(Figure *f) {
    // Index of the first encountered cube.
    int answer = -1;
    auto points = *(f->getCoordinates());
    for (size_t i = 0; i < points.size(); i += 2) {
        for (size_t cube = 0; cube < fly_cubes->size(); ++cube) {
            //  Counting current coordinates of the flying cube.
            int x = (*fly_cubes)[cube].getCoordinates()->at(0) + (*fly_cubes)[cube].offsetx;
            int y = (*fly_cubes)[cube].getCoordinates()->at(1) + (*fly_cubes)[cube].offsety;
            //  int x_diff = points[i] + f->offsetx - x;
            //  int y_diff = points[i+1] + f->offsety -y;
            if (std::abs(points[i] + f->offsetx - x) <= 1 && std::abs(points[i + 1] + f->offsety - y) <= 1) {
                if (answer < 0) {
                    answer = cube;
                    //  If we found second cube to crush in the figure.
                } else if (answer != cube) {
                    f->collapse({answer, (int) cube}, fly_cubes);
                    return;
                }
            }
        }
    }
    if (answer >= 0)f->collapse({answer}, fly_cubes);
}

void Field::checkFreeCubes() {
    if (field[0]->filled) {
        for (int x = 1; x < column_number - 1; ++x) {
            if (field[0]->arr[x] != Qt::GlobalColor::white && field[0]->arr[x - 1] == Qt::GlobalColor::white &&
                field[0]->arr[x + 1] == Qt::GlobalColor::white
                && field[1]->arr[x] != Qt::GlobalColor::white && field[1]->arr[x - 1] == Qt::GlobalColor::white &&
                field[1]->arr[x + 1] == Qt::GlobalColor::white) {
                ready_fly_cubes.push_back(Figure({x, 0}, field[0]->arr[x]));
            }
        }
    }
    for (size_t y = 1; y < field.size() - 1; ++y) {
        if (field[y]->filled) {
            for (int x = 1; x < column_number - 1; ++x) {
                if (field[y]->arr[x] != Qt::GlobalColor::white && field[y]->arr[x - 1] == Qt::GlobalColor::white &&
                    field[y]->arr[x + 1] == Qt::GlobalColor::white
                    && field[1 + y]->arr[x] != Qt::GlobalColor::white &&
                    field[1 + y]->arr[x - 1] != Qt::GlobalColor::white &&
                    field[1 + y]->arr[x + 1] != Qt::GlobalColor::white
                    && field[y - 1]->arr[x] == Qt::GlobalColor::white &&
                    field[y - 1]->arr[x - 1] == Qt::GlobalColor::white &&
                    field[y - 1]->arr[x + 1] == Qt::GlobalColor::white) {
                    ready_fly_cubes.push_back(Figure({x, (int) y}, field[y]->arr[x]));
                }
            }
        }
    }
    if (field[row_number - 1]->filled) {
        for (int x = 1; x < column_number - 1; ++x) {
            if (field[row_number - 1]->arr[x] != Qt::GlobalColor::white &&
                field[row_number - 1]->arr[x - 1] == Qt::GlobalColor::white &&
                field[row_number - 1]->arr[x + 1] == Qt::GlobalColor::white
                && field[row_number - 2]->arr[x] == Qt::GlobalColor::white &&
                field[row_number - 2]->arr[x - 1] == Qt::GlobalColor::white &&
                field[row_number - 2]->arr[x + 1] == Qt::GlobalColor::white) {
                ready_fly_cubes.push_back(Figure({x, row_number - 1}, field[row_number - 1]->arr[x]));
            }
        }
    }

}

void Field::checkFullRow() {
    for (size_t i = 0; i < field.size(); ++i) {
        if (field[i]->filled == column_number) {
            rows_to_remove->push_back(i);
        }
    }
}

void Field::removeRows() {
    for (size_t i = 0; i < rows_to_remove->size(); ++i) {
        delete field[rows_to_remove->at(i)];
        field.erase(field.begin() + rows_to_remove->at(i));
        field.insert(field.begin(), new CoordinatesX());
    }
}

void Field::launchCubes() {
    fly_cubes->reserve(fly_cubes->size() + ready_fly_cubes.size());
    for (size_t i = 0; i < ready_fly_cubes.size(); ++i) {
        fly_cubes->push_back(ready_fly_cubes[i]);
    }
    for (size_t i = 0; i < ready_fly_cubes.size(); ++i) {
        int x = ready_fly_cubes[i].getCoordinates()->at(0);
        int y = ready_fly_cubes[i].getCoordinates()->at(1);
        field[y]->arr[x] = Qt::GlobalColor::white;
        field[y]->filled--;
    }
    ready_fly_cubes.clear();
}

void Field::startOver() {
    this->fly_cubes->clear();
    this->ready_fly_cubes.clear();
    for (size_t i = 0; i < field.size(); ++i) {
        field[i]->filled = 0;
        for (int j = 0; j < column_number; ++j) {
            field[i]->arr[j] = Qt::GlobalColor::white;
        }
    }
    game_over = false;
}

std::vector <Figure> *Field::getReadyFlyCubes() {
    return &ready_fly_cubes;
}
