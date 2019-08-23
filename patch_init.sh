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
#       Dortmund University of Applied Sciences and Arts - initial API and implementation
#*******************************************************************************
echo "RTFP patching Script version 1.0"
echo "welcome to the world of tomorrow"

#Path to the cloned RTFP repo
#RTFP_REPO_PATH="~/boardExec"
KERNEL_FOLDER="RTFP_RTOS_KERNEL_PATCHED"
echo "*****************************************************"
echo "cloning dependencies..."
#clone FreeRTOS kernel from svn
echo "cloning FreeRTOS kernel"
svn checkout svn://svn.code.sf.net/p/freertos/code/trunk@2712 $KERNEL_FOLDER > /dev/null
#Clone original port repo
echo "cloning contribute Epiphany port"
mkdir Epiphay_port
cd Epiphay_port
git clone https://github.com/mahmood1994ha/FreeRTOS.git
cd FreeRTOS
#create patch file of original port commit
git format-patch -1 78e8efaf3ca9aa56c389e00411f2c3bcb0222b64
PORT_PATCH_FILE="0001-runs-on-the-epiphany-processor-core-as-present-on-th.patch"
#apply the patch to kernel
echo "exit FreeRTOS"
cd ..
echo "exit Epiphay_port"
cd ..
echo "enter KERNEL_FOLDER"
cd $KERNEL_FOLDER
echo "enter patch dir"
cd FreeRTOS
echo "*****************************************************"
echo "patching port"
patch -p2 < ../../Epiphay_port/FreeRTOS/0001-runs-on-the-epiphany-processor-core-as-present-on-th.patch 
#patch Epiphany port with RTFP fix
cd ../../
patch $KERNEL_FOLDER/FreeRTOS/Source/portable/GCC/Epiphany/port.c < port_patch.patch 
patch $KERNEL_FOLDER/FreeRTOS/Source/portable/GCC/Epiphany/portasm.s < portasm.patch
echo "port patched with RTFP fix"
echo "*****************************************************"
echo "cleaning kernel and port"
pwd
echo "***************************************"
echo "done"
