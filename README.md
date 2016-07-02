# ArrayRef

ArrayRef is an array container that refers other one.

## Requirements

Need to compile with C++11 features.

## Install

Just copy `aref` to your project directory.

## Usage

```cpp
#include <iostream>
#include <numeric>
#include <vector>

#include <aref/array_ref.h>

int main() {
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 0);

    auto ref = v | aref::slice(3, 7);

    for (auto e : ref) {
        std::cout << e << std::endl;
    }
}
```

### Output

```bash
$ 3
$ 4
$ 5
$ 6
```

## License

The MIT License (MIT)
