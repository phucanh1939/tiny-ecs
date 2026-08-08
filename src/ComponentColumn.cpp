#include <tinyecs/ComponentColumn.h>

namespace tinyecs
{
    ComponentColumn::ComponentColumn(ComponentInfo info)
    {
    }

    ComponentType ComponentColumn::type() const
    {
        return 0;
    }

    std::size_t ComponentColumn::count() const
    {
        return 0;
    }

    void* ComponentColumn::get(std::uint32_t index)
    {
        return nullptr;
    }

    const void* ComponentColumn::get(std::uint32_t index) const
    {
        return nullptr;
    }

    std::uint32_t ComponentColumn::add()
    {
        return 0;
    }

    void ComponentColumn::remove(std::uint32_t index)
    {
    }
}