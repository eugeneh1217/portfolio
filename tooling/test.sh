#!/bin/bash

for project in *; do
    if [ -d "$project" ]; then
        printf "testing \"%s\" project...\n" $project
        if [ -d "$project/src" ]; then
            printf "language: c\n"
            if [ -d "$project/test" ]; then
                cd $project && cmake -B build
                cd build && make && valgrind ./tests
                cd ../..
                printf "\n"
            fi
        elif [ -f "$project/__init__.py" ]; then
            printf "language: python\n"
            if [ -d "$project/test" ]; then
                virtualenv .test
                source .test/bin/activate
                pip install -r $project/requirements.txt
                python -m unittest discover $project
                deactivate
                rm -rf .test
                printf "\n"
            else
                printf "no tests found\n\n"
            fi
        else
            printf "no tests found\n\n"
        fi
    fi
done
echo -e  "\e[1;32mAll Tests Passed\e[0m"
