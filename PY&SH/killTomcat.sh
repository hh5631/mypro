#!/bin/bash

process_name="tomcat"
# 查找进程PID
pid=$(pgrep -f "$process_name")

if [ -z "$pid" ]; then
  echo "未找到进程：$process_name"
else
  # 终止进程
  kill -9 "$pid"
  echo "进程 $process_name (PID: $pid) 已被终止"
fi

filepath='/opt/tomcat/logs/catalina.out'
echo "正在删除"$filepath
rm $filepath

/opt/tomcat/bin/startup.sh