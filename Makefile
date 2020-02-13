
# 

#默认参数
DIR_SRC ?= ./source
DIR_INC ?= ./include
DIR_OBJ ?= ./object
DIR_LIB ?= ./lib

TARGET ?= kernel
#
#（1）Makefile中的 符号 $@, $^, $< 的意思：
#　　$@  表示目标文件
#　　$^  表示所有的依赖文件
#　　$<  表示第一个依赖文件
#　　$?  表示比目标还要新的依赖文件列表
#
#（2）wildcard、notdir、patsubst的意思：
#
#　　wildcard : 扩展通配符
#　　notdir ： 去除路径
#　　patsubst ：替换通配符

GNU = arm-none-eabi-
CFLAGS +=   -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s \
			-nostartfiles   -g -Wl,--verbose -c -I ${DIR_INC} 
LFLAGS +=    -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s \
			-nostartfiles    -g -Wl,-T,${DIR_SRC}/pi.x -Wl,-Map,${TARGET}.map 


SRC = $(wildcard  ${DIR_SRC}/*.c)
ASB = $(wildcard  ${DIR_SRC}/*.s)
OBJ = $(patsubst  %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))
#暂时不链接USB驱动
INC = $(wildcard ${DIR_INC}/*.h)

all: 
	make kernel.img

	
${DIR_OBJ}/%.o: ${DIR_SRC}/%.c  Makefile
	${GNU}gcc ${CFLAGS} -c $< -o $@
	
${TARGET}.img: Makefile  ${TARGET}.elf
	${GNU}objcopy  ${TARGET}.elf -O binary $@

${TARGET}.elf:${OBJ}   ${ASB} ${DIR_SRC}/pi.x  
#${TARGET}.elf: Makefile ${OBJ}  ${ASB} ${DIR_SRC}/pi.x   
	@echo ${OBJ}   ${LIB} ${ASB}  ${SRC} ${GNU}
	${GNU}gcc ${LFLAGS} ${OBJ} ${ASB} -L ${DIR_LIB}   -o ${TARGET}.elf  
	#${GNU}gcc ${LFLAGS} ${OBJ} ${ASB}  -o ${TARGET}.elf  
	
disasm:${TARGET}.elf 
	${GNU}objdump -S  $< > ${TARGET}.disasm

clean:
	rm -rf  ./object/*.o
	rm -rf *.img *.elf *.disasm *.map

install:kernel.img
	sudo mkdir /media/breaker/boot
	sudo mount /dev/sdc1 /media/breaker/boot
	sudo cp kernel.img /media/breaker/boot
	sudo umount /dev/sdc1
	sudo rm -r /media/breaker/boot 
rm:
	sudo rm -rf /media/breaker/boot



