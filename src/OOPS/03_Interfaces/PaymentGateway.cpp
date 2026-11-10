#include <iostream>

class PaymentGateway{
public:
    virtual ~PaymentGateway() = default; // Proper cleanup
    virtual void initiatePayment(double amount) = 0; // Pure virtual function
};

class StripePayment: public PaymentGateway{
private:
    double amount;
public:
    ~StripePayment(){

    }

    void initiatePayment(double amount) override{
        std::cout << "Processing Payment "<< amount << " through Stripe" << std::endl;
    }
};

class RazorpayPayment: public PaymentGateway{
private:
    double amount;
public:

    ~RazorpayPayment(){

    }
    
    void initiatePayment(double amount) override{
        std::cout << "Processing Payment " << amount << " through Razorpay" << std::endl;
    }
};


class CheckoutService{
private:
    PaymentGateway *_gateway;
public:
    CheckoutService(PaymentGateway* gateway):_gateway{gateway}{

    }

    void setPaymentGateway(PaymentGateway* gateway){
        _gateway = gateway;
    }

    void checkout(double amount){
        if(_gateway!=nullptr){
            _gateway->initiatePayment(amount);
        }
    }

};

/*
    This pattern is called dependency injection: instead of creating its own dependencies, the class receives them from the outside
    . ANd it only works because the dependency is typed as interface, not a concrete class.
*/


int main(){

    StripePayment stripeGateway;
    CheckoutService service(&stripeGateway);
    service.checkout(1000);

    RazorpayPayment razorPayment;
    CheckoutService service2(&razorPayment);
    service2.checkout(500);

    return 0;
}