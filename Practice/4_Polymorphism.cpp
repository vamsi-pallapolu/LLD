// Bad Design


#include <iostream>
#include <string>

// struct AdmissionTicket{
//     std::string _visitor;
//     std::string _category;
// };

// void printReceipt(const AdmissionTicket& ticket){
//     int priceCents;
//     if(ticket._category == "adult")
//         priceCents = 1000;
//     else if(ticket._category == "child")
//         priceCents = 800;
//     else{
//         std::cout << "Unknown ticket category\n";
//         return;
//     }
        
//     std::cout << ticket._visitor << " :" << priceCents << std::endl;
// }


// Good Design
class AdmissionTicket{
private:
    std::string _visitor{};
public:
    AdmissionTicket(const std::string& visitor):_visitor{visitor}{}
    
    virtual ~AdmissionTicket() = default;

    const std::string& getVisitor() const {return _visitor;}

    virtual int getPriceCents() const = 0;

};

class AdultTicket: public AdmissionTicket{
private:
    int _priceCents = 1500;

public:
    AdultTicket(const std::string& visitor):AdmissionTicket(visitor){}

    int getPriceCents() const override {
        return _priceCents;
    }
};

class ChildTicket: public AdmissionTicket{
private:
    int _priceCents = 800;

public:
    ChildTicket(const std::string& visitor):AdmissionTicket(visitor){}

    int getPriceCents() const override {
        return _priceCents;
    }
};

void printReceipt(const AdmissionTicket& ticket){
    std::cout << ticket.getVisitor() << " :" << ticket.getPriceCents() << std::endl; 
}

int main(){
    AdultTicket adult{"Maya"};
    ChildTicket child{"Leo"};

    printReceipt(adult);
    printReceipt(child);
    return 0;
}
