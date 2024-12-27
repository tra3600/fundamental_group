#include <iostream>
#include <vector>
#include <cmath>
#include <complex>

// Définition d'un point sur le cercle unitaire
struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

// Fonction pour créer un point sur le cercle unitaire à un angle donné
Point point_on_circle(double angle) {
    return Point(std::cos(angle), std::sin(angle));
}

// Classe représentant une boucle sur le cercle
class Loop {
private:
    std::vector<Point> points;
public:
    // Constructeur prenant une séquence d'angles pour définir la boucle
    Loop(const std::vector<double>& angles) {
        for (double angle : angles) {
            points.push_back(point_on_circle(angle));
        }
    }

    // Fonction pour afficher la boucle
    void print() const {
        for (const Point& p : points) {
            std::cout << "(" << p.x << ", " << p.y << ") ";
        }
        std::cout << std::endl;
    }

    // Fonction pour composer deux boucles (concaténation)
    Loop compose(const Loop& other) const {
        std::vector<double> combined_angles;
        for (const Point& p : points) {
            combined_angles.push_back(std::atan2(p.y, p.x));
        }
        for (const Point& p : other.points) {
            combined_angles.push_back(std::atan2(p.y, p.x));
        }
        return Loop(combined_angles);
    }
};

int main() {
    // Définir deux boucles sur le cercle
    std::vector<double> angles1 = {0, M_PI / 2, M_PI, 3 * M_PI / 2, 2 * M_PI};
    std::vector<double> angles2 = {0, M_PI / 4, M_PI / 2, 3 * M_PI / 4, M_PI};

    Loop loop1(angles1);
    Loop loop2(angles2);

    // Afficher les boucles
    std::cout << "Loop 1: ";
    loop1.print();
    std::cout << "Loop 2: ";
    loop2.print();

    // Composer les deux boucles et afficher le résultat
    Loop composed_loop = loop1.compose(loop2);
    std::cout << "Composed Loop: ";
    composed_loop.print();

    return 0;
}