#include <iostream>

#include "plugin.hpp"
#include "model.hpp"

#include <vom/hw.hpp>
#include <vom/om.hpp>

using namespace std;

int sr_plugin_init_cb(sr_session_ctx_t *session, void **private_ctx)
{
    UNUSED(session);
    plugin_main_t plugin_main;

    std::cout << "Loading Module" << std::endl;

    /* Init connection to VPP */
    VOM::HW::init(new VOM::HW::cmd_q());

    /* Init connection to VOM database */
    VOM::OM::init();

    while (VOM::HW::connect() != true);

    VOM::OM::populate("populate"); //TODO how to use it?

    plugin_main.session = session;
    plugin_main.subscription = NULL;

    model_register(&plugin_main, ietf_interfaces_xpaths, IETF_INTERFACES_SIZE);

    *private_ctx = plugin_main.subscription; //TODO unused

    return SR_ERR_OK;
}

int sr_plugin_cleanup_cb(sr_session_ctx_t *session, void *private_ctx)
{
    UNUSED(session); UNUSED(private_ctx);
    std::cout << "Unloading Module" << std::endl;

    VOM::HW::disconnect();

    return SR_ERR_OK;
}
