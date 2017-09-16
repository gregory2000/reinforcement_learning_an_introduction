//
// Created by George Gregory on 9/14/17.
//

#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <chrono>
#include <array>

using namespace std;
using namespace std::chrono;

const int WORLD_SIZE= 5;
pair<int, int> A_POS       {0, 1};
pair<int, int> A_PRIME_POS {4, 1};
pair<int, int> B_POS       {0, 3};
pair<int, int> B_PRIME_POS {2, 3};
double discount= 0.9;

array<array<double, WORLD_SIZE>, WORLD_SIZE> world {0.0};


// Left, Up, Right, Down
const set<char> actionSet {'L', 'U', 'R', 'D'};
const int NUM_ACTIONS= 4;

map<char, double> prob {{'L', 0.25}, {'U', 0.25}, {'R', 0.25}, {'D', 0.25}};

pair<map<char, pair<int, int>>, map<char, double>> setNextAndRewardMaps(int, int);
void copyWorld(double target[WORLD_SIZE][WORLD_SIZE], double source[WORLD_SIZE][WORLD_SIZE]);
void printW(array<array<double, WORLD_SIZE>, WORLD_SIZE> w);

map<char, double> actionProb[WORLD_SIZE][WORLD_SIZE];

void setActionProb(){
    for (int i= 0; i < WORLD_SIZE; ++i)
        for (int j = 0; j < WORLD_SIZE; ++j)
            actionProb[i][j]= prob;
}

map<char, pair<int, int>> nextState[WORLD_SIZE][WORLD_SIZE];
map<char, double> actionReward[WORLD_SIZE][WORLD_SIZE];

void setNextStateActionReward(){
    for (int i= 0; i < WORLD_SIZE; ++i){
        for (int j = 0; j < WORLD_SIZE; ++j){
            nextState[i][j] = setNextAndRewardMaps(i, j).first;
            actionReward[i][j] = setNextAndRewardMaps(i, j).second;
        }
    }
}

pair<map<char, pair<int, int>>, map<char, double>> setNextAndRewardMaps(int i, int j){
    pair<int, int> location= {i, j};
    map<char, pair<int, int>> next;
    map<char, double> reward;

    if (i == 0) {
        next['U']= {i, j};
        reward['U']= -1.0;
    }
    else {
        next['U']= {i - 1, j};
        reward['U']= 0.0;
    }

    if (i == WORLD_SIZE - 1) {
        next['D']= {i, j};
        reward['D']= -1.0;
    }
    else {
        next['D']= {i + 1, j};
        reward['D']= 0.0;
    }

    if (j == 0) {
        next['L']= {i, j};
        reward['L']= -1.0;
    }
    else {
        next['L']= {i, j - 1};
        reward['L']= 0.0;
    }

    if (j == WORLD_SIZE - 1) {
        next['R']= {i, j};
        reward['R']= -1.0;
    }
    else {
        next['R']= {i, j + 1};
        reward['R']= 0.0;
    }

    if (location == A_POS) {
        next['L']= next['R']= next['D']= next['U']= A_PRIME_POS;
        reward['L']= reward['R']= reward['D']= reward['U']= 10.0;
    }

    if (location == B_POS) {
        next['L']= next['R']= next['D']= next['U']= B_PRIME_POS;
        reward['L']= reward['R']= reward['D']= reward['U']= 5.0;
    }
    pair<map<char, pair<int, int>>, map<char, double>> result= {next, reward};
    return result;

};

double absWorld(array<array<double, WORLD_SIZE>, WORLD_SIZE> w1, array<array<double, WORLD_SIZE>, WORLD_SIZE> w2){
    double sum= 0.0;
    for (int i= 0; i < WORLD_SIZE; ++i){
        for (int j = 0; j < WORLD_SIZE; ++j){
            sum += abs(w1[i][j] - w2[i][j]);
        }
    }
    return sum;
}


void computeRandomPolicy(){
    double diff;
    pair<int, int> newPosition;

    do {
        array<array<double, WORLD_SIZE>, WORLD_SIZE> newWorld {0.0};
        for (int i= 0; i < WORLD_SIZE; ++i){
            for (int j = 0; j < WORLD_SIZE; ++j){
                for (char action : actionSet){
                    newPosition= nextState[i][j].at(action);
                    //bellman equation
                    newWorld[i][j] += actionProb[i][j][action] * (actionReward[i][j][action] + discount * world[newPosition.first][newPosition.second]);
                }
            }
        }
        diff= absWorld(world, newWorld);
        //cout << diff << endl;
        world = newWorld;

    } while (diff > 0.00000001);
    cout << "Random Policy (refactored):" << endl;
    printW(world);

}


void printW(array<array<double, WORLD_SIZE>, WORLD_SIZE> w){
    for (int i= 0; i < WORLD_SIZE; ++i){
        for (int j = 0; j < WORLD_SIZE; ++j){
            cout << w[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    setActionProb();
    setNextStateActionReward();
    computeRandomPolicy();



    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << duration;
}

