#include "Gpio.h"

GpioInput::GpioInput(const struct device* port, const gpio_pin_t pin) : port_{port}, pin_{pin}
{
}

GpioInput::GpioInput(const struct gpio_dt_spec gpio_dt) : port_{gpio_dt.port}, pin_{gpio_dt.pin}, dt_flags_{gpio_dt.dt_flags}
{
}

int GpioInput::IsReady()
{
    return (int) !device_is_ready(port_);
}

int GpioInput::Configure(Gpio::Bias const bias, Gpio::Drive const drive)
{
    flags_ = GPIO_INPUT;
    flags_ |= static_cast<gpio_flags_t>(bias);
    flags_ |= static_cast<gpio_flags_t>(drive);
    return gpio_pin_configure(port_, pin_, dt_flags_ | flags_);
}

int GpioInput::ConfigureInterrupt(Gpio::ModeInterrupt const mode_int)
{
    flags_int_ = static_cast<gpio_flags_t>(mode_int);
    return gpio_pin_interrupt_configure(port_, pin_, flags_int_);
}

int GpioInput::Disconnect()
{
    flags_ = 0;
    return gpio_pin_configure(port_, pin_, GPIO_DISCONNECTED);
}

int GpioInput::IsInput()
{
    return gpio_pin_is_input(port_, pin_);
}

int GpioInput::Get(Gpio::Level& level)
{
    int status = gpio_pin_get(port_, pin_);
    if(status < 0)
        return status;

    status == 0 ? level = Gpio::Level::LOW : level = Gpio::Level::HIGH;
    return 0;
}

int GpioInput::AddCallback(gpio_callback_handler_t handler)
{
    gpio_init_callback(&gpio_callback_, handler, BIT(pin_));
    return gpio_add_callback(port_, &gpio_callback_);
}

int GpioInput::RemoveCallback()
{
    return gpio_remove_callback(port_, &gpio_callback_);
}