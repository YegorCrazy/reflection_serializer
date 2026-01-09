#pragma once

#include <experimental/meta>
#include <ranges>
#include <sstream>

#include "nlohmann/json.hpp"

namespace serializer
{

template <class T>
nlohmann::json serialize(const T& obj)
{
    constexpr auto ctx = std::meta::access_context::unchecked();

    nlohmann::json result;
    template for (constexpr auto& field : define_static_array(nonstatic_data_members_of(^^T, ctx)))
        result[identifier_of(field)] = obj.[:field:];

    return result;
}

template <class T>
T deserialize(const nlohmann::json& j)
{
    constexpr auto ctx = std::meta::access_context::unchecked();
    T result;

    template for (constexpr auto& field : define_static_array(nonstatic_data_members_of(^^T, ctx)))
    {
        try
        {
            result.[:field:] = j[identifier_of(field)].template get<typename[:type_of(field):]>();
        }
        catch (...)
        {
            std::cerr << "Error deSerializing field " << identifier_of(field) << std::endl;
        }
    }

    return result;
}

} // namespace serializer