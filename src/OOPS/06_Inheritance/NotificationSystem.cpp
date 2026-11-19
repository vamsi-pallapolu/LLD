#include <iostream>
#include <string>
#include <ctime>

/*

### When to use inheritance ####
- is-a relationshio
- reuse the code
- shared behavior
- refer child object through parent object and make sure parent behivor does not change

### When not to use inheritance ####
- has-a relationship
- combine behavior from multiple sources dynamically (use composition)
- don't want strict parent - child relationship


*/

class Notification
{
protected:
    std::string _recipient;
    std::string _message;
    std::string _timestamp;

public:
    Notification(const std::string &recipient, const std::string &message) : _recipient{recipient}, _message{message}
    {
        std::time_t now = std::time(nullptr);
        char timestamp[20];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        _timestamp = timestamp;
    }

    virtual ~Notification() {}

    std::string formatHeader()
    {
        return  "[" + _timestamp + "] To: " + _recipient;
    }

    virtual void send()
    {
        std::cout << formatHeader() << std::endl;
        std::cout << "Message:" << _message << std::endl;
    }
};


class EmailNotification: public Notification{
private:
    std::string _subject;
public:
    EmailNotification(const std::string &recipient, const std::string &message,
    const std::string& subject):Notification(recipient, message), _subject{subject} {}

    void send() override{
        std::cout << formatHeader() << std::endl;
        std::cout << "Subject: " << _subject << std::endl;
        std::cout << "Body: " << _message << std::endl;
        std::cout << "Status: Email delivered" << std::endl;
    }

};

int main()
{

    EmailNotification email("alice@example.com",
        "Your order has been shipped!", "Order Update");
    email.send();

    return 0;
}