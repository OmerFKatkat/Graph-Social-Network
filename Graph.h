#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <utility>
#include "Person.h"
#include <unordered_map>
#include <queue>
#include <set>
#include <cmath>

class Graph {
private:
    std::vector<std::pair<int, Person>> graph;
    int size;

public:
    void addPerson(int id, const Person &person);
    Person* getPerson(int id) const;
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
    std::vector<Person*> suggestFriends(int person_id, int mode) const;
    std::vector<Person*> suggestFriendsByCommonFriends(const Person *person) const;
    std::vector<Person*> suggestFriendsByOccupation(const Person *person) const;
    std::vector<Person*> suggestFriendsByAge(const Person *person) const;
    int getSize() const;
    void degreeCentrality() const;
    double clusteringCoefficient(int id) const;
    std::vector<std::pair<int, Person>> getGraph() const;
    std::vector<std::vector<Person*>> girvanNewman(int iterations) const;
    static int edgeWeight(const Graph &graph, int u, int v);
    void removeFriendship(int id1, int id2);
    void removeEdge(int u, int v);
};

#endif // GRAPH_H