#include "ComponentSignatureTest.h"

#include <cassert>
#include <iostream>

#include <tinyecs/ComponentSignature.h>

#include "TestUtils.h"

namespace tinyecs::test
{
    void ComponentSignatureTest::run()
    {
        testAdd();
        testRemove();
        testEmpty();
        testEquality();
        testIterator();
        testRangeBasedFor();
        testEmptyIterator();
    }

    void ComponentSignatureTest::testAdd()
    {
        ComponentSignature signature;

        assert(!signature.contains(1));

        signature.add(1);

        assert(signature.contains(1));

        printPassed("ComponentSignature::add");
    }

    void ComponentSignatureTest::testRemove()
    {
        ComponentSignature signature;

        signature.add(1);
        signature.remove(1);

        assert(!signature.contains(1));

        printPassed("ComponentSignature::remove");
    }

    void ComponentSignatureTest::testEmpty()
    {
        ComponentSignature signature;

        assert(signature.empty());

        signature.add(1);
        assert(!signature.empty());

        signature.remove(1);
        assert(signature.empty());

        printPassed("ComponentSignature::empty");
    }

    void ComponentSignatureTest::testEquality()
    {
        ComponentSignature a;
        ComponentSignature b;

        assert(a == b);

        a.add(1);
        assert(a != b);

        b.add(1);
        assert(a == b);

        b.add(2);
        assert(a != b);

        printPassed("ComponentSignature::operator==");
    }

    void ComponentSignatureTest::testIterator()
    {
        ComponentSignature signature;

        signature.add(1);
        signature.add(4);
        signature.add(7);

        auto it = signature.begin();
        auto end = signature.end();

        assert(it != end);
        assert(*it == 1);

        ++it;
        assert(*it == 4);

        ++it;
        assert(*it == 7);

        ++it;
        assert(it == end);

        printPassed("ComponentSignature::Iterator");
    }

    void ComponentSignatureTest::testRangeBasedFor()
    {
        ComponentSignature signature;

        signature.add(2);
        signature.add(5);
        signature.add(10);

        ComponentType expected[] = {2, 5, 10};
        int index = 0;

        for (ComponentType type : signature)
        {
            assert(type == expected[index]);
            ++index;
        }

        assert(index == 3);

        printPassed("ComponentSignature::range-based for");
    }

    void ComponentSignatureTest::testEmptyIterator()
    {
        ComponentSignature signature;

        assert(signature.begin() == signature.end());

        int count = 0;

        for (ComponentType type : signature)
        {
            (void)type; // tell compiler we dont use type
            ++count;
        }

        assert(count == 0);

        printPassed("ComponentSignature::empty iterator");
    }
}