# Embeddeed Linux Build System(嵌入式Linux构建系统)
## 含义：
> 针对特定嵌入式设备构建`定制化的Linux操作系统`,它提供了一整套完整的工具和流程，来管理整个构建流程。
## 关键步骤：
### 1.针对选择的Embedded System CPU架构来构建交叉编译器
> `交叉编译器`是一种在一种计算机体系结构上编译程序，而该程序在另一种体系结构上运行的编译器。在嵌入式Linux开发中，由于目标硬件（如CPU架构、内存等）与开发环境（如x86 PC）的架构不同，因此需要构建交叉编译器。
- `选择目标架构`：确定嵌入式系统所使用的cpu架构
- `下载源代码`：获取适合目标架构的交叉编译器源代码，如binutils、gcc、glibc等。
- `配置和编译`：根据目标架构配置这些源代码，并编译生成交叉编译器。这通常涉及设置环境变量、指定安装路径等。
### 2.管理bootloader/kernel/system programs配置
> `Bootloader`是嵌入式系统启动时首先运行的软件，负责初始化硬件设备、加载操作系统等。`Kernel`是操作系统的核心，管理系统的硬件和软件资源。`System programs`是操作系统提供的各种工具和应用程序。
- `配置Bootloader`：选择合适的Bootloader(如U-Boot)，并根据目标硬件的配置对其进行配置
- `配置kernel`：下载linux内核源代码，并根据目标系统的需求进行配置(如使用`make menuconfig`)
- `配置System programs`:根据需求选择合适的系统程序和库，并进行配置和编译
### 3.管理bootloader/kernel/system programs编译
> 配置完成后，需要编译这些程序以`生成可执行文件或镜像文件`
- `编译Bootloader`：使用交叉编译器编译Bootloader源代码，生成可执行的Bootloader镜像
- `编译kernel`：使用交叉编译器和之前配置的kernel配置文件编译linux内核，生成zlmage或bzlmage等格式的内核镜像
- `编译System programs`：使用交叉编译器编译系统程序，生成可在目标系统上运行的可执行文件
### 4.负责准备操作系统所需的root filesystem和boot device images
> `Root filesystem`是Linux系统启动时挂载的根目录，包含了系统运行所需的所有文件和目录。`Boot device images`是包含了Bootloader和Kernel镜像的启动镜像，用于从非易失性存储设备（如NAND Flash、SD卡等）启动系统
- `构建Root filesystem`：根据需求选择合适的文件系统和工具(如Buildroot、Yocto Project等)，构建包含必要文件和库的Root filesystem
- `生成Boot device images`：`将Bootloader镜像、Kernel镜像和Root filesystem打包成适合目标硬件的启动镜像`。这可能涉及到创建分区、设置引导扇区等步骤。
- `测试和验证`：在目标硬件上测试生成的启动镜像，确保系统能够正确启动并运行。
## Buildroot
> Buildroot 是一个为嵌入式系统提供轻量级、易于使用的交叉编译环境的工具。它自动化了许多构建嵌入式Linux系统的任务，包括交叉编译工具链、内核、库、引导加载程序（bootloader）和根文件系统的构建
### 1.安装Buildroot并解压缩
>` mkdir ~/train/buildroot`
`wget https://buildroot.org/downloads/buildroot-2024.02.3.tar.gz`
`tar zxvf buildroot-2024.02.3.tar.gz`
## 2.列出当前buildroot支持的target board(嵌入式设备)
- `make list-defconfigs`指令
- config档案在`buildroot/configs/<boardname>_defconfig`
## 3.选择要使用的\<boardname>_defconfig
- buildroot通过读取.config内容进行编译，.config和\<boardname>_defconfig内容不相同
- 使用`make <boardname>_defconfig`指令，将\<boardname>_defconfig文件展开后复制成.config
### 4.配置Buildroot
> `make menuconfig`进入图形化菜单配置各种选项
在菜单中可以：
- 选择目标架构(如ARM/MIPS等)
- 配置交叉编译工具链(buildroot可自动构建)
- 选择和配置Linux内核(buildroot支持使用外部内核或下载并构建自己的内核)
- 选择和配置bootloader(如u-boot)
- 配置根文件系统，包括要包含的文件和库
### 5.构建系统
> `make`

