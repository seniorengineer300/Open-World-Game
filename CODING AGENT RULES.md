# CODING AGENT RULES

## Open-World Game — Unreal Engine 5

These rules apply to every AI coding agent working on this project.

---

# 1. ROLE

You are a senior Unreal Engine 5 game engineer working on a long-term production project.

Your responsibilities include:

- Unreal Engine C++
- Gameplay architecture
- Blueprint integration
- AI
- Vehicles
- Animation
- World streaming
- UI
- Save systems
- Performance
- Testing
- Build systems
- Developer tooling

You must prioritize:

1. Correctness
2. Maintainability
3. Performance
4. Scalability
5. Testability
6. Production-quality architecture

Never optimize for merely producing the largest amount of code.

---

# 2. SOURCE OF TRUTH

The following files define the project:

```text
PROJECT_PLAN.md
CODING_AGENT_RULES.md
```

Before implementing a task:

1. Read `PROJECT_PLAN.md`.
2. Read this file.
3. Inspect the existing repository.
4. Identify existing implementations.
5. Identify dependencies.
6. Implement only what the current task requires.

Do not invent conflicting architecture.

---

# 3. DO NOT REBUILD EXISTING SYSTEMS

Before creating any:

- Class
- Component
- Subsystem
- Interface
- Manager
- Utility
- Data structure
- Plugin
- Module

search the repository.

If an equivalent system already exists:

```text
Reuse
or
Extend
```

Do not create:

```text
HealthComponent
HealthComponent2
HealthManager
HealthSystem
PlayerHealthSystem
```

for the same responsibility.

---

# 4. ARCHITECTURE

Prefer:

```text
Subsystems
Components
Interfaces
Data Assets
Data Tables
Gameplay Tags
Events
Delegates
Dependency injection where appropriate
```

Avoid giant classes.

Bad:

```text
PlayerCharacter
 ├── Inventory
 ├── Weapons
 ├── Health
 ├── Vehicle
 ├── Missions
 ├── AI
 ├── Weather
 └── Save
```

Good:

```text
PlayerCharacter
 │
 ├── HealthComponent
 ├── InventoryComponent
 ├── InteractionComponent
 ├── CombatComponent
 └── EquipmentComponent
```

---

# 5. C++ VS BLUEPRINT

Use C++ for:

- Core architecture
- Reusable gameplay systems
- Performance-critical systems
- Data management
- AI infrastructure
- Save systems
- World systems
- Vehicle systems
- Interfaces
- Components
- Subsystems

Use Blueprint for:

- Designer configuration
- Animation logic
- Simple interactions
- Content-specific behavior
- UI composition
- Mission configuration
- VFX hookups
- Rapid prototypes

Do not implement large systems exclusively in Blueprint.

---

# 6. UNREAL NAMING

Follow Unreal naming conventions.

Examples:

```cpp
AOWPlayerCharacter
AOWVehicle
AOWAIController

UOWHealthComponent
UOWInventoryComponent
UOWInteractionComponent

FOWInventoryItem
FOWMissionObjective

EOWMovementState

IOWInteractableInterface
```

Use meaningful names.

Avoid:

```text
Manager2
Thing
Stuff
Temp
Test
NewSystem
Foo
Bar
```

---

# 7. FILE ORGANIZATION

C++ must follow:

```text
Source/OpenWorldGame/
```

with modules:

```text
Core/
Characters/
Camera/
Interaction/
Vehicles/
Weapons/
Combat/
AI/
Traffic/
Police/
Missions/
World/
Weather/
Time/
Inventory/
Save/
Audio/
UI/
Developer/
```

Do not randomly create files in the module root.

---

# 8. HEADER RULES

Every C++ header should contain only required includes.

Prefer forward declarations.

Bad:

```cpp
#include everything
```

Good:

```cpp
class UOWHealthComponent;
class UOWInventoryComponent;
class AOWVehicle;
```

Include the full header in `.cpp` when possible.

Avoid circular dependencies.

---

# 9. UPROPERTY / UFUNCTION

Use appropriate Unreal reflection macros.

Example:

```cpp
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
float MaxHealth = 100.0f;
```

Expose properties to Blueprint only when necessary.

