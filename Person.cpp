#include "Person.h"

Person::Person(int id, std::string name, int age, std::string gender, std::string occupation, std::vector<int> friends)
{
    this->id = id;
    this->name = name;
    this->age = age;
    this->gender = gender;
    this->occupation = occupation;
    this->friends = friends;
}

int Person::getFriendsCount() const
{
    return friends.size();
}

void Person::removeFriend(int friendId) {
    friends.erase(std::remove(friends.begin(), friends.end(), friendId), friends.end());
}

int Person::getMutualFriendsCount(const Person& other) const {
    int count = 0;
    for (int friendId : friends) {
        if (std::find(other.friends.begin(), other.friends.end(), friendId) != other.friends.end()) {
            count++;
        }
    }
    return count;
}