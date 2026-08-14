// include/tinyecs/MemoryUtils.h
#pragma once

#include <cstddef>

namespace tinyecs
{
    constexpr std::size_t alignUp(std::size_t value, std::size_t alignment)
    {
        return (value + alignment - 1) / alignment * alignment;
    }
}