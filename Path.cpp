//
// Created by Edzia on 2018-01-01.
//

#include "Path.h"

int Path::numberOfCities = 0;
GraphMatrix Path::citiesDistances;

void Path::swap(int left, int right) {
    if (right == left) return;
    int tmp = permutation[left];
    permutation[left] = permutation[right];
    permutation[right] = tmp;
}

void Path::insert(int left, int right) {
    if (right == left) return;
    if (right < left){
        int tmp = permutation[left];
        for (int i = left; i > right; i--){
            permutation[i] = permutation[i-1];
        }
        permutation[right] = tmp;
    }
    else {
        int tmp = permutation[left];
        for (int i = left; i < right; i++){
            permutation[i] = permutation[i+1];
        }
        permutation[right] = tmp;
    }
}

void Path::invert(int left, int right) {
    if (right == left) return;
    if (right < left){int tmp = left; left = right; right = tmp;}
    for (int i = 0; i < (right - left +1)/2; i++){
        swap(left+i,right-i);
    }
}

void Path::countPath() {
    length = 0;
    int end;
    for (int i = 1; i < numberOfCities; i++){
        length += citiesDistances.getEdgeLength(permutation[i-1],permutation[i]);
        end = i;
    }
    length += citiesDistances.getEdgeLength(permutation[end],permutation[0]);
}

void Path::setPath(const int *path) {
    for (int i = 0; i < numberOfCities; i++){
        permutation[i] = path[i];
    }
    countPath();
}

Path::~Path() {
    delete[] permutation;
    permutation = nullptr;
}

void Path::setPath(const Path& p) {
    for (int i = 0; i < numberOfCities; i++){
        permutation[i] = p[i];
    }
    countPath();
}

int Path::operator[](int i) const {
    return permutation[i];
}

Path &Path::operator=(Path const& p) {
    if (permutation == nullptr) initialize();
    this->setPath(p);
    return *this;
}

void Path::setRandom() {
    if (permutation == nullptr) initialize();
    bool* visited = new bool[numberOfCities];
    for (int i = 0; i < numberOfCities; i++){
        visited[i] = false;
    }
    for (int i = 0; i < numberOfCities; i++){
        do{
            permutation[i] = rand()%numberOfCities;
        }while(visited[permutation[i]]);
        visited[permutation[i]] = true;
    }
    countPath();
    delete[] visited;
}

void Path::setGreedy() {
    if (permutation == nullptr) initialize();
    bool *visited = new bool[numberOfCities];

    for (int i = 0; i < numberOfCities; i++){
        visited[i] = false;
    }
    int k = rand()%numberOfCities;
    permutation[0] = k;

    for(int i = 1; i < numberOfCities; i++) { //algorytm zachlanny szuka ustawienia poczatkowego
        visited[k] = true;
        int min = INT32_MAX;
        int position = -1;
        for (int j = 0; j < numberOfCities; j++) { //szukanie minimum od obecnej pozycji
            if (!visited[j] && citiesDistances.getEdgeLength(k, j) != -1 && citiesDistances.getEdgeLength(k, j) < min) {
                min = citiesDistances.getEdgeLength(k, j);
                position = j;
            }
        }
        k = position;
        permutation[i] = position;
    }

    countPath();
    delete[] visited;
}

bool Path::isShorter(const Path& p)const {
    return this->length < p.getLength();
}

bool Path::operator<(const Path& p) const{
    return this->isShorter(p);
}

int Path::getLength() const {
    return length;
}

void Path::setCitiesDistances(GraphMatrix& citiesDistances) {
    Path::citiesDistances = citiesDistances;
    Path::numberOfCities = citiesDistances.getNumberOfVertexes();
}

void Path::print(std::ostream &str) const {
    for (int i = 0; i < numberOfCities; i++){
        str << permutation[i] << " ";
    }
    str << "\n";
}

Path::Path(){
    initialize();
}

Path::Path(const Path &p) {
    initialize();
    this->setPath(p);
}

void Path::initialize() {
    permutation = nullptr;
    if (numberOfCities > 0) permutation = new int[numberOfCities];
}

bool Path::operator==(const Path &p) const {
    if(this->length != p.getLength()) return false;
    for (int i = 0; i < numberOfCities; i++){
        if (permutation[i] != p[i]) return false;
    }
    return true;
}

Path Path::crossoverPMXfirstChild(const Path &p, int left, int right) const{
    if (left == right) return Path(*this);
    if (left > right) {
        int tmp = right;
        right = left;
        left = tmp;

    }
    Path toReturn = *this;
    int length = right - left + 1;
    int toChange[length];
    int notUsed[length];

    for (int i = left, j = 0; i <= right; i++, j++){
        toChange[j] = p.permutation[i];
        notUsed[j] = toReturn.permutation[i];
        toReturn.permutation[i] = p.permutation[i];
    }

    for (int i = 0; i < length; i++){
        for (int j = 0; j < length; j++){
            if (toChange[i] == notUsed[j]){
                toChange[i] = -1;
                notUsed[j] = -1;
                j = length;
            }
        }
    }

    int k = 0;
    for (int i = 0; i < numberOfCities; i++){
        if (i == left) i += length;
        if (i == right) i++;
        for (int j = 0; j < length; j++){
            if (toReturn.permutation[i] == toChange[j]){
                while (notUsed[k] == -1) k++;
                toReturn.permutation[i] = notUsed[k++];
                j = length;
            }
        }
    }
    return toReturn;
}

Path Path::crossoverPMXsecondChild(const Path &p, int left, int right) const {
    return p.crossoverPMXfirstChild(*this, left, right);
}

Path Path::crossoverOXfirstChild(const Path &p, int left, int right) const {
    if (left == right) return Path(*this);
    if (left > right) {
        int tmp = right;
        right = left;
        left = tmp;
    }
    Path toReturn;
    int length = right - left + 1;
    int toChange[length];
    int notUsed[numberOfCities-1];
    for (int i = left, j = 0; i <= right; i++, j++){
        toChange[j] = p.permutation[i];
        toReturn.permutation[i] = p.permutation[i];
    }
    int k = 0;
    for (int i = right+1; i < numberOfCities; i++){
        notUsed[k++] = this->permutation[i];
    }
    for(int i = 0; i <= right; i++){
        notUsed[k++] = this->permutation[i];
    }

    k = 0;
    for (int i = right+1; i < numberOfCities;){
        bool insert = true;
        for (int j = 0; j < length; j++){
            if (notUsed[k] == toChange[j]){
                insert = false;
            }
        }
        if (insert){
            toReturn.permutation[i++] = notUsed[k++];
        }
        else k++;
    }
    for(int i = 0; i < left; ){
        bool insert = true;
        for (int j = 0; j < length; j++){
            if (notUsed[k] == toChange[j]){
                insert = false;
            }
        }
        if (insert){
            toReturn.permutation[i++] = notUsed[k++];
        }
        else k++;
    }
    return toReturn;
}

Path Path::crossoverOXsecondChild(const Path &p, int left, int right) const {
    return p.crossoverOXfirstChild(*this,left,right);
}

bool Path::operator!=(const Path &p) const {
    return !operator==(p);
}

bool Path::operator>=(const Path &p) const {
    return !operator<(p);
}

bool Path::operator<=(const Path &p) const {
    return operator<(p) || operator==(p);
}

bool Path::operator>(const Path &p) const {
    return !operator<=(p);
}


