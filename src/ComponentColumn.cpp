#include <cassert>
#include <cstring>

#include <tinyecs/ComponentColumn.h>

namespace tinyecs
{
    ComponentColumn::ComponentColumn(ComponentInfo info)
        : _info(info)
    {
    }

    ComponentType ComponentColumn::type() const
    {
        return _info.type;
    }

    std::size_t ComponentColumn::count() const
    {
        return _data.size() / _info.size;
    }

    void *ComponentColumn::get(std::uint32_t index)
    {
        assert(index < count());

        return _data.data() + index * _info.size;
    }

    const void *ComponentColumn::get(std::uint32_t index) const
    {
        assert(index < count());

        return _data.data() + index * _info.size;
    }

    std::uint32_t ComponentColumn::add()
    {
        const std::uint32_t index = static_cast<std::uint32_t>(count());

        _data.resize(_data.size() + _info.size);

        return index;
    }

    void ComponentColumn::remove(std::uint32_t index)
    {
        assert(index < count());

        const std::uint32_t lastIndex = static_cast<std::uint32_t>(count() - 1);

        if (index != lastIndex)
        {
            std::memcpy(get(index), get(lastIndex), _info.size);
        }

        _data.resize(_data.size() - _info.size);
    }
}