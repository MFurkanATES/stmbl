#include "hal.h"

HAL_COMP(pos_filter);

HAL_PIN(pos_in);
HAL_PIN(vel_in);

HAL_PIN(pos_out);
HAL_PIN(vel_out);
HAL_PIN(acc_out);

HAL_PIN(bandwidth);

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct pos_filter_ctx_t *ctx      = (struct pos_filter_ctx_t *)ctx_ptr;
  struct pos_filter_pin_ctx_t *pins = (struct pos_filter_pin_ctx_t *)pin_ptr;

  float ki = 2.0 * PIN(bandwidth);
  float kp = 0.25 * ki * ki;

  float pos_error = PIN(pos_in) - PIN(pos_out);
  float vel_error = PIN(vel_in) - PIN(vel_out);
  
  PIN(acc_out) = kp * pos_error + ki * vel_error;
  PIN(vel_out) += period * PIN(acc_out);
  PIN(pos_out) += period * PIN(vel_out);
}

const hal_comp_t pos_filter_comp_struct = {
    .name      = "pos_filter",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .hw_init   = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct pos_filter_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
