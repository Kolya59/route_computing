#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <cmath>
#include <limits>

const char DELIMETER = ';';

class dot {
    int x, y;

public:
    dot(){
        this->x = 0;
        this->y = 0;
    }

    dot(int x, int y) {
        this->x = x;
        this->y = y;
    }

    int get_x() const {
        return this->x;
    }

    int get_y() const {
        return this->y;
    }
};


class dimension {
    dot dot;
    // |v|
    int phi;

public:
    dimension() {
        // TODO: Сделать нормалный вызов конструктора для создания значения, а не ссылки
        this->dot = *(new class dot());
        this-> phi = 0;
    }

    dimension(class dot dot, int phi) {
        this->dot = dot;
        this-> phi = phi;
    }

    class dot get_dot() {
        return this->dot;
    }

    int get_phi() const {
        return this->phi;
    }
};

inline bool check_file_existence (const std::string& name) {
    struct stat buffer{};
    return (stat (name.c_str(), &buffer) == 0);
}

// Считывание трассы
std::vector<dot>* get_trace(const std::string& filename) {
    if (!check_file_existence(filename)) {
        std::cout << "File is not exists";
        exit(1);
    }

    std::ifstream f;
    try {
        f.open(filename);
    } catch (...) {
        std::cout << "Failed to open file";
        exit(1);
    };

    auto dots = new std::vector<dot>;
    std::string dot_str;
    while (f >> dot_str) {
        auto pos = dot_str.find(DELIMETER);
        // Сделай проверку данных (функция strtol)
        auto x = atoi(dot_str.substr(0, pos).data());
        auto y = atoi(dot_str.substr(pos + 1).data());
        dots->push_back(dot(x, y));
    }
    f.close();

    return dots;
}

// Считывание показаний машины
std::vector<dimension>* get_dimensions(const std::string& filename) {
    if (!check_file_existence(filename)) {
        std::cout << "File is not exists";
        exit(1);
    }

    std::ifstream f;
    try {
        f.open(filename);
    } catch (...) {
        std::cout << "Failed to open file";
        exit(1);
    };

    auto dimensions = new std::vector<dimension>;
    std::string dot_str;
    while (f >> dot_str) {
        auto pos = dot_str.find(DELIMETER);
        // Сделай проверку данных (функция strtol)
        auto x = atoi(dot_str.substr(0, pos).data());
        auto y = atoi(dot_str.substr(pos + 1).data());
        auto phi = atoi(dot_str.substr(pos + 1).data());
        dimensions->push_back(dimension(dot(x, y), phi));
    }
    f.close();

    return dimensions;
}

// Поиск ближайшей точки трассы к координате машины
// Методом вычисления расстояния до каждой точки
dot get_nearest(dot source, std::vector<dot>* trace) {
    /*// Multithread
     * auto work = [](dot source, dot target) -> int {
        return sqrt((target.get_x()-source.get_x())*(target.get_x()-source.get_x()) + (target.get_y()-source.get_y())*(target.get_y()-source.get_y()));
    };
    for (auto & curr : *trace) {
        std::thread tread(work, source, curr);
    }*/
    // Iterative
    auto min = std::numeric_limits<double>::max();
    dot min_dot;
    double dist;
    for (auto & curr : *trace) {
        dist = sqrt(
                (curr.get_x() - source.get_x()) *
                (curr.get_x() - source.get_x()) +
                (curr.get_y() - source.get_y()) *
                (curr.get_y() - source.get_y()));
        if (dist >= min) {
            continue;
        }

        min = dist;
        min_dot = curr;
    }

    return min_dot;
}

// Вычисление пути машины
std::vector<dot>* compute_route(std::vector<dot>* trace, std::vector<dimension>* dimensions) {
    auto* route = new std::vector<dot>;
    for (auto & curr : *dimensions) {
        route->push_back(get_nearest(curr.get_dot(), trace));
    }
    return route;
}

int main() {
    auto trace = get_trace("/Users/ndnikitas/CLionProjects/car/data/trace.txt");
    auto dimensions = get_dimensions("/Users/ndnikitas/CLionProjects/car/data/dimensions.txt");
    auto route = compute_route(trace, dimensions);
    for (auto & curr : *route) {
        std::cout << "X = " << curr.get_x() << ", Y = " << curr.get_y() << std::endl;
    }
    return 0;
}
