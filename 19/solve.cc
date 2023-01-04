#include "../aoc.h"
#include <set>
#include <unistd.h>
#include <iostream>
#include <cassert>
#include "solve.h"

struct Runner {
   const Blueprint &robotCosts;
   MaterialMap robots;
   MaterialMap maxMaterials;
   int bestRun;
   void tick(int, MaterialMap &, Material newRobot);
   MaterialMap accumulateMaterials(const MaterialMap &);
};

MaterialMap Runner::accumulateMaterials(const MaterialMap &orig) {
   // Now run the current robots to generate more material.
   MaterialMap updated{};
   for (int i = 0; i < Material::LAST; ++i)
      updated[i] = robots[i] + orig[i];
   return updated;
}

void Runner::tick(int minutesLeft, MaterialMap &materials, Material newRobot) {

   // Work out a sequence of all the robots we can build with our current materials
   std::set<Material> affordable;

   auto newMaterials = accumulateMaterials(materials);

   if (minutesLeft == 0) {
      if (bestRun < newMaterials[GEODE])
         bestRun = newMaterials[GEODE];
      return;
   }

   // If we can't do better than our current best, then bail
   int achievable = (robots[GEODE] + minutesLeft / 2 + 1) * minutesLeft;
   if (bestRun > achievable + newMaterials[GEODE])
      return;

   // can we afford to build a newRobot? If not, accumulate a day's stuff, and try again.
   auto &costs = robotCosts.at(newRobot);
   for (int material = 0; material < Material::LAST; ++material) {
      int cost = costs[material];
      if (materials[material] < cost) {
         tick(minutesLeft - 1, newMaterials, newRobot);
         return;
      }
   }

   robots[newRobot]++; // now use our new robot.

   // Build each possible robot.
   for (int i = 0; i < GEODE + 1; ++i) {
      if (robots[i] >= maxMaterials[i])
         // no point - we have enough of these that we can create enough of
         // that material in a day for any robot already.0
         continue;
      auto &costs = robotCosts.at(newRobot);
      MaterialMap afterCosts = newMaterials;
      for (int material = 0; material < Material::LAST; ++material)
         afterCosts[material] -= costs[material];
      tick(minutesLeft - 1, afterCosts, Material(i));
   }
   --robots[newRobot];
}

const State & parse() {
   static State state{};
   static bool parsed = false;
   if (!parsed) {
      extern FILE *yyin;
      yyin = stdin;
      yyparse(state);
   }
   return state;
}

int parts(std::ostream &os, int days, int maxbps, std::function<void(int, int)> accum) {
   const State &state = parse();
   int idx = 1;
   int total = 0;
   for (auto &bp : state.blueprints) {
      Runner runner { bp, {}, {}, 0 };
      for (int material = 0; material <= GEODE; material++) {
         // max amount of robots for material "i" that are useful.
         for (int robot = 0; robot <= GEODE; robot++) {
            auto &costs = runner.robotCosts.at(Material(robot));
            if (costs[material] > runner.maxMaterials[material])
               runner.maxMaterials[material] = costs[material];
         }
      }
      runner.maxMaterials[GEODE] = std::numeric_limits<int>::max();
      MaterialMap materials = bp.at(ORE); // start with materials to build the ORE robot we mention below.
      runner.tick(days, materials, ORE);
      accum(idx, runner.bestRun);
      if (idx++ == maxbps)
         break;
   }
   return total;
}

void part1(std::istream &is, std::ostream &os) {
   int total = 0;
   parts(os, 24, 10000, [&total] (int idx, int best) { total += idx * best; });
   std::cout << "part1: " << total << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   int prod = 1;
   parts(os, 32, 3, [&] (int idx, int best) { prod *= best; });
   std::cout << "part2: " << prod << "\n";
}
