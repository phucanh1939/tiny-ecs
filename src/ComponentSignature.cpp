#include <cassert>

#include <tinyecs/ComponentSignature.h>

namespace tinyecs
{
    ComponentSignature::Iterator::Iterator(const std::bitset<MaxComponentTypes> &bits, ComponentType current)
        : _bits(bits), _current(current)
    {
        assert(false && "Not implemented");
    }

    ComponentType ComponentSignature::Iterator::operator*() const
    {
        assert(false && "Not implemented");
        return 0;
    }

    ComponentSignature::Iterator &ComponentSignature::Iterator::operator++()
    {
        assert(false && "Not implemented");
        return *this;
    }

    bool ComponentSignature::Iterator::operator==(const Iterator &other) const
    {
        assert(false && "Not implemented");
        return false;
    }

    bool ComponentSignature::Iterator::operator!=(const Iterator &other) const
    {
        assert(false && "Not implemented");
        return false;
    }

    void ComponentSignature::Iterator::skipToNextSetBit()
    {
        assert(false && "Not implemented");
    }

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

    bool ComponentSignature::operator!=(const ComponentSignature &other) const
    {
        return _bits != other._bits;
    }

    ComponentSignature::Iterator ComponentSignature::begin() const
    {
        assert(false && "Not implemented");
        return Iterator(_bits, 0);
    }

    ComponentSignature::Iterator ComponentSignature::end() const
    {
        assert(false && "Not implemented");
        return Iterator(_bits, MaxComponentTypes);
    }

}