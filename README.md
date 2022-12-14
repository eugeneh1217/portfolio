# Portfolio

This monorepo is Eugene Hong's Engineering Portfolio.

## Projects

### Hashmap
Implementation of hashmap in C supporting insertion access by key, deletion,
clearing all elements, and load balancing.

Demo displays that insertion , access by key, and deletion are O(1) time complexity.


### Demos
Project containing explanations and demonstrations of technology features that I struggled to find find satisfactory explanations for online. See project "README.md" for more details.

### Tooling
Custom monorepo tooling written in bash and tested with Python's builtin `unittest` module.

Currently, tooling contains:
    - `new_project.sh` script: generates projects with or without tests in the following languages:
        - c
        - cpp
    - `test.sh`: locates project tests based on project set up and runs them for github workflows.

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
