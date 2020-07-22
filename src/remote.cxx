#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "ir.hpp"
#include "statemachine.hpp"
#include "util_libopencm3.hpp"

// NOTE: there is an an error when calculating the prescaler value at
// compile-time, therefore the calculation is performed manually.
uint32_t const inter_command_timer_freq = 100 * KILO;
// (rcc_apb1_frequency * 2)/ input_segment_timer_freq = 720
uint32_t const inter_command_prescaler = 720;
util::Timer inter_command_timer{
    TIM1, TIM_OC1, inter_command_prescaler,
    util::ns2count(inter_command_timer_freq, 500 * MEGA)};
uint32_t const input_inter_segment_timer_freq = 2 * MEGA;
// (rcc_apb1_frequency * 2)/ input_segment_timer_freq = 36
uint32_t const input_inter_segment_prescaler = 36;
util::Timer input_inter_segment_timer{
    TIM3, TIM_OC1, input_inter_segment_prescaler,
    util::ns2count(input_inter_segment_timer_freq, 24 * MEGA)};
util::Timer output_inter_segment_timer{
    TIM4, TIM_OC1, input_inter_segment_prescaler,
    util::ns2count(input_inter_segment_timer_freq, 24 * MEGA)};
// Do not divide clock for highest possible resolution
// Frequency is ABP1 clock * 2 = 72 MHz.
// 1 period = 947 * (1/72MHz) = 13.15277... us <=> 76 KHz = 2*38 Khz
util::Timer output_carrier_timer{TIM2, TIM_OC1, 0, 947};

// constexpr const util::io_t output_ir{GPIOA,GPIO8}; // TIM1 CH1 output
constexpr const util::io_t output_ir{GPIOA, GPIO0}; // TIM2 CH1 output
// constexpr const util::io_t output_ir{GPIOA,GPIO6}; // TIM3 CH1 output

constexpr size_t kNumButtons{12};
using Buttons = std::array<util::Io, kNumButtons>;
constexpr Buttons buttons{{
    {GPIOA, GPIO1},
    {GPIOA, GPIO2},
    {GPIOA, GPIO3},
    {GPIOA, GPIO4},
    {GPIOA, GPIO5},
    {GPIOA, GPIO6},
    {GPIOA, GPIO7},
    {GPIOA, GPIO8},
    {GPIOA, GPIO9},
    {GPIOA, GPIO10},
    {GPIOA, GPIO11},
    {GPIOA, GPIO12},
}};

constexpr const util::io_t input_ir{GPIOA, GPIO1};
constexpr const util::io_t led_ir{GPIOC, GPIO13};
constexpr const util::io_t led_fail{GPIOA, GPIO2};

InputHandler<hal::HwTag> input_handler{input_inter_segment_timer};
OutputHandler<hal::HwTag> output_handler{output_inter_segment_timer,
                                         output_carrier_timer};

using STable = RemoteStateTable<hal::HwTag>;

util::StateMachine<STable> state_machine{
    std::in_place_type_t<STable::Sending>{},
    STable::CommonState{
        {{1, {67, {16197, 8671, 1234, 3147, 1249, 3127, 1258, 936,  1261, 932,
                   1263,  928,  1256, 933,  1260, 931,  1256, 3119, 1260, 3116,
                   1194,  1000, 1261, 3114, 1224, 970,  1248, 3124, 1217, 979,
                   1261,  3113, 1262, 932,  1162, 1030, 1260, 3113, 1252, 943,
                   1262,  3113, 1164, 3215, 1236, 958,  1261, 954,  1228, 3122,
                   1217,  3161, 1261, 934,  1269, 3105, 1241, 953,  1228, 965,
                   1257,  3119, 1260, 3118, 1263, 929,  1262}}}}},
    0};

/*
 * Setup
 */
static void clock_setup(void) {
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  // Enable GPIO clock for leds.
  rcc_periph_clock_enable(RCC_GPIOC);
  // Enable GPIO clock for IR input.
  rcc_periph_clock_enable(RCC_GPIOA);
  // Enable GPIO clock for fail led.
  rcc_periph_clock_enable(RCC_GPIOA);

  // Enable AFIO clock for timers.
  rcc_periph_clock_enable(RCC_AFIO);
}

static void gpio_setup(void) {
  // Enable led as output
  gpio_set_mode(led_ir.port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                led_ir.pin);
  gpio_set(led_ir.port, led_ir.pin);

  // Enable fail led as output
  gpio_set_mode(led_fail.port, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, led_fail.pin);
  gpio_set(led_ir.port, led_ir.pin);

  // carrier timer output compare value on pin, connect to ir led.
  gpio_set_mode(output_ir.port, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, output_ir.pin);

  // Ir input interrupt.
  gpio_set_mode(input_ir.port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                input_ir.pin);
  nvic_enable_irq(NVIC_EXTI1_IRQ);
  exti_select_source(EXTI1, input_ir.port);
  exti_set_trigger(EXTI1, EXTI_TRIGGER_BOTH);
  exti_enable_request(EXTI1);
}

