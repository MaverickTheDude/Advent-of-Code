#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

using std::string;
using std::cout;
using std::endl;

const int Nparticles = 1000;
const int Nruns = 5000;

class Vector3d {
public:
friend class particle;
    Vector3d(const string& _vec_) {
        std::smatch wynik;
        std::regex wzorzec("<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>");
        std::regex_search(_vec_, wynik, wzorzec);
        x = std::stoi(wynik[1]);
        y = std::stoi(wynik[2]);
        z = std::stoi(wynik[3]);
    }

    void operator +=(const Vector3d _vec_) {
        x += _vec_.x;
        y += _vec_.y;
        z += _vec_.z;
    }

    int magnitude() const {
        return abs(x) + abs(y) + abs(z);
    }

private:
    int x;
    int y;
    int z;
};

class particle {
public:
    particle(const std::smatch& _input_) : position(_input_[1]), velocity(_input_[2]), acceleration(_input_[3]) { }
    void print() const {
        std::cout << "position = [" << position.x << ", " << position.y << ", " << position.z << "] \n";
    }

    int update() {
        velocity += acceleration;
        position += velocity;
        return position.magnitude();
    }

    int pos_mag() const { return position.magnitude(); }

protected:
    Vector3d position;
    Vector3d velocity;
    Vector3d acceleration;
};

int main() {
    string line;
    std::ifstream myfile("input.txt");
    std::smatch wynik;
    std::regex wzorzec("(<-?[0-9]+,-?[0-9]+,-?[0-9]+>), v=(<-?[0-9]+,-?[0-9]+,-?[0-9]+>), a=(<-?[0-9]+,-?[0-9]+,-?[0-9]+>)");
    std::vector<particle> frame;
    frame.reserve(Nparticles);

    while( getline(myfile, line) ) {
std::regex_search(line, wynik, wzorzec);
frame.emplace_back(wynik);
}

    /* simulate the system for Nruns iterations */
    for (int i = 0; i < Nruns; i++) {
        for (particle& p : frame)
            p.update();
    }

    unsigned int min = frame[0].pos_mag();
    int minId = 0;
    for (int i = 1; i < Nparticles; i++) {
        const unsigned int test_distance = frame[i].pos_mag();
        if (test_distance < min) {
            minId = i;
            min = test_distance;
        }
    }
    cout << "min = " << min << "; min id = " << minId << endl;

    std::cout << "done!" << endl;
    return 0;
}