all: qemu-agent-test qemu-agent-passwd

qemu-agent-test: qemu-agent-test.c
qemu-agent-passwd: qemu-agent-passwd.c

clean:
	rm -rf qemu-agent-test qemu-agent-passwd
