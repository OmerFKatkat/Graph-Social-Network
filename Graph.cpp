#include "Graph.h"

using namespace std;

void Graph::addPerson(int id, const Person &person)
{
    graph.push_back(make_pair(id, person));
    size++;
}

Person* Graph::getPerson(int id) const
{
    for (const auto& p : graph)
    {
        if (p.first == id)
        {
            return &const_cast<Person&>(p.second);
        }
    }
    return nullptr;
}

vector<Person*> Graph::suggestFriends(int person_id, int mode) const
{
    vector<Person*> suggestedFriends;

    const Person* person = getPerson(person_id);
    if (!person) {
        return suggestedFriends;
    }

    switch(mode) {
        case 1:
            suggestedFriends = suggestFriendsByCommonFriends(person);
            break;
        case 2:
            suggestedFriends = suggestFriendsByOccupation(person);
            break;
        case 3:
            suggestedFriends = suggestFriendsByAge(person);
            break;
        default:
            cout << "Invalid mode\n";
            cout << endl;
            break;
    }

    return suggestedFriends;
}

vector<Person*> Graph::suggestFriendsByCommonFriends(const Person *person) const
{
    vector<Person*> suggestedFriends;
    vector<int> Friends = person->getFriends();

    for (const auto& pair : graph) {
        const Person* potentialFriend = &pair.second;
        if (potentialFriend == person) continue;

        vector<int> potentialFriendFriends = potentialFriend->getFriends();
        vector<int> commonFriends;

        set_intersection(Friends.begin(), Friends.end(),
                         potentialFriendFriends.begin(), potentialFriendFriends.end(),
                         back_inserter(commonFriends));
        if (!commonFriends.empty()) {
            suggestedFriends.push_back(const_cast<Person*>(potentialFriend));
        }
    }

    return suggestedFriends;
}

vector<Person*> Graph::suggestFriendsByOccupation(const Person *person) const
{
    vector<Person*> suggestedFriends;
    string Occupation = person->getOccupation();
    for (const auto& pair : graph) {
        const Person* pfriend = &pair.second;
        if (pfriend->getOccupation() == Occupation && pfriend != person) {
            suggestedFriends.push_back(const_cast<Person*>(pfriend));
        }
    }
    if (suggestedFriends.empty()) {
        cout << "Couldn't find any person with the same occupation.\n";
        cout << endl;
    }
    return suggestedFriends;
}

vector<Person*> Graph::suggestFriendsByAge(const Person *person) const
{
    vector<Person*> suggestedFriends;
    int Age = person->getAge();
    int low = Age - 3;
    int high = Age + 3;
    for (const auto& pair : graph) {
        const Person* pfriend = &pair.second;
        if (pfriend->getAge() >= low && pfriend->getAge() <= high && pfriend != person) {
            suggestedFriends.push_back(const_cast<Person*>(pfriend));
        }
    }

    return suggestedFriends;
}

int Graph::getSize() const
{
    return graph.size();
}

void Graph::degreeCentrality() const
{
    for (const auto& pair : graph)
    {
        int id = pair.first;
        const Person& person = pair.second;
        int degreeCentrality = person.getFriendsCount();
        cout << "ID: " << id << " has degree centrality: " << degreeCentrality << endl;
    }
}

double Graph::clusteringCoefficient(int id) const {
    Person* person = getPerson(id);
    if (!person) {
        throw std::runtime_error("Person not found in the graph");
    }
    
    const std::vector<int>& friends = person->getFriends();
    int numFriends = friends.size();
    
    if (numFriends < 2) {
        return 0.0;
    }

    int actualEdges = 0;
    int possibleEdges = numFriends * (numFriends - 1) / 2;
    
    for (size_t i = 0; i < numFriends; ++i) {
        Person* friend1 = getPerson(friends[i]);
        for (size_t j = i + 1; j < numFriends; ++j) {
            Person* friend2 = getPerson(friends[j]);
            const std::vector<int>& friend2Friends = friend2->getFriends();
            if (std::find(friend2Friends.begin(), friend2Friends.end(), friends[i]) != friend2Friends.end()) {
                ++actualEdges;
            }
        }
    }

    return static_cast<double>(actualEdges) / possibleEdges;
}


vector<pair<int, Person>> Graph::getGraph() const
{
    return graph;
}

