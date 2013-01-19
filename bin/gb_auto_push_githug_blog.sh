#!/bin/sh

###
# It is a simple shell script for write blog with github.
# It use to update remove repo.
###

bloglist="/home/scr/.push_to_github_blog_file_list"
blogdir="/Volumes/Macintosh-HD-2/RT/home/scr/.vercol/git/lang/blog/github/nuoerlz.github.com"
blogsouredir="$blogdir""/source/_posts"

cat $bloglist | while read line; do
	srcFullFileName="$line"
	srcShortFileName="${srcFullFileName##*/}"
	titleFileName="${srcShortFileName%.*}"
	createDate=`sed -ne '/^date: /s///p' $srcFullFileName | awk '{ print $1 }'`
	dstFullFileName="$blogsouredir"/$createDate-$titleFileName.markdown
	if [[ -e $dstFullFileName ]]; then
		srcTime=`stat -f "%m" $srcFullFileName`
		dstTime=`stat -f "%m" $dstFullFileName`
		if [[ $srcTime > $dstTime ]]; then
			cat $srcFullFileName > $dstFullFileName
		fi
	else
		cat $srcFullFileName > $dstFullFileName
	fi
done

cd $blogdir
ret=`git status . | grep "^nothing to commit"`
[[ -z $ret ]] && {
	/bin/bash -l -c 'rake generate'
	/bin/bash -l -c 'rake deploy'
	git add .
	git commit -a -m 'update blog'
	git push origin source
}

exit 0
