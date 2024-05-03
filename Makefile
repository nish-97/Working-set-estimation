CFLAGS = -Wall -g -Wextra -Werror -O0 -Werror=array-bounds=0

.PHONY: run
run: simple-kvm
	./simple-kvm -l

simple-kvm: simple-kvm.o payload.o
	$(CC) $^ $(CFLAGS) -o $@

payload.o: payload.ld guest16.o guest32.img.o guest64.img.o
	$(LD) -r -T $< -o $@

guest64.o: guest.c
	$(CC) $(CFLAGS) -m64 -O0 -ffreestanding -fno-pic -c -o $@ $^

guest64.img: guest64.o
	$(LD) -T guest.ld $^ -o $@

guest32.o: guest.c
	$(CC) $(CFLAGS) -m32 -ffreestanding -fno-pic -c -o $@ $^

guest32.img: guest32.o
	$(LD) -T guest.ld -m elf_i386 $^ -o $@

%.img.o: %.img
	$(LD) -b binary -r $^ -o $@

.PHONY: clean
clean:
	$(RM) simple-kvm simple-kvm.o payload.o guest16.o \
		guest32.o guest32.img guest32.img.o \
		guest64.o guest64.img guest64.img.o
