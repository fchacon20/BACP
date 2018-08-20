//
// Created by fchacon on 22-07-18.
//

#include <vector>
#include "Period.h"
#include "Course.h"

int evaluate(std::vector<Period> solution){
    int sum = 0;
    for (int i = 0; i < solution.size(); ++i) {
        sum = sum + solution[i].getNCourses();
    }
    return sum;
}

std::vector<Course> restartProbs(std::vector<Course> courses, float alpha, float beta){
    for (int i = 0; i < courses.size(); ++i) {
        courses[i].restartProbs(alpha, beta);
    }
    return courses;
}

std::vector<Course> prepForSA(std::vector<Course> courses){
    for (int i = 0; i < courses.size(); ++i) {
        for (int j = 0; j < courses[i].probs.size(); ++j) {
            courses[i].probs[j] = 1;
        }
    }
    return courses;
}

std::vector<float> getStatistics(std::vector<Period> solution) {
    float sum = 0.0, mean, standardDeviation = 0.0;

    for (int i = 0; i < solution.size(); ++i)
        sum += solution[i].getNCredits();

    mean = sum/solution.size();

    for (int i = 0; i < solution.size(); ++i)
        standardDeviation += pow(solution[i].getNCredits() - mean, 2);

    std::vector<float> ret;
    ret.push_back(mean);
    ret.push_back(sqrt(standardDeviation/solution.size()));

    return ret;
}

std::vector<Course> evaporate(std::vector<Course> courses) {
    for (int i = 0; i < courses.size(); ++i)
        courses[i].evaporate();

    return courses;
}

std::vector<Course> stagnate(std::vector<Course> courses) {
    for (int i = 0; i < courses.size(); ++i)
        courses[i].stagnate();

    return courses;
}
