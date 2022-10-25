# SlowLib
Slow library for slow gamedev

# Some things about library

* No inline functions (if need, make inline version with suffix '_inl')
* Library by default will be as a collection of static libraries, but it should have option to use it as dynamic link libraries.

# Coding

* All functions, struct and other types must have names with letters 'sl' in the beginning.
* Do not use `namespace` in API. You can use it for internal things.
* Do not use STL in API. You can use it for internal things.
* Prefer C-style programming (c arrays, c strings)
* Comment everything
* Use code-guard and #pragma once. 
