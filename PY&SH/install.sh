echo nameserver 114.114.114.114 >  /etc/resolv.conf
    apt-get update
    echo "开始下载安装驱动.................................."
    apt-get install nfs-common ffmpeg sysstat lm-sensors fio -y
    apt install vlc -y
    sed -i 's/geteuid/getppid/' /usr/bin/vlc
    apt install cheese -y
    apt install smartmontools -y
    apt-get install libtbb2 -y
    apt-get install freeglut3 -y
    apt-get purge libreoffice? -y
    echo "余下内容请自行下载或者拷贝：
    1.### install QT(20.04), 2202 not need, lib is in image accessory

    - [http://ubuntuhandbook.org/index.php/2020/07/install-qt4-ubuntu-20-04/](http://ubuntuhandbook.org/index.php/2020/07/install-qt4-ubuntu-20-04/)
    - 
    > add-apt-repository ppa:rock-core/qt4
    - 
    > apt update
    - 
    > apt install libqt4-default

- 然后拷贝原来的 libqwt.so 到 /usr/lib/libqwt.so.6

    2.修改一下 /usr/lib/systemd/system/upower.service 中的 #Namespaces的配置，注释掉下面两行
            #Namespaces
            # PrivateUsers=yes
            # RestrictNamespaces=yes
    然后执行journalctl -xe

    3.拷贝几个lib（貌似是intel的math库）到 /usr/lib 下：libimf.so libintlc.so.5 libirng.so libsvml.so
"  