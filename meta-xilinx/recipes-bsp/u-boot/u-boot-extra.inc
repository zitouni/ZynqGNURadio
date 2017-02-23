
inherit xilinx-utils

# Appends the '<layer>/conf/machine/boards' path to FILESEXTRAPATHS for all
# layers (using the ${BBPATH})
FILESEXTRAPATHS_append := "${@get_additional_bbpath_filespath('conf/machine/boards', d)}"

# Append the xparameters file to the SRC_URI if set
SRC_URI_append += " ${@paths_affix(d.getVar("MACHINE_XPARAMETERS", True) or '', prefix = 'file://')}"

# Full path to the xparameters.h file
UBOOT_XPARAMETERS ?= "${@expand_workdir_paths("MACHINE_XPARAMETERS", d)}"

# Install the MicroBlaze System configuration into the board configuration,
# and generate a u-boot specific config.mk
python do_configmk_generate () {
    import re, sys, os, shutil

    machine_xparameters = d.getVar("UBOOT_XPARAMETERS", True)

    if machine_xparameters and os.path.exists(machine_xparameters):
        boardpath = os.path.join("board", "xilinx", "microblaze-generic")
        configmk = os.path.join(d.getVar("S", True), boardpath, "config.mk")
        xparameters = os.path.join(d.getVar("S", True), boardpath, "xparameters.h")

        shutil.copyfile(machine_xparameters, xparameters)

        # Search the xparameters.h file for the RAM base addr and size.
        ram = None
        with open(machine_xparameters, "r") as f:
            filedata = f.read()
            ramstart = re.search("XILINX_RAM_START.*?(0x.*)", filedata)
            ramsize = re.search("XILINX_RAM_SIZE.*?(0x.*)", filedata)
            if ramstart and ramsize:
                ram = (int(ramstart.group(1), 16), int(ramsize.group(1), 16))

        # build up the config.mk file from known info
        with open(configmk, "wb") as f:
            f.write("# This file is generated by the meta-xilinx layers.\n")
            f.write("\n")

            for i in d.getVar("TUNE_CCARGS", True).split():
                f.write("PLATFORM_CCPFLAGS += %s\n" % i)
            f.write("\n")

            if ram != None:
                base_offset = ram[0] + ram[1] - 0x400000
                f.write("TEXT_BASE = 0x%x\n" % base_offset)
                f.write("CONFIG_SYS_TEXT_BASE = 0x%x\n" % base_offset)
}
addtask configmk_generate before do_configure after do_unpack
