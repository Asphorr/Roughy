#ifndef PCI_H
#define PCI_H

#include <stdint.h>

void pci_init(void);
uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_write(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data);

#endif // PCI_H
