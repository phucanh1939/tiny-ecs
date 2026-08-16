# Changelog

All notable changes to TinyECS are documented here.

## [0.1.0] - 2026-08-16

### Added

* Entity creation and destruction with reusable entity IDs and version validation.
* Entity location tracking for archetype migration.
* Archetype-based entity storage.
* Chunk-based component storage with fixed-size chunks.
* Component signatures using a 256-bit component mask.
* Automatic archetype creation and lookup.
* Component registration and type identification.
* Add, get, try-get, and remove component APIs.
* Entity creation with multiple initialized components in a single operation.
* Component and entity migration between archetypes.
* Swap-back entity removal.
* Query API for iterating over entities matching component signatures.
* Command buffer supporting:
  * Entity creation
  * Entity destruction
  * Component addition
  * Component removal
  * Deferred command playback
* Basic tests covering world, entity, component, archetype, and query behavior.
* Example demonstrating a small ECS-based game simulation.
* CMake-based library build.

### Architecture

* `World` owns ECS state and archetypes.
* `Archetype` groups entities with the same component signature.
* `Chunk` provides contiguous component storage.
* `Query` provides iteration over matching archetypes.
* `CommandBuffer` defers structural changes until playback.
* Entity versions prevent stale entity handles from becoming valid after ID reuse.
