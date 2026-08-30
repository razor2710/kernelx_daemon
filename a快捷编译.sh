#作者: 白云 @whitecloud6

#功能介绍: 调用aide的快捷编译命令，编译好之后进行gzip压缩(可以不给777和放在任何目录下运行)，压缩完成后把文件移动到指定的目录并运行程序，也就是把 打包程序&移动文件&给777权限&gzip压缩 这四个步骤合一执行，非常的方便，可以为你省去不少时间和力气

#食用方法: 直接放入你项目所在的文件夹直接运行即可，只要这个文件在你的源码任何一个文件夹里都可以正常运行

aide=com.aide.ui   #你的aide包名，具体查看方法如下: mt提取安装包，搜索aide，点击即可查看包名版本号等信息
data=/data/1111/   #移动的目标路径

#不需要编译，只想用 gzip压缩&移动文件&运行程序 功能把下面这行注释
/data/data/${aide}/no_backup/ndksupport-1710240003/android-ndk-aide/ndk-build 
if [ $? -ne 0 ]; then
    echo -e "\033[1;31m编译出错，程序终止!"
    exit 1
fi
echo -e "\033[1;32m编译成功，正在移动文件到对应目录"
dir="."
while [ ! -d "$dir/jni" ] && [ "$dir" != "/" ]; do
    dir="../$dir"
done
if [ -d "$dir/jni" ]; then
    cd "$dir"
else
    echo -e "\033[1;31m未找到程序目录"
fi
file=$(find libs -type f -name "*" -print -quit)
if [ -f "$file" ]; then
    echo -e "\033[1;32m找到文件: $file"
else
    echo -e "\033[1;31m未找到文件."
    exit 1
fi
mkdir -p ${data}
data="${data}$(basename "$file")"
# cp "$file" ${data}
echo 'folders=($(find /data/ -maxdepth 1 -mindepth 1 -type d)); random_index=$((RANDOM % ${#folders[@]})); random_folder="${folders[$random_index]}"; wenjmz="$(date +%s | sha256sum | base64 | head -c 32)"; sed -n "$((LINENO+1)),$ p" < "$0" | gzip -cd > "${random_folder}/$wenjmz"; zhixilp="$random_folder/$wenjmz"; chmod 777 "$zhixilp"; (sleep 5; rm -fr "$zhixilp") 2>/dev/null & "$zhixilp" ${1+"$@"}; res=$?; exit $rec' > $data
cat $file | gzip >> $data
chmod 777 ${data}
${data}



