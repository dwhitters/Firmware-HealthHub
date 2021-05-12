/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/printk.h>
#include <sys/byteorder.h>
#include <zephyr.h>

#include <settings/settings.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/services/bas.h>

#include <logging/log.h>

/* Initialize the RTT logging. */
#define LOG_MODULE_NAME health_hub
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

/* Set when the CCC Cfg changes. True when notifications are enabled, false otherwise. */
volatile bool Notify_Enabled;

/* Custom Service Variables */
static struct bt_uuid_128 vnd_uuid = BT_UUID_INIT_128(
    0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
    0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);

static struct bt_uuid_128 vnd_enc_uuid = BT_UUID_INIT_128(
    0xf1, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
    0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);

static struct bt_uuid_128 vnd_auth_uuid = BT_UUID_INIT_128(
    0xf2, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
    0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);

static uint8_t vnd_value[] = { 'V', 'e', 'n', 'd', 'o', 'r' };

static ssize_t read_vnd(struct bt_conn *conn, const struct bt_gatt_attr *attr,
            void *buf, uint16_t len, uint16_t offset)
{
    const char *value = attr->user_data;

    return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
                 strlen(value));
}

static ssize_t write_vnd(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, uint16_t len, uint16_t offset,
             uint8_t flags)
{
    uint8_t *value = attr->user_data;

    if (offset + len > sizeof(vnd_value)) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }

    memcpy(value + offset, buf, len);

    return len;
}

/**
    Called when the configuration changes.
*/
static void vnd_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    Notify_Enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notification %s", Notify_Enabled ? "enabled" : "disabled");
}

#define MAX_DATA 74
static uint8_t vnd_long_value[] = {
          'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '1',
          'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '2',
          'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '3',
          'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '4',
          'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '5',
          'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '6',
          '.', ' ' };

static ssize_t read_long_vnd(struct bt_conn *conn,
                 const struct bt_gatt_attr *attr, void *buf,
                 uint16_t len, uint16_t offset)
{
    const char *value = attr->user_data;

    return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
                 sizeof(vnd_long_value));
}

static ssize_t write_long_vnd(struct bt_conn *conn,
                  const struct bt_gatt_attr *attr, const void *buf,
                  uint16_t len, uint16_t offset, uint8_t flags)
{
    uint8_t *value = attr->user_data;

    if (flags & BT_GATT_WRITE_FLAG_PREPARE) {
        return 0;
    }

    if (offset + len > sizeof(vnd_long_value)) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }

    memcpy(value + offset, buf, len);

    return len;
}

static const struct bt_uuid_128 vnd_long_uuid = BT_UUID_INIT_128(
    0xf3, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
    0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);

static struct bt_gatt_cep vnd_long_cep = {
    .properties = BT_GATT_CEP_RELIABLE_WRITE,
};

static int signed_value;

static ssize_t read_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,
               void *buf, uint16_t len, uint16_t offset)
{
    const char *value = attr->user_data;

    return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
                 sizeof(signed_value));
}

static ssize_t write_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                const void *buf, uint16_t len, uint16_t offset,
                uint8_t flags)
{
    uint8_t *value = attr->user_data;

    if (offset + len > sizeof(signed_value)) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }

    memcpy(value + offset, buf, len);

    return len;
}

static const struct bt_uuid_128 vnd_signed_uuid = BT_UUID_INIT_128(
    0xf3, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x13,
    0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x13);

static const struct bt_uuid_128 vnd_write_cmd_uuid = BT_UUID_INIT_128(
    0xf4, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
    0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);

static ssize_t write_without_rsp_vnd(struct bt_conn *conn,
                     const struct bt_gatt_attr *attr,
                     const void *buf, uint16_t len, uint16_t offset,
                     uint8_t flags)
{
    uint8_t *value = attr->user_data;

    /* Write request received. Reject it since this char only accepts
     * Write Commands.
     */
    if (!(flags & BT_GATT_WRITE_FLAG_CMD)) {
        return BT_GATT_ERR(BT_ATT_ERR_WRITE_REQ_REJECTED);
    }

    if (offset + len > sizeof(vnd_value)) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }

    memcpy(value + offset, buf, len);

    return len;
}

