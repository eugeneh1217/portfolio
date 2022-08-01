#!/bin/bash

# Function definitions

print_usage() {
    echo "usage: bash new_project.sh [-t] <language> <project_name>"
}

generate_directory() {
    # create directory with verbosity

    # first argument is directory path
    # second argument is type of directory
    printf "generating %s directory: %s\n" $1 $2
    mkdir $2
}

replace_in_file() {
    # replace string in text file

    # first argument is string to replace
    # second argument is string to replace with
    # third arugment is file to replace in
    sed -i "s/$1/$2/" $3
}

fill_cmakelist_template() {
    # fill name and language in to cmakelist template
    # name placeholder is "--name--"
    # language placehold is "--lang--"

    # first argument is project name
    # second argument is language
    # third argument is path of template to fill

    replace_in_file "--name--" $1 $3
    replace_in_file "--lang--" $2 $3
}

# Flag parsing

testing=0

while getopts htn:l: flag
do
    case "${flag}" in
        l)
            lang="$OPTARG"
            if [ $lang == "c++" ];
            then
                lang="cpp"
            fi
            ;;
        n)
            name="$OPTARG"
            ;;
        t)
            testing=1
            ;;
        h)
            print_usage
            exit 0
            ;;
        ?)
            print_usage
            exit 1
            ;;
    esac
done

# Constants

TEMPLATE_PATH=tooling/templates
CMAKELISTS_PATH=$name/CMakeLists.txt

# Project generation

printf "Creating new \"%s\" project: \"%s\"\n\n" $lang $name

rm -rf $name
mkdir $name

case $lang in
    c | cpp)

        generate_directory src $name/src
        > $name/src/main.$lang

        generate_directory include $name/include

        printf "generating CMakeLists.txt: %s\n" $CMAKELISTS_PATH
        cp $TEMPLATE_PATH/cmakelists_main.txt $CMAKELISTS_PATH
        fill_cmakelist_template $name $lang $CMAKELISTS_PATH

        if [ $testing == 1 ];
        then
            generate_directory test $name/test
            mkdir $name/test/src
            mkdir $name/test/include

            echo "adding tests to CMakeLists.txt"
            cat $TEMPLATE_PATH/cmakelists_test.txt >> $CMAKELISTS_PATH
            fill_cmakelist_template $name $lang $CMAKELISTS_PATH
        else
            echo "skipping test generation"
        fi
        ;;
        
esac
