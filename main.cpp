#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Course.h"
#include "Period.h"
#include "functions.h"
#include <map>
#include <string>

using namespace std;

int main() {

    clock_t begin = clock();
    string line;
    ifstream infile("/home/fchacon/CLionProjects/IAA/bacp10.txt");

    // Getting first parameters (p, a, b, c, d)
    // p: number of periods
    // a: minimum academic load allowed per period
    // b: maximum academic load allowed per period
    // c: minimum amount of courses allowed per period
    // d: maximum amount of courses allowed per period

    vector<int> parameters;
    getline(infile, line);
    while (getline(infile, line)){
        istringstream iss(line);
        string s;
        iss >> s;
        if (s.length() == 0)
            break;
        if(s.length() > 3) {
            s = s.replace(s.end()-1, s.end(), "");
            s = s.substr(2, s.length());
            parameters.push_back(stoi(s));
        }
    }

    // Getting courses
    vector<Course> courses;
    getline(infile, line);
    while (getline(infile, line)){
        istringstream iss(line);
        string s;
        iss >> s;
        if (s.length() == 0)
            break;
        Course c (s);
        c.addPheromone(parameters[0]);
        courses.push_back(c);
    }

    // Getting credits
    int idCourse = 0;
    getline(infile, line);
    while (getline(infile, line)){
        istringstream iss(line);
        string s;
        iss >> s;
        // Assumption: There is no course with 10 credits or more
        if(s.length() == 1) {
            courses[idCourse].setCredits(stoi(s));
            break;
        }
        s = s.replace(s.end()-1, s.end(), "");
        courses[idCourse].setCredits(stoi(s));
        idCourse = idCourse + 1;
    }

    // Getting prerrequisites
    getline(infile, line);
    getline(infile, line);
    while (getline(infile, line)){
        istringstream iss(line);
        for(string s; iss >> s;) {
            if (s.length() < 4 )
                continue;
            for (unsigned int j = 0; j < courses.size(); ++j) {
                if(courses[j].getName().compare(s) == 0){
                    iss >> s;
                    iss >> s;
                    courses[j].prerreq.push_back(s);
                    for (unsigned int k = 0; k < courses.size(); ++k)
                        if (s.compare(courses[k].getName()) == 0)
                            courses[k].isPrerreqOf.push_back(courses[j].getName());
                    break;
                }
            }
        }
    }
    infile.close();

    map<string, int> decoder;
    for (int c = 0; c < courses.size(); ++c)
        decoder.insert(pair<string, int>(courses[c].getName(), c));

    // Initial Pheromones based on prerrequisites
    for (int i = 0; i < courses.size(); ++i)
        courses[i].addHeuristic(int(courses[i].prerreq.size()-
                                            courses[i].isPrerreqOf.size()));

    // Ants parameters
    int h = 15;
    float alpha = 1;
    float beta = 1;
    int iterations = 1000;
    int maxPheromone = 10;

    srand(3);
    vector<Period> bestSolution;

    for (int iteration = 0; iteration < iterations; ++iteration) {

        if (iteration % 100)
            evaporate(courses);
        if (iteration % 200)
            stagnate(courses);

        vector<vector<Period>> solutions;

        for (int ant = 0; ant < h; ++ant) {
            courses = restartProbs(courses, alpha, beta);
            vector<Course> currentAnt = courses;
            vector<Period> periods;

            for (int i = 0; i < parameters[0]; ++i) {
                Period p;
                periods.push_back(p);
            }

            for (int l = 0; l < courses.size(); ++l) {

                int currCredits = 0;
                int currCourses = 0;
                Course current = courses[l];
                current.updateProbs();

                double r;
                int index = 0;
                for (int i = 0; i < parameters[0]; ++i)
                    current.probs[i] = current.probs[i] / current.getTotalProbs();

                r = ((double) rand() / (RAND_MAX));
                for (int i = 0; i < parameters[0]; ++i) {
                    if (current.probs[i] < r) {
                        r = r - current.probs[i];
                    } else {
                        currCourses = periods[i].getNCourses() + 1;
                        currCredits = periods[i].getNCredits() + current.getCredits();
                        if (currCourses > parameters[4] || currCredits > parameters[2]) {
                            index = parameters[0];
                            break;
                        }
                        index = i;
                        break;
                    }
                }

                if (current.getTotalProbs() == 0 || index >= parameters[0]) {
                    //cout << l + 1 << ".- Curso " << current.getName() << " no fue asignado." << endl;
                    for (unsigned int i = 0; i < current.isPrerreqOf.size(); ++i)
                        for (int j = 0; j < courses.size(); ++j)
                            if (courses[j].getName().compare(current.isPrerreqOf[i]) == 0) {
                                for (int k = 0; k < parameters[0]; ++k)
                                    courses[j].probs[k] = 0;
                                break;
                            }
                } else {
                    //cout << l + 1 << ".- Curso " << current.getName() << " asignado al periodo " << index << endl;
                    periods[index].courses.push_back(l);
                    periods[index].addCourse();
                    periods[index].addCredit(current.getCredits());

                    // Evita que los cursos que necesitan del asignado se asignen antes o en el mismo periodo
                    for (unsigned int i = 0; i < current.isPrerreqOf.size(); ++i)
                        for (int j = 0; j < courses.size(); ++j)
                            if (courses[j].getName().compare(current.isPrerreqOf[i]) == 0) {
                                for (int k = 0; k <= index; ++k)
                                    courses[j].probs[k] = 0;
                                break;
                            }

                    // Evita que los prerrequisitos sean asignados durante la busqueda local
                    // en el mismo periodo o despues
                    for (unsigned int i = 0; i < current.prerreq.size(); ++i)
                        for (int j = 0; j < courses.size(); ++j)
                            if (courses[j].getName().compare(current.prerreq[i]) == 0) {
                                for (int k = index; k < parameters[0]; ++k)
                                    courses[j].probs[k] = 0;
                                break;
                            }
                }

            }

            solutions.push_back(periods);
        }

        int best = int(courses.size());
        int bestAnt = -1;
        int worst = 0;
        int worstAnt = -1;
        int score;

        for (int ant = 0; ant < h; ++ant) {
            score = int(courses.size()) - evaluate(solutions[ant]);
            if (best > score) {
                best = score;
                bestAnt = ant;
            }

            if (worst <= score) {
                worst = score;
                worstAnt = ant;
            }
        }

        // Local Search
        for (int m = 0; m < 1; ++m) {
            int upper = 0;
            int lower = 0;
            int c;
            for (int i = 0; i < parameters[0]; ++i) {
                c = solutions[bestAnt][i].getNCredits();
                if (solutions[bestAnt][upper].getNCredits() <= c)
                    upper = i;
                if (solutions[bestAnt][lower].getNCredits() >= c)
                    lower = i;
            }

            int courseToSwap;
            vector<Period> swapping = solutions[bestAnt];
            for (int i = 0; i < swapping[upper].courses.size(); ++i) {
                courseToSwap = swapping[upper].courses[i];
                if (courses[courseToSwap].probs[lower] != 0) {
                    solutions[bestAnt][lower].courses.push_back(courseToSwap);
                    solutions[bestAnt][lower].addCourse();
                    solutions[bestAnt][lower].addCredit(courses[courseToSwap].getCredits());
                    solutions[bestAnt][upper].reduceCredit(courses[courseToSwap].getCredits());
                    solutions[bestAnt][upper].reduceCourse();
                    solutions[bestAnt][upper].courses.erase(solutions[bestAnt][upper].courses.begin() + i);
                    break;
                }
            }
        }

        // Increase pheromones
        for (int i = 0; i < parameters[0]; ++i) {
            vector<int> aux = solutions[bestAnt][i].courses;
            for (int j = 0; j < aux.size(); ++j) {
                if (courses[aux[j]].pheromones[i] < maxPheromone)
                    courses[aux[j]].pheromones[i] += 1;
            }
        }

        // Decrease pheromones
        for (int i = 0; i < parameters[0]; ++i) {
            vector<int> aux = solutions[worstAnt][i].courses;
            for (int j = 0; j < aux.size(); ++j) {
                if(courses[aux[j]].pheromones[i] > 0)
                    courses[aux[j]].pheromones[i] -= 1;
            }
        }

        // Detecting stagnation
        /*
        if(iteration != 0) {
            int isStagnated = true;
            for (int i = 0; i < parameters[0]; ++i) {
                if(solutions[bestAnt][i].courses.size() == solutions[worstAnt][i].courses.size()) {
                    for (int j = 0; j < solutions[bestAnt][i].courses.size(); ++j) {
                        if (solutions[worstAnt][i].courses[j] == solutions[bestAnt][i].courses[j])
                            continue;
                        else
                            isStagnated = false;
                    }
                } else{
                    isStagnated = false;
                    break;
                }
            }

            if (isStagnated) {
                cout << "they are equals in iteration " << iteration << endl;

                cout << "Period " <<  1 << " (" << solutions[worstAnt][0].getNCredits() << "): ";
                for (int j = 0; j < solutions[worstAnt][0].courses.size(); ++j) {
                    cout << courses[solutions[worstAnt][0].courses[j]].getName() << "-";
                }
                cout << endl;

                cout << "Period " <<  1 << " (" << solutions[bestAnt][0].getNCredits() << "): ";
                for (int j = 0; j < solutions[bestAnt][0].courses.size(); ++j) {
                    cout << courses[solutions[bestAnt][0].courses[j]].getName() << "-";
                }
                cout << endl;

            }
        }*/

        if(evaluate(bestSolution) <= evaluate(solutions[bestAnt])) {
            bestSolution = solutions[bestAnt];
        }
    }

    // Simulated Annealing
    iterations = 10000;
    float temp = 100;

    vector<float> bestStatistics = getStatistics(bestSolution);
    vector<float> newStatistics;
    vector<Period> newSolution = bestSolution;
    vector<Period> auxSolution = newSolution;

    // Updating availabilities
    courses = prepForSA(courses);
    for (int p = 0; p < parameters[0]; ++p) {
        for (int c = 0; c < newSolution[p].courses.size(); ++c) {
            int current = newSolution[p].courses[c];

            for (unsigned int l = 0; l < courses[current].isPrerreqOf.size(); ++l) {
                int aux = decoder[courses[current].isPrerreqOf[l]];
                for (int m = 0; m <= p; ++m)
                    courses[aux].probs[m] = 0;
            }

            for (unsigned int l = 0; l < courses[current].prerreq.size(); ++l) {
                int aux = decoder[courses[current].prerreq[l]];
                for (int m = p; m < parameters[0]; ++m)
                    courses[aux].probs[m] = 0;
            }
        }
    }

    for (int iter = 0; iter < iterations; ++iter) {
        int first = rand() % parameters[0];
        int second = rand() % parameters[0];
        while (first == second)
            second = rand() % parameters[0];

        for (int k = 0; k < newSolution[first].courses.size(); ++k) {
            int courseToSwap = newSolution[first].courses[k];
            if (courses[courseToSwap].probs[second] != 0) {
                newSolution[second].courses.push_back(courseToSwap);
                newSolution[second].addCourse();
                newSolution[second].addCredit(courses[courseToSwap].getCredits());
                newSolution[first].reduceCredit(courses[courseToSwap].getCredits());
                newSolution[first].reduceCourse();
                newSolution[first].courses.erase(newSolution[first].courses.begin() + k);

                // Updating availabilities
                for (int p = 0; p < parameters[0]; ++p) {
                    for (int c = 0; c < newSolution[p].courses.size(); ++c) {
                        int current = newSolution[p].courses[c];

                        for (unsigned int l = 0; l < courses[current].isPrerreqOf.size(); ++l) {
                            int aux = decoder[courses[current].isPrerreqOf[l]];
                            for (int m = 0; m <= p; ++m)
                                courses[aux].probs[m] = 0;
                        }

                        for (unsigned int l = 0; l < courses[current].prerreq.size(); ++l) {
                            int aux = decoder[courses[current].prerreq[l]];
                            for (int m = p; m < parameters[0]; ++m)
                                courses[aux].probs[m] = 0;
                        }
                    }
                }
                if (temp > 1)
                    temp -= 1;
                break;
            }
        }

        newStatistics = getStatistics(newSolution);
        double r = ((double) rand() / (RAND_MAX));
        float dif = newStatistics[1] - bestStatistics[1];
        if(newStatistics[1] < bestStatistics[1]){
            bestSolution = newSolution;
            bestStatistics = newStatistics;
        }else if(exp(dif/temp) > r){
            newSolution = auxSolution;
        }

        auxSolution = newSolution;

    }


    cout << evaluate(bestSolution) << endl;
    for (int i = 0; i < parameters[0]; ++i) {
        cout << "Period " << i + 1 << " (" << bestSolution[i].getNCredits() << "): ";
        for (int j = 0; j < bestSolution[i].courses.size(); ++j) {
            cout << courses[bestSolution[i].courses[j]].getName() << "-";
        }
        cout << endl;
    }

    bestStatistics = getStatistics(bestSolution);
    cout << "final deviation: " << bestStatistics[1] << endl;

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << time_spent << " segundos" << endl;


    return 0;
}
