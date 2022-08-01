# Portfolio

This monorepo is Eugene Hong's Engineering Portfolio.

## Projects
### Tooling
Custom monorepo tooling written in bash and tested with Python's builtin `unittest` module.

Currently, tooling contains a `new_project.sh` script. This script supports project generation with or without tests in the following languages:
- c
- cpp

Tooling scripts should be called from repository root directory.

## Testing

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
