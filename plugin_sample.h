#ifndef __included_plugin_sample_h__
#define __included_plugin_sample_h__

#include <vnet/vnet.h>
#include <vnet/ip/ip.h>

#include <vppinfra/hash.h>
#include <vppinfra/error.h>
#include <vppinfra/elog.h>

typedef struct {
    /* API message ID base */
    u16 msg_id_base;

    /* convenience */
    vnet_main_t * vnet_main;
} plugin_sample_main_t;

extern plugin_sample_main_t plugin_sample_main;

extern vlib_node_registration_t plugin_sample_node;

#define PLUGIN_SAMPLE_PLUGIN_BUILD_VER "1.0"

#endif /* __included_plugin_sample_h__ */

