### qemu-agent-command ###

qemu-agent-command <guest agent socket file>

root@hz-cloud:/hz/qemu-passwd# ./qemu-agent-test  /tmp/qga.sock 
qga: { "execute": "guest-set-user-password", "arguments": { "crypted": false,  "username": "root",  "password": "MDA=" }}

return:	{"return": {}}

qga: help

return:	 plz input the json code!! 
like this: 
{"execute": "guest-info"}
qga: {"execute": "guest-info"}

return:	{"return": {"version": "2.3.50", "supported_commands": [{"enabled": true, "name": "guest-get-memory-block-info", "success-response": true}, {"enabled": true, "name": "guest-set-memory-blocks", "success-response": true}, {"enabled": true, "name": "guest-get-memory-blocks", "success-response": true}, {"enabled": true, "name": "guest-set-user-password", "success-response": true}, {"enabled": true, "name": "guest-get-fsinfo", "success-response": true}, {"enabled": true, "name": "guest-set-vcpus", "success-response": true}, {"enabled": true, "name": "guest-get-vcpus", "success-response": true}, {"enabled": true, "name": "guest-network-get-interfaces", "success-response": true}, {"enabled": true, "name": "guest-suspend-hybrid", "success-response": false}, {"enabled": true, "name": "guest-suspend-ram", "success-response": false}, {"enabled": true, "name": "guest-suspend-disk", "success-response": false}, {"enabled": true, "name": "guest-fstrim", "success-response": true}, {"enabled": true, "name": "guest-fsfreeze-thaw", "success-response": true}, {"enabled": true, "name": "guest-fsfreeze-freeze-list", "success-response": true}, {"enabled": true, "name": "guest-fsfreeze-freeze", "success-response": true}, {"enabled": true, "name": "guest-fsfreeze-status", "success-response": true}, {"enabled": true, "name": "guest-file-flush", "success-response": true}, {"enabled": true, "name": "guest-file-seek", "success-response": true}, {"enabled": true, "name": "guest-file-write", "success-response": true}, {"enabled": true, "name": "guest-file-read", "success-response": true}, {"enabled": true, "name": "guest-file-close", "success-response": true}, {"enabled": true, "name": "guest-file-open", "success-response": true}, {"enabled": true, "name": "guest-shutdown", "success-response": false}, {"enabled": true, "name": "guest-info", "success-response": true}, {"enabled": true, "name": "guest-set-time", "success-response": true}, {"enabled": true, "name": "guest-get-time", "success-response": true}, {"enabled": true, "name": "guest-ping", "success-response": true}, {"enabled": true, "name": "guest-sync", "success-response": true}, {"enabled": true, "name": "guest-sync-delimited", "success-response": true}]}}

qga: quit
root@hz-cloud:/hz/qemu-passwd#


### qemu-agent-passwd ###
qemu-agent-passwd <guest agent socket file> <user name[must existes]> <password>

root@hz-cloud:/hz/qemu-passwd# ./qemu-agent-passwd  /tmp/qga.sock  root 00
exec json command: { "execute": "guest-set-user-password", "arguments": { "crypted": false,  "username": "root",  "password": "MDA=" }}
return: {"return": {}}

root@hz-cloud:/hz/qemu-passwd# ./qemu-agent-passwd  /tmp/qga.sock  centos 123
exec json command: { "execute": "guest-set-user-password", "arguments": { "crypted": false,  "username": "centos",  "password": "MTIz" }}
return: {"return": {}}


