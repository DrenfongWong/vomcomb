#include <iostream>

#include "plugin.hpp"
#include "model.hpp"

#include <vom/interface.hpp>
#include <vom/interface_cmds.hpp>

using namespace std;

int sr_plugin_init_cb(sr_session_ctx_t *session, void **private_ctx)
{
    VOM::connection con;
    VOM::interface_cmds::dump_cmd dmp;
    int rc;

    std::cout << "Loading Module" << std::endl;

    // Blocking call until connection is performed
    rc = con.connect();
    if (rc != 0) {
        cout << "Failed to connect to VPP" << endl;
        exit(1);
    }

    cout << dmp.to_string() << endl;

    con.disconnect();
    return 0;
}

int sr_plugin_cleanup_cb(sr_session_ctx_t *session, void *private_ctx)
{
    std::cout << "Unloading Module" << std::endl;

    return 0;
}
