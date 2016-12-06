
import paramiko
import json

"""
class conn(object):

    def __init__(self):
        ip = '10.9.8.1'
        port = 22
        usr = 'root'
        passwd = 'WwIT123$$$'
        self.ssh = paramiko.SSHClient()
        self.ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        self.ssh.connect(ip, port, usr, passwd)

    def ssh(self):
        return self.ssh

"""


class vmadm(object):
    #  __conn = conn()

    def __init__(self, ip='10.9.8.1', port=22, usr='root',
                 passwd='WwIT123$$$'):
        self.ssh = paramiko.SSHClient()
        self.ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        self.ssh.connect(ip, port, usr, passwd)

        self.vmuuid = set()
        self.vminfo = {}

    def vm_list(self):
        stdin, stdout, stderr = self.ssh.exec_command("vmadm list")
        valist = stdout.readlines()[1:]
        return valist

    def vm_uuid(self, valist):
        # vmuuid
        for x in valist:
            self.vmuuid.add(x.split()[0])
        return self.vmuuid

    def vm_get_info(self, id):
        comm = "vmadm get " + id
        stdin, stdout, stderr = self.ssh.exec_command(comm)
        properties = stdout.read()
        data = json.loads(properties.decode('utf-8'))
        return data

    def vm_get_all_info(self, vmuuids):
        # vminfo = {}
        for id in vmuuids:
            self.vminfo['id'] = self.vm_get_info(id)
        return self.vminfo

            # nics = data['nics']
            # ips = nics[0]['ips'][0]
            # print("uuid = %s,ips = %s " % (id, ips))

    def exit(self):
        self.ssh.close()


def imgadm(object):

    def __init__(self):
        pass


if __name__ == '__main__':
    # conn = conn()
    vmadm = vmadm()
    id = vmadm.vm_uuid(vmadm.vm_list())
    print(vmadm.vm_get_all_info(id))
    vmadm.exit()

