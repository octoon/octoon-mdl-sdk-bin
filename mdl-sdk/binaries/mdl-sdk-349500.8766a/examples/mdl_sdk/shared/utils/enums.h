/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

 // examples/mdl_sdk/shared/utils/enum_flags.h
 //
 // Code shared by all examples

#ifndef EXAMPLE_SHARED_UTILS_ENUMS_H
#define EXAMPLE_SHARED_UTILS_ENUMS_H

#include <type_traits>
#include <stdint.h>

namespace mi { namespace examples { namespace enums
{
    template<typename TEnum>
    using is_scoped_enum =
        std::integral_constant<bool, std::is_enum<TEnum>::value &&
        !std::is_convertible<TEnum, size_t>::value>;

    // Cast an enum (mask) to the underlying integer type
    template<class TEnum, typename = typename std::enable_if<is_scoped_enum<TEnum>::value>::type>
    constexpr typename std::underlying_type<TEnum>::type to_integer(TEnum e) noexcept {
        return static_cast<typename std::underlying_type<TEnum>::type>(e);
    }

    // Cast from the underlying integer type to an enum (mask).
    template<class TEnum, typename = typename std::enable_if<is_scoped_enum<TEnum>::value>::type>
    constexpr TEnum from_integer(typename std::underlying_type<TEnum>::type v) noexcept {
        return static_cast<TEnum>(v);
    }

    // Check if an enum bit-mask has a certain bit set.
    template<class TEnum, typename = typename std::enable_if<is_scoped_enum<TEnum>::value>::type>
    inline bool has_flag(const TEnum mask, const TEnum flag_to_check) {
        return (to_integer(mask) & to_integer(flag_to_check)) > 0;
    }

    // Set a certain bit in an enum bit-mask.
    template<class TEnum, typename = typename std::enable_if<is_scoped_enum<TEnum>::value>::type>
    inline TEnum set_flag(const TEnum& mask, const TEnum flag_to_add) {
        return static_cast<TEnum>(to_integer(mask) | to_integer(flag_to_add));
    }

    // Remove a certain bit from an enum bit-mask.
    template<class TEnum, typename = typename std::enable_if<is_scoped_enum<TEnum>::value>::type>
    inline TEnum remove_flag(const TEnum& mask, const TEnum flag_to_add) {
        return static_cast<TEnum>(to_integer(mask) & ~to_integer(flag_to_add));
    }

    // Toggle a certain bit from an enum bit-mask.
    template<class TEnum, typename = typename std::enable_if<is_scoped_enum<TEnum>::value>::type>
    inline TEnum toggle_flag(const TEnum& mask, const TEnum flag_to_add) {
        return static_cast<TEnum>(to_integer(mask) ^ to_integer(flag_to_add));
    }

}}}
#endif
