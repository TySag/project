FILE=$1
OUT=$2
xxd -s 96 -p $FILE > $OUT
cat $OUT | tr -d " " > tmp
cat -v tmp | tr -d "\n" | tr -d "^M" | tr -d "\t" | tr -d "." > $OUT
echo "finish output hex txt file"
