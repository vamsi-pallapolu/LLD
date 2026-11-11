#include <iostream>

class NotificationService
{
public:
    virtual void send(const std::string &recipient, const std::string &message) = 0;

    virtual ~NotificationService() = default;
};

class EmailNotifier:public NotificationService{
public:
    void send(const std::string &recipient, const std::string &message) override {
        std::cout << "Email Notifier" << " Recipient:" << message << std::endl;
    }
};

class SlackNotifier:public NotificationService{
public:
    void send(const std::string &recipient, const std::string &message) override {
        std::cout << "Slack Notifier" << " Recipient:" << message << std::endl;
    }
};

class AlertSerive{
private:
    NotificationService* _notifier;
public:
    AlertSerive(NotificationService* notifier):_notifier{notifier}{}
    
    void triggerAlert(const std::string &recipient, const std::string &issue){
        std::string messgae = "Alert " + issue;
        _notifier->send(recipient, messgae);
    }
};

int main()
{

    EmailNotifier emailNotifier;
    AlertSerive alertService(&emailNotifier);
    alertService.triggerAlert("vpalla@abc.com", "CPU usage is high");

    SlackNotifier slacklNotifier;
    alertService = &slacklNotifier; // does not work with explicit keyword
    alertService.triggerAlert("vpalla@abc.com", "DISK usage is high");


    return 0;
}