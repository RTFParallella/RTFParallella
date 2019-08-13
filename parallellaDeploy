#!/usr/bin/env bash
echo "Parallella Deployment Script version 1.0"
echo "welcome to the world of tomorrow"

#Board connection parameters
HOST_NAME=172.22.167.10
HOST_USER=parallella
PORT=22
HOST_OFFLOAD_PATH="~/boardExec"

#SSH key file
KEY=~/.ssh/id_rsa
#path and file name inputs
DEPOLYMENT_BINARY=${1?Error: no host binary given}

#proper ssh, use this to open an ssh connection to parallella for execution
#sudo ssh parallella@idial.institute -p 32767 -i ~/.ssh/key

#clear hostOfflad path before copying new files
#ssh $HOST_NAME -p$PORT -l$HOST_USER -i$KEY "cd $HOST_OFFLOAD_PATH "

#transfer host and device binaries to parallella board
echo "---------------------------------------"
echo "copying binaries to remote board..."
scp -4 -C -P $PORT -i $KEY $DEPOLYMENT_BINARY $HOST_USER@$HOST_NAME:$HOST_OFFLOAD_PATH
if [[ $? != 0 ]]; then
  echo "Transfer failed!"
  exit 1
else
  echo "Transfer complete."
fi
echo "---------------------------------------"
echo "Running program $HOST_BINARY"
echo "***************************************"
#run the binaries remotely on parallella
#ssh $HOST_NAME -p$PORT -l$HOST_USER -i$KEY "EPIPHANY_HOME=/opt/adapteva/esdk && . $EPIPHANY_HOME/setup.sh && cd $HOST_OFFLOAD_PATH && ./$DEPOLYMENT_BINARY"
echo "***************************************"
echo "done"
