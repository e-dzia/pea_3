//
// Created by Edzia on 2018-01-01.
//

#ifndef PEA_3_PATH_H
#define PEA_3_PATH_H

#include "GraphMatrix.h"

class Path {
private:
    static GraphMatrix citiesDistances;
    static int numberOfCities;

    int *permutation;
    int length;

    int operator[](int i) const;
public:

    Path();
    virtual ~Path();

    bool isShorter(Path p);

    void swap(int left, int right);
    void insert(int left, int right);
    void invert(int left, int right);

    void countPath();

    void setPath(const int path[numberOfCities]);
    void setPath(Path p);
    void setRandom();
    static void setCitiesDistances(const GraphMatrix &citiesDistances);
    int getLength() const;

    Path& operator=(Path p);
    void print(std::ostream &str) const;

    friend std::ostream& operator<<(std::ostream& str, Path const& p){
        p.print(str);
        return str;
    }
};


#endif //PEA_3_PATH_H
