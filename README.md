# SlowLib
Slow library for slow gamedev

# Some things about library

* No inline functions (if need, make inline version with suffix '_inl')
* Library will be a collection of static libraries.

# Coding

* All functions, struct and other types must have names with letters 'sl' in the beginning.
* Do not use `namespace` in API. You can use it for internal things.
* Do not use STL in API. You can use it for internal things.
* Prefer C-style programming (c arrays, c strings)
* Comment everything
* Use code-guard and `#pragma once`. 
* You can use `goto`, just don't jump back, and don't use it in large function.
* Use char* strings, not wchar* or other.