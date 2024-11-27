#! /bin/bash

set -eou pipefail

CMAKE_CURRENT_SOURCE_DIR="$1"
CMAKE_CURRENT_BINARY_DIR="$2"

xxd --name html_data -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.html | sed 's/^unsigned/static const/' >  ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
xxd --name style_data -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.css | sed 's/^unsigned/static const/' >> ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
xxd --name script_data -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.js | sed 's/^unsigned/static const/' >> ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
