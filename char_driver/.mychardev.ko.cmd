cmd_/home/pasha/Device-Driver-Generic/char_driver/mychardev.ko := ld -r  -m elf_x86_64 -z noexecstack   --build-id=sha1  -T scripts/module.lds -o /home/pasha/Device-Driver-Generic/char_driver/mychardev.ko /home/pasha/Device-Driver-Generic/char_driver/mychardev.o /home/pasha/Device-Driver-Generic/char_driver/mychardev.mod.o;  true