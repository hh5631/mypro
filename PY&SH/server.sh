#! /bin/bash

dev=/dev/loop22
image_path=/home/dds/image/upgrade_image_kernel/opt/share/contents/image


#制作镜像，然后开启tgt
echo "是否需要镜像制作过程？(Y/N)"
read flag
if [ "$flag" == "Y" ] || [ "$flag" == "y" ]; then
    echo "正在创建镜像......................................................."
    sudo mkdir -p /mnt/media/iscsi
    sudo dd if=/dev/zero of=/mnt/media/iscsi/tris.img bs=512 count=30000000
    sudo losetup -o 1048576 $dev /mnt/media/iscsi/tris.img
    sudo tune2fs -U 00000000-0000-0000-0000-000000000001 $dev
    sudo mkfs.ext4 $dev
else
    echo "已跳过镜像制作过程---------------------------------"
fi


#tgt
echo "是否需要tgt安装和配置？ (Y/N)"
read flag1
if [ "$flag" == "Y" ] || [ "$flag" == "y" ]; then
    echo "正在安装tgt,写入配置................................................"
    apt-get install tgt -y
    echo "<target iqn.2020-12.com.toshiba:tris-server>
    backing-store /mnt/media/iscsi/tris.img
    </target>">>/etc/tgt/conf.d/tris.conf   
    systemctl restart tgt
else
    echo "已跳过tgt安装和配置-----------------------------------"
fi


#挂载镜像，然后向镜像中安装驱动
echo "是否需要挂载镜像和驱动拷贝过程？(Y/N): "
read choice
if [ "$choice" == "Y" ] || [ "$choice" == "y" ]; then
    echo "开始挂载镜像..........................................."
    sudo mkdir /mnt/media/iscsi/tris
    sudo mount -t ext4 -o loop,offset=1048576 /mnt/media/iscsi/tris.img /mnt/media/iscsi/tris
    echo "请手动删除镜像中/etc/fstab文件中'UUID=4A00-31B3  /boot/efi       vfat    umask=0077      0       1'"
    echo "开始从image文件中拷贝驱动...................."
    sudo cp ${image_path}/accessory/testprepare/setif /mnt/media/iscsi/tris/
    sudo chmod 777 /mnt/media/iscsi/tris/setif
    sudo cp ${image_path}/accessory/etc/network/interfaces /mnt/media/iscsi/tris/etc/network/interfaces
    sudo cp ${image_path}/accessory/usr/bin/mystartx /mnt/media/iscsi/tris/usr/bin
    sudo chmod 777 /mnt/media/iscsi/tris/usr/bin/mystartx
    sudo cp ${image_path}/accessory/etc/rc.local /mnt/media/iscsi/tris/etc
    sudo cp ${image_path}/accessory/etc/systemd/logind.conf /mnt/media/iscsi/tris/etc/systemd/logind.conf
    sudo cp ${image_path}/firmware/* /mnt/media/iscsi/tris/lib/firmware
    sudo cp ${image_path}/accessory/usr/lib/libqwt.so.6 /mnt/media/iscsi/tris/usr/lib
    sudo cp -r ${image_path}/accessory/root/3D-Heaven /mnt/media/iscsi/tris/root/
else
    echo "已跳过挂载镜像和驱动拷贝过程--------------------------------------------"
fi

#chroot到tris目录，然后安装驱动
echo "是否需要chroot？(Y/N): "
read flag2
if [ "$flag2" == "Y" ] || [ "$flag2" == "y" ]; then
    chroot /mnt/media/iscsi/tris
else
    echo "已跳过chroot---------------------------------"
fi