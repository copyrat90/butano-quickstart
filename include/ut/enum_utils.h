#pragma once

#include <bn_string_view.h>

#include <meta>
#include <string_view>
#include <type_traits>
#include <utility>

namespace bq::ut
{

template <typename Enum>
    requires std::is_enum_v<Enum>
constexpr int size_of_enum()
{
    return static_cast<int>(std::meta::enumerators_of(^^Enum).size());
}

template <typename Enum>
    requires std::is_enum_v<Enum>
constexpr auto enum_to_string(Enum value) -> bn::string_view
{
    bn::string_view result;
    template for (constexpr auto e : std::define_static_array(std::meta::enumerators_of(^^Enum)))
    {
        if (value == [:e:])
        {
            std::string_view sv = std::meta::identifier_of(e);
            result = bn::string_view(sv.data(), sv.size());
        }
    }
    return result;
}

template <typename ToEnum, typename FromEnum>
    requires std::is_enum_v<ToEnum> && std::is_enum_v<FromEnum>
constexpr auto enum_to_enum(FromEnum from) -> ToEnum
{
    template for (constexpr std::meta::info from_info : std::define_static_array(std::meta::enumerators_of(^^FromEnum)))
    {
        if (from == [:from_info:])
        {
            template for (constexpr std::meta::info to_info :
                          std::define_static_array(std::meta::enumerators_of(^^ToEnum)))
            {
                // Don't forget `constexpr` here, or you might get run-time string comparison in unoptimized build.
                if constexpr (std::meta::identifier_of(from_info) == std::meta::identifier_of(to_info))
                {
                    return [:to_info:];
                }
            }

            // Don't forget `break;` here, or there's no jump table even for optimized build.
            break;
        }
    }

    BN_ERROR("Invalid enum conversion: ", static_cast<int>(from));
    std::unreachable();
}

} // namespace bq::ut
