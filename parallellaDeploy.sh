#!/usr/bin/env bash
#*******************************************************************************
#   Copyright (c) 2019 Dortmund University of Applied Sciences and Arts and others.
#  
#   This program and the accompanying materials are made
#   available under the terms of the Eclipse Public License 2.0
#   which is available at https://www.eclipse.org/legal/epl-2.0/
#  
#   SPDX-License-Identifier: EPL-2.0
#  
#   Contributors:
#        Dortmund University of Applied Sciences and Arts - initial API and implementation
#*******************************************************************************
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

echo "---------------------------------------"
echo "copying binaries to remote board..."

#transfer host and device binaries to parallella board

for DEPLOYMENT_BINARY_INDEXED in "$@"

do

 scp -4 -C -P $PORT -i $KEY $DEPLOYMENT_BINARY_INDEXED $HOST_USER@$HOST_NAME:$HOST_OFFLOAD_PATH
 if [ $? != 0 ]; then
	echo "Transfer failed!"
  	exit 1
 else
	echo "Transfer complete."
 fi
done
echo "---------------------------------------"

#proper ssh, use this to open an ssh connection to parallella for execution
#sudo ssh $HOST_USER@$HOST_NAME -p $PORT -i ~/.ssh/key

#clear hostOfflad path before copying new files
#ssh $HOST_NAME -p$PORT -l$HOST_USER -i$KEY "cd $HOST_OFFLOAD_PATH "
echo "done"


