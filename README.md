# ARMThumbDisassembler

Building the Project
make

Usage
./disassembler <input_file> [options]

Options
-o <output_file>: Write output to file instead of stdout
-v: Verbose mode (show instruction bytes)
-h: Display help message

Example
./disassembler firmware.bin -v -o firmware.asm
