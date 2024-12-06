#ifndef __SHELL_TRANSPORT_OPTS_H__
#define __SHELL_TRANSPORT_OPTS_H__

#include <types.h>
#include <device.h>

enum shell_transport_event {
	SHELL_TRANSPORT_RX_RDY,
	SHELL_TRANSPORT_TX_RDY,
};

typedef void (*shell_transport_handler_t)(enum shell_transport_event event,
										  void *context);

struct shell_transport;
struct shell_transport_ops {
	bool (*init)(struct shell_transport *transport, struct device *device,
				 shell_transport_handler_t handler, void *context);
	bool (*deinit)(struct shell_transport *transport);
	bool (*enable)(struct shell_transport *transport, bool blocking);
	int32_t (*write)(struct shell_transport *transport, const char *data,
					 uint32_t size);
	int32_t (*read)(struct shell_transport *transport, char *data,
					uint32_t size);
};

#endif
