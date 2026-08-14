# Memory Alignment

When storing components inside a raw memory block, each component must start at a memory address that satisfies its **alignment requirement**.

### `sizeof` vs `alignof`

```cpp
sizeof(T)
```

= how many bytes one object occupies.

```cpp
alignof(T)
```

= what alignment the object's starting address must satisfy.

Example:

```cpp
struct Position
{
    float x;
    float y;
};
```

Typically:

```text
sizeof(Position)  = 8
alignof(Position) = 4
```

So each `Position` occupies 8 bytes, and its address should be aligned to a 4-byte boundary.

---

### Why alignment exists

Alignment is ultimately related to **CPU hardware memory access**.

Some CPUs require aligned accesses, while others allow unaligned accesses but may handle them less efficiently.

C++ therefore defines alignment requirements for types. Placing an object at an address that doesn't satisfy its required alignment can result in **undefined behavior**.

---

### Why ECS needs it

Our ECS chunk is one raw memory block:

```text
┌──────────────────────────┐
│ Entity[]                 │
├──────────────────────────┤
│ padding                  │
├──────────────────────────┤
│ Position[]               │
├──────────────────────────┤
│ padding                  │
├──────────────────────────┤
│ Velocity[]               │
└──────────────────────────┘
```

The padding ensures every column starts at a correctly aligned address.

For example:

```text
Position alignment = 4
Velocity alignment = 8
```

We cannot simply place `Velocity` immediately after `Position`. We may need padding so that:

```text
Velocity start address % 8 == 0
```

---

### Layout metadata

Therefore a component column can store:

```cpp
struct ColumnLayout
{
    ComponentType type;
    std::size_t offset;
    std::size_t elementSize;
    std::size_t alignment;
};
```

Where:

```text
type          → which component
offset        → where its array starts in the chunk
elementSize   → sizeof(component)
alignment     → alignof(component)
```

Then the address of component `i` is conceptually:

```text
address = chunkBase + columnOffset + i × elementSize
```

The layout calculation is responsible for making sure `columnOffset` is properly aligned.