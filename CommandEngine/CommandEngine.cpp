#include <iostream>
#include "Engine.hpp"
#include "Wrapper.hpp"
#include "Subject.hpp"

int main()
{
    Subject subj;

    Wrapper<Subject, int, int, int> sumWrapper(
        &subj,
        &Subject::sum,
        { {"a", 0}, {"b", 0} }
    );

    Wrapper<Subject, double, double, double, double> weightedWrapper(
        &subj,
        &Subject::weighted_sum,
        { {"a", 0.0}, {"b", 0.0}, {"w", 0.5} }
    );

    Wrapper<Subject, std::string, std::string, int> repeatWrapper(
        &subj,
        &Subject::repeat,
        { {"s", std::string("x")}, {"n", 1} }
    );

    Wrapper<Subject, int> magicWrapper(
        &subj,
        &Subject::get_magic_number,
        { }
    );

    Engine engine;

    engine.register_command(&sumWrapper, "sum");
    engine.register_command(&weightedWrapper, "weighted");
    engine.register_command(&repeatWrapper, "repeat");
    engine.register_command(&magicWrapper, "magicNumber");


    ArgValue r1 = engine.execute("sum",
        { {"a", 4}, {"b", 5} });
    std::cout << "Result of sum: " << r1 << "\n\n";

    ArgValue r2 = engine.execute("weighted",
        { {"a", 10.0}, {"b", 20.0}, {"w", 0.25} });
    std::cout << "Result of weighted_sum: " << r2 << "\n\n";

    ArgValue r3 = engine.execute("repeat",
        { {"s", std::string("ab")}, {"n", 3} });
    std::cout << "Result of repeat: " << r3 << "\n\n";

    ArgValue r4 = engine.execute("magicNumber",
        {  });
    std::cout << "Result of repeat: " << r4 << "\n\n";

    return 0;
}
