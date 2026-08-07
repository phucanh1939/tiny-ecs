#include <tinyecs/ComponentSignature.h>

namespace tinyecs
{
    void ComponentSignature::add(ComponentType componentType)
    {
        _bits.set(componentType);
    }

    void ComponentSignature::remove(ComponentType componentType)
    {
        _bits.reset(componentType);
    }

    bool ComponentSignature::contains(ComponentType componentType) const
    {
        return _bits.test(componentType);
    }

    bool ComponentSignature::empty() const
    {
        return _bits.none();
    }

    bool ComponentSignature::operator==(const ComponentSignature &other) const
    {
        return _bits == other._bits;
    }

    const std::bitset<MaxComponentTypes> &ComponentSignature::bits() const
    {
        return _bits;
    }
}