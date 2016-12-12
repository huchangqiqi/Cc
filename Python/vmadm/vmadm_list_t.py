
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
        self.vmlist = {}

    def list_vm(self):
        comm = "vmadm list"
        stdin, stdout, stderr = self.ssh.exec_command(comm)
        valist = stdout.readlines()[1:]
        return valist

    """
        keys = stdout.readline().split()
        valist = stdout.readlines()
        for line in valist:
            v_dict = {}
            values = line.split()
            for i in range(len(keys)):
                v_dict[keys[i]] = values[i]

    """

    def create_new_vm(self, zone):
        # zonejs = json.loads(zone)
        print("starting create vm")
        comm = "vmadm create -f " + zone
        print(comm)
        stdin, stdout, stderr = self.ssh.exec_command(comm)

    def get_all_uuid(self, valist):
        # vmuuid
        for x in valist:
            self.vmuuid.add(x.split()[0])
        return self.vmuuid

    def get_a_vm_properties(self, id):
        comm = "vmadm get " + id
        stdin, stdout, stderr = self.ssh.exec_command(comm)
        properties = stdout.read()
        data = json.loads(properties.decode('utf-8'))
        return data

    def get_all_vm_properties(self, vmuuids):
        # vminfo = {}
        for id in vmuuids:
            self.vminfo['id'] = self.vm_get_info(id)
        return self.vminfo

    # nics = data['nics']
    # ips = nics[0]['ips'][0]
    # print("uuid = %s,ips = %s " % (id, ips))

    def lookup_a_vm_by_ip(self, ip):
        comm = "vmadm lookup -j nics.*.ip=" + ip
        stdin, stdout, stderr = self.ssh.exec_command(comm)

    # update from comm
    def update_a_vm(self, id, **kwargs):
        comm = ''
        for key in kwargs:
            comm += key + "=" + '%d' % kwargs[key]
        comm = "vmadm update " + id + comm
        stdin, stdout, stderr = self.ssh.exec_command(comm)

    def delete_a_vm(self, uuid):
        print("starting delete ...")
        comm = "vmadm delete " + uuid
        stdin, stdout, stderr = self.ssh.exec_command(comm)


    def exit(self):
        self.ssh.close()


def imgadm(object):

    def __init__(self):
        pass


if __name__ == '__main__':
    vmadm = vmadm()
    # zone = '/sysimg/kvm03'
    # vmadm.create_new_vm(zone)
    # uuid = '79b7ae12-0b3c-e425-c65c-8b0e6bb4bc44'
    uuid2 = '3a55c47d-e4d9-e647-c1db-9a689d36de87'
    vmadm.delete_a_vm(uuid2)
    print(vmadm.list_vm())

#
#    with open('./kvm03', 'r') as f:
#        zonejs = f.read()
#        print(zonejs)
#        vmadm.create_new_vm(zonejs)
#        print(vmadm.list_vm)
    vmadm.exit()
