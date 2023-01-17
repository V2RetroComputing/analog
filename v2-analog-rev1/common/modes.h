
void vgamain();
void vga_businterface();

void z80main();
void z80_businterface();

void serialmain();
void serial_businterface();

void parallelmain();
void parallel_businterface();

void diag_businterface();
void diagmain();

void fs_businterface();
void fsmain();

void flash_reboot() __attribute__ ((noreturn));
