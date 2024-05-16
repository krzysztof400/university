#include <iostream>
#include <cmath>
#include <stdexcept>

class Figure {
public:
    virtual double calculate_area() const = 0;
    virtual double calculate_perimeter() const = 0;
    virtual std::string name() const = 0;
    virtual ~Figure() {}
};

class Quadrilateral : public Figure {
protected:
    double side1, side2, side3, side4, angle;
public:
    Quadrilateral(double s1, double s2, double s3, double s4)
        : side1(s1), side2(s2), side3(s3), side4(s4) {
            if(s1<=0 || s2<=0 || s3<=0 || s4<=0)
            {
                throw std::invalid_argument("side values in quadrilateral cant be nonpositive");
            }
        }
};

class Square : public Quadrilateral {
public:
    Square(double side) : Quadrilateral(side, side, side, side) {}

    double calculate_area() const override {
        return side1 * side1;
    }

    double calculate_perimeter() const override {
        return side1 * 4;
    }

    std::string name() const override {
        return "Square";
    }
};

class Rhombus : public Quadrilateral 
{
private: double angle;
public:
    Rhombus(double side, double ang) : Quadrilateral(side, side, side, side), angle(ang) {
        if(ang<=0 || ang>=180)
        {
            throw std::invalid_argument("angle in rhomb must be in range from 0 to 180 degrees");
        }
    }

    double calculate_area() const override {
        return sin(angle * M_PI / 180) * side1 * side1;
    }

    double calculate_perimeter() const override {
        return side1 * 4;
    }

    std::string name() const override {
        return "Rhombus";
    }
};

class Rectangle : public Quadrilateral {
public:
    Rectangle(double s1, double s2) : Quadrilateral(s1, s2, s1, s2) {}

    double calculate_area() const override {
        return side1 * side2;
    }

    double calculate_perimeter() const override {
        return 2 * side1 + 2 * side2;
    }

    std::string name() const override {
        return "Rectangle";
    }
};

class Pentagon : public Figure {
private:
    double side;
public:
    Pentagon(double s) : side(s) {
        if(s<=0)
        {
            throw std::invalid_argument("siedes in pentagon cant have nonpositive values");
        }
    }

    double calculate_area() const override {
        return 0.25 * sqrt(5 * (5 + 2 * sqrt(5))) * side * side;
    }

    double calculate_perimeter() const override {
        return 5 * side;
    }

    std::string name() const override {
        return "Pentagon";
    }
};

class Hexagon : public Figure {
private:
    double side;
public:
    Hexagon(double s) : side(s) {
        if(s<=0)
        {
            throw std::invalid_argument("sides in hexagon cant have nonpositive values");
        }
    }

    double calculate_area() const override {
        return 3 * sqrt(3) * side * side / 2;
    }

    double calculate_perimeter() const override {
        return 6 * side;
    }

    std::string name() const override {
        return "Hexagon";
    }
};

class Circle : public Figure {
private:
    double radius;
public:
    Circle(double r) : radius(r) {
        if(r<=0)
        {
            throw std::invalid_argument("radius value cant be non positive");
        }
    }

    double calculate_area() const override {
        return M_PI * radius * radius;
    }

    double calculate_perimeter() const override {
        return 2 * M_PI * radius;
    }

    std::string name() const override {
        return "Circle";
    }
};


int main(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::invalid_argument("Too few arguments. Usage: <figure type> <figure property 1> <figure property 2> ...");
    }

    double parameters[argc-2];
        for (int i = 0; i < argc-2; i++) {
            try {
                parameters[i] = std::stod(argv[i+2]);
            } catch (const std::invalid_argument& e) {
                throw std::invalid_argument("Figure properties must be doubles.");
            }
        }

    Figure* fig = nullptr;
    switch (argv[1][0]) {
        case 'c':
        if(argc-2 != 5)
            {
                throw std::invalid_argument("To create quadrilateral you need 5 arguments");
            }
            if(parameters[0] == parameters[1] && parameters[1] == parameters[2] && parameters[2] == parameters[3])
            {
                if(parameters[4] == 90)
                {
                    fig = new Square(parameters[0]);
                }
                else
                {
                    fig = new Rhombus(parameters[0], parameters[4]);
                }
            }
            else if (parameters[4] == 90)
            {
                if(parameters[0] == parameters[1] && parameters[2] == parameters[3])
                {
                    fig = new Rectangle(parameters[0], parameters[2]);
                }
                else if(parameters[0] == parameters[2] && parameters[1] == parameters[3])
                {
                    fig = new Rectangle(parameters[0], parameters[2]);
                }
                else if(parameters[0] == parameters[3] && parameters[1] == parameters[2])
                {
                    fig = new Rectangle(parameters[0], parameters[1]);
                }
                else
                {
                    throw std::invalid_argument("Sides of the 'Recktangle' arent pairwise equal, therefor we cant crete any of available figures");
                }
            }
            else
            {
                throw std::invalid_argument("Sides arent equall and angle isnt 90 degrees");
            }
            break;
        case 'o':
            if (argc-2 != 1) {
                throw std::invalid_argument("To create a circle, you need 1 argument.");
            }
            fig = new Circle(parameters[0]);
            break;
        case 's':
            if (argc-2 != 1) {
                throw std::invalid_argument("To create a hexagon, you need 1 argument.");
            }
            fig = new Hexagon(parameters[0]);
            break;
        case 'p':
            if (argc-2 != 1) {
                throw std::invalid_argument("To create a pentagon, you need 1 argument.");
            }
            fig = new Pentagon(parameters[0]);
            break;
            
        default:
            throw std::invalid_argument("Figure type must be c, o, s or p.");
    }

    std::cout << "Figure type: " << fig->name() << std::endl;
    std::cerr << "Area: " << fig->calculate_area() << std::endl;
    std::cerr << "Perimeter: " << fig->calculate_perimeter() << std::endl;

    delete fig;

    return 0;
}