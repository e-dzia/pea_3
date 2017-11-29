//
// Created by Edzia on 2017-04-29.
//
#include <iostream>
//#include "Array2.h"

#ifndef SDIZO_2_GRAPH_H
#define SDIZO_2_GRAPH_H


class Graph {
protected:
    int vertexes;//liczba wierzcholkow
    int edges; //liczba krawedzi
    int density; //gestosc

    virtual void countDensity();
    virtual void countEdges() = 0;

public:
    virtual ~Graph();

    int getDensity();
    int getNumberOfVertexes() const;
    int getNumberOfEdges() const;


    virtual void createRandom(int vertexes, int density) = 0;
    virtual void loadFromFile(std::string filename) = 0;
    virtual void print(std::ostream& str)const = 0;
    virtual void makeBothWaysEqual() = 0;
    virtual void setEdge(int start, int end, int length) = 0;
    virtual int getEdgeLength(int start, int end) = 0;

    friend std::ostream& operator<<(std::ostream& str, Graph const& g){
        g.print(str);
        return str;
    }
};


#endif //SDIZO_2_GRAPH_H
