#pragma once

#include <functional>
#include <string>

#include <tinyecs/ComponentSignature.h>

namespace std
{
    template<>
    struct hash<tinyecs::ComponentSignature>
    {
        std::size_t operator()(const tinyecs::ComponentSignature& signature) const
        {
            return std::hash<std::string>{}(signature.bits().to_string());
        }
    };
}