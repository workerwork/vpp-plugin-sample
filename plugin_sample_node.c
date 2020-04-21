#include <vlib/vlib.h>
#include <vnet/vnet.h>
#include <vnet/pg/pg.h>
#include <vnet/ethernet/ethernet.h>
#include <vppinfra/error.h>
#include <plugin_sample/plugin_sample.h>

typedef enum
{
  PLUGIN_SAMPLE_NEXT_IP4,
  PLUGIN_SAMPLE_DROP,
  PLUGIN_SAMPLE_NEXT_N,
} plugin_sample_next_t;

typedef struct
{
  u32 next_index;
  u32 sw_if_index;
  u8 new_src_mac[6];
  u8 new_dst_mac[6];
} plugin_sample_trace_t;

#define foreach_plugin_sample_error \
_(SHOWED, "show packets processed")

typedef enum
{
#define _(sym,str) SAMPLE_ERROR_##sym,
  foreach_plugin_sample_error
#undef _
    SAMPLE_N_ERROR,
} plugin_sample_error_t;


static char *plugin_sample_error_strings[] = {
#define _(sym, str) str,
        foreach_plugin_sample_error
#undef _
};

extern vlib_node_registration_t plugin_sample_node;

static u8 *
format_plugin_sample_trace (u8 * s, va_list * args)
{
        s = format(s, "To Do!\n");
        return s;
}

//Node处理packet主函数
static uword plugin_sample_node_fn(vlib_main_t *vm, vlib_node_runtime_t *node,
        vlib_frame_t * frame)
{
        u32 n_left_from, *from, *to_next;
        plugin_sample_next_t     next_index;

        from        = vlib_frame_vector_args(frame);
        n_left_from = frame->n_vectors;
        next_index  = node->cached_next_index;

        while(n_left_from > 0){
                u32 n_left_to_next;
				// 从流程中获取包
                vlib_get_next_frame(vm, node, next_index, to_next, n_left_to_next);

                while(n_left_from > 0 && n_left_to_next > 0){
                        vlib_buffer_t  *b0;
                        u32             bi0, next0 = 0;

                        bi0 = to_next[0] = from[0];
                        from           += 1;
                        to_next        += 1;
                        n_left_to_next -= 1;
                        n_left_from    -= 1;

                        b0 = vlib_get_buffer(vm, bi0);
						//获取到IP header
						void *en0 = vlib_buffer_get_current(b0);
                        int i = 0;
						//打印前20 bytes, 即ip header
                        for (i = 0; i < 20; i++)
                        {
                                printf("%02x ", *(u8*)(en0+i));
                        }
                        printf("\n");
                        vlib_validate_buffer_enqueue_x1(vm, node, next_index,
                                to_next, n_left_to_next, bi0, next0);
                }
				// 包放回流程中去
                vlib_put_next_frame(vm, node, next_index, n_left_to_next);
        }

        return frame->n_vectors;
}

//注册NODE
//node初始化信息，生成一堆该Node的构造/析构函数
VLIB_REGISTER_NODE (plugin_sample_node) = {
        .name		= "plugin_sample",
        .function       = plugin_sample_node_fn,
        .vector_size    = sizeof(u32),
        .format_trace   = format_plugin_sample_trace,
        .type           = VLIB_NODE_TYPE_INTERNAL,
        .n_errors       = ARRAY_LEN(plugin_sample_error_strings),
        .error_strings  = plugin_sample_error_strings,
        .n_next_nodes   = PLUGIN_SAMPLE_NEXT_N,
        .next_nodes     = {
                [PLUGIN_SAMPLE_NEXT_IP4]    = "ip4-lookup",
                [PLUGIN_SAMPLE_DROP]        = "error-drop",
        },
};





