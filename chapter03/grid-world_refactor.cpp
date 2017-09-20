//
// Created by George Gregory on 9/14/17.
//

#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <chrono>
#include <array>
#include <vector>

const int WORLD_SIZE= 5;

using namespace std;
using namespace std::chrono;
using State= pair<int, int>;
using StateAction= pair<pair<int, int>, char>;
using Action= char;
using Reward= double;
using Prob= double;
using Value= double;
using World= array<array<double, WORLD_SIZE>, WORLD_SIZE>;
using ValFunc= map<State, Value>;

pair<int, int> A_POS       {0, 1};
pair<int, int> A_PRIME_POS {4, 1};
pair<int, int> B_POS       {0, 3};
pair<int, int> B_PRIME_POS {2, 3};
double discount= 0.9;

ValFunc valFunc;

// Left, Up, Right, Down
const set<Action> actionSet {'L', 'U', 'R', 'D'};

map<Action, Prob> prob {{'L', 0.25}, {'U', 0.25}, {'R', 0.25}, {'D', 0.25}};

pair<map<char, pair<int, int>>, map<char, double>> setNextAndRewardMaps(State);
void printValFunc(ValFunc);

void printValFunc(ValFunc valFunc){
    for (int i= 0; i < WORLD_SIZE; ++i){
        for (int j = 0; j < WORLD_SIZE; ++j){
            State s= make_pair(i, j);
            cout << valFunc[s] << " ";
        }
        cout << endl;
    }
}

set<State> stateSpace {};

void setStateSpace(){
    for (int i= 0; i < WORLD_SIZE; ++i) {
        for (int j = 0; j < WORLD_SIZE; ++j) {
            State s {i, j};
            stateSpace.insert(s);
        }
    }
}


void initValueFunctionZero(ValFunc valFunc){
    for (State s : stateSpace) {
        valFunc[s]= 0.0;
    }
}

map<StateAction, Prob> actionProb;

void setActionProb(){
    for (State s : stateSpace)
        for (Action a : actionSet)
            actionProb[make_pair(s, a)]= prob[a];
}

map<StateAction, State> nextState;
map<StateAction, Reward> actionReward;
void setNextStateActionReward(){
    for (State s : stateSpace){
        for(Action a : actionSet) {
            nextState[make_pair(s, a)] = setNextAndRewardMaps(s).first[a];
            actionReward[make_pair(s, a)] = setNextAndRewardMaps(s).second[a];
        }
    }
}

pair<map<char, pair<int, int>>, map<char, double>> setNextAndRewardMaps(State location){
    int i= location.first;
    int j= location.second;
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

    return {next, reward};

};

double absValue(ValFunc vf1, ValFunc vf2){
    double sum= 0.0;
    for (State s : stateSpace)
        sum += abs(vf1[s] - vf2[s]);

    return sum;
}

void computeRandomPolicy(){
    double diff;
    State newState;

    do {
        ValFunc newValFunc;
        initValueFunctionZero(newValFunc);


        for (State s : stateSpace){
            for (Action a : actionSet){
                newState= nextState[make_pair(s, a)];
                //bellman equation
                newValFunc[s] = newValFunc[s] + actionProb[make_pair(s, a)] *
                                                  (actionReward[make_pair(s, a)] + discount * valFunc[newState]);
            }
        }
        diff= absValue(valFunc, newValFunc);
        valFunc = newValFunc;

    } while (diff > 0.00000001);
    cout << "Random Policy (refactored):" << endl;
    printValFunc(valFunc);

}

void computeOptimalPolicy(){
    initValueFunctionZero(valFunc);
    double diff;
    State newState;

    do {
        ValFunc newValFunc;
        initValueFunctionZero(newValFunc);


        for (State s : stateSpace){
            vector<double> values;
            for (Action a : actionSet){
                newState= nextState[make_pair(s, a)];
                //bellman equation
                values.push_back(actionReward[make_pair(s, a)] + discount * valFunc[newState]);
                newValFunc[s]= *max_element(values.cbegin(), values.cend());
            }
        }
        diff= absValue(valFunc, newValFunc);
        valFunc = newValFunc;

    } while (diff > 0.00000001);
    cout << "Optimal Policy (refactored):" << endl;
    printValFunc(valFunc);

}


int main() {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    setStateSpace();
    setActionProb();
    setNextStateActionReward();
    initValueFunctionZero(valFunc);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    computeRandomPolicy();

    cout << "" << endl;
    high_resolution_clock::time_point t3 = high_resolution_clock::now();


    computeOptimalPolicy();

    cout << "" << endl;
    high_resolution_clock::time_point t4 = high_resolution_clock::now();
    auto dur1 = duration_cast<microseconds>( t2 - t1 ).count();
    auto dur2 = duration_cast<microseconds>( t3 - t2 ).count();
    auto dur3 = duration_cast<microseconds>( t4 - t3 ).count();
    cout << "setup: " << dur1 << "  random: " << dur2 << "  optimal: " << dur3 << "   (microseconds)" << endl;
}

