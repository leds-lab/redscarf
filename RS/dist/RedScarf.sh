#---------------------------------------------------------
# Copyright (C) 2015 LEDS - Univali <zeferino@univali.br>
# Laboratory of Embedded and Distributed Systems
# University of Vale do Itajaí
#---------------------------------------------------------
#
# Use this file to start application without set LD_LIBRARY_PATH
# to find Qwt and Qt shared objects.
#
# This file and shared objects (.so) must be in the 
# same executable directory.
#
#!/bin/sh
appname=`basename $0 | sed s,\.sh$,,`

dirname=`dirname $0`
tmp="${dirname#?}"

if [ "${dirname%$tmp}" != "/" ]; then
dirname=$PWD/$dirname
fi
LD_LIBRARY_PATH=$dirname
export LD_LIBRARY_PATH
$dirname/$appname "$@"
