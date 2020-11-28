#!/bin/sh  
exe="H_G" #你需要发布的程序名称
des="/home/chao/Desktop/H_G_realease" #创建文件夹的位置
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')  
cp $deplist $des
