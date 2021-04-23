.PHONY: steering steering_make steering_flash driving driving_make driving_flash
all: steering/main.c steering/steering.c steering/steering.h driving/main.c driving/driving.c comms/comms.h
	avr-gcc -mmcu=atmega328p steering/*.c comms/comms.c -o steering/output/steering.elf
	avr-objcopy -h -S steering/output/steering.elf > steering/output/steering.lst
	avr-objcopy -j .text -j .data -O ihex steering/output/steering.elf steering/output/steering.hex
	avr-gcc -mmcu=atmega328p driving/*.c comms/comms.c -o driving/output/driving.elf
	avr-objcopy -h -S driving/output/driving.elf > driving/output/driving.lst
	avr-objcopy -j .text -j .data -O ihex driving/output/driving.elf driving/output/driving.hex

steering: steering/main.c steering/steering.c comms/comms.h
	avr-gcc -mmcu=atmega328p steering/*.c comms/comms.c -o steering/output/steering.elf
	avr-objcopy -h -S steering/output/steering.elf > steering/output/steering.lst
	avr-objcopy -j .text -j .data -O ihex steering/output/steering.elf steering/output/steering.hex
	avrdude -p m328p -c arduino -P /dev/ttyACM0 -D -U flash:w:steering/output/steering.hex:i

steering_make: steering/main.c steering/steering.c comms/comms.h
	avr-gcc -mmcu=atmega328p steering/*.c comms/comms.c -o steering/output/steering.elf
	avr-objcopy -h -S steering/output/steering.elf > steering/output/steering.lst
	avr-objcopy -j .text -j .data -O ihex steering/output/steering.elf steering/output/steering.hex

steering_flash: steering/output/steering.hex
	avrdude -p m328p -c arduino -P /dev/ttyACM0 -D -U flash:w:steering/output/steering.hex:i

driving: driving/main.c driving/driving.c comms/comms.h
	avr-gcc -mmcu=atmega328p driving/*.c comms/comms.c -o driving/output/driving.elf
	avr-objcopy -h -S driving/output/driving.elf > driving/output/driving.lst
	avr-objcopy -j .text -j .data -O ihex driving/output/driving.elf driving/output/driving.hex
	avrdude -p m328p -c arduino -P /dev/ttyACM0 -D -U flash:w:driving/output/driving.hex:i

driving_flash: driving/output/driving.hex
	avrdude -p m328p -c arduino -P /dev/ttyACM0 -D -U flash:w:driving/output/driving.hex:i

driving_make: driving/main.c driving/driving.c comms/comms.h
	avr-gcc -mmcu=atmega328p driving/*.c comms/comms.c -o driving/output/driving.elf
	avr-objcopy -h -S driving/output/driving.elf > driving/output/driving.lst
	avr-objcopy -j .text -j .data -O ihex driving/output/driving.elf driving/output/driving.hex
