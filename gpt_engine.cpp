#include <cmath>
#include <iostream>

struct Vec2 {
    float x, y;

    // Constructors
    Vec2() : x(0.0f), y(0.0f) {}

    Vec2(float xVal, float yVal) : x(xVal), y(yVal) {}

    // Overloads
    Vec2 operator+(const Vec2& o) const { return Vec2(x + o.x, y + o.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator*(float s) const { return Vec2(x * s, y * s); }
    Vec2 operator/(float s) const { return Vec2(x / s, y / s); }

    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
    Vec2& operator*=(float s) { x *= x; y *= s; return *this; }

    // Methods
    float length() const { return std::sqrt(x*x + y*y); }

    Vec2 unit() const {
        float len = length();
        if (len == 0.0f) return Vec2(0.0f, 0.0f);
        return *this / len;
    }

    float angle() { return std::atan2(y, x); }

    float angleDeg() {
        float angle = std::atan2(y, x);
        return angle * 180 / 3.1415926;
    }
};

class Particle {
    private:

    public:
        Vec2 position, velocity;
        float mass = 1.0f;
        Vec2 forceAccum{0.0f, 0.0f};

        Particle()
            : position(Vec2()), velocity(Vec2()), mass(0.0f) {}

        // There is no need to construct this with an acceleration because we are going to have a function that calculates this particles acceleration due
        // to the sum of the forces on this particle
        Particle(Vec2 pos, Vec2 vel, float m) {
            position = pos;
            velocity = vel;
            mass = m;
        }

        void addForce(const Vec2& f) {
            forceAccum += f;
        }

        void applyImpulse(const Vec2& impulse) {
            if (mass <= 0.0f) return;
            velocity += impulse / mass;
        }

        void integrate (float dt) {
            if (mass <= 0.0f) return;
            Vec2 acceleration = forceAccum / mass;
            velocity += acceleration * dt;
            position += velocity * dt;
            clearForces();
        }

        void clearForces() { forceAccum = Vec2(0.0f, 0.0f); }

        void print() {
            std::cout << "Position: (" << position.x << ", " << position.y << ") |"
                      << "Velocity: (" << velocity.x << ", " << velocity.y << ")\n";
        }
};


float calculate_range(float acceleration, float theta, float vo) {
    return std::sin(2.0f * theta) * vo * vo / acceleration;
}


int main() {

    auto position = Vec2(0.0f, 0.0f);
    auto velocity = Vec2(1.0f, 1.0f);
    auto gravity = Vec2(0.0f, -9.81f);
    float mass = 5.0f; // kg

    Particle p1(position, velocity, mass);

    float dt = 0.0001;
    float max_time = 5.0f;
    float time = 0;

    for (int i = 0; i < 5000; i++) {
        p1.addForce(gravity * p1.mass);

        // example: engine thrust for the first 0.2s
        if (i * dt < 0.2f) {
            Vec2 thrust = Vec2(50.0f, 0.0f); // Newtons
            p1.addForce(thrust);
        }

        // Linear Drag
        float k = 0.1f;
        p1.addForce(p1.velocity * (-k));

        p1.integrate(dt);
        if (i % 500 == 0) {
            std::cout << "Time: " << i * dt << " ";
            p1.print();
        }
    }

    std::cout << "Time: " << time << " ";
    p1.print();

    // Calculate the range of this projectile
    float range = calculate_range(gravity.length(), velocity.angle(), velocity.length());

    std::cout << "-----------------------------------------------\n";
    std::cout << "The expected range is: " << range << std::endl;

    std::cout << "-----------------------------------------------\n";
    std::cout << "The actual range is: " << p1.position.x << std::endl;

    return 0;
}
