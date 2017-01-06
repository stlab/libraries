mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_CXX_COMPILER=$COMPILER -DBOOST_ROOT=boost_libraries ./..
make CXX=$COMPILER -j3
test/future_test --log_level=message
echo all done