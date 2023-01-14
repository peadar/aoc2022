#include "../aoc.h"
#include <unistd.h>
#include <set>
#include <vector>
#include <cassert>
#include <map>
using Num = long;

struct Item {
   Num value;
   Num pos;
};

void printit(std::ostream &os, const std::vector<Item> &items) {
   auto clone = items;
   std::sort(clone.begin(), clone.end(), [](const Item &l, const Item &r) { return l.pos < r.pos; } );
   for (size_t i = 0; i < clone.size(); ++i)
      os << clone[i].value << " ";
   os << "\n";
}

void
part1(std::istream &is, std::ostream &os) {
   std::vector<Item> items;
   Num v;
   while (is >> v)
      items.push_back({v, Num(items.size())});

   for (size_t idx = 0; idx < items.size(); ++idx) {
      auto &item = items[idx];
      if (item.value == 0)
         continue;
      Num oldpos = item.pos;
      Num newpos = (oldpos + item.value + items.size()) % items.size();
      for (auto &updateItem : items) {
         if (item.value > 0) {
            // going forwards
            if (newpos > oldpos) {
               if (updateItem.pos > oldpos && updateItem.pos <= newpos)
                  updateItem.pos = (updateItem.pos - 1) % items.size();
            } else {
               if (updateItem.pos > oldpos || updateItem.pos <= newpos)
                  updateItem.pos = (updateItem.pos + items.size() - 1) % items.size(); // this one might underflow - avoid -1.
            }
         } else {
            // going backwards
            if (newpos < oldpos) {
               if (updateItem.pos < oldpos && updateItem.pos >= newpos)
                  updateItem.pos = (updateItem.pos + 1) % items.size();
            } else {
               if (updateItem.pos < oldpos || updateItem.pos >= newpos)
                  updateItem.pos = (updateItem.pos + 1) % items.size();
            }
         }
      }
      item.pos = newpos;
   }
   printit(os, items);

   size_t zeropospos = items.size();
   for (zeropospos = 0; items[zeropospos].value != 0 ; ++zeropospos)
      assert(zeropospos < items.size());

   assert(items[zeropospos].value == 0);
   Num zeropos = items[zeropospos].pos;

   std::cout << "zero pos is " << zeropos << "\n";
   Num total = 0;

   std::map<Num, Num> targets;
   targets[(zeropos + 1000) % items.size()] = 1000;
   targets[(zeropos + 2000) % items.size()] = 2000;
   targets[(zeropos + 3000) % items.size()] = 3000;

   for (size_t i = 0; i < items.size(); ++i) {
      auto pos = targets.find(items[i].pos);
      if (pos != targets.end()) {
         std::cout << "item " << pos->second << " (" << items[i].pos << ") is " << items[i].value << "\n";
         total += items[i].value;
      }
   }
   std::cout << "part1: " << total << "\n";
}

void
part2(std::istream &is, std::ostream &os) {
}
