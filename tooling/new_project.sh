#!/bin/bash

print_usage() {
    echo "usage: bash new_project.sh [-t] <language> <project_name>"
}

generate_directory() {
    # first argument is directory path
    # second argument is type of directory
    printf "generating %s directory: %s\n" $1 $2
    mkdir $2
}

replace_in_file() {
    # first argument is string to replace
    # second argument is string to replace with
    # third arugment is file to replace in
    sed -i "s/$1/$2/" $3
}

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

printf "Creating new \"%s\" project: \"%s\"\n\n" $lang $name

rm -rf $name

mkdir $name

case $lang in
    c | cpp)

        generate_directory src $name/src

        generate_directory include $name/include

        printf "generating CMakeLists.txt: %s\n" $name/CMakeLists.txt
        cp tooling/templates/cmakelists_main.txt $name/CMakeLists.txt
        replace_in_file "--name--" $name $name/CMakeLists.txt
        replace_in_file "--lang--" $lang $name/CMakeLists.txt

        if [ $testing == 1 ];
        then
            generate_directory test $name/test
            mkdir $name/test/src
            mkdir $name/test/include

            echo "adding tests to CMakeLists.txt"
            cat tooling/templates/cmakelists_test.txt >> $name/CMakeLists.txt
            replace_in_file "--name--" $name $name/CMakeLists.txt
            replace_in_file "--lang--" $lang $name/CMakeLists.txt
        else
            echo "skipping test generation"
        fi
        ;;
        
esac
