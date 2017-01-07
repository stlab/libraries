#!/bin/bash

# Run a command, and echo before doing so. Also checks the exit
# status and quits if there was an error.
echo_run ()
{
    echo "EXEC : $@"
    "$@"
    r=$?
    if test $r -ne 0 ; then
        exit $r
    fi
}

BOOST_MAJOR=1
BOOST_MINOR=61
BOOST_DOT=0
BOOST_DT_VER=${BOOST_MAJOR}.${BOOST_MINOR}.${BOOST_DOT}
BOOST_US_VER=${BOOST_MAJOR}_${BOOST_MINOR}_${BOOST_DOT}
BOOST_DIR=boost_${BOOST_US_VER}
BOOST_TAR=$BOOST_DIR.tar.bz2

#
# If need be, download Boost and unzip it, moving it to the appropriate location.
#

if [ ! -e 'boost_libraries' ]; then
    echo "INFO : boost_libraries not found: setting up."

    if [ ! -e $BOOST_TAR ]; then
        echo "INFO : $BOOST_TAR not found: downloading."

        echo_run curl -L "https://sourceforge.net/projects/boost/files/boost/$BOOST_DT_VER/$BOOST_TAR/download" -o $BOOST_TAR;
    else
        echo "INFO : $BOOST_TAR found: skipping download."
    fi

    echo_run rm -rf $BOOST_DIR

    echo_run tar -jxf $BOOST_TAR

    echo_run rm -rf boost_libraries

    echo_run mv $BOOST_DIR boost_libraries
else
    echo "INFO : boost_libraries found: skipping setup."
fi

#
# Create b2/bjam via boostrapping, again if need be.
#

if [ ! -e './boost_libraries/b2' ]; then
    echo "INFO : b2 not found: boostrapping."

    echo_run cd boost_libraries;
    
    #${TOOLSET:-clang}
    echo_run ./bootstrap.sh --with-toolset=gcc 
    echo_run ./b2 --with-system --with-thread --with-atomic --with-chrono --with-test

    echo_run cd ..
else
    echo "INFO : b2 found: skipping boostrap."
fi

#
# A blurb of diagnostics to make sure everything is set up properly.
#

ls -alF

echo "INFO : You are ready to go!"
