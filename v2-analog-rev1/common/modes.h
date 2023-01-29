
void vgamain();
void vga_businterface(uint32_t address, uint32_t value);

void z80main();
void z80_businterface(uint32_t address, uint32_t value);

void serialmain();
void serial_businterface(uint32_t address, uint32_t value);

void parallelmain();
void parallel_businterface(uint32_t address, uint32_t value);

void diagmain();
void diag_businterface(uint32_t address, uint32_t value);

void fsmain();
void fs_businterface(uint32_t address, uint32_t value);

void flash_reboot() __attribute__ ((noreturn));
