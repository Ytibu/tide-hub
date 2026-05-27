#!/bin/sh

if [ ! -d bin/module ]
then
    mkdir bin/module
else
    unlink bin/myblog
    unlink bin/module/libmyblog.so
fi

cp ../bin/tide bin/myblog
cp lib/libmyblog.so bin/module/