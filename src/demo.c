/* Includes ------------------------------------------------------------------*/

#include "app.h"
#include "bsp/bsp.h"
#include "stm32f1xx_hal.h"
#include "mesh/mesh.h"
#include "mesh/main.h"
#include "mesh/glue.h"
#include "mesh/porting.h"
#include "mesh/model_srv.h"

/* Private define ------------------------------------------------------------*/

#define CID_VENDOR                      0x038F

/* Private function prototypes -----------------------------------------------*/

static void health_pub_init(void);
static int  gen_onoff_get(struct bt_mesh_model *model, u8_t *state);
static int  gen_onoff_set(struct bt_mesh_model *model, u8_t state);
static void node_complete(u16_t net_idx, u16_t addr);
static void node_reset(void);

/* Private variables ---------------------------------------------------------*/

static u32_t dev_uuid[4];

static struct bt_mesh_cfg_srv cfg_srv = {
  .relay = BT_MESH_RELAY_ENABLED,
  .beacon = BT_MESH_BEACON_ENABLED,
#if MYNEWT_VAL(BLE_MESH_FRIEND)
  .frnd = BT_MESH_FRIEND_DISABLED,
#else
  .frnd = BT_MESH_FRIEND_NOT_SUPPORTED,
#endif
#if MYNEWT_VAL(BLE_MESH_GATT_PROXY)
  .gatt_proxy = BT_MESH_GATT_PROXY_ENABLED,
#else
  .gatt_proxy = BT_MESH_GATT_PROXY_NOT_SUPPORTED,
#endif
  .default_ttl = 7,

  /* 3 transmissions with 20ms interval */
  .net_transmit = BT_MESH_TRANSMIT(2, 20),
  .relay_retransmit = BT_MESH_TRANSMIT(2, 20),
};

static const struct bt_mesh_health_srv_cb health_srv_cb = {

};

static struct bt_mesh_health_srv health_srv = {
  .cb = &health_srv_cb,
};

static struct bt_mesh_model_pub health_pub;

static struct bt_mesh_gen_onoff_srv_cb gen_onoff_srv_cb = {
  .get = gen_onoff_get,
  .set = gen_onoff_set,
};

static struct bt_mesh_model_pub gen_onoff_pub;

static struct bt_mesh_model root_models[] = {
  BT_MESH_MODEL_CFG_SRV(&cfg_srv),
  BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
  BT_MESH_MODEL_GEN_ONOFF_SRV(&gen_onoff_srv_cb, &gen_onoff_pub),
};

static struct bt_mesh_elem elements[] = {
  BT_MESH_ELEM(0, root_models, BT_MESH_MODEL_NONE),
};

static const struct bt_mesh_prov prov = {
  .uuid = (u8_t *)dev_uuid,
  .complete = node_complete,
  .reset = node_reset,
};

static const struct bt_mesh_comp comp = {
  .cid = CID_VENDOR,
  .elem = elements,
  .elem_count = ARRAY_SIZE(elements),
};

/* Exported functions --------------------------------------------------------*/

void mesh_demo_init(void)
{
  int ret;
  ble_addr_t addr;

  /* Use NRPA */
  ret = ble_hs_id_gen_rnd(1, &addr);
  if (ret) {
    console_printf("Initializing random address failed (err %d)\n", ret);
    return;
  } else {
    console_printf("The device address: %02x-%02x-%02x-%02x-%02x-%02x (type %d)\n",
                      addr.val[5], addr.val[4], addr.val[3], addr.val[2],
                      addr.val[1], addr.val[0], addr.type);
  }

  ret = ble_hs_id_set_rnd(addr.val);
  if (ret) {
    console_printf("Setting random address failed (err %d)\n", ret);
    return;
  }

  /* Init the mesh stack */
  health_pub_init();
  HAL_GetUID(&dev_uuid[0]);
  memcpy(&dev_uuid[3], addr.val, sizeof(u32_t));

  ret = bt_mesh_init(addr.type, &prov, &comp);
  if (ret) {
    console_printf("Initializing mesh failed (err %d)\n", ret);
    return;
  } else {
    console_printf("Mesh initialized\n");
  }

  if (pdPASS != xTaskCreate(mesh_adv_thread, "mesh", APP_TASK_BLE_MESH_SIZE,
                            NULL, APP_TASK_BLE_MESH_PRIORITY, NULL)) {

    console_printf("Creating mesh task failed\n");
    return;
  } else {
    console_printf("Creating mesh task succeed\n");
  }
}

/* Private prototypes --------------------------------------------------------*/

static void health_pub_init(void)
{
  health_pub.msg = BT_MESH_HEALTH_FAULT_MSG(0);
}

static int gen_onoff_get(struct bt_mesh_model *model, u8_t *state)
{
  *state = led_state() ? 0x01 : 0x00;
  return 0;
}

static int gen_onoff_set(struct bt_mesh_model *model, u8_t state)
{
  if (state) {
    led_on();
  } else {
    led_off();
  }

  return 0;
}

static void node_complete(u16_t net_idx, u16_t addr)
{
  led_on();
  console_printf("Node is provisioned with address 0x%04x in network 0x%04x\n",
                  addr, net_idx);
}

static void node_reset(void)
{
  led_reset();
  console_printf("Node is reset and wait for provision\n");
  bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
}
