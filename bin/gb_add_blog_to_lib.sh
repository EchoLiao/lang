#!/bin/sh

###
# It is a simple shell script for write blog with github.
# It use to add&update a blog source file to file list.
###

bloglist="/home/scr/.push_to_github_blog_file_list"
tmplist="/tmp/tmp_github_blog_me_234234_242_4444_fssx_x_xh_y_s_3"

[[ $# != 1 ]] && {
	echo "Nothing to do?!"
	exit 1
}

file="`pwd -P`"/"${1}"
[[ ! -e "$file" ]] && {
	echo "file \"$file\" NOT found!"
	exit 2
}

title=`sed -ne '/^title: /s///p' $file`
cdate=`sed -ne '/^date: /s///p' $file`
[[ -z $title || -z $cdate ]] && {
	echo "NOT set title or create date!"
	exit 3
}

more=`sed -ne '/<!--more-->/p' $file`
[[ -z $more ]] && {
	echo "NOT set <!--more--> flag!"
	exit 4
}


echo "$file" >> $bloglist
cat $bloglist | sort -u > $tmplist
mv -f $tmplist $bloglist
echo "Add $file ---to--> $bloglist"

exit 0
