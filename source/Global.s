 

.align 4
.global font , bmp, bg_bmp, color_bmp
.global tasktable, taskstack
font:			/*字体首地址*/
.incbin "./source/font0.bin"

bmp:
.incbin "./bmp/test.bmp"

bg_bmp:
.incbin "./bmp/bg.bmp"

color_bmp:
.incbin "./bmp/color.bmp"

//在这里添加bmp图片


