# 2024 I2P Mini Project 2 - Tower Defense

## Grading

| **Hackathon Tasks**              | **Score** | **Check** |
| :------------------------------- | :-------: | :-------: |
| Add `StartScene`                 |    5%     |     ✅    |
| Add `SettingsScene`              |    10%    |     ✅    |
| Fix Bugs in Scene                |    15%    |     ✅    |

| **Project Tasks**           | **Score** | **Check** |
| :-------------------------- | :-------: | :-------: |
| Add `ScoreboardScene`       |    40%    |     ✅    |
| Enemy Pathfinding           |    10%    |     ✅    |
| 2 New Turrets/Enemies/Tools |    20%    |     ✅    |

<!-- Please describe the new turret/enemy/tool you have implemented in the above table. -->

| **Bonus Functions**   | **Score** | **Check** |
| :-------------------- | :-------: | :-------: |
| Scoreboard Date Time  |    3%     |     ✅    |
| Record User Name      |    7%     |     ✅    |
| Other Bonus Functions |   ≤10%    |     ✅    |

<!-- Please describe the bonus optimizations or features you have implemented in the above table. -->

## TODO
- Add a Table component that accept a vector of components
  - auto distribute margin left between the columns and rows
  - highly abstract table like something in HTML parser
- abtract the logic of `Turret` and `Tool`
  - define `PlaceableObject.hpp/cpp` that means it is a "object" that can be used in `PlayScene`
  - use `virtual functions` to define common behaviour on both class
  - in `Tool.cpp` `Turret.cpp` define the virtual function in `PlaceableObject.hpp/cpp`
  - in other tools/turrets, keep overriding the virtual function
  - in `PlayScene`'s `OnMouseUp`, call preview's (which is a `PlaceableObject`) virtual function
