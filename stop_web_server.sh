#!/bin/bash
# 1. 查找进程ID
pid=$(ps -ef | grep "web_server" | grep -v grep | awk '{print $2}')

# 2. 杀死进程
kill $pid

