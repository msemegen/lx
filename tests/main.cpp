#include <catch2/catch_session.hpp>

int main(int argc, char* argv[])
{
    std::int32_t result = Catch::Session().run(argc, argv);

    return result;
}