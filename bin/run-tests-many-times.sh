#!/usr/bin/env bash

set -o errexit
set -o pipefail
set -o nounset
# set -o xtrace

__dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
__root="$(cd "$(dirname "${__dir}")" && pwd)"

###

cd "${__root}"

count=500
for i in $(seq $count); do
    echo "Attempt $i ---------"
    rm -rf ./io-uring-soup-bin_test.log
    ./build/io-uring-soup-bin_test > ./io-uring-soup-bin_test.log
done
