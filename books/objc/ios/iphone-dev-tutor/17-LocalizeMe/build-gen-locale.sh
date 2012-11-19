
find . -type f -name "*.[chm]" | xargs genstrings 
mv Localizable.strings en.lproj/

