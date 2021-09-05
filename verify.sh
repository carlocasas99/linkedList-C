#!/bin/bash
e=0; [[ $(sha512sum list.h | cut -d " " -f 1) == e473fcf179e1385c4c536d0073ca35dae0bf26b1e49e337de32c5c2b7c2da77dbf061ef489a6898a40be16a23ddcaa23b434007eed27ee0b77d4a4b9cec94e6f ]] || { printf 'error: list.h was modified! re-download it and try again\n'>&2; e=1; }; exit $e
