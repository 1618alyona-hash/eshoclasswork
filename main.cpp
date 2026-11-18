#include <iostream>
#include <stdexcept>

enum class Color { RED, GREEN, BLUE };

class Planar {
public:
    virtual ~Planar() = default;

    virtual std::ostream& info(std::ostream& out) = 0;
    virtual int x() const = 0;
    virtual int y() const = 0;
    virtual int abs_sqr() const = 0;
};

class Colored {
public:
    virtual ~Colored() = default;

    virtual Color getColor() const = 0;
    virtual void setColor(Color color) = 0;
};

class Point : public Planar {
public:
    Point(int x, int y) : x(x), y(y) {}

    std::ostream& info(std::ostream& out) override {
        return out << "Point";
    }

    int x() const override { return x; }
    int y() const override { return y; }
    int abs_sqr() const override { return x * x + y * y; }

private:
    int x, y;
};

class Vector : public Planar {
public:
    Vector(Point p1, Point p2) : p1(p1), p2(p2) {}

    std::ostream& info(std::ostream& out) override {
        return out << "Vector";
    }

    int x() const override { return p2.x() - p1.x(); }
    int y() const override { return p2.y() - p1.y(); }
    int abs_sqr() const override { return x() * x() + y() * y(); }

private:
    Point p1, p2;
};

class ColoredPoint : public Planar, public Colored {
public:
    ColoredPoint(Point p, Color col) : p(p), col(col) {}

    std::ostream& info(std::ostream& out) override {
        return out << "Colored Point";
    }

    int x() const override { return p.x(); }
    int y() const override { return p.y(); }
    int abs_sqr() const override { return p.abs_sqr(); }

    Color getColor() const override { return col; }
    void setColor(Color color) override { col = color; }

private:
    Point p;
    Color col;
};

class Brush : public Colored {
public:
    Brush(Color col) : col(col) {}

    Color getColor() const override { return col; }
    void setColor(Color color) override { col = color; }

private:
    Color col;
};

class Circle : public Planar {
public:
    Circle(Point c, int r) : c(c), r(r) {
        if (r < 0) {
            throw std::invalid_argument("Radius cannot be negative");
        }
    }

    std::ostream& info(std::ostream& out) override {
        return out << "Circle";
    }

    int x() const override { return c.x(); }
    int y() const override { return c.y(); }
    int abs_sqr() const override { return c.abs_sqr(); }

    double sqr() const {
        return 3.14 * r * r;
    }

    void move(Point new_c) {
        c = new_c;
    }

    void resize(int new_r) {
        if (new_r < 0) {
            throw std::invalid_argument("Radius cannot be negative");
        }
        r = new_r;
    }

protected:
    Point c;
    int r;
};

class ColoredCircle : public Circle, public Colored {
public:
    ColoredCircle(Point c, int r, Color col) : Circle(c, r), col(col) {}

    std::ostream& info(std::ostream& out) override {
        return out << "Colored Circle";
    }

    Color getColor() const override { return col; }
    void setColor(Color color) override { col = color; }

private:
    Color col;
};

std::ostream& print_abs_sqr(std::ostream& out, Planar* p) {
    return p->info(out) << ' ' << p->abs_sqr();
}

void del(Planar* arr[], size_t n) {
    for (size_t i = 0; i < n; i++) {
        delete arr[i];
    }
}

int main() {
    size_t const MAX = 10;
    Planar* arr[MAX];
    size_t n = 0;

    try {
        arr[n++] = new Point(1, 2);
        arr[n++] = new Point(3, 4);

        arr[n++] = new Vector(Point(1, 2), Point(3, 4));
        arr[n++] = new Vector(Point(3, 4), Point(1, 2));

        arr[n++] = new ColoredPoint(Point(1, 2), Color::RED);
        arr[n++] = new ColoredPoint(Point(3, 4), Color::GREEN);

        arr[n++] = new Circle(Point(0, 0), 5);
        arr[n++] = new ColoredCircle(Point(10, 10), 3, Color::BLUE);

    } catch (const std::exception& e) {
        del(arr, n);
        std::cerr << "Exception: " << e.what() << '\n';
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        print_abs_sqr(std::cout, arr[i]) << '\n';
    }

    std::cout << '\n';

    Circle c1(Point(0, 0), 1);

    std::cout << "Area: " << c1.sqr() << '\n';

    c1.move(Point(5, 5));
    std::cout << "X: " << c1.x() << '\n';

    c1.resize(10);
    std::cout << "New area: " << c1.sqr() << '\n';

    del(arr, n);

    return 0;
}
