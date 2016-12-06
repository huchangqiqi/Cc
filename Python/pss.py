
import paramiko

ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh.connect("10.9.5.28", 22, "wwit", "WwIT123$$$")
stdin, stdout, stderr = ssh.exec_command("cat /home/wwit/org/todo.org")
print(stdout.readlines())
ssh.close()

