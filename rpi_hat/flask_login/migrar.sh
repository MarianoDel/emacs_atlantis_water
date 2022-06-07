#!/bin/bash

#########################
# Origin ZIP File Check #
#########################
# prompt for original zip file
echo ""
echo "Checking origin zipped file..."
sleep 0.5

if [ -n "$(ls -A Archive.zip 2>/dev/null)" ]
then
    echo "zip file finded"
    sleep 0.1
else
    echo "no origin zip file"
    exit
fi

# # delete original directory if exists
# if [ -n "$(ls -d Archive 2>/dev/null)" ]
# then
#     echo "origin directory finded, removing it..."
#     sleep 0.1
#     rm -rf Archive
# fi

echo "Unzipping file..."
sleep 0.1
unzip -uo Archive.zip -d Archive

######################
# Directory Checking #
######################
# prompt for directory checking
echo ""
echo "Checking origin and destination directories..."
sleep 0.5

# check for origin directory
if [ -n "$(ls -d Archive 2>/dev/null)" ]
then
    echo "origin directory finded"
    sleep 0.1
else
    echo "origin directory Archive, not found"
    exit
fi

# check all destination directories
if [ -n "$(ls -d templates 2>/dev/null)" ]
then
    echo "templates finded"
    sleep 0.1
else
    echo "templates not finded"    
    exit
fi

if [ -n "$(ls -d static/css 2>/dev/null)" ]
then
    echo "static/css finded"
    sleep 0.1    
else
    echo "static/css not finded"    
    exit
fi

if [ -n "$(ls -d static/fonts 2>/dev/null)" ]
then
    echo "static/fonts finded"
    sleep 0.1    
else
    echo "static/fonts not finded"    
    exit
fi

if [ -n "$(ls -d static/img 2>/dev/null)" ]
then
    echo "static/img finded"
    sleep 0.1    
else
    echo "static/img not finded"    
    exit
fi

if [ -n "$(ls -d static/js 2>/dev/null)" ]
then
    echo "static/js finded"
    sleep 0.1    
else
    echo "static/js not finded"    
    exit
fi

################
# Coping Files #
################
# prompt for coping
echo ""
echo "Start to copy files..."
sleep 0.5
# Coping html files to templates
echo "coping *.html files..."
cp -f Archive/*.html templates
sleep 0.1
# Coping ico files to templates
echo "coping *.ico files..."
cp -f Archive/*.ico templates
sleep 0.1

# Coping entire directories
echo "coping css recursively..."
cp -rf Archive/css/* static/css
sleep 0.1

echo "coping img recursively..."
cp -rf Archive/img/* static/img
sleep 0.1

echo "coping js recursively..."
cp -rf Archive/js/* static/js
sleep 0.1

#################################
# Updating html files positions #
#################################
# update href css with static/css
for f in templates/*.html; do
    echo "updating href in: $f"
    cp -f $f $f"_old"
    sed 's#href="css#href="static/css#g' $f"_old" > $f
done
sleep 0.1
echo ""

for f in templates/*.html; do
    echo "updating src img: $f"
    cp -f $f $f"_old"
    sed 's#src="img#src="static/img#g' $f"_old" > $f
done
sleep 0.1
echo ""

for f in templates/*.html; do
    echo "updating src js: $f"
    cp -f $f $f"_old"
    sed 's#src="js#src="static/js#g' $f"_old" > $f
done
sleep 0.1
echo ""

# removing old files
echo "removing tmp and old files..."
rm -rf templates/*.html_old
sleep 0.1

# to create patching files
# diff -u file1.html file2.html > patchfile.patch
#
# patching the new files
echo "patching new files files..."
patch static/js/meter_validation.js < meter_validation.patch
sleep 0.1
patch templates/meter.html < meter.patch
sleep 0.1
echo "conversion ended!"
