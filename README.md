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


# How it works:
The loader attaches to this function in its own program:
```c++
int uprobed_function(int a, int b)
{
	return a + b;
}
```

Once the eBPF program is loaded, it will call this function in a loop:
```c++
	for (i = 0; ; i++) {
		ret = uprobed_function(i, i + 1);
		sleep(1);
	}
```

The eBPF Program attaches to the function looks like this:
```c++
SEC("uretprobe/func")
int BPF_KRETPROBE(uretprobe, int ret)
{
	bpf_printk("UPROBE EXIT: return = %d\n", ret);
	// Raise a signal when return reaches a threshold
	if (ret == 5) {
		bpf_printk("Raising SIGTERM\n");
		bpf_send_signal_thread(SIGTERM);
	}
	return 0;
}
```
i.e., when `uprobed_function` returns a 5, raise a `SIGTERM` signal.

If not being debugged, the program will trap this signal itself and cleanly shut down.

But if run using `gdb` or another debugger, this will trigger the breakpoint:
```bash
sudo gdp ./uprobe
(gdb) r
# the loader will print a bunch of stuff until the breakpoint is triggered
Program received signal SIGTERM, Terminated.
0x0000555555558355 in main (argc=1, argv=0x7fffffffe668) at uprobe.c:148
148                     ret = uprobed_function(i, i + 1);
(gdb) p ret
$1 = 3
(gdb) p i
$2 = 2
```


# Aknowledgements
- [@williballenthin](https://twitter.com/williballenthin) for the idea!
- [Libpf-Bootstrap](https://github.com/libbpf/libbpf-bootstrap) team
