
ROOTDIR=`dirname $SIMSCRIPTS_DIR`

if test `uname -o` = "Cygwin"; then
  ROOTDIR=`cygpath -w $ROOTDIR`
fi

export EXAMPLES=$ROOTDIR
export SIMPLE_DPI=$EXAMPLES/simple_dpi
export DEFAULT_SIM=qs

