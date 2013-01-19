#!/bin/sh

###
# It is a simple shell script for write blog with github.
# It use to find the original source and directory of the blog.
###

cd2vimf=/tmp/pwd2vim.tmp
vimtmpf=/tmp/open_in_vim_md5
bloglist="/home/scr/.push_to_github_blog_file_list"

[[ $# != 1 ]] && {
	echo "No file to find?!"
	exit 1
}

fileName="${1}"
noDateFileName=`echo $fileName | sed -e 's/^[0-9][0-9]*-[0-9][0-9]*-[0-9][0-9]*-//'`
fTitleFileName="${noDateFileName%.*}"

cat $bloglist | while read line; do
	srcFullFileName="$line"
	srcDir="${srcFullFileName%/*}"
	srcShortFileName="${srcFullFileName##*/}"
	titleFileName="${srcShortFileName%.*}"
	if [[ "$titleFileName" == "$fTitleFileName" ]]; then
		echo "$srcDir"			> $cd2vimf
		echo "$srcFullFileName" > $vimtmpf
		echo "origin of \"$fileName\" :"
		echo "\t$srcDir"
		echo "\t$srcFullFileName"
		break;
	fi
done

exit 0
