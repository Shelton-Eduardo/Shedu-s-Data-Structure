#include <iostream>
#include <sstream>
#include <cassert>
#include "array.hpp"
#include <stdlib.h>

#define SEP(x) std::cout << x << std::endl


using namespace Container;

class TesteObject{
    private:
        int id;
    public:
    inline static int alive_state = 0;
    inline static int ghost_state = 0;

        int getid() const {
            return id;
        }

        TesteObject() : id(0){
            ++alive_state;
        }
        explicit TesteObject(int i) : id(i){
            ++alive_state;
        }
        TesteObject(const TesteObject& source) noexcept : id(source.id){
            ++alive_state;
        }
        TesteObject(TesteObject&& source) noexcept : id(source.id){
            source.id = -1;
            ++ghost_state;
        }
        TesteObject& operator=(const TesteObject& source) noexcept{
            if(id == -1) --ghost_state;
            id = source.id;
            ++alive_state;
            return *this;
        }
        TesteObject& operator=(TesteObject&& source) noexcept{
            if(id == -1) --ghost_state;
            id = source.id;
            source.id = -1;
            ++ghost_state;
            return *this;
        }
        ~TesteObject(){
            if(id == -1) --ghost_state;
            else --alive_state;
        }

        bool operator==(const TesteObject& rhs) const{
            return id==rhs.id;
        }
        bool operator!=(const TesteObject& rhs) const{
            return id!=rhs.id;
        }

        friend std::ostream& operator<<(std::ostream& printer, const TesteObject& source){
            printer << "Object with ID: " << source.id;
            return printer;
        }
};

class TesteObject_c{
    private:
        int id;
    public:
    inline static int alive_state_c = 0;
    inline static int ghost_state_c = 0;

        TesteObject_c() : id(0){
            ++alive_state_c;
        }
        TesteObject_c(int i) : id(i){
            ++alive_state_c;
        }
        TesteObject_c(const TesteObject_c& source) : id(source.id){
            ++alive_state_c;
        }
        TesteObject_c(TesteObject_c&& source) noexcept : id(source.id){
            source.id = -1;
            ++ghost_state_c;
        }
        TesteObject_c& operator=(const TesteObject_c& source){
            id = source.id;
            ++alive_state_c;
            return *this;
        }
        TesteObject_c& operator=(TesteObject_c&& source) noexcept{
            id = source.id;
            source.id = -1;
            ++ghost_state_c;
            return *this;
        }
        ~TesteObject_c(){
            if(id == -1) {
                std::cout << "Deleted a ghost\n";
                --ghost_state_c;
            }
            else {
                std::cout << "Deleted a Live\n";
                --alive_state_c;
            }
        }

        bool operator==(const TesteObject_c& rhs) const{
            return id==rhs.id;
        }
        bool operator!=(const TesteObject_c& rhs) const{
            return id!=rhs.id;
        }
};

void hello(){
    Array<TesteObject> arr(5);
    arr.append({TesteObject(3), TesteObject(4), TesteObject(5)});
    arr.insert(0, {TesteObject(1), TesteObject(2)});
    std::cout << arr << std::endl;
    std::cout << "Live Elements: " << TesteObject::alive_state << "\nGhost Elements: " << TesteObject::ghost_state << std::endl;
    std::cout << "Testing Addition" << std::endl;
    // arr.append({TesteObject(6), TesteObject(7), TesteObject(8)});
    std::cout << "Testing Iterator" << std::endl;
    // for(Array<TesteObject>::iterator i{arr.begin()}; i != arr.end(); i++){
    //     std::cout << *i << std::endl;
    // }
    for(auto& item : arr){
        std::cout << item << std::endl;
    }
    // for(auto& item : arr){
    //     std::cout << item << std::endl;
    // }
}

int main(){
    std::system("cls");
    hello();
    std::cout << "Live Elements: " << TesteObject::alive_state << "\nGhost Elements: " << TesteObject::ghost_state;
}
