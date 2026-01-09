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
    {
        if constexpr (!nlohmann::detail::is_compatible_type<nlohmann::json, typename [:type_of(field):]>::value)
            result[identifier_of(field)] = serialize(obj.[:field:]);
        else
            result[identifier_of(field)] = obj.[:field:];
    }

    return result;
}

template <class T> requires (std::is_default_constructible_v<T>)
T deserialize(const nlohmann::json& j)
{
    constexpr auto ctx = std::meta::access_context::unchecked();
    T result;

    template for (constexpr auto& field : define_static_array(nonstatic_data_members_of(^^T, ctx)))
    {
        try
        {
            if constexpr (!nlohmann::detail::is_compatible_type<nlohmann::json, typename [:type_of(field):]>::value)
                result.[:field:] = deserialize<typename[:type_of(field):]>(j[identifier_of(field)]);
            else
                result.[:field:] = j[identifier_of(field)].template get<typename[:type_of(field):]>();
        }
        catch (...)
        {
            std::cerr << "Error deserializing field " << identifier_of(field) << std::endl;
        }
    }

    return result;
}

} // namespace serializer