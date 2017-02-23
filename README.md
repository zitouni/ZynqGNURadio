# ZynqGNURadio

This page provides an overview of the FIR Filter FPGA accelerator example in GNU Radio with the Zynq SoC and a tutorial on how to setup the necessary hardware and software.

Hardware and Software Setup Tutorial:

Prerequisite Hardware and Software
Zynq Development Board:

Zedboard
Low cost educational board in the spirit of RaspberryPi / Beagleboard
ZC7020 Speed Grade -1, 667 MHz processor clock, 512MB RAM
For most users the Zedboard is a good choice

ZC702
Mid range development board
Same Zynq device as Zedboard, 1GB RAM

ZC706
High end development board
Faster Zynq device with more FPGA resources
ZC7045 Speed Grade -2, 766 MHz ARM processor, 1GB RAM
Important: FPGA design software requires a license i.e. not free!

Xilinx ISE Software
FPGA software suite needed to synthesize HDL designs
Choose ISE Design Tools (version v14.6 or higher), not Vivado
(Optional) Icarus Verilog Simulator
Useful tool for simulating and debugging FPGA HDL code

Building the Linux Kernel, U-Boot, & Root Filesystem with OpenEmbedded:

1/ entre in the oe-repo repository

Run the OE initialization script:

	TEMPLATECONF=`pwd`/meta-zynq-gnuradio/conf source ./oe-core/oe-init-build-env ./build ./bitbake

2/The default OE machine is set to zedboard, but if you are using a ZC702 or ZC706 make sure to update the MACHINE environmental variable.

      	export MACHINE="zc702-zynq7" 

Run bitbake to build everything we will need (kernel, u-boot, rootfs, and boot.bin) to boot our development board. The root file system will include GNU Radio, SSH, and other useful development tools.
Note: Bitbake takes several hours and uses ~40 GB of hard drive space

   	bitbake gnuradio-dev-imager

The output files end up in oe/build/tmp-eglibc/deploy/images/$MACHINE/. The directory will contain many files. The files we need are:


**uImage: Linux kernel with modified header for U-Boot
**u-boot.elf: Das U-Boot boot loader
**uImage-$MACHINE-user-peripheral.dtb: Device tree blob
**gnuradio-dev-image-$MACHINE.tar.gz: Root filesystem
**BOOT-$MACHINE.BIN: Xilinx special boot file that includes the first stage boot loader and FPGA bit stream
**uEnv-$MACHINE.txt: Plain text file to set U-Boot environmental variables to boot from the SD card

2/refere to the gnuradio example to prepare the SD Card.

http://gnuradio.org/redmine/projects/gnuradio/wiki/Zynq#Warning-Instructions-of-out-date

3/ Build Instructions for the FPGA Bit Stream, First Stage Boot Loader.

****** refere to the gnuradio example section Appendix A 

4/Build BOOT.BIN :

When booting the Zynq, it looks for a special file called boot.bin on the SD card. The Xilinx program bootgen creates this file using a configuration file called boot.bif. The script build_boot.sh automates this process.

     cd zynq-acp/top/zedboard/boot/
     ./build_boot.sh


5/Copy Files to SD Card:

BOOT partition files

	cd zynq-acp/top/zedboard/boot/
	cp BOOT-$MACHINE.BIN /<path-to-sd-card>/BOOT/BOOT.BIN
	cp uEnv-$MACHINE.txt /<path-to-sd-card>/BOOT/uEnv.txt
	cp uImage /<path-to-sd-card>/BOOT/uImage
	cp uImage-$MACHINE-user-peripheral.dtb /path-to-sd-card/BOOT/$MACHINE.dtb

rootfs partition files:

        cd oe-repo/build/tmp-eglibc/deploy/images/$MACHINE/
	sudo tar -C /<path-to-sd-card>/rootfs/ -xzpf gnuradio-dev-image-$MACHINE.tar.gz
	sudo tar -C /<path-to-sd-card>/rootfs/ -xzpf module
You can also copy gr-zynq and the zynq-fir-filter-example files as well:

 	cd oe-repo/
	cp -r gr-zynq /<path-to-sd-card>/rootfs/home/root/
	cp -r zynq-fir-filter-example /<path-to-sd-card>/rootfs/home/root/

after this steps you can insert the sd card in the ZC702 board and boot it to execute the fir filter example

for the next steps on the board see section "Switching to the Zynq Development Board" in the gnuradio website example " http://gnuradio.org/redmine/projects/gnuradio/wiki/Zynq#Warning-Instructions-of-out-date" .

 





