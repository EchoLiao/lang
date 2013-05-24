#!/bin/sh

wdir=`pwd -P`
pname=VPlayer


function control_file_example()
{
    echo ""
    echo "control file example:"
    echo "
    Package: com.yixia.VPlayer
    Version: 0.0.5
    Name: VPlayer Video Player
    Section: [Utilities]
    Architecture: iphoneos-arm
    Author: VPlayer<VPlayer-ios@yixia.com>
    Maintainer: VPlayer<www.VPlayer.net>
    Homepage: www.VPlayer.net
    Icon: file:///Applications/VPlayer.app/Icon.png
    Installed-Size: 9102
    Priority: Optional
    Essential: no
    Description: hello VPlayer!
    tag: purpose:: uikit, role::enduser
    "
}

function assert_file_exist()
{
	[[ ! -e "$1" ]] && {
		exec >&2
		echo "file \"$1\" NOT found!"
		exit 1
	}
}



### main ####

[[ ! -e "`ls $pname*.ipa`" ]] && {
    exec >&2
    echo "no ipa file found!"
    exit 1
}
[[ ! -e control ]] && {
    exec >&2
    echo "file \"control\" NOT found!"
    control_file_example
    exit 1
}
assert_file_exist preinst
assert_file_exist postinst
assert_file_exist prerm
assert_file_exist postrm
assert_file_exist extrainst_


rm -rf Payload deb
[[ "$1" == "-c" || "$1" == "--clean" ]] && {
	echo "Clean end."
	exit 0
}

. /sw/bin/init.sh
unzip $pname*.ipa

mkdir -p deb
cd $wdir/deb
mkdir -p $pname/DEBIAN
mkdir -p $pname/Applications
cp -r ../Payload/*.app ./$pname/Applications

cd $wdir
cp ./control	./deb/$pname/DEBIAN/
cp preinst		./deb/$pname/DEBIAN/
cp postinst		./deb/$pname/DEBIAN/
cp prerm		./deb/$pname/DEBIAN/
cp postrm		./deb/$pname/DEBIAN/
cp extrainst_	./deb/$pname/DEBIAN/

cd $wdir/deb
chmod -R 0755 $pname/DEBIAN
dpkg-deb -b $pname
dpkg-name $pname.deb

ls -lh $wdir/deb


exit 0
