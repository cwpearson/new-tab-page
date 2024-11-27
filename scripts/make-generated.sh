#! /bin/bash

set -eou pipefail

CMAKE_CURRENT_SOURCE_DIR="$1"
CMAKE_CURRENT_BINARY_DIR="$2"

version_string=$(xxd -v)

if [[ $version_string =~ ([0-9]{4})-([0-9]{2})-([0-9]{2}) ]]; then
    year="${BASH_REMATCH[1]}"
    month="${BASH_REMATCH[2]}"
    day="${BASH_REMATCH[3]}"
    
    version_num=$((year * 10000 + month * 100 + day))
    compare_version=20230000  # 2023-00-00
    
    if [ "$version_num" -ge "$compare_version" ]; then
        xxd --name html_data -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.html | sed 's/^unsigned/static const/' >  ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
        xxd --name style_data -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.css | sed 's/^unsigned/static const/' >> ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
        xxd --name script_data -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.js | sed 's/^unsigned/static const/' >> ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
        exit 0
    else
        xxd -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.html | sed 's/^unsigned/static const/' >  ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
        xxd -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.css | sed 's/^unsigned/static const/' >> ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
        xxd -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.js | sed 's/^unsigned/static const/' >> ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
    fi
fi

echo "Invalid version format: $version_string"
exit 1