这个命令将开始`构建交叉编译工具链（如果尚未构建）、内核、bootloader 和根文件系统`
## Yocto
### 概述
> Yocto项目本质是一个构建工具，其帮助开发者能够快速地、简单地`设计一个基于Linux的系统`。该项目由Linux基金会赞助，于2010年启动。同时，在谈到Yocto项目的时候，就不得不提到`OpenEmbedded项目`，该项目源于夏普的一个开源项目OpenZaurus(服务于SL-5000D)，后来结合了其他几个类似的嵌入式项目（Familiar Linux， OpenSIMpad），其目的是`为嵌入式系统构建Linux发行版`。OpenEmbedded项目维护了一个构建工具`BitBake`和一个`元数据层`（详细描述了构建一个Linux发行版所需要的包及构建过程）。`Poky` Linux是OpenEmbedded项目的一个扩展应用，同时也是Yocto项目的前身。在2011年的时候，OpenEmbedded项目宣布于Yocto项目进行合作，共同维护BitBake以及一个核心数据层
### 核心组件
- Recipes(配方)：以元数据的形式存在，用于`描述如何构建软件包`。Recipes为构建系统提供了构建特定软件包所需的所有信息，包括`下载源代码的位置、配置选项、编译参数、依赖关系`等。
- Metadata（元数据）：在Yocto Project中，Metadata指的是`描述软件包、配方（recipes）、类（classes）、图像（images）等构建项目所需信息的文件集合`。这些文件（通常是XML或BitBake（Yocto Project的构建系统）的.bb或.inc文件）包含了构建系统如何找到、下载、编译和集成软件组件到最终产品镜像中的指令和配置。Metadata是Yocto Project的核心，它使得定制Linux发行版变得简单和灵活。
- Poky：Poky是`Yocto Project的一个参考发行版`（Reference Distribution），它提供了一套基本的Metadata，用于`构建和测试Yocto Project的组件`。Poky不是一个面向最终用户的产品级发行版，而是一个高度可定制的起点，用于`演示如何定制Linux发行版、测试Yocto Project的组件`，以及作为`用户下载Yocto Project的媒介`。Poky建立在oe-core之上，oe-core是Yocto Project的开放嵌入式核心，包含了构建系统（BitBake）和构建系统所需的许多基础类和工具。
- BitBake：BitBake是一个`任务调度器和执行系统`，用于`解析和执行Metadata`。它能够评估所有可用的元数据，管理动态变量扩展、依赖关系和代码生成，从而生成并运行任务。
- OpenEmbedded Core:OpenEmbedded Core是Poky构建系统的核心，提供了核心功能，并支持多种处理器架构。
- Layers：Layers是各种meta-xxx目录，用于`将Metadata按层进行分类`，有助于项目的维护。它们告诉OpenEmbedded构建系统如何构建目标，并保存特定硬件的所有配置
### 使用方法
#### 1.环境准备
- 确保系统满足Yocto的要求，如足够的存储空间、内存和处理器性能
- 安装必要的依赖包，如Git、Python等
#### 2.获取Yocto项目
- 通过Git克隆Yocto项目的仓库，如Poky
- 切换到所需的Yocto版本
#### 3.配置构建环境
- 运行`oe-init-build-env`脚本来初始化构建环境
- 编辑`conf/local.conf`文件，配置构建选项，如目标硬件架构、使用的交叉编译器等
#### 4.构建Yocto
- 使用`BitBake`命令来构建Yocto镜像，例如`bitbake <image-name>`，其中\<image-name>是想要构建的镜像名称
- 根据需要，可以创建新的Layers来组织和扩展Recipes
#### 5.部署镜像
- 将生成的镜像烧录到目标设备中。
- 进行测试和部署，确保系统按预期工作

