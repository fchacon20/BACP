//
// Created by fchacon on 22-07-18.
//

#ifndef IAA_COUESE_H
#define IAA_COUESE_H

#include <string>
#include <vector>
#include <math.h>

class Course {
    int credits;
    std::string name;
    double totalProbs;

public:
    std::vector<int> pheromones;
    std::vector<int> heuristic;
    std::vector<double> probs;
    std::vector<std::string> prerreq;
    std::vector<std::string> isPrerreqOf;
    Course(std::string);
    std::string getName();
    void updateProbs();
    float getTotalProbs();
    int getCredits();
    void setCredits(int);
    void addPheromone(int);
    void addHeuristic(int);
    void restartProbs(float, float);
};


#endif //IAA_COUESE_H
