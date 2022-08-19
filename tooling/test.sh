#!/bin/bash

set -e

for project in *; do

    if [ -d "$project" ]; then

        printf "testing \"%s\" project...\n" $project

        if [ -d "$project/src" ]; then # project identified as c/c++

            printf "language: c/c++\n"

            if [ -d "$project/test" ]; then

                # build project
                cd $project
                cmake -B build
                cd build
                make

                # run tests
                valgrind ./tests

                # back to root directory
                cd ../..

                printf "\n"
            fi
        elif [ -f "$project/__init__.py" ]; then # project identified as python

            printf "language: python\n"

            if [ -d "$project/test" ]; then

                pip install -r $project/requirements.txt

                python3 -m unittest discover $project

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
