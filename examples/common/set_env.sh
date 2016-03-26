
rootdir=`pwd`

while test "x$rootdir" != "x" && test ! -d $rootdir/common/scripts; do
	rootdir=`dirname $rootdir`
done

if test "x$rootdir" = "x"; then
  echo "Error: Failed to find root directory"
else
  PATH=${rootdir}/common/scripts:$PATH
fi

export ROOTDIR=$rootdir

# Environment-specific variables
if test -f $rootdir/common/scripts/env.sh; then
	. $rootdir/common/scripts/env.sh
fi


