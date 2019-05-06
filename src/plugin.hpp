extern "C" {
#include <sysrepo.h>
}

extern "C" int sr_plugin_init_cb(sr_session_ctx_t *session, void **private_ctx);
extern "C" int sr_plugin_cleanup_cb(sr_session_ctx_t *session, void *private_ctx);
