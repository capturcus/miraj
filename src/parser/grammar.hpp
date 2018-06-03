#include <memory>
#include <string>
#include <vector>
#include <jsoncpp/json/json.h>

class Nut;
class Terminal;
class NonTerminal;

struct Production {
    int number;
    std::vector<Nut*> nuts;
};

class Nut {
private:
    std::string name;

public:
    Nut(std::string name) : name(std::move(name)) {}
    virtual ~Nut() {}

    inline const std::string& GetName() const {
        return name;
    }

    virtual Terminal* AsTerminal() {
        return nullptr;
    }

    virtual NonTerminal* AsNonTerminal() {
        return nullptr;
    }

    virtual void Dump() const = 0;
};

class Terminal final
    : public Nut
{
private:
    std::string contents;

public:
    Terminal(std::string name, std::string contents)
            : Nut(std::move(name)) // TODO: Does moving here works?
            , contents(std::move(contents)) {
    }

    inline const std::string& GetValue() const {
        return contents;
    }

    virtual Terminal* AsTerminal() override {
        return this;
    }

    void Dump() const override;
};

class NonTerminal final
    : public Nut
{
private:
    std::vector<Production> productions;

public:
    NonTerminal(std::string name)
            : Nut(name) {
    }

    virtual NonTerminal* AsNonTerminal() override {
        return this;
    }

    inline const std::vector<Production> GetProductions() const {
        return productions;
    }

    inline void AddProduction(Production prod) {
        productions.emplace_back(std::move(prod));
    }

    void Dump() const override;
};

struct GrammarDescription {
private:
    inline GrammarDescription(std::map<std::string, std::unique_ptr<Nut>> nuts)
        : nuts(std::move(nuts))
    {}

    std::map<std::string, std::unique_ptr<Nut>> nuts;

public:
    GrammarDescription() {};

    static GrammarDescription FromJsonValue(const Json::Value& description);

    std::vector<std::pair<std::string, std::string>> GetTokenList() const;
    inline const std::map<std::string, std::unique_ptr<Nut>>& GetNuts() const {
        return nuts;
    }
};
