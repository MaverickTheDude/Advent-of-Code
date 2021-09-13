#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <regex>

using std::string;
using std::cout;
using std::endl;

const int Nparticles = 1000;
const int Nruns = 500;

class Vector3d {
public:
friend bool operator<(const Vector3d&, const Vector3d&);
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
    Vector3d pos() const {return position; }

private:
    Vector3d position;
    Vector3d velocity;
    Vector3d acceleration;
};

bool operator<(const Vector3d& v1, const Vector3d& v2) {
    if ( v1.magnitude() == v2.magnitude() ) {
        if (v1.x == v2.x) {
            return v1.y < v2.y ? true : false;
        }
        return v1.x < v2.x ? true : false;
    }
    return v1.magnitude() < v2.magnitude() ? true : false;
}

int main() {
    string line;
    std::ifstream myfile("input.txt");
    std::smatch wynik;
    std::regex wzorzec("(<-?[0-9]+,-?[0-9]+,-?[0-9]+>), v=(<-?[0-9]+,-?[0-9]+,-?[0-9]+>), a=(<-?[0-9]+,-?[0-9]+,-?[0-9]+>)");
    std::list<particle> frame;

    while( getline(myfile, line) ) {
        std::regex_search(line, wynik, wzorzec);
        frame.emplace_back(wynik);
    }

    using list_iter = std::list<particle>::iterator;
    using map_iter = std::map<Vector3d, std::vector<list_iter> >::iterator;

    for (int i = 0; i < Nruns; i++) {
        /* map<position, location_in_list> */
        std::map<Vector3d, std::vector<list_iter> > hitset;

        /* create map of matching frames */
        for (list_iter it = frame.begin(); it != frame.end(); it++)
            hitset[it->pos()].push_back(it);    // if the vector for a key doesn't exist, it will be created automatically

        /* remove matching frames */
        for (map_iter it = hitset.begin(); it != hitset.end(); it++) {
            const std::vector<list_iter>& indices = it->second; // iterators showing potentially doubled elements in frame-list
            if (indices.size() > 1) {
                for (const list_iter& idx : indices)
                    frame.erase(idx);
            }
        }

        /* update left-out frames */
        for (particle& p : frame)
            p.update();
        std::cout << "particles left after " << i+1 << " cycles: " << frame.size() << std::endl;
    }



    std::cout << "done!" << endl;
    return 0;
}