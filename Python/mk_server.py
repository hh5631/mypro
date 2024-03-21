import os
import sys
from PyQt6.QtWidgets import *
from PyQt6 import uic


if __name__=="__main__":

        app = QApplication(sys.argv)
        w = uic.loadUi("./makeKernel.ui")
        dev="/dev/loop22"
        image_path="/home/dds/image/upgrade_image_kernel/opt/share/contents/image"
        def deal():
                w.textEdit.setText("");
                if w.checkBox.isChecked():
                        w.textEdit.setText("镜像制作---------------")
                        os.system("echo '正在创建镜像.......................................................'")
                        os.system("sudo mkdir -p /mnt/media/iscsi")
                        os.system("sudo dd if=/dev/zero of=/mnt/media/iscsi/tris.img bs=512 count=30000000")
                        os.system("sudo losetup -o 1048576 $dev /mnt/media/iscsi/tris.img")
                        os.system("sudo tune2fs -U 00000000-0000-0000-0000-000000000001 $dev")
                        os.system("sudo mkfs.ext4 $dev")
                if w.checkBox_2.isChecked():
                        w.textEdit.append("tgt安装和配置---------------")
                        os.system("echo '正在安装tgt，写入配置.......................................................'")
                        os.system("apt-get install tgt -y")
                        os.system("echo '<target iqn.2020-12.com.toshiba:tris-server><br>backing-store /mnt/media/iscsi/tris.img<br></target>'>>/etc/tgt/conf.d/tris.conf ")  
                        os.system("systemctl restart tgt")
                if w.checkBox_3.isChecked():
                        w.textEdit.append("挂载镜像---------------")
                        os.system("echo '开始挂载镜像...........................................'")
                        os.system("sudo mkdir /mnt/media/iscsi/tris")
                        os.system("sudo mount -t ext4 -o loop,offset=1048576 /mnt/media/iscsi/tris.img /mnt/media/iscsi/tris")
                if w.checkBox_4.isChecked():
                        w.textEdit.append("拷贝驱动---------------")

                        image_path = "/path/to/image"  # Replace with the actual image path

                        os.system("sudo rm /boot/efi")
                        os.system("echo '开始从image文件中拷贝驱动....................'")
                        os.system("sudo cp {}/accessory/testprepare/setif /mnt/media/iscsi/tris/".format(image_path))
                        os.system("sudo chmod 777 /mnt/media/iscsi/tris/setif")
                        os.system("sudo cp {}/accessory/etc/network/interfaces /mnt/media/iscsi/tris/etc/network/interfaces".format(image_path))
                        os.system("sudo cp {}/accessory/usr/bin/mystartx /mnt/media/iscsi/tris/usr/bin".format(image_path))
                        os.system("sudo chmod 777 /mnt/media/iscsi/tris/usr/bin/mystartx")
                        os.system("sudo cp {}/accessory/etc/rc.local /mnt/media/iscsi/tris/etc".format(image_path))
                        os.system("sudo cp {}/accessory/etc/systemd/logind.conf /mnt/media/iscsi/tris/etc/systemd/logind.conf".format(image_path))
                        os.system("sudo cp {}/firmware/* /mnt/media/iscsi/tris/lib/firmware".format(image_path))
                        os.system("sudo cp {}/accessory/usr/lib/libqwt.so.6 /mnt/media/iscsi/tris/usr/lib".format(image_path))
                        os.system("sudo cp -r {}/accessory/root/3D-Heaven /mnt/media/iscsi/tris/root/".format(image_path))
                if w.checkBox_5.isChecked():
                        w.textEdit.append("chroot---------------")
                        os.system("sudo chroot /mnt/media/iscsi/tris /bin/bash")                       

                if w.checkBox_6.isChecked():
                        w.textEdit.append("执行install.sh---------------")
                        os.system("./install.sh")


        w.buttonBox.accepted.connect(deal)
        w.show()
        app.exec();

