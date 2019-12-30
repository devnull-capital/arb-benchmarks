echo "Running unit tests:"

for i in ./*_tests
do
  if test -f $i
  then
    echo "yup"
    if $VALGRIND ./$i 2>> ./tests.log
    then
      echo $i PASS
    else
      echo "ERROR in test $i: here's ./tests.log"
      echo "------"
      tail ./tests.log
      exit 1
    fi
  else
    echo "nope"
  fi
done

echo ""
