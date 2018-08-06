//
// Created by fchacon on 22-07-18.
//

#ifndef IAA_FUNCTIONS_H
#define IAA_FUNCTIONS_H

#include <vector>
#include "Period.h"
#include "Course.h"

int evaluate(std::vector<Period> solution);
std::vector<Course> restartProbs(std::vector<Course> courses, float alpha, float beta);
std::vector<float> getStatistics(std::vector<Period> solution);

#endif //IAA_FUNCTIONS_H
