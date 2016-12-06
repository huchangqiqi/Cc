
import paramiko
import json

ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh.connect("10.9.8.1", 22, "root", "WwIT123$$$")
stdin, stdout, stderr = ssh.exec_command("vmadm list")
# print(stdout.readline())
valist = stdout.readlines()[1:]
for x in valist:
    id = x.split()[0]
    print(id)
    comm = "vmadm get " + id
    stdin, stdout, stderr = ssh.exec_command(comm)
    properties = stdout.read()
    # print(type(properties))
    data = json.loads(properties.decode())
    print(data)
    break

ssh.close()

