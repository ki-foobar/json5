# JSON5 parser

It is developed and maintained for [this project](https://github.com/elonafoobar/elonafoobar) and not for general purpose.



**WARNING**: it is *NOT* production ready!



## Examples

```cpp
#include <iostream>
#include "json5/json5.hpp"



int main()
{
    const auto features = json5::parse(R"(
{
  JSON5: "supports naked key like ECMAScript 5",
  'You can also use single quotes': 'like this.',
  // Furthermore, you can write single line comment
  /*
   * and multi-line ones!
   */
  Special_numeric_literal: Infinity,
  and: NaN,
  are: "also supported.",
  "Other decimal notations": [.0, 42., +28],
  // Trailing comma is allowed in JSON5 like the above line!
}
)");

    std::cout << json5::stringify(features) << std::endl;
}
```



## License

MIT

See [LICENSE](LICENSE) for details.
