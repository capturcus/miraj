#include <string>
#include <vector>
#include <fstream>

class Nut {
public:
    Nut(std::string n) : lhs(n) {}
    virtual ~Nut() {}

    virtual void dump() const = 0;

    std::string lhs;
};

struct Production;

class Terminal final
    : public Nut
{
    using Nut::Nut;
public:
    ~Terminal() {}

    void dump() const override;
};

class NonTerminal final
    : public Nut
{
    using Nut::Nut;
public:
    ~NonTerminal() {}

    void dump() const override;

    std::vector<Production> productions;
};

struct Production {
    int number;
    std::vector<Nut*> nuts;
};