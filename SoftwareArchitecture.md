# 简述 #

  * ezbox软件系统由以下几个部分组成
  1. 内核使用Linux
  1. C函数库使用uClibc
  1. 应用层核心工具集使用Busybox
  1. 图形界面使用Nano-X
  1. 图形开发工具集使用Fltk-1.3

  * ezbox软件系统编译工具集基于openwrt的系统构建工具
  * ezbox核心软件系统使用squashfs文件系统存储，用户扩展软件挂载到/data目录下，并通过overlayfs与核心系统绑定，通常/data目录挂载一个可读写的文件系统

# 详细说明 #

## 设计原则 ##
> ### 简单 ###
  * 我们坚持任何作品都是思想交流的载体，简单是思想易于让对方理解的前提，也是思想得以交流的保障。
> ### 开放 ###
  * 我们承认个体思维的局限性，为了突破这个局限，开放是最有效的途径。只有在开放的环境中，我们的思想才能得以传承发展。
> ### 忠实原作 ###
  * 作为思想的集散地，我们将尽可能保持原创者的思维结晶，任何融合都要保证对原作尽量少的修改。

## 支持的硬件 ##
  * ezbox内核使用Linux，因此理论上只要是Linux内核支持的硬件系统都能够用于构建ezbox。
  * 目前ezbox的开发与测试主要集中在x86体系结构的硬件系统上，测试使用VirtualBox(?1)进行。

## 目录结构 ##
  * ezbox不仅仅是一个嵌入式系统学习平台，同时也以产品化为导向。从用户角度看，系统的内核与核心软件系统构成系统固件部分，通常不可随意修改，因此，我们设计了如下的目录结构(?2)
  * /--（根目录）
  * |--bin/--（系统核心应用软件目录）
  * |--boot/--（系统启动配置文件目录）
  * |--data/--（系统数据存储目录）
  * |--data/rootfs/--（用户当前工作系统存储目录）
  * |--data/backup/--（用户系统备份存储目录）
  * |--dev/--（系统设备节点目录）
  * |--etc/--（系统配置文档目录）
  * |--lib/--（系统核心库目录）
  * |--proc/--（内核运行状态目录）
  * |--sbin/--（系统核心管理软件目录）
  * |--sys/--（内核sysfs文件系统挂载目录）
  * |--tmp/--（系统临时文件目录）
  * |--var/--（系统运行时数据文件目录）
  * |--usr/--
  * |--usr/bin/--（系统常用工具目录）
  * |--usr/sbin/--（系统管理工具目录）
  * |--usr/lib/--（系统运行库目录）
  * |--usr/sbin/--（系统管理工具目录）
  * |--usr/share/--（系统数据存储目录）
  * |--usr/local/--（用户扩展挂载目录）

  * 其中，dev/目录使用tmpfs文件系统，在系统初始化时mdev在该目录下自动产生系统的设备节点文件
  * etc/目录存放系统配置文档，需要在运行时改变，因此也使用tmpfs文件系统，该目录的内容根据所安装的系统软件包在系统初始化时自动产生
  * var/目录存放系统运行时数据文件，它是一个可读写的目录，因此底层文件系统必须支持读写操作，如tmpfs文件系统或ext4文件系统
  * tmp/目录存放系统临时文件，它是一个符号链接指向/var/tmp目录

# 参考资料 #

  * ?1: VirtualBox软件请参考http://www.virtualbox.org/
  * ?2: 软件系统目录结构请参考http://www.pathname.com/fhs/