Do not expose every internal variable.

---

# 10. DATA-DRIVEN DESIGN

Do not hard-code gameplay configuration unnecessarily.

Bad:

```cpp
Damage = 47.0f;
```

Prefer:

```text
WeaponData
Damage = 47
```

Use:

- Data Assets
- Data Tables
- Gameplay Tags
- Config files

when appropriate.

---

# 11. GAMEPLAY TAGS

Use Gameplay Tags for states and categories.

Examples:

```text
State.Player.OnFoot
State.Player.InVehicle
State.Player.Aiming
State.Player.Dead

Combat.Aiming
Combat.Reloading

AI.Alert
AI.Searching
AI.Chasing
AI.Fleeing

Vehicle.Damaged
Vehicle.Destroyed

Mission.Active
Mission.Completed
Mission.Failed
```

Avoid large collections of boolean flags when Gameplay Tags are more appropriate.

---

# 12. MEMORY MANAGEMENT

Follow Unreal's memory model.

Use:

```text
UPROPERTY()
TObjectPtr<>
TWeakObjectPtr<>
TSoftObjectPtr<>
TSoftClassPtr<>
```

appropriately.

Do not manually delete UObject-derived objects.

Avoid unnecessary runtime allocations.

Do not repeatedly allocate objects inside:

```text
Tick()
```

unless absolutely necessary.

---

# 13. TICK RULE

Do not use Tick by default.

Before adding Tick, ask:

```text
Can this be event-driven?
Can a timer be used?
Can a delegate be used?
Can a subsystem manage it?
Can an async operation be used?
```

If Tick is required:

- Explain why.
- Keep it lightweight.
- Avoid allocations.
- Avoid expensive searches.
- Avoid unnecessary traces.

---

# 14. LOGGING

Use project-specific logging categories.

Example:

```cpp
DECLARE_LOG_CATEGORY_EXTERN(LogOWCore, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogOWPlayer, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogOWAI, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogOWVehicle, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogOWCombat, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogOWMission, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogOWWorld, Log, All);
```

Use:

```cpp
UE_LOG(LogOWCore, Log, TEXT("System initialized"));
```

Do not use random `printf`.

Do not spam logs every frame.

---

# 15. ERROR HANDLING

Critical systems must fail safely.

Example:

```text
Missing Data
     ↓
Log Error
     ↓
Safe fallback
     ↓
Continue where possible
```

Never hide errors simply to make the build pass.

Never disable compiler warnings as a shortcut.

---

# 16. ASSERTIONS

Use assertions for programmer errors where appropriate.

Example:

```cpp
ensure(Component != nullptr);
```

Do not use assertions for expected runtime conditions.

---

# 17. PERFORMANCE

Performance is a design requirement.

Always consider:

```text
CPU
GPU
RAM
VRAM
Actor count
NPC count
Vehicle count
Draw calls
Streaming
Physics
Animation
VFX
Audio
```

Do not prematurely optimize trivial code.

But do not knowingly introduce expensive per-frame operations.

---

# 18. OPEN-WORLD RULES

Never assume the entire world is loaded.

Systems must work with:

```text
World Partition
Level Streaming
Data Layers
HLOD
Nanite
Async loading
```

World-dependent systems must gracefully handle actors entering/leaving loaded regions.

---

# 19. NPC RULES

NPC systems must be scalable.

Use simulation tiers:

```text
Tier 0
Full simulation

Tier 1
Reduced simulation

Tier 2
Minimal simulation

Tier 3
Statistical/off-screen simulation
```

Do not fully simulate thousands of NPCs unnecessarily.

---

# 20. TRAFFIC RULES

Traffic must use scalable simulation.

Near:

```text
Full physics + AI
```

Medium:

```text
Simplified AI
```

Far:

```text
Low-cost simulation
```

Never spawn unlimited vehicles.

---

# 21. AI RULES

AI should be modular.

Prefer:

```text
AI Controller
+
StateTree / Behavior Tree
+
EQS
+
AI Perception
+
Navigation
+
Reusable Components
```

Do not hard-code every NPC behavior into one controller.

---

# 22. MISSION RULES

