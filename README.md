# BigInt

A simple BigInt library for C++. Note that, at least for now, the emphasis is on ease of use, not speed or performance.

## Usage

```
#include <iostream>

#include "BigInt.h"

int main()
{
	BigInt big1("10000000000"); // Values can be arbitrarily large
	BigInt big2("30000000000");

	BigInt big3 = big1 + big2;

	std::cout << big3 << std::endl;
}
```

Compiling and running this example displays `40000000000`.
