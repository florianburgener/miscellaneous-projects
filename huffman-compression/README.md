Tests:
```sh
COMP_FILE="/tmp/compressed.bin"
DECOMP_FILE="/tmp/decompressed.txt"

# Case 1 (1KB file):
INPUT_FILE="./resources/1KB.txt"
rm -f $COMP_FILE $DECOMP_FILE && ./bin/compress $INPUT_FILE $COMP_FILE && ./bin/decompress $COMP_FILE $DECOMP_FILE && diff $INPUT_FILE $DECOMP_FILE

# Case 2 (100KB file):
INPUT_FILE="./resources/100KB.txt"
rm -f $COMP_FILE $DECOMP_FILE && ./bin/compress $INPUT_FILE $COMP_FILE && ./bin/decompress $COMP_FILE $DECOMP_FILE && diff $INPUT_FILE $DECOMP_FILE

# Case 3 (1MB file):
INPUT_FILE="./resources/1MB.txt"
rm -f $COMP_FILE $DECOMP_FILE && ./bin/compress $INPUT_FILE $COMP_FILE && ./bin/decompress $COMP_FILE $DECOMP_FILE && diff $INPUT_FILE $DECOMP_FILE

# Case 4 (multiple same char file):
INPUT_FILE="./resources/multiple_same_char.txt"
rm -f $COMP_FILE $DECOMP_FILE && ./bin/compress $INPUT_FILE $COMP_FILE && ./bin/decompress $COMP_FILE $DECOMP_FILE && diff $INPUT_FILE $DECOMP_FILE

# Case 5 (single char file):
INPUT_FILE="./resources/single_char.txt"
rm -f $COMP_FILE $DECOMP_FILE && ./bin/compress $INPUT_FILE $COMP_FILE && ./bin/decompress $COMP_FILE $DECOMP_FILE && diff $INPUT_FILE $DECOMP_FILE

# Case 6 (empty file):
INPUT_FILE="./resources/empty.txt"
rm -f $COMP_FILE $DECOMP_FILE && ./bin/compress $INPUT_FILE $COMP_FILE && ./bin/decompress $COMP_FILE $DECOMP_FILE && diff $INPUT_FILE $DECOMP_FILE
```
