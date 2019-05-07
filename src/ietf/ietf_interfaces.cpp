#include <stdio.h>
#include <sys/socket.h>

#include "../model.hpp"

#include <vom/interface.hpp>

using namespace std;

/**
 * @brief Callback to be called by any config change under "/ietf-interfaces:interfaces-state/interface" path.
 * Does not provide any functionality, needed just to cover not supported config leaves.
 */
static int
ietf_interface_change_cb(sr_session_ctx_t *session, const char *xpath,
                         sr_notif_event_t event, void *private_ctx)
{
    UNUSED(session); UNUSED(xpath); UNUSED(event); UNUSED(private_ctx);

    SRP_LOG_INF("In %s", __FUNCTION__);

    return SR_ERR_OK;
}

/**
 * @brief Callback to be called by any config change of
 * "/ietf-interfaces:interfaces/interface/enabled" leaf.
 */
static int
ietf_interface_enable_disable_cb(sr_session_ctx_t *session, const char *xpath,
                                 sr_notif_event_t event, void *private_ctx)
{
    UNUSED(private_ctx);
    char *if_name = NULL;
    sr_change_iter_t *iter = NULL;
    sr_change_oper_t op = SR_OP_CREATED;
    sr_val_t *old_val = NULL;
    sr_val_t *new_val = NULL;
    sr_xpath_ctx_t xpath_ctx = {0};
    int rc = SR_ERR_OK, op_rc = SR_ERR_OK;

    SRP_LOG_INF("In %s", __FUNCTION__);

    /* no-op for apply, we only care about SR_EV_{ENABLED,VERIFY,ABORT} */
    if (SR_EV_APPLY == event)
        return SR_ERR_OK;

    SRP_LOG_DBG("'%s' modified, event=%d", xpath, event);

    /* get changes iterator */
    rc = sr_get_changes_iter(session, xpath, &iter);
    if (SR_ERR_OK != rc) {
        sr_free_change_iter(iter);
        SRP_LOG_ERR("Unable to retrieve change iterator: %s", sr_strerror(rc));
        return rc;
    }

    foreach_change (session, iter, op, old_val, new_val) {
        SRP_LOG_DBG("A change detected in '%s', op=%d",
                    new_val ? new_val->xpath : old_val->xpath, op);
        if_name = sr_xpath_key_value(new_val ? new_val->xpath : old_val->xpath,
                                     "interface", "name", &xpath_ctx);
        switch (op) {
            case SR_OP_CREATED:
            case SR_OP_MODIFIED:
            {
                if (new_val->data.bool_val) { // iface up
                    VOM::interface iface(if_name, VOM::interface::type_t::ETHERNET,
                                         VOM::interface::admin_state_t::UP);
                    try {
                        op_rc = VOM::OM::write("sthg", iface);
                    } catch (exception &exc) {
                        cout << exc.what() << endl;
                    }
                } else {
                    try {
                        VOM::interface iface(if_name, VOM::interface::type_t::ETHERNET,
                                             VOM::interface::admin_state_t::DOWN);
                        op_rc = VOM::OM::write("sthg", iface);
                    } catch (exception &exc) {
                        cout << exc.what() << endl;
                    }
                }

                break;
            }
            case SR_OP_DELETED:
            {
                VOM::OM::remove("sthg");
                break;
            }
            default:
                break;
        }
        sr_xpath_recover(&xpath_ctx);
        if (SR_ERR_INVAL_ARG == op_rc) {
            sr_set_error(session, "Invalid interface name.",
                         new_val ? new_val->xpath : old_val->xpath);
        }
        sr_free_val(old_val);
        sr_free_val(new_val);
    }
    sr_free_change_iter(iter);

    return op_rc;
}

const xpath_t ietf_interfaces_xpaths[IETF_INTERFACES_SIZE] = {
    {
        .xpath = "/ietf-interfaces:interfaces/interface",
        .method = XPATH,
        .datastore = SR_DS_RUNNING,
        .cb = {
            .scb = ietf_interface_change_cb,
        },
        .private_ctx = NULL,
        .priority = 0,
        .opts = SR_SUBSCR_CTX_REUSE | SR_SUBSCR_EV_ENABLED
    },
    {
        .xpath = "/ietf-interfaces:interfaces/interface/enabled",
        .method = XPATH,
        .datastore = SR_DS_RUNNING,
        .cb = {
            .scb = ietf_interface_enable_disable_cb,
        },
        .private_ctx = NULL,
        .priority = 100,
        .opts = SR_SUBSCR_CTX_REUSE | SR_SUBSCR_EV_ENABLED
    }
};
