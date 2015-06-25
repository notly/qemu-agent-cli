cflags += `pkg-config --cflags --libs json`
glib-flags ?= `pkg-config --cflags --libs glib-2.0`

all: qemu-agent-test qemu-agent-passwd gdecode-base64
## add json-c support
qemu-agent-test: qemu-agent-test.c
qemu-agent-passwd: qemu-agent-passwd.c
	cc qemu-agent-passwd.c ${cflags} -o qemu-agent-passwd
gdecode-base64: gdecode-base64.c
	cc  $^  $(glib-flags) -o $@
## $@ target
## $< first prerequisites 
## $^ all prerequisites 

clean:
	rm -rf qemu-agent-test qemu-agent-passwd  gdecode-base64
