/*
    Inheritance Examples
*/
#include <iostream>
#include <string>

class AdmissionTicket
{
private:
    std::string _visitor{};
    bool _admitted = false;

public:
    AdmissionTicket(const std::string &visitor) : _visitor{visitor}
    {
    }

    const std::string &getVisitor() const { return _visitor; }

    bool isAdmitted() const { return _admitted; }

    bool admit()
    {
        if (_admitted)
        {
            return false;
        }
        _admitted = true;
        return true;
    }
};

class AdultTicket : public AdmissionTicket
{
public:
    AdultTicket(const std::string &visitor) : AdmissionTicket(visitor)
    {
    }

    int getPriceCents() const { return 1500; }
};

class ChildTicket : public AdmissionTicket
{
private:
    int _priceCents{800};

public:
    ChildTicket(const std::string &visitor) : AdmissionTicket(visitor) {}

    int getPriceCents() { return _priceCents; }
};

class GuidedTourTicket : public AdmissionTicket
{
private:
    std::string _slot{};
    bool _joinedTour = false;

public:
    GuidedTourTicket(const std::string &visitor, const std::string &slot) : AdmissionTicket(visitor), _slot{slot} {}

    const std::string &getSlot() const { return _slot; }

    bool getJoinedTour() const
    {
        return _joinedTour;
    }

    bool joinTour()
    {
        // Deny if already admitted and already joined tour
        if (!isAdmitted() || _joinedTour)
        {
            return false;
        }
        _joinedTour = true;
        return true;
    }
};

void enterMuseum(AdmissionTicket &ticket)
{
    bool entered = ticket.admit();
    std::cout << ticket.getVisitor() << (entered ? " admitted\n" : " already admitted\n");
}

int main()
{

    AdultTicket adult("Maya");
    ChildTicket child("leo");

    enterMuseum(adult);
    enterMuseum(adult); // The inherited rule prevents second admission

    GuidedTourTicket tour("Eva", "11:00");
    std::cout << "Tour before admission" << (tour.joinTour()?" joined\n": " denied\n");

    enterMuseum(tour);
    std::cout << tour.getVisitor() << " tour at: " << tour.getSlot() <<
    (tour.joinTour()? " entered\n": " denied\n");
    std::cout << "Second tour check-in " <<
    (tour.joinTour()? " entered\n": " denied\n");

    return 0;
}
