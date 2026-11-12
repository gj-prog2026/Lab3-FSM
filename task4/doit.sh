rm -rf obj_dir
rm -f top.vcd

~/Documents/iac/lab0-devtools/tools/attach_usb.sh || echo "attach_usb returned non-zero (ok if already attached)"
sleep 2

verilator -Wall -Wno-fatal --cc --trace rtl/top.sv rtl/f1_fsm.sv rtl/clktick.sv rtl/delay.sv rtl/lfsr_7.sv --exe top_tb.cpp || { echo "VERILATOR FAILED"; exit 1; }

make -j -C obj_dir/ -f Vtop.mk Vtop || { echo "MAKE FAILED"; exit 1; }

obj_dir/Vtop