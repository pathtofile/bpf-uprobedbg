# BPF-UprobeDBG
Super simple example of how you can raise signals in eBPF Uprobe programs, to be trapped by a debugger.
This means you can use eBPF to determine when to break into a program.

90% of the code is from [Libpf-Bootstrap](https://github.com/libbpf/libbpf-bootstrap)

# Build
```bash
git clone --recursive https://github.com/pathtofile/bpf-uprobedbg.git
cd bpf-uprobedbg/src
make
```

# Running
To show how it works, run via `gdb`. When the loop runs for the 3rd time,
the program will throw a `SIGTERM`, which will be trapped by GDB so you can inspect it:
```bash
sudo gdb ./uprobe
```
Run the program, and gdb will break when the eBPF program raises the SIGINT.

In a seperate window, you can also run the following to see the debug logs from the bpf program:
```bash
sudo cat /sys/kernel/debug/tracing/trace_pipe
```

# Aknowledgements
- [@williballenthin](https://twitter.com/williballenthin) for the idea!
- [Libpf-Bootstrap](https://github.com/libbpf/libbpf-bootstrap) team
