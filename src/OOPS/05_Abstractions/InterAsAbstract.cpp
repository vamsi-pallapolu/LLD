/*

Interfaces as abstraction

- While abstract class abstract a family of related classes that share behavior, interfaces abstarct a capability that unrelated
classes share.


*/

#include <iostream>

class Exportable
{
public:
    virtual ~Exportable() = default;
    virtual std::string exportData() = 0;
};

class CSVExporter : public Exportable
{
public:
    std::string exportData() override
    {
        return "name, email, age\nAlicie, hello@gmail.com, 23";
    }
};

class JSONExporter : public Exportable
{
public:
    std::string exportData() override
    {
        return "{\"name\": \"Alice\", \"email\": \"alice@example.com\"}";
    }
};

void export_data(Exportable &exportable)
{
    std::string data = exportable.exportData();
    std::cout << data << std::endl;
}

int main()
{
    CSVExporter cexp;

    export_data(cexp);
    JSONExporter jexp;
    export_data(jexp);

    return 0;
}
