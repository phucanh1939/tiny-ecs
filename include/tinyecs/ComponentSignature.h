#pragma once

#include <bitset>
#include <cstdint>

#include <tinyecs/ComponentType.h>

namespace tinyecs
{
    class ComponentSignature
    {
    public:
        static constexpr std::size_t MaxComponentTypes = 256;

        class Iterator
        {
        public:
            Iterator(const std::bitset<MaxComponentTypes>& bits, ComponentType current);

            ComponentType operator*() const;

            Iterator& operator++();

            bool operator==(const Iterator& other) const;

            bool operator!=(const Iterator& other) const;

        private:
            const std::bitset<MaxComponentTypes>& _bits;

            ComponentType _current;
        };

        const std::bitset<MaxComponentTypes> &bits() const { return _bits; }

        void add(ComponentType type);

        void remove(ComponentType type);

        bool contains(ComponentType type) const;

        bool empty() const;

        bool operator==(const ComponentSignature& other) const;

        bool operator!=(const ComponentSignature& other) const;

        Iterator begin() const;

        Iterator end() const;

    private:
        std::bitset<MaxComponentTypes> _bits;
    };
}