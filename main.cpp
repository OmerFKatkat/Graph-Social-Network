#include <iostream>
#include "utils.cpp"
#include <windows.h>
#include <fstream>
#include <stdexcept>
using namespace std;

class SocialNetwork {
public:
    void displayNetwork() {
        try {
            Graph graph = readData("social_network.csv");
            cout << graph;
        } catch (const runtime_error& e) {
            cout << e.what() << endl;
        }
    }

    void suggestFriends() {
        int person_id, mode;
        cout << "Enter the person's ID: ";
        cin >> person_id;
        cout << "\nModes;\n1- Common Friends\n2- Occupation\n3- Age\nEnter the mode: ";
        cin >> mode;
        cout << endl;
        try {
            Graph graph = readData("social_network.csv");
            vector<Person *> suggestedFriends = graph.suggestFriends(person_id, mode);

            for (Person *person : suggestedFriends) {
                cout << "Person ID: " << person->getId() << "\n";
                cout << "Name: " << person->getName() << "\n";
                cout << "Age: " << person->getAge() << "\n";
                cout << "Gender: " << person->getGender() << "\n";
                cout << "Occupation: " << person->getOccupation() << "\n";
                cout << "Friends: ";
                for (int friendId : person->getFriends()) {
                    cout << friendId << " ";
                }
                cout << endl;
                cout << endl;
            }
        } catch (const runtime_error& e) {
            cout << e.what() << endl;
        }
    }
    
    void calculateDegreeCentrality() {
        Graph graph = readData("social_network.csv");
        graph.degreeCentrality();
    }

    void calculateClusteringCoefficient() {
    try {
        Graph graph = readData("social_network.csv");
        for (const auto& node : graph.getGraph()) {
            int person_id = node.first;
            double coefficient = graph.clusteringCoefficient(person_id);
            cout << "The clustering coefficient for person with ID " << person_id << " is: " << coefficient << endl;
        }
        cout << endl;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
}


    void detectCommunities() {
        int iterations;
        std::cout << "Enter the number of iterations: ";
        std::cin >> iterations;
        std::cout << std::endl;

        try {
            Graph graph = readData("social_network.csv");
            std::vector<std::vector<Person *>> communities = graph.girvanNewman(iterations);

            for (int i = 0; i < communities.size(); ++i) {
                std::cout << "Community " << i + 1 << ":\n";
                for (Person *person : communities[i]) {
                    std::cout << "Person ID: " << person->getId() << "\n";
                    std::cout << "Name: " << person->getName() << "\n";
                    std::cout << "Age: " << person->getAge() << "\n";
                    std::cout << "Gender: " << person->getGender() << "\n";
                    std::cout << "Occupation: " << person->getOccupation() << "\n";
                    std::cout << "Friends: ";
                    for (int friendId : person->getFriends()) {
                        std::cout << friendId << " ";
                    }
                    std::cout << "\n\n";
                }
            }
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
    }



};

int main() {
    SocialNetwork sn;
    int choice;
    do{
        cout << "1. Display the social network\n";
        cout << "2. Suggest friends for users\n";
        cout << "3. Calculate degree centrality for each user\n";
        cout << "4. Calculate clustering coefficient for each user\n";
        cout << "5. Detect communities using the Girvan-Newman algorithm\n";
        cout << "6. Exit the program\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;
        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please try again.\n"<<endl;
            continue;
        }

        switch (choice) {
        case 1:
            sn.displayNetwork();
            break;
        case 2:
            sn.suggestFriends();
            break;
        case 3:
            sn.calculateDegreeCentrality();
            break;
        case 4:
            sn.calculateClusteringCoefficient();
            break;
        case 5:
            sn.detectCommunities();
            break;
        case 6:
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
            cout << endl;
            break;
        }
    }while(choice != 6);

    return 0;
}