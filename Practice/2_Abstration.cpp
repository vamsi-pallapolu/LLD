// Museum receipt example
//Bad example


#include <iostream>
#include <string>
#include <stdexcept>

// struct AdmissionTicket{
//     std::string visitor{};
//     int priceCents{};
// };

// // Here receipt depends on ticket internal details
// void printReceipt(const AdmissionTicket& ticket){
//     std::cout << ticket.visitor << " :" << ticket.priceCents << std::endl;
// }


class AdmissionTicket{
private:
    std::string _visitor{};
    int _priceCents{};
public:
    
    AdmissionTicket(const std::string& visitor, int price):_visitor{visitor}{
        if(price<0)
            throw std::invalid_argument("Price must not be negative");
        _priceCents = price;
    }

    void setPriceCents(int price){
         if(price<0)
            throw std::invalid_argument("Price must not be negative");
        _priceCents = price;   
    }

    const std::string& getVisitor() const{
        return _visitor;
    }

    int getPriceCents() const {
        return _priceCents;
    }

};


void printReceipt(const AdmissionTicket& ticket){
    // Here printReceipt does not know how the price is stored and is calculated
    // AdmissionTicket exposes its public methods as interface for Receipt
    std::cout << ticket.getVisitor() << " :" << ticket.getPriceCents() << std::endl;
}

int main(){

    AdmissionTicket adult{"Maya", 1000};
    AdmissionTicket child{"Leo", 500};

    printReceipt(adult);



    return 0;
}