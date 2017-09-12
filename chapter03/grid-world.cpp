//
// Created by George Gregory on 9/12/17.
//

#include <iostream>
#include <map>

using namespace std;

const int WORLD_SIZE= 5;
pair<int, int> A_POS       {0, 1};
pair<int, int> A_PRIME_POS {4, 1};
pair<int, int> B_POS       {0, 3};
pair<int, int> B_PRIME_POS {2, 3};
double discount= 0.9;

double world[WORLD_SIZE][WORLD_SIZE] {0.0};

// Left, Up, Right, Down
enum actions {L, U, R, D};

map<int, double> prob {{L, 0.25}, {U, 0.25}, {R, 0.25}, {D, 0.25}};

int main() {
    cout << prob.at(U) << endl;

}