void buttons_setup() {
  for (const auto &button : buttons) {
    uint32_t exti{util::GetExtiIrqn(button.pin_).value()};
    gpio_set_mode(button.port_, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  button.pin_);
    nvic_enable_irq(exti);
    exti_select_source(exti, button.port_);
    exti_set_trigger(exti, EXTI_TRIGGER_RISING);
    exti_enable_request(exti);
  }
}

/*
 * Isrs
 */
extern "C" {
void exti0_isr(void) {
  exti_reset_request(EXTI1);
  state_machine.send(STable::ReceiveToggle{});
}

void exti1_isr(void) { state_machine.send(STable::ButtonNumber{0}); }

void exti2_isr(void) { state_machine.send(STable::ButtonNumber{1}); }

void exti3_isr(void) { state_machine.send(STable::ButtonNumber{2}); }

void exti4_isr(void) { state_machine.send(STable::ButtonNumber{3}); }

void exti5_isr(void) { state_machine.send(STable::ButtonNumber{4}); }

void exti6_isr(void) { state_machine.send(STable::ButtonNumber{5}); }

void exti7_isr(void) { state_machine.send(STable::ButtonNumber{6}); }

void exti8_isr(void) { state_machine.send(STable::ButtonNumber{7}); }

void exti9_isr(void) { state_machine.send(STable::ButtonNumber{8}); }

void exti10_isr(void) { state_machine.send(STable::ButtonNumber{9}); }

void exti11_isr(void) { state_machine.send(STable::ButtonStop{}); }

void exti12_isr(void) { state_machine.send(STable::ButtonNext{}); }

void tim3_isr(void) { state_machine.send(STable::SendNextSegment{}); }

void tim4_isr(void) {
  state_machine.send(STable::SendNextSegment{});
  gpio_toggle(led_fail.port, led_fail.pin);
}
}

// Normally used when calling destructors for global objects
// Since we will never return from main it will be unused.
void *__dso_handle = nullptr;
/*!
 * Main
 */
int main(void) {
  nvic_get_irq_enabled(NVIC_EXTI1_IRQ);
  clock_setup();
  gpio_setup();
  buttons_setup();

  // prescaler 36
  //{16213, 8792, 1142, 1052, 1097, 1096, 1091, 3306, 1141, 1053, 1143, 1048,
  // 1092, 1099, 1094, 1097, 1091, 3304, 1101, 1095, 1142, 1050, 1091, 3306,
  // 1145, 1048, 1091, 3306, 1091, 3303, 1093, 3307, 1143, 1050, 1144, 1049,
  // 1143, 1050, 1144, 1048, 1143, 1048, 1091, 3306, 1142, 1050, 1093, 1099,
  // 1093, 3304, 1092, 3304, 1092, 3308, 1090, 3305, 1093, 3305, 1094, 1100,
  // 1088, 3308, 1091, 3306, 1145, 1047, 1092};

  /* Power projector
  $1 = {static MAX_TIMING_LIMIT = 100, array_ = {_buffer = {16189, 8691, 1158,
  3224, 1150, 3228, 1157, 1039, 1162, 1030, 1164, 1027, 1164, 1029, 1223, 970,
  1221, 3153, 1150, 3229, 1223, 973, 1219, 3155, 1149, 1048, 1221, 3152, 1152,
  1045, 1222, 3154, 1153, 1043, 1163, 1029, 1212, 982, 1222, 972, 1163, 1031,
  1224, 3149, 1156, 1041, 1221, 971, 1223, 3154, 1226, 3153, 1162, 3217, 1148,
  3232, 1163, 3217, 1156, 1040, 1163, 3213, 1148, 3232, 1161, 1035, 1162, 0
  <repeats 33 times>}}, size_ = 67}
  */

  /* projector menu
    $2 = {static MAX_TIMING_LIMIT = 100, array_ = {_buffer = {16197, 8671, 1234,
    3147, 1249, 3127, 1258, 936, 1261, 932, 1263, 928, 1256, 933, 1260, 931,
    1256, 3119, 1260, 3116, 1194, 1000, 1261, 3114, 1224, 970, 1248, 3124, 1217,
    979, 1261, 3113, 1262, 932, 1162, 1030, 1260, 3113, 1252, 943, 1262, 3113,
    1164, 3215, 1236, 958, 1261, 954, 1228, 3122, 1217, 3161, 1261, 934, 1269,
    3105, 1241, 953, 1228, 965, 1257, 3119, 1260, 3118, 1263, 929, 1262, 0
    <repeats 33 times>}}, size_ = 67}

   */

  while (1) {
    gpio_toggle(led_ir.port, led_ir.pin);
    for (size_t i = 0; i < 4000000; i++) // Wait a bit.
      __asm__("nop");
    ;
  }

  return 0;
}
