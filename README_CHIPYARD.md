# Chipyard Integration Guide

This branch contains all modifications needed to run the EKF library on Chipyard RISC-V targets.

## Integration Steps

### 1. Add as Submodule in Chipyard

```bash
cd /path/to/chipyard
git submodule add -b chipyard https://github.com/eminakgun/ekf.git software/ekf
```

### 2. Build

```bash
source env.sh
cd software/ekf
./build.sh
```

## Running

### Spike (Fast Functional Test)
```bash
spike $RISCV/riscv64-unknown-elf/bin/pk build/demo_app
```

### Verilator RTL Simulation
```bash
cd ../../sims/verilator

# Build fast simulator (first time)
make CONFIG=FastRTLSimRocketConfig

# Run
make run-binary-fast CONFIG=FastRTLSimRocketConfig \
    BINARY=../../software/ekf/build/demo_app LOADMEM=1
```

### With VCD Waveforms
```bash
make run-binary-debug CONFIG=FastRTLSimRocketConfig \
    BINARY=../../software/ekf/build/demo_app LOADMEM=1
```

## What This Branch Changes

| File | Change |
|------|--------|
| `CMakeLists.txt` | Added HTIF specs, medany model, rv64imafd arch |
| `riscv_toolchain.cmake` | Changed to `riscv64-unknown-elf-gcc` |
| `build.sh` | New helper script that sources env.sh |

## Troubleshooting

- **Slow simulation**: Use `LOADMEM=1` and `FastRTLSimRocketConfig`
- **No output**: Add `setbuf(stdout, NULL)` at start of main()
- **Timeout**: Reduce iterations in `main.cpp` or increase `+max-cycles`