ostream& operator<<(ostream& os, const Graph& graph)
{
    for (const auto& pair : graph.getGraph()) {
        os << "Person ID: " << pair.first << "\n";
        os << "Name: " << pair.second.name << "\n";
        os << "Age: " << pair.second.age << "\n";
        os << "Gender: " << pair.second.gender << "\n";
        os << "Occupation: " << pair.second.occupation << "\n";
        os << "Friends: ";
        for (int friendId : pair.second.friends) {
            os << friendId << " ";
        }
        os << "\n\n";
    }

    return os;
}

void Graph::removeFriendship(int id1, int id2)
{
    Person* person1 = getPerson(id1);
    Person* person2 = getPerson(id2);
    if (person1 && person2) {
        person1->removeFriend(id2);
        person2->removeFriend(id1);
    }
}

void Graph::removeEdge(int u, int v)
{
    removeFriendship(u, v);
}

int Graph::edgeWeight(const Graph &graph, int u, int v)
{
    const Person* person1 = graph.getPerson(u);
    const Person* person2 = graph.getPerson(v);
    if (person1 && person2) {
        return person1->getMutualFriendsCount(*person2);
    }
    return 0;
}

std::vector<std::vector<Person*>> Graph::girvanNewman(int iterations) const {
    Graph workingGraph = *this;
    std::vector<std::vector<Person*>> communities;

    for (int iter = 0; iter < iterations; ++iter) {
        std::map<std::pair<int, int>, double> edgeBetweenness;

        for (const auto& node : workingGraph.graph) {
            for (int friendId : node.second.getFriends()) {
                edgeBetweenness[std::make_pair(std::min(node.first, friendId), std::max(node.first, friendId))] = 0.0;
            }
        }
        for (const auto& node : workingGraph.graph) {
            std::queue<int> queue;
            std::map<int, std::vector<int>> predecessors;
            std::map<int, int> shortestPaths;
            std::map<int, double> dependency;
            std::set<int> visited;

            queue.push(node.first);
            shortestPaths[node.first] = 1;
            visited.insert(node.first);

            std::vector<int> Order;

            while (!queue.empty()) {
                int current = queue.front();
                queue.pop();
                Order.push_back(current);

                for (int neighbor : workingGraph.getPerson(current)->getFriends()) {
                    if (visited.find(neighbor) == visited.end()) {
                        queue.push(neighbor);
                        visited.insert(neighbor);
                    }
                    if (shortestPaths.find(neighbor) == shortestPaths.end()) {
                        shortestPaths[neighbor] = 0;
                    }
                    if (std::find(predecessors[neighbor].begin(), predecessors[neighbor].end(), current) == predecessors[neighbor].end()) {
                        predecessors[neighbor].push_back(current);
                        shortestPaths[neighbor] += shortestPaths[current];
                    }
                }
            }
            std::map<int, double> dependencyScore;
            for (auto it = Order.rbegin(); it != Order.rend(); ++it) {
                int node = *it;
                for (int pred : predecessors[node]) {
                    double ratio = (double)shortestPaths[pred] / shortestPaths[node];
                    double delta = (1.0 + dependency[node]) * ratio;
                    edgeBetweenness[std::make_pair(std::min(pred, node), std::max(pred, node))] += delta;
                    dependency[pred] += delta;
                }
            }
        }
        auto maxEdge = std::max_element(edgeBetweenness.begin(), edgeBetweenness.end(),
                                        [](const std::pair<std::pair<int, int>, double> &a, const std::pair<std::pair<int, int>, double> &b) {
                                            return a.second < b.second;
                                        });

        if (maxEdge != edgeBetweenness.end()) {
            workingGraph.removeFriendship(maxEdge->first.first, maxEdge->first.second);
        }
        std::set<int> visited;
        communities.clear();
        for (const auto& node : workingGraph.graph) {
            if (visited.find(node.first) == visited.end()) {
                std::vector<Person*> community;
                std::queue<int> queue;
                queue.push(node.first);
                visited.insert(node.first);
                while (!queue.empty()) {
                    int current = queue.front();
                    queue.pop();
                    community.push_back(workingGraph.getPerson(current));

                    for (int neighbor : workingGraph.getPerson(current)->getFriends()) {
                        if (visited.find(neighbor) == visited.end()) {
                            queue.push(neighbor);
                            visited.insert(neighbor);
                        }
                    }
                }
                communities.push_back(community);
            }
        }
    }

    return communities;
}