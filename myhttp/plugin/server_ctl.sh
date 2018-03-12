#!bin/bash
ROOT_PATH=$(shell pwd)
BIN=$ROOT_PATH/httpd
CONF=$ROOT_PATH/conf/server.conf

proc=$(basename $0)

server_pid=''

function usage()
{
	peintf "%s [start(-s) | stop(-t) | restart(-rs) ]\n" "$(proc)"
}

function is_exist()
{
	name=$(basename $BIN)
	server_pid=$(pidof $name)
	if [  $? -eq 0 -a $(server_pid) -gt 0 ];then
		return 0
	else
		return 1
	fi
}

function restart_server()
{
	if is_exist;then
		echo "server is exist,pid is:$server_pid"
	else
		ip=$(awk -F:'/^ip/{print $NF}' $CONF)
		port=$(awk -F: '/^port/{print $NF}') $CONF
		$BIN $ip $port
		echo "start ...done"
	fi
}

function stop_server()
{
	if is_exist;then
		kill -9 $server_pid
		start_servet
	else
		echo "servet not exist,please don not stop!"
	fi
}




if [ $# -ne 1 ];then
	usage
	exit 1
fi

case $1 in
	start | -s )
		start_server
	;;
	stop | -t )
		stop_servr
	;;
	restart | -rs )
		restart_server
	;;
esac

