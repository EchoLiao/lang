#!/bin/sh

wdir=`pwd -P`
pname=VPlayer


rm -rf Payload deb

[[ ! -e "`ls $pname*.ipa`" ]] && {
    echo "no ipa file found!"
    exit 1
}
[[ ! -e control ]] && {
    echo "no control file!"
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
