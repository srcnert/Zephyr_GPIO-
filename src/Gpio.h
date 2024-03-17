#ifndef PIN_H__
#define PIN_H__

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

namespace Gpio
{
    enum class ModeOutput
    {
        OUTPUT_LOW = GPIO_OUTPUT_LOW, // Configures GPIO pin as output and initializes it to a low state.
        OUTPUT_HIGH = GPIO_OUTPUT_HIGH, // Configures GPIO pin as output and initializes it to a high state.
        OUTPUT_INACTIVE = GPIO_OUTPUT_INACTIVE, // Configures GPIO pin as output and initializes it to a logic 0.
        OUTPUT_ACTIVE = GPIO_OUTPUT_ACTIVE, // Configures GPIO pin as output and initializes it to a logic 1.
    };

    enum class ModeInterrupt
    { 
        DISABLE = GPIO_INT_DISABLE, // Disables GPIO pin interrupt.
        EDGE_RISING = GPIO_INT_EDGE_RISING, // Configures GPIO interrupt to be triggered on pin rising edge and enables it.
        EDGE_FALLING = GPIO_INT_EDGE_FALLING, // Configures GPIO interrupt to be triggered on pin falling edge and enables it.
        EDGE_BOTH = GPIO_INT_EDGE_BOTH, // Configures GPIO interrupt to be triggered on pin rising or falling edge and enables it.
        LEVEL_LOW = GPIO_INT_LEVEL_LOW, // Configures GPIO interrupt to be triggered on pin physical level low and enables it.
        LEVEL_HIGH = GPIO_INT_LEVEL_HIGH, // Configures GPIO interrupt to be triggered on pin physical level high and enables it.
        EDGE_TO_INACTIVE = GPIO_INT_EDGE_TO_INACTIVE, // Configures GPIO interrupt to be triggered on pin state change to logical level 0 and enables it.
        EDGE_TO_ACTIVE = GPIO_INT_EDGE_TO_ACTIVE, // Configures GPIO interrupt to be triggered on pin state change to logical level 1 and enables it.
        LEVEL_INACTIVE = GPIO_INT_LEVEL_INACTIVE, // Configures GPIO interrupt to be triggered on pin logical level 0 and enables it.
        LEVEL_ACTIVE = GPIO_INT_LEVEL_ACTIVE, // Configures GPIO interrupt to be triggered on pin logical level 1 and enables it.
    };

    enum class Drive
    {
        PUSH_PULL = 0,
        OPEN_DRAIN = GPIO_OPEN_DRAIN, // Configures GPIO output in open drain mode (wired AND).
        OPEN_SOURCE = GPIO_OPEN_SOURCE, // Configures GPIO output in open source mode (wired OR).
    };

    enum class Bias
    {
        PULL_NO = 0,
        PULL_UP = GPIO_PULL_UP, // Enables GPIO pin pull-up.
        PULL_DOWN = GPIO_PULL_DOWN, // Enable GPIO pin pull-down.
    };

    enum class Level
    {
        LOW = 0,
        HIGH = 1,
    };
};

class GpioBase
{
    public:
        virtual int IsReady() = 0;
        virtual int Configure(Gpio::Bias const bias, Gpio::Drive const drive) = 0;
        virtual int Disconnect() = 0;
};

class GpioInput : public GpioBase
{
    public:
        GpioInput(const struct device* port, const gpio_pin_t pin);
        GpioInput(const struct gpio_dt_spec gpio_dt);

        int IsReady();
        int Configure(Gpio::Bias const bias = Gpio::Bias::PULL_NO, 
                      Gpio::Drive const drive = Gpio::Drive::PUSH_PULL);
        int ConfigureInterrupt(Gpio::ModeInterrupt const mode_int);
        int Disconnect();
        int IsInput();
        int AddCallback(gpio_callback_handler_t handler);
        int RemoveCallback();
        int Get(Gpio::Level& level);

        int operator () (Gpio::Bias const bias = Gpio::Bias::PULL_NO,
                         Gpio::Drive const drive = Gpio::Drive::PUSH_PULL) 
        {
            return Configure(bias, drive);
        }

        int operator () (Gpio::ModeInterrupt const mode_int) 
        {
            return ConfigureInterrupt(mode_int);
        }

        int operator >> (Gpio::Level& level) 
        {
            return Get(level);
        }

    private:
        const struct device* port_ = nullptr;
        const gpio_pin_t pin_ = 0;
        const gpio_dt_flags_t dt_flags_ = 0;  
        gpio_flags_t flags_ = 0;  
        gpio_flags_t flags_int_ = 0;
        struct gpio_callback gpio_callback_ = {0};
};

class GpioOutput : public GpioBase
{
    public:
        GpioOutput(const struct device* port, const gpio_pin_t pin);
        GpioOutput(const struct gpio_dt_spec gpio_dt);

        int IsReady();
        int Configure(Gpio::Bias const bias = Gpio::Bias::PULL_NO, 
                      Gpio::Drive const drive = Gpio::Drive::PUSH_PULL);
        int Configure(Gpio::ModeOutput const mode, 
                      Gpio::Bias const bias = Gpio::Bias::PULL_NO, 
                      Gpio::Drive const drive = Gpio::Drive::PUSH_PULL);
        int Disconnect();
        int IsOutput();
        int Set();
        int Clear();
        int Toggle();

        int operator () (Gpio::Bias const bias = Gpio::Bias::PULL_NO, 
                         Gpio::Drive const drive = Gpio::Drive::PUSH_PULL) 
        {
            return Configure(bias, drive);
        }

        int operator () (Gpio::ModeOutput const mode, 
                         Gpio::Bias const bias = Gpio::Bias::PULL_NO, 
                         Gpio::Drive const drive = Gpio::Drive::PUSH_PULL) 
        {
            return Configure(mode, bias, drive);
        }

        void operator = (bool state) 
        {
            if (state) {
                Set();
            } else {
                Clear();
            }        
        }

        int operator ! () 
        {
            return Toggle();
        }

        int operator << (bool value) 
        {
            if (value == true) 
                return Set();
            
            return Clear();
        }

    private:

        const struct device* port_ = nullptr;
        const gpio_pin_t pin_ = 0;
        const gpio_dt_flags_t dt_flags_ = 0;  
        gpio_flags_t flags_ = 0;
};

#endif