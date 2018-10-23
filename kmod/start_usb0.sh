#!/bin/sh
set -e

GADGET_DIR=/sys/kernel/config/usb_gadget/g1

modprobe libcomposite
modprobe usb_f_ecm

mount -t configfs none /sys/kernel/config
mkdir ${GADGET_DIR}
cd ${GADGET_DIR}
echo 0x05e8 > idVendor
echo 0xa4a1 > idProduct
mkdir strings/0x409
echo serialnumber > strings/0x409/serialnumber
echo manufacturer > strings/0x409/manufacturer
echo ECM Gadget > strings/0x409/product
mkdir functions/ecm.usb0
mkdir configs/c.1
mkdir configs/c.1/strings/0x409
echo Conf 1 > configs/c.1/strings/0x409/configuration
echo 120 > configs/c.1/MaxPower
ln -s functions/ecm.usb0 configs/c.1
echo musb-hdrc.0 > UDC

ifup usb0

sleep 2
/etc/init.d/dhcp-server start
