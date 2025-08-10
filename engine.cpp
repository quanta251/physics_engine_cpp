#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

struct Vec2 {
    float x, y;

    // Constructors
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float xVal, float yVal) : x(xVal), y(yVal) {}

    // Overloads
    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator/(float scalar) const { return operator*(1/scalar); }

    Vec2 operator+=(const Vec2& other) { return Vec2( x += other.x, y += other.y); }

    // Methods
    float length() {
        float length = x*x + y*y;
        return std::sqrt(length);
    }

    Vec2 unitVec() { return *this/length(); }
    float angle() { return std::atan2(y, x); }
    float angleDeg() {
        float angle = this->angle();
        return angle * 180 / 3.1415926;
    }
};

class Particle {
    private:
        int m_id;

    public:
        Vec2 position, velocity, forces;
        float mass;

        // Constructors
        Particle() : position(Vec2()), velocity(Vec2()), mass(0.0f), m_id(rand()) {}
        Particle(Vec2 pos, Vec2 vel, float m, int id) : position(pos), velocity(vel), mass(m), forces(0,0), m_id(id) {}

        void addForce(const Vec2& force) {
            forces += force; // Sum Forces
        }

        void integrate(float dt) {
            Vec2 acceleration = forces / mass;
            velocity += acceleration * dt;
            position += velocity * dt;
            clearForces();
        }

        void clearForces() {
            forces = Vec2{};
        }

        void print() {
            std::cout << "Particle: " << m_id << " | Position: (" << position.x << ", " << position.y << ") | Velocity: (" << velocity.x << ", " << velocity.y << ")\n";
        }
};

class PhysicsEngine {
    public:
        // This will be a vector containing the bodies in the simulation
        std::vector<Particle> particles;

        // Universal Forces/Accelerations here
        Vec2 gravity = {0.0f, -9.81f}; // m/s^2

        // Add particles to the simulation
        void addParticle(const Particle& p) {
            particles.push_back(p);
        }

        // Step through time
        void step(float dt) {
            // 1. Add forces for each particle
            for (auto& p : particles) {
                p.addForce(gravity * p.mass);
            }

            // 2. Integrate all particles
            for (auto& p : particles) {
                p.integrate(dt);
            }

            for (auto& p : particles) {
                p.print();
            }
            std::cout << std::endl;

            accumulateForces();
            integrateParticles(dt);
            detectCollisions();
            resolveCollisions();
            clearForces();
        }

    private:
        void accumulateForces() {}
        void integrateParticles(float dt) {}
        void detectCollisions() {}
        void resolveCollisions() {}
        void clearForces() {}
};

int main() {
    PhysicsEngine engine;
    engine.addParticle(Particle({0,0}, {5,5}, 2.0f, 1));
    engine.addParticle(Particle({0,0}, {0,10}, 1.0f, 2) );

    float dt = 1.0f/60.0f;
    for (int i = 0; i < 300; i++) {
        std::cout << "Time: " << i * dt << std::endl;
        engine.step(dt);
    }
}
