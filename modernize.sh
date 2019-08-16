#!/bin/sh
set -e

#XXX FIX= # uncoment to check only!

RULES=`clang-tidy --list-checks -checks='-*,modernize-use-*' | grep -vw Enabled`

# $ clang-tidy --list-checks -checks='-*,modernize-*'
# Enabled checks:
#   modernize-avoid-bind
#   modernize-deprecated-headers
#   modernize-loop-convert
#   modernize-make-shared
#   modernize-make-unique
#   modernize-pass-by-value
#   modernize-raw-string-literal
#   modernize-redundant-void-arg
#   modernize-replace-auto-ptr
#   modernize-replace-random-shuffle
#   modernize-return-braced-init-list
#   modernize-shrink-to-fit
#   modernize-unary-static-assert
#   modernize-use-auto
#   modernize-use-bool-literals
#   modernize-use-default-member-init
#   modernize-use-emplace
#   modernize-use-equals-default
#   modernize-use-equals-delete
#   modernize-use-noexcept
#   modernize-use-nullptr
#   modernize-use-override
#   modernize-use-transparent-functors
#   modernize-use-uncaught-exceptions
#   modernize-use-using

: ${RULES:="
modernize-use-auto
modernize-loop-convert
modernize-pass-by-value
modernize-redundant-void-arg
modernize-deprecated-headers
"}

: ${FIX:="-fix"}

set -u
set -x

for rule in ${RULES}; do
    run-clang-tidy.py -header-filter='.*' -checks='-*,'${rule} -j 2 ${FIX}
    make
    #XXX make test
    ## git commit -a
done