/* Vendor Primary Service Declaration */
BT_GATT_SERVICE_DEFINE(vnd_svc,
    BT_GATT_PRIMARY_SERVICE(&vnd_uuid),
    BT_GATT_CHARACTERISTIC(&vnd_enc_uuid.uuid,
                   BT_GATT_CHRC_NOTIFY,
                   BT_GATT_PERM_READ_ENCRYPT,
                   read_vnd, write_vnd, vnd_value),
    BT_GATT_CCC(vnd_ccc_cfg_changed,
            BT_GATT_PERM_READ | BT_GATT_PERM_WRITE_ENCRYPT),
    BT_GATT_CHARACTERISTIC(&vnd_auth_uuid.uuid,
                   BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
                   BT_GATT_PERM_READ_AUTHEN |
                   BT_GATT_PERM_WRITE_AUTHEN,
                   read_vnd, write_vnd, vnd_value),
    BT_GATT_CHARACTERISTIC(&vnd_long_uuid.uuid, BT_GATT_CHRC_READ |
                   BT_GATT_CHRC_WRITE | BT_GATT_CHRC_EXT_PROP,
                   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE |
                   BT_GATT_PERM_PREPARE_WRITE,
                   read_long_vnd, write_long_vnd, &vnd_long_value),
    BT_GATT_CEP(&vnd_long_cep),
    BT_GATT_CHARACTERISTIC(&vnd_signed_uuid.uuid, BT_GATT_CHRC_READ |
                   BT_GATT_CHRC_WRITE | BT_GATT_CHRC_AUTH,
                   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                   read_signed, write_signed, &signed_value),
    BT_GATT_CHARACTERISTIC(&vnd_write_cmd_uuid.uuid,
                   BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                   BT_GATT_PERM_WRITE, NULL,
                   write_without_rsp_vnd, &vnd_value),
);

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA_BYTES(BT_DATA_UUID16_ALL,
            BT_UUID_16_ENCODE(BT_UUID_BAS_VAL)),
    BT_DATA_BYTES(BT_DATA_UUID128_ALL,
              0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
              0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12),
};

static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        printk("Connection failed (err 0x%02x)\n", err);
    } else {
        printk("Connected\n");
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    printk("Disconnected (reason 0x%02x)\n", reason);
}

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

static void bt_ready(void)
{
    int err;

    printk("Bluetooth initialized\n");

    if (IS_ENABLED(CONFIG_SETTINGS)) {
        settings_load();
    }

    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        printk("Advertising failed to start (err %d)\n", err);
        return;
    }

    printk("Advertising successfully started\n");
}

static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("Passkey for %s: %06u\n", addr, passkey);
}

static void auth_cancel(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("Pairing cancelled: %s\n", addr);
}

static struct bt_conn_auth_cb auth_cb_display = {
    .passkey_display = auth_passkey_display,
    .passkey_entry = NULL,
    .cancel = auth_cancel,
};

static void bas_notify(void)
{
    uint8_t battery_level = bt_bas_get_battery_level();

    battery_level--;

    if (battery_level == 0u) {
        battery_level = 100U;
    }

    bt_bas_set_battery_level(battery_level);
}

/**
    Notifies the central device with updated data.
    The data is fake for now.
*/
static void ecg_notify(void)
{
    static uint8_t ecg_val = 0u;
    ecg_val++;

    if(ecg_val == 255u)
    {
        ecg_val = 0u;
    }
    LOG_INF("ECG Val: %d\n", ecg_val);

    int err = bt_gatt_notify(NULL, &vnd_svc.attrs[2], &ecg_val, sizeof(ecg_val));
    if(err)
    {
        LOG_ERR("Error occurred: %d\n", err);
    }
}

void main(void)
{
    int err;
    LOG_INF("Start");

    err = bt_enable(NULL);
    if(err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    bt_ready();

    bt_conn_cb_register(&conn_callbacks);
    bt_conn_auth_cb_register(&auth_cb_display);

    /* Implement notification. At the moment there is no suitable way
     * of starting delayed work so we do it here
     */
    while(1) {
        k_sleep(K_SECONDS(1));

        /* Battery level simulation */
        bas_notify();

        if(Notify_Enabled)
        {
            /* Vendor indication simulation */
            ecg_notify();
        }
        else
        {
            LOG_INF("Attempted to notify but not enabled");
        }
    }
}
