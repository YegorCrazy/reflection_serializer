#pragma once

#include <experimental/meta>
#include <ranges>
#include <sstream>

namespace serializer
{

// TODO: ДОБАВЛЕНИЕ НОВОГО ТИПА
template <class T>
using PtrVariant = std::variant<int T::*, double T::*, std::string T::*>;

template <class T>
consteval auto allMembers()
{
    constexpr auto ctx = std::meta::access_context::unchecked();

    constexpr size_t N = std::meta::nonstatic_data_members_of(^^T, ctx).size();
    std::array<std::pair<std::string, PtrVariant<T>>, N> result;

    size_t i = 0;
    template for (constexpr auto field : std::define_static_array(std::meta::nonstatic_data_members_of(^^T, ctx)))
    {
        result[i] = { std::string(identifier_of(field)), std::meta::extract<[: type_of(field) :] T::*>(field)};
        ++i;
    }

    return result;
}

template <class T>
void serializeOld(std::stringstream& ss, const T& obj)
{
    for (const auto& [memberName, memberPtrVar] : allMembers<T>())
    {
        ss << memberName << " ";
        std::visit([&](auto&& v) { ss << obj.*v; }, memberPtrVar);
        ss << std::endl;
    }
}

template <class T>
T deserializeOld(std::string str)
{
    T result;

    for (const auto& line : str | std::ranges::views::split('\n')
             | std::ranges::to<std::vector<std::string>>())
    {
        if (!line.empty())
        {
            auto delimPos = line.find(' ');
            auto name = line.substr(0, delimPos);
            auto strVal = line.substr(delimPos + 1);
            for (const auto& [memberName, memberPtrVar] : allMembers<T>())
            {
                if (memberName == name)
                    std::visit([&](auto&& v)
                    {
                        // TODO: ДОБАВЛЕНИЕ НОВОГО ТИПА
                        using memType = std::decay_t<decltype(v)>;
                        if constexpr (std::is_same_v<memType, int T::*>)
                            result.*v = stoi(strVal);
                        else if constexpr (std::is_same_v<memType, double T::*>)
                            result.*v = stod(strVal);
                        else if constexpr (std::is_same_v<memType, std::string T::*>)
                            result.*v = strVal;
                    },
                    memberPtrVar);
            }
        }
    }

    return result;
}

} // namespace serializer