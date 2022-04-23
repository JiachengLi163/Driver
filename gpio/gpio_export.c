// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2020 Rockchip Electronics Co. Ltd.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/bitops.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/gpio/driver.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/regmap.h>
#include <linux/gpio/consumer.h>

#include "gpiolib.h"

static int export_gpio_probe(struct platform_device *pdev)
{
//	struct device *dev = &pdev->dev;
//	struct device_node *np = pdev->dev.of_node;
    int index = 0, len, ret, index_offset = 0;
    struct gpio_descs *export_gpio;
    const char *name;
    char buffer[100];

    if (of_property_read_string(pdev->dev.of_node, "export-name", &name))
        sprintf(buffer, "%s", "dio");
    else
        sprintf(buffer, "%s", name);

    len = strlen(buffer);

    if (of_property_read_u32(pdev->dev.of_node, "index-offset", &index_offset) < 0)
        index_offset = 0; 
    
    //export dio1-n at /sys/devicess/platform/gpio_function/
    export_gpio = devm_gpiod_get_array(&pdev->dev, "export", GPIOD_OUT_HIGH);

    if (export_gpio) {
        for (; index < export_gpio->ndescs; index++) {
            sprintf(&buffer[len], "%d", index+1 + index_offset);
            gpiod_request(export_gpio->desc[index], buffer);
            gpiod_export(export_gpio->desc[index], true);
            ret = gpiod_export_link(&pdev->dev, buffer, export_gpio->desc[index]); //Set up symlink from /sys/.../dev/name to /sys/class/gpio/gpioN node
        }
    }

    return 0;
	
}

static const struct of_device_id export_gpio_match[] = {
	{ .compatible = "export,gpio-function", },
};

static struct platform_driver export_gpio_driver = {
	.probe		= export_gpio_probe,
	.driver		= {
		.name	= "export-gpio",
		.of_match_table = export_gpio_match,
	},
};

static int __init export_gpio_init(void)
{
	return platform_driver_register(&export_gpio_driver);
}
postcore_initcall(export_gpio_init);

static void __exit export_gpio_exit(void)
{
	platform_driver_unregister(&export_gpio_driver);
}
module_exit(export_gpio_exit);

MODULE_DESCRIPTION(" gpio export driver");
MODULE_ALIAS("export-gpio");
MODULE_LICENSE("GPL v2");
