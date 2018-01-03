//
// Created by Edzia on 2018-01-01.
//

#ifndef PEA_3_PATH_H
#define PEA_3_PATH_H

#include "GraphMatrix.h"

class Path {
private:
    int *permutation;
    int length = INT32_MAX;

    int operator[](int i) const;
public:
    Path();
    Path(const Path& p);
    virtual ~Path();

    static GraphMatrix citiesDistances;
    static int numberOfCities;

    bool isShorter(const Path& p)const;
    bool operator<(const Path& p) const;
    bool operator<=(const Path& p) const;
    bool operator>(const Path& p) const;
    bool operator>=(const Path& p) const;
    bool operator==(const Path& p) const;
    bool operator!=(const Path& p) const;

    void swap(int left, int right);
    void insert(int left, int right);
    void invert(int left, int right);

    Path crossoverPMXfirstChild(const Path &p, int left, int right) const;
    Path crossoverPMXsecondChild(const Path &p, int left, int right) const;

    Path crossoverOXfirstChild(const Path &p, int left, int right) const;
    Path crossoverOXsecondChild(const Path &p, int left, int right) const;

    void countPath();

    void setPath(const int *path);
    void setPath(const Path& p);
    void setRandom();
    void setGreedy();
    static void setCitiesDistances(GraphMatrix& citiesDistances);
    int getLength() const;

    Path& operator=(Path const& p);
    void print(std::ostream &str) const;

    friend std::ostream& operator<<(std::ostream& str, Path const& p){
        p.print(str);
        return str;
    }

    void initialize();
};


#endif //PEA_3_PATH_H
