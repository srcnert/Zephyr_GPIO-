#include <zephyr/kernel.h>
#include <zephyr/device.h>

#include "Gpio.h"

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

// Method-1 to setup GPIO pins.
// static const struct device *port0 = DEVICE_DT_GET(DT_NODELABEL(gpio0));

// GpioOutput  led0(port0, 13);
// GpioOutput  led1(port0, 14);
// GpioInput   btn1(port0, 11);

// Method-2 to setup GPIO pins.
static const struct gpio_dt_spec dt_led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec dt_led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec dt_btn1 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

GpioOutput  led0(dt_led0);
GpioOutput  led1(dt_led1);
GpioInput   btn1(dt_btn1);

// Global variables
bool state = false;


void btn1_handler(const struct device *dev, struct gpio_callback *cb,
           uint32_t pins)
{
    LOG_INF("btn1 interrupt occurs...");
}

int main(void)
{
	LOG_INF("Zephyr RTOS C++ Gpio example...");	

    int status = led0.IsReady();
    if(status != 0)
        LOG_ERR("led0 port is not ready!");

    status = led1.IsReady();
    if(status != 0)
        LOG_ERR("led1 port is not ready!");

    status = btn1.IsReady();
    if(status != 0)
        LOG_ERR("btn1 port is not ready!");

    status = btn1(Gpio::Bias::PULL_UP);
    if(status != 0)
        LOG_ERR("btn1 configuration error!");

    status = btn1(Gpio::ModeInterrupt::EDGE_FALLING);
    if(status != 0)
        LOG_ERR("btn1 interrupt configuration error!");

    status = btn1.AddCallback(btn1_handler);
    if(status != 0)
        LOG_ERR("btn1 add callback error!");
    
    status = led0();
    if(status != 0)
        LOG_ERR("led0 configuration error!");

    status = led1(Gpio::ModeOutput::OUTPUT_HIGH);
    if(status != 0)
        LOG_ERR("led1 configuration error!");

    led0 = true; // Set led0 pin to high...

    while(1)
    {
        status = !led0;
        if(status != 0)
            LOG_ERR("led0 toggle error!");

        status = led1 << state;
        state = !state;
        if(status != 0)
            LOG_ERR("led1 set error!");

        Gpio::Level btn_state = Gpio::Level::LOW;
        status = btn1>>btn_state;
        if(status != 0)
            LOG_ERR("btn1 read error!");

        LOG_INF("btn1 level is %d...", static_cast<int>(btn_state));

        k_sleep(K_MSEC(1000));
    }
}
