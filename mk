#!/usr/bin/env bash
#
# 2025-03-22 16:21:10 dpw
#

set -eu

export project=tiny-app

export verbose=''
export os="$(uname -s)"
export arch="$(uname -m)"

[ $os == "Linux" ] && {
    # export CC=/usr/bin/clang
    # export CXX=/usr/bin/clang++
    export CC=/usr/local/bin/gcc
    export CXX=/usr/local/bin/g++
    export FLAGS="-j8"
} || {
    export FLAGS="-j20"
}

# parse the cli
while [[ $# -gt 0 ]]
do
    case $1 in
        verbose)
            verbose='VERBOSE=1'

            shift
        ;;
        init)
            [ -d build ] || mkdir build
            (cd build && cmake ..)

            shift
        ;;
        build)
            clear

            (cd build && time make ${verbose} $FLAGS || exit 1)

            shift
        ;;
        unit|test)
            # (cd build && time make $FLAGS)
            ./build/unit_tests

            shift
        ;;
        run)
            ./build/$project

            shift
        ;;
        format)
            clang-format -i include/app/*.hpp src/*.cpp
            git status -s

            shift
        ;;
        clean)
            (cd build && make clean && /bin/rm -f datalogger-unit)
        
            shift
        ;;
        clob*)
            /bin/rm -fr build/

            shift
        ;;
        watch)
            watchexec -c -w src/ -w include/ -w tests/ -e h,hpp,cpp ./mk build unit

            exit 0
        ;;
        pull)
            git pull

            shift
        ;;

        help)
            echo "Targets:"
            echo ""
            echo "   init     : run the cmake command to create the build folder"
            echo "   build    : compile cryptor to the build folder"
            echo "   run      : runs the web server app"
            echo "   format   : runs clang-format over includes and src"
            echo "   watch    : run watcher over source and include"
            echo "   pull     : pull the latest repo changes"
            echo "   clean    : remove binary builds but leave the build folder"
            echo "   clobber  : remove the entire build folder"
            echo "   verbose  : show a verbose build"
            echo "   help     : show this help"
            
            exit 0
        ;;
        *)
            ./mk help
            exit 0
        ;;

    esac
done


