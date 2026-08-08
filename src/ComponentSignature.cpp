#include <cassert>

#include <tinyecs/ComponentSignature.h>

namespace tinyecs
{
    ComponentSignature::Iterator::Iterator(const std::bitset<MaxComponentTypes> &bits, ComponentType current)
        : _bits(bits), _current(current)
    {
    }

    ComponentType ComponentSignature::Iterator::operator*() const
    {
        return _current;
    }

    ComponentSignature::Iterator &ComponentSignature::Iterator::operator++()
    {
        ++_current;

        // Find the index of the next set bit in the bitmask 
        while (_current < MaxComponentTypes && !_bits[_current])
        {
            ++_current;
        }

        return *this;
    }

    bool ComponentSignature::Iterator::operator==(const Iterator &other) const
    {
        // 2 iterator consider equals when they point to the same bit of the same signature
        return &_bits == &other._bits && _current == other._current;
    }

    bool ComponentSignature::Iterator::operator!=(const Iterator &other) const
    {
        // reuse the == operator
        return !(*this == other);
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
        ComponentType current = 0;

        // Find the index of the first set bit in the mask as the begin iterator
        while (current < MaxComponentTypes && !_bits[current])
        {
            ++current;
        }

        return Iterator(_bits, current);
    }

    ComponentSignature::Iterator ComponentSignature::end() const
    {
        // Iterator ++ operator implement allow the _current to reach MaxComponentTypes after the last set bit
        // So end() iterator at the MaxComponentTypes will matched that _current iterator
        return Iterator(_bits, MaxComponentTypes);
    }

}