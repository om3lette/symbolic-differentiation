## How to add more tests

1. Create a new file `tests/groups/*_tests.cpp` and paste the following template
2. Add an entry to `test_groups.hpp`
3. Call a new function in `tests.cpp`

## The template

```cpp
#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_x(void) {
test::Tester tester_x("");
return tester_x.run_tests();
}
} // namespace test
```
