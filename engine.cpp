#include <cmath>
#include <iostream>

struct Vec2 {
    float x, y;

    // Constructors
    Vec2()
        : x(0.0f), y(0.0f) {}

    Vec2(float xVal, float yVal)
        : x(xVal), y(yVal) {}

    // Overloads
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    // This is probably not necessary
    Vec2 operator/(float scalar) const {
        return operator*(1/scalar);
    }

    // Methods
    float length() {
        float length = x*x + y*y;
        return std::sqrt(length);
    }

    Vec2 unitVec() {
        return *this/length();
    }

    float angle() {
        return std::atan2(y, x);
    }

    float angleDeg() {
        float angle = std::atan2(y, x);
        return angle * 180 / 3.1415926;
    }
};

class Particle {
    private:

    public:
        Vec2 position, velocity, acceleration;
        float mass;

        Particle()
            : position(Vec2()), velocity(Vec2()), acceleration(Vec2()), mass(0.0f) {}

        // There is no need to construct this with an acceleration because we are going to have a function that calculates this particles acceleration due
        // to the sum of the forces on this particle
        Particle(Vec2 pos, Vec2 vel, Vec2 acc, float mass) {
            position = pos;
            velocity = vel;
            acceleration = acc;
            mass = mass;
        }

        void StepThroughTime(float dt) {
            velocity = velocity + (acceleration * dt);
            position = position + (velocity * dt);
        }

        void print() {
            std::cout << "Position: (" << position.x << ", " << position.y << ") | Velocity: (" << velocity.x << ", " << velocity.y
                << ") | Acceleration: (" << acceleration.x << ", " << acceleration.y << ")\n";
        }
};


float calculate_range(float acceleration, float theta, float vo) {
    return std::sin(2.0f * theta) * vo * vo / acceleration;
}


int main() {

    auto position = Vec2(0.0f, 0.0f);
    auto velocity = Vec2(1.0f, 1.0f);
    auto acceleration = Vec2(0.0f, -9.81f);
    float mass = 5.0f; // kg

    Particle p1(position, velocity, acceleration, mass);

    float dt = 0.0001;
    float max_time = 5.0f;
    float time = 0;

    for (; time <= max_time; time+=dt) {
        p1.StepThroughTime(dt);

        std::cout << "Time: " << time << " | ";
        p1.print();
    }

    std::cout << "Time: " << time << " | ";
    p1.print();

    // Calculate the range of this projectile
    float range = calculate_range(acceleration.length(), velocity.angle(), velocity.length());

    std::cout << "-----------------------------------------------\n";
    std::cout << "The expected range is: " << range << std::endl;

    std::cout << "-----------------------------------------------\n";
    std::cout << "The actual range is: " << p1.position.x << std::endl;

    return 0;
}
