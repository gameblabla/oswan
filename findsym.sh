findsym () {
  [[ -z $1 ]] && return 1
  SYMBOL=$1
  LIBDIR=${2:-/usr/lib}
  for lib in $LIBDIR/*.a
  do
    nm $lib &> /dev/null | grep -q $SYMBOL && \
      print "symbol found in $lib\n -L$LIBDIR -l${${lib:t:r}#lib}"
  done
}
