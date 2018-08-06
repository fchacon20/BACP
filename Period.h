//
// Created by fchacon on 22-07-18.
//

#ifndef IAA_PERIOD_H
#define IAA_PERIOD_H

#include <string>
#include <vector>

class Period {
    int nCourses = 0;
    int nCredits = 0;

public:
    int getNCourses();
    int getNCredits();
    void addCourse();
    void reduceCredit(int);
    void reduceCourse();
    void addCredit(int);
    std::vector<int> courses;
};


#endif //IAA_PERIOD_H
