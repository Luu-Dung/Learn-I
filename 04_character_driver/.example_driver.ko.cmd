cmd_/media/tma/DATA/03_Practice/Learn-I/04_character_driver/example_driver.ko := ld -r -m elf_x86_64 -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /media/tma/DATA/03_Practice/Learn-I/04_character_driver/example_driver.ko /media/tma/DATA/03_Practice/Learn-I/04_character_driver/example_driver.o /media/tma/DATA/03_Practice/Learn-I/04_character_driver/example_driver.mod.o ;  true