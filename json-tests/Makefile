all:  qemu-agent-key github_commits

qemu-agent-key: qemu-agent-key.c qemu-b64.c qemu-b64.h
	gcc qemu-b64.c  qemu-agent-key.c -o qemu-agent-key

github_commits: github_commits.c
	gcc -o github_commits github_commits.c -ljansson -lcurl
clean:
	rm -rf github_commits qemu-agent-key
