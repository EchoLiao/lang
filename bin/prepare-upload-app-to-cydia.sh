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




rm -rf Payload deb

[[ ! -e "`ls $pname*.ipa`" ]] && {
    echo "no ipa file found!"
    exit 1
}
[[ ! -e control ]] && {
    echo "no control file!"
    control_file_example
    exit 1
}


. /sw/bin/init.sh

unzip $pname*.ipa

mkdir -p deb
cd $wdir/deb
mkdir -p $pname/DEBIAN
mkdir -p $pname/Applications
cp -r ../Payload/*.app ./$pname/Applications

cd $wdir
cp ./control ./deb/$pname/DEBIAN/
cd $wdir/deb
dpkg-deb -b $pname
dpkg-name $pname.deb
ls -lh $wdir/deb


exit 0
