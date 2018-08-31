#!/bin/bash  

process_name="chatserver"
if [ $# -lt 1 ];
then
  echo "usage:"
  echo "	./chatshell.sh [start] [stop]"
  echo "	start start $process_name"
  echo "	stop kill $process_name"
  exit 1
fi 

if [[ $1 == "start" ]];
then
	ps -ef|grep $process_name |grep -v grep|grep -v PPID 
	if [ $? -ne 0 ];
	then
		cd bin
		echo "start $process_name ..."
		./chatserver
	else
		echo "$process_name is running"
	fi
fi 

if [[ $1 == "stop" ]];
then 
	process=`ps -ef|grep $process_name |grep -v grep|grep -v PPID|awk '{ print $2}'`
	for i in $process 
	do
		echo "Kill the chatserver process [$i]"
		kill -9 $i
	done
fi 

