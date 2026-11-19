#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Notification
{
protected:
    std::string _recipient;
    std::string _message;

public:
    Notification(const std::string &recipient, const std::string &message) : _recipient{recipient}, _message{message} {}

    virtual ~Notification() {}

    virtual void send()
    {
        std::cout << "Notification send to " << _recipient << std::endl;
    }
};

class EmailNotification : public Notification
{
private:
    std::string _subject;

public:
    EmailNotification(const std::string &recipient, const std::string &message, const std::string &subject) : Notification(recipient, message), _subject{subject} {}

    virtual ~EmailNotification() {}

    void send()
    {
        std::cout << "Sending EMAIL to " << _recipient
                  << " | Subject: " << _subject << std::endl;
    }
};

class SMSNotification : public Notification
{
private:
    std::string _phoneNumber;

public:
    SMSNotification(const std::string &recipient, const std::string &message, std::string phoneNumber) : Notification(recipient, message), _phoneNumber{phoneNumber} {}

    virtual ~SMSNotification() {}

    void send()
    {
        std::cout << "Sending SMS to " << _phoneNumber
                  << " | Message: " << _message << std::endl;
    }
};

int main()
{
    std::vector<std::unique_ptr<Notification>> notifications;
    notifications.push_back(std::make_unique<EmailNotification>("alice@example.com", "Your order shipped!", "Order Update"));
    notifications.push_back(std::make_unique<SMSNotification>("Bob", "Code: 482910", "+1-555-0123"));

    for (std::unique_ptr<Notification> &n : notifications)
    {
        n->send();
    }

    return 0;
}