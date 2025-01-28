#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include <algorithm>

class Person {
public:
    int id;
    std::string name;
    int age;
    std::string gender;
    std::string occupation;
    std::vector<int> friends;

    Person(int id, std::string name, int age, std::string gender, std::string occupation, std::vector<int> friends);

    int getFriendsCount() const;

    void removeFriend(int friendId);

    int getMutualFriendsCount(const Person& other) const;
    
    
    
    
    // Getters for strings
    const std::string& getName() const {
        return name;
    }

    const std::string& getGender() const {
        return gender;
    }

    const std::string& getOccupation() const {
        return occupation;
    }

    // Getters for ints
    int getId() const {
        return id;
    }

    int getAge() const {
        return age;
    }

    // Getters for vectors
    const std::vector<int>& getFriends() const {
        return friends;
    }
};

#endif // PERSON_H