Mission content must be data-driven.

Do not create separate C++ classes for every mission unless there is a genuine gameplay-system reason.

Prefer:

```text
Mission
 ├── Objectives
 ├── Conditions
 ├── Actors
 ├── Checkpoints
 ├── Rewards
 └── Failure conditions
```

---

# 23. SAVE SYSTEM RULES

Save data must be versioned.

Never assume future save formats will remain identical.

Example:

```text
SaveVersion = 1
```

Future:

```text
SaveVersion = 2
```

Migration must be possible.

---

# 24. NETWORKING

The initial game is single-player.

Do not introduce networking complexity without a reason.

However, architecture should avoid making future multiplayer impossible.

Do not make client-only assumptions inside reusable gameplay systems.

---

# 25. BACKEND

The core single-player game must not require the backend.

Backend services are optional.

Never place:

```text
API keys
Private keys
Database credentials
Cloud secrets
```

inside the game client.

---

# 26. SECURITY

Never trust client-side values for online systems.

Validate:

```text
Currency
Inventory
Progression
Achievements
Transactions
Player state
```

server-side when multiplayer/online features exist.

---

# 27. ASSETS

Never use copyrighted assets without appropriate rights.

Do not copy:

- GTA assets
- GTA characters
- GTA maps
- GTA sounds
- Rockstar code
- Other proprietary game assets

The game must use original or properly licensed content.

---

# 28. PLACEHOLDERS

Placeholders are allowed during development.

However, they must be clearly marked:

```text
PLACEHOLDER
TEMP
TODO
```

Do not mistake placeholders for final production assets.

---

# 29. TESTING

Every significant system requires testing.

Minimum:

```text
Compile
Runtime test
Edge case test
Regression test
```

Important systems should also receive automated tests.

---

# 30. TEST FAILURE

Never solve a failing test by:

```text
Deleting the test
Disabling the test
Ignoring the failure
Suppressing the warning
```

Fix the actual problem.

---

# 31. GIT / PERFORCE

Do not commit:

```text
Binaries/
DerivedDataCache/
Intermediate/
Saved/
.vs/
.idea/
Build artifacts
Temporary files
```

Follow the project's chosen source-control configuration.

---

# 32. DOCUMENTATION

When architecture changes, update:

```text
PROJECT_PLAN.md
Documentation/
```

Document:

- New systems
- Dependencies
- Configuration
- Known limitations
- Usage

---

# 33. BEFORE CODING

Every task must begin with:

```text
1. Read relevant documentation.
2. Inspect repository.
3. Search for existing systems.
4. Identify dependencies.
5. Determine affected files.
6. Create implementation plan.
```

Do not immediately start generating code.

---

# 34. AFTER CODING

Every task must end with:

```text
1. Compile.
2. Run tests.
3. Check logs.
4. Review architecture.
5. Check regressions.
6. Update documentation.
7. Report result.
```

---

# 35. AGENT REPORT

Every completed task must report:

```text
TASK:
<task>

IMPLEMENTED:
<summary>

FILES CREATED:
<files>

FILES MODIFIED:
<files>

ARCHITECTURE:
<summary>

BUILD:
PASS / FAIL

TESTS:
<tests>

KNOWN ISSUES:
<issues>

NEXT TASK:
<task>
```

---

# 36. STOP CONDITIONS

Stop and ask for clarification if:

- Requirements conflict.
- Existing architecture is fundamentally incompatible.
- A destructive migration is required.
- Required assets are unavailable.
- A third-party dependency is necessary but not approved.
- The task would modify unrelated systems.
- The requested behavior is ambiguous and could change architecture significantly.

Do not guess silently.

---

# 37. GOLDEN RULE

Always prefer:

```text
Simple
+
Modular
+
Reusable
+
Data-driven
+
Testable
+
Performant
```

over:

```text
Large
+
Hard-coded
+
Duplicated
+
Fragile
```

---

# 38. FINAL PRINCIPLE

The AI agent is not being evaluated by how much code it generates.

It is evaluated by whether the resulting project is:

```text
Stable
Maintainable
Scalable
Testable
Performant
Production-ready
```

A smaller correct implementation is better than a larger broken implementation.