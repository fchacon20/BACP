//
// Created by fchacon on 22-07-18.
//

#include "Period.h"

int Period::getNCourses(){
    return this->nCourses;
}

int Period::getNCredits(){
    return this->nCredits;
}

void Period::addCourse(){
    this->nCourses = this->nCourses + 1;
}

void Period::reduceCredit(int c){
    this->nCredits = this->nCredits - c;
}

void Period::addCredit(int c){
    this->nCredits = this->nCredits + c;
}

void Period::reduceCourse() {
    this->nCourses = this->nCourses - 1;
}
