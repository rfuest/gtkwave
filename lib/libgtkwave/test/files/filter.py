#!/bin/env python

import sys

prefix = sys.argv[1] if len(sys.argv) == 2 else "filter"

while True:
    try:
        str = input()
        if str == "crash":
            exit(1)
    except EOFError:
        break

    print(f"{prefix}: '{str}'")
