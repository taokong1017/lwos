#ifndef __ARM64_GIC_H__
#define __ARM64_GIC_H__

/* CPU Interrupt numbers */
#define GIC_INT_VIRT_MAINT 25
#define GIC_INT_HYP_TIMER 26
#define GIC_INT_VIRT_TIMER 27
#define GIC_INT_LEGACY_FIQ 28
#define GIC_INT_PHYS_TIMER 29
#define GIC_INT_NS_PHYS_TIMER 30
#define GIC_INT_LEGACY_IRQ 31

/* BIT(0) reserved for IRQ_ZERO_LATENCY */
#define IRQ_TYPE_LEVEL (1UL << 1)
#define IRQ_TYPE_EDGE (1UL << 2)

#define GIC_SPI 0x0
#define GIC_PPI 0x1

#define IRQ_DEFAULT_PRIORITY 0xa0

#endif