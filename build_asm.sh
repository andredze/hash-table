nasm -f elf64 -g -F dwarf src/hash_table/crc32.s -o obj/hash_table/crc32.o
nasm -f elf64 -g -F dwarf src/hash_table/hash_table_find.s -o obj/hash_table/hash_table_find.o
nasm -f elf64 -g -F dwarf src/hash_table/hash_table_find_nocrc32.s -o obj/hash_table/hash_table_find_nocrc32.o