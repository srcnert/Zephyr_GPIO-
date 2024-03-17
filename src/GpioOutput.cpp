#include "Gpio.h"

GpioOutput::GpioOutput(const struct device* port, const gpio_pin_t pin) : port_{port}, pin_{pin}
{
}

GpioOutput::GpioOutput(const struct gpio_dt_spec gpio_dt) : port_{gpio_dt.port}, pin_{gpio_dt.pin}, dt_flags_{gpio_dt.dt_flags}
{
}

int GpioOutput::IsReady()
{
    
    return (int) !device_is_ready(port_);
}

int GpioOutput::Configure(Gpio::Bias const bias, Gpio::Drive const drive)
{
    flags_ = GPIO_OUTPUT;
    flags_ |= static_cast<gpio_flags_t>(bias);
    flags_ |= static_cast<gpio_flags_t>(drive);
    return gpio_pin_configure(port_, pin_, dt_flags_ | flags_);
}

int GpioOutput::Configure(Gpio::ModeOutput const mode, Gpio::Bias const bias, Gpio::Drive const drive)
{
    flags_ |= static_cast<gpio_flags_t>(mode);
    flags_ |= static_cast<gpio_flags_t>(bias);
    flags_ |= static_cast<gpio_flags_t>(drive);
    return gpio_pin_configure(port_, pin_, dt_flags_ | flags_);
}

int GpioOutput::Disconnect()
{
    flags_ = 0;
    return gpio_pin_configure(port_, pin_, GPIO_DISCONNECTED);
}

int GpioOutput::IsOutput()
{
    return gpio_pin_is_output(port_, pin_);
}

int GpioOutput::Set()
{
    return gpio_pin_set(port_, pin_, static_cast<int>(Gpio::Level::HIGH));
}

int GpioOutput::Clear()
{
    return gpio_pin_set(port_, pin_, static_cast<int>(Gpio::Level::LOW));
}

int GpioOutput::Toggle()
{
    return gpio_pin_toggle(port_, pin_);
}