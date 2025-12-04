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

    Wrapper<Subject, std::string, bool> yesNoWrapper(
        &subj,
        &Subject::yes_no,
        { {"flag", false} }
    );

    Wrapper<Subject, std::string, std::string, double> mixWrapper(
        &subj,
        &Subject::mix,
        { {"s", std::string("default")}, {"x", 0.0} }
    );

    Wrapper<Subject, void, int, int, int> log3Wrapper(
        &subj,
        &Subject::log3,
        { {"a", 0}, {"b", 0}, {"c", 0} }
    );

    Wrapper<Subject, double, double, double, double, double, double> linear5Wrapper(
        &subj,
        &Subject::linear5,
        { {"a", 0.0}, {"b", 0.0}, {"c", 0.0}, {"d", 0.0}, {"e", 0.0} }
    );

    Engine engine;

    engine.register_command(&sumWrapper, "sum");
    engine.register_command(&weightedWrapper, "weighted");
    engine.register_command(&repeatWrapper, "repeat");
    engine.register_command(&magicWrapper, "magicNumber");
    engine.register_command(&yesNoWrapper, "yes_no");
    engine.register_command(&mixWrapper, "mix");
    engine.register_command(&log3Wrapper, "log3");
    engine.register_command(&linear5Wrapper, "linear5");


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

    ArgValue r5 = engine.execute("yes_no", { {"flag", true} });
    std::cout << "yes_no(true): " << r5 << "\n\n";

    ArgValue r6 = engine.execute("mix", { {"s", std::string("hello")}, {"x", 2.5} });
    std::cout << "mix: " << r6 << "\n\n";

    ArgValue r7 = engine.execute("log3", { {"a", 1}, {"b", 2}, {"c", 3} });
    std::cout << "log3 result: " << r7 << "\n\n";

    ArgValue r8 = engine.execute("linear5",
        { {"a", 1.0}, {"b", 2.0}, {"c", 3.0}, {"d", 4.0}, {"e", 5.0} });
    std::cout << "linear5: " << r8 << "\n";

    return 0;
}