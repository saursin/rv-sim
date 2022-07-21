SRC:= basic
IMGDIR = ../../build

default: ../../build/rom.bin $(SRC).objdump

$(IMGDIR)/rom.bin: $(SRC).bin
	cp $< $@

$(SRC).bin: $(SRC).elf
	riscv64-unknown-elf-objcopy -O binary $< $@

$(SRC).objdump: $(SRC).elf
	riscv64-unknown-elf-objdump -htd $< > $@

$(SRC).elf: $(SRC).S
	riscv64-unknown-elf-gcc -mabi=ilp32 -march=rv32i -nostartfiles -ffreestanding -T link.ld $< -o $@


.PHONY: clean
clean: 
	rm *.elf *.bin *.objdump
	rm $(IMGDIR)/*.bin