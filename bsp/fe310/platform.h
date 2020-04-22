#ifndef PLATFORM_H
#define PLATFORM_H
#include <stdint.h>

#define RTCK_FREQ 32768
#define CORECLK_FREQ 192000000

// Padding so struct fields align correctly to memory map
#define _PAD(name, start, end) char name[end - start]

typedef volatile struct clic {
    uint32_t msip;
    _PAD(pad1, 0x4, 0x4000);
    uint32_t mtimecmp;
    uint32_t mtimecmph;
    _PAD(pad2, 0x4008, 0xbff8);
    uint32_t mtime;
    uint32_t mtimeh;
} clic_t;

typedef volatile struct plic {
    _PAD(pad1, 0x0, 0x4);
    uint32_t source_prio[52];
    _PAD(pad2, 0xD4, 0x1000);
    uint32_t pending[2];
    _PAD(pad3, 0x1008, 0x2000);
    uint32_t ie[2];
    _PAD(pad4, 0x2008, 0x200000);
    uint32_t prio_thres;
    uint32_t claim_complete;
} plic_t;

typedef volatile struct gpio {
    uint32_t input_val;
    uint32_t input_en;
    uint32_t output_val;
    uint32_t output_en;
    uint32_t pue;
    uint32_t ds;
    uint32_t rise_ie;
    uint32_t rise_ip;
    uint32_t fall_ie;
    uint32_t fall_ip;
    uint32_t high_ie;
    uint32_t high_ip;
    uint32_t low_ie;
    uint32_t low_ip;
    uint32_t iof_en;
    uint32_t iof_sel;
    uint32_t out_xor;
} gpio_t;

typedef volatile struct uart {
    int32_t txdata;
    int32_t rxdata;
    uint32_t txctrl;
    uint32_t rxctrl;
    uint32_t ie;
    uint32_t ip;
    uint32_t div;
} uart_t;

typedef volatile struct prci {
    uint32_t hfrosccfg;
    uint32_t hfxosccfg;
    uint32_t pllcfg;
    uint32_t plloutdiv;
    uint32_t procmoncfg;
} prci_t;

extern clic_t CLIC;
extern plic_t PLIC;
extern prci_t PRCI;
extern gpio_t GPIO;
extern uart_t UART0;
extern uart_t UART1;
#endif // PLATFORM_H
