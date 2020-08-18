#!/bin/bash
cd `dirname $0`

# 编译
echo "开始编译"
mkdir -p build
qmake src/one-cuter.pro -spec linux-g++ CONFIG+=qtquickcompiler -o build/
cd build/
make
cd ..
echo "编译完成"
echo "-------------------"
#echo "更新翻译"
#lrelease ./translations/*.ts

#打包
echo "构建软件包"
mv build/OneCuteR dabao/extract/opt/apps/top.yzzi.onecuter/files/
#cp translations/*.qm dabao/extract/opt/apps/top.yzzi.onecuter/files/translations/
rm -rf build
sh dabao/dabao.sh
mkdir -p outdeb
mv dabao/build/* outdeb/
rm -rf dabao/build
