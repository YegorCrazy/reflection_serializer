#pragma once

#include <experimental/meta>
#include <ranges>
#include <sstream>

namespace serializer
{

template <class T>
void serialize(std::stringstream& ss, const T& obj)
{
    constexpr auto ctx = std::meta::access_context::unchecked();

    template for (constexpr auto field : define_static_array(nonstatic_data_members_of(^^T, ctx)))
        ss << identifier_of(field) << " " << obj.[:field:] << std::endl;
}

template <class T>
T deserialize(std::string str)
{
    constexpr auto ctx = std::meta::access_context::unchecked();
    T result;

    for (const auto& line : str | std::ranges::views::split('\n')
             | std::ranges::to<std::vector<std::string>>())
    {
        if (!line.empty())
        {
            auto delimPos = line.find(' ');
            auto name = line.substr(0, delimPos);
            auto strVal = line.substr(delimPos + 1);
            template for (constexpr auto field : define_static_array(nonstatic_data_members_of(^^T, ctx)))
            {
                if (identifier_of(field) == name)
                {
                    auto iss = std::stringstream(strVal);
                    if (iss >> result.[:field:]; !iss) {
                        std::print(stderr, "Failed to parse field {} into a {}\n", strVal, display_string_of(type_of(field)));
                        std::exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }

    return result;
}

} // namespace serializer