Test:
```sh
rm -f /tmp/compressed.bin /tmp/output.txt && ./bin/compress ./resources/input.txt /tmp/compressed.bin && ./bin/decompress /tmp/compressed.bin /tmp/output.txt && diff ./resources/input.txt /tmp/output.txt
```
