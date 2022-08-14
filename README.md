# Portfolio

This monorepo is Eugene Hong's Engineering Portfolio.

## Projects

### Hashmap
Implementation of hashmap in C.
This Hashmap implementation is dynamically allocated and supports insertion, deletion, access by key, and load balancing.

How to grow hashmap -> deletion and reinsertion requires a way to differentiate valid pairs and unused pairs.
- Keeping track of keys on insertion and deletion requires set to remain amortized O(1).
- Adding a valid flag to pairs violates pair_t name -> pair_t should be renamed.
- Copying bucket array does not rehash elements of hashmap.

Solution: Keep valid bit in key (extra bit before key memory starts).



### Tooling
Custom monorepo tooling written in bash and tested with Python's builtin `unittest` module.

Currently, tooling contains a `new_project.sh` script. This script supports project generation with or without tests in the following languages:
- c
- cpp

Tooling scripts should be called from repository root directory.

## Testing

### C/C++
Both C and C++ projects are tested with the [Gtest](https://github.com/google/googletest) testing framework.
Memory leaks are detected with [Valgrind](https://valgrind.org/).

Valgrind can be installed with apt
```bash
sudo apt install valgrind
```

Tests can be run after they are compiled and linked by running the executable with valgrind.
```bash
valgrind ./<executable>
```

For example, if the test executable is called `tests`, they can be executed with Valgrind with
```bash
valgrind ./tests
```

### Python
Python tests can be run from root with
```bash
python -m unittest discover <project_name>
```

For example, unit tests for the tooling project can be run with
```bash
python -m unittest discover tooling
```

## Python Linting
Python projects are linted with [Pylint](https://pypi.org/project/pylint/).

`.pylintrc` files are specified in the project's root directory if applicable. Project specific `.pylintrc` files can be used when calling pylint from root with
```bash
pylint <project> --rcfile <project>/.pylintrc
```

For example, the tooling project can be linted with
```bash
pylint tooling --rcfile tooling/.pylintrc
```
