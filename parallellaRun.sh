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
echo "Parallella remote Script version 1.0"
echo "welcome to the world of tomorrow"

#Board connection parameters
HOST_NAME=172.22.167.10
HOST_USER=parallella
PORT=22
HOST_OFFLOAD_PATH="~/boardExec"

#SSH key file
KEY=~/.ssh/key
#path and file name inputs
DEPOLYMENT_BINARY=${1?Error: no host binary given}

echo "---------------------------------------"
echo "Running program $HOST_BINARY"
echo "***************************************"
#run the binaries remotely on parallella
ssh $HOST_NAME -p$PORT -l$HOST_USER -i$KEY "EPIPHANY_HOME=/opt/adapteva/esdk && . $EPIPHANY_HOME/setup.sh && cd $HOST_OFFLOAD_PATH && ./$DEPOLYMENT_BINARY"
echo "***************************************"
echo "done"
