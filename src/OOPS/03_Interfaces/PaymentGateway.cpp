class PaymentGateway{
public:
    virtual ~PaymentGateway(); // Proper cleanup
    virtual void initiatePayment(double amount) = 0; // Pure virtual function
};