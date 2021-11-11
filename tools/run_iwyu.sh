#!/bin/bash

set -u
set -e
set -o pipefail
set -x

jobs="${1:-1}"
tools_dir="$(dirname "$0")"
compile_db="$tools_dir/../build/compile_commands.json"

if [ ! -r "$compile_db" ]
then
    echo "Expected compilation database to exist at '$compile_db'" >&2
    exit 1
fi

for prog in iwyu_tool.py fix_includes.py run-clang-tidy.py
do
    if ! which $prog &>/dev/null
    then
        echo "You need '$prog' on your PATH" >&2
        exit 1
    fi
done

# Must use absolute paths to mapping file because IWYU changes
# directory.
mapping_file="$(realpath "$tools_dir/iwyu/cata.imp")"

function run_iwyu {
    pass="$1"
    iwyu_tool.py -j "$jobs" -p "$compile_db" -- \
        -Xiwyu --mapping_file="$mapping_file" | \
        tee fixes."$pass" | \
        ( fix_includes.py --nosafe_headers --reorder || true; )
}

clang_tidy_flags=
if [[ "$(uname -s)" == "Darwin" ]]; then
    clang_tidy_flags="-clang-tidy-binary $(brew --prefix llvm)/bin/clang-tidy -clang-apply-replacements-binary $(brew --prefix llvm)/bin/clang-apply-replacements"
fi

function run_clang_tidy {
    run-clang-tidy.py -j "$jobs" -p "$tools_dir/../build" -fix -checks='-*,modernize-deprecated-headers' $clang_tidy_flags
}

# First IWYU pass
run_iwyu 1
# Make sure we didn't break the build
make -j "$jobs"
# Clean up old-style headers that might have been added
run_clang_tidy
# Second IWYU pass
run_iwyu 2
# Tidy up
make astyle
# Make sure we didn't break the build
make -j "$jobs"
