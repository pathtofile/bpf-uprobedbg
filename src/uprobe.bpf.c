// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/* Copyright (c) 2020 Facebook */
#include <linux/bpf.h>
#include <linux/ptrace.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

#ifndef SIGTERM
#define SIGTERM 15
#endif

char LICENSE[] SEC("license") = "Dual BSD/GPL";

SEC("uprobe/func")
int BPF_KPROBE(uprobe, int a, int b)
{
	bpf_printk("UPROBE ENTRY: a = %d, b = %d\n", a, b);
	return 0;
}

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
