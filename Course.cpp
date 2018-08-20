//
// Created by fchacon on 22-07-18.
//

#include "Course.h"

Course::Course(std::string n) {
    this->name = n;
}

void Course::restartProbs(float alpha, float beta) {
    for (int i = 0; i < this->probs.size(); ++i) {
        this->probs[i] = pow(this->pheromones[i], alpha) * pow(this->heuristic[i], beta);
    }
}

std::string Course::getName() {
    return this->name;
}

float Course::getTotalProbs(){
    return float(this->totalProbs);
}

int Course::getCredits() {
    return this->credits;
}

void Course::setCredits(int c) {
    this->credits = c;
}

void Course::updateProbs(){
    double sum = 0;
    for (int i = 0; i < this->probs.size(); ++i) {
        sum = sum + this->probs[i];
    }
    this->totalProbs = sum;
}

void Course::addPheromone(int n){
    for (int i = 0; i < n; ++i) {
        this->pheromones.push_back(1);
        this->probs.push_back(1);
        this->heuristic.push_back(1);
    }
}

void Course::addHeuristic(int type) {
    int add = 3;
    int n = int(this->heuristic.size());
    if (type <= 0){
        for (int i = 0; i < n; ++i) {
            this->heuristic[i] = add;
            add -= 1;
            if (add == 1)
                break;
        }
    } else if (type == 1){
        for (int i = (n/2)-2; i < (n/2)+1; ++i) {
            add -= 1;
            this->heuristic[i] = add;
            add += 2;
            if (i == n/2)
                add -= 2;
        }
    } else{
        for (int i = n-1; i > 0; --i) {
            this->heuristic[i] = add;
            add -= 1;
            if (add == 1)
                break;
        }
    }

}

void Course::evaporate() {
    for (int i = 0; i < this->pheromones.size(); ++i) {
        if (this->pheromones[i] > 2)
            this->pheromones[i] -= 2;
        else
            this->pheromones[i] = 1;
    }
}

void Course::stagnate() {
    for (int i = 0; i < this->pheromones.size(); ++i)
        this->pheromones[i] = 1;
}


