#!/bin/bash

#UPDATE_PACK=up1.flatIcons
#UPDATE_PACK_ZIPNAME=$UPDATE_PACK.zip

UPDATE_PACK_PATH=$1
DEST_DIR=/opt/minibloq/v0.84.0/blocks/

if [ -z $UPDATE_PACK_PATH ]; then
	echo "No se suministro nombre de update pack"
	echo "Saliendo..."
	exit -1

fi

UPDATE_PACK_ZIPNAME=`basename $UPDATE_PACK_PATH`
UPDATE_PACK_NAME=`basename $UPDATE_PACK_PATH .zip`

echo "Se va a instalar $UPDATE_PACK_ZIPNAME"
echo

TMP=`mktemp -d`
cp -v $UPDATE_PACK_PATH $TMP/

cd $TMP
unzip $UPDATE_PACK_ZIPNAME

cd $UPDATE_PACK_NAME
cd blocks

for i in `ls .`; do
	if [ -d $i ]; then
		cp -afv $i/* $DEST_DIR/$i
	else
		cp -afv $i $DEST_DIR/
	fi
done

cd ..
rm -rf $TMP

exit 0
