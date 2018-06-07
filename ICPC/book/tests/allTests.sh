#!/bin/bash

RAND_PTS_INT="rand_pts_int_10 rand_pts_int_100 rand_pts_int_1000 rand_pts_int_10000 rand_pts_int_100000"

RAND_PTS_REAL="rand_pts_real_10 rand_pts_real_100 rand_pts_real_1000 rand_pts_real_10000 rand_pts_real_100000"

RAND_BIG_INTS="rand_big_int_10 rand_big_int_100 rand_big_int_1000 rand_big_int_10000 rand_big_int_100000"

RAND_MAT_INT="rand_int_matrix_10 rand_int_matrix_100"

RAND_BIP_MATS="rand_bip_adj_10 rand_bip_adj_100 rand_bip_adj_1000 rand_bip_adj_10000 rand_bip_adj_15000"

# Set these to anything but TRUE if you do not want to run the test
GEOMETRY="FALSE"
BIG_INT="FALSE"
MATCHING="TRUE"

if [ ${GEOMETRY} == "TRUE" ]; then
echo -n "Attempting to Compile CCW... ";
bash runTest.sh compile ccw
for x in ${RAND_PTS_INT} ${RAND_PTS_REAL}; do
    echo -n "CCW Test on ${x}... ";
    bash runTest.sh ${x};
done
echo

echo "This doesn't actually work because the polygon may be self intersecting..."
echo -n "Attempting to Compile Area Poly... ";
bash runTest.sh compile areapoly
for x in ${RAND_PTS_INT} ${RAND_PTS_REAL}; do
    echo -n "Area Poly Test on ${x}... ";
    bash runTest.sh ${x};
done
echo

echo -n "Attempting to Compile Circle Through 3 Points... ";
bash runTest.sh compile circle_3pts
for x in ${RAND_PTS_INT} ${RAND_PTS_REAL}; do
    echo -n "Circle Through 3 Points Test on ${x}... ";
    bash runTest.sh ${x};
done
echo

echo -n "Attempting to Compile Convex Hull... ";
bash runTest.sh compile convex_hull
for x in ${RAND_PTS_INT} ${RAND_PTS_REAL}; do
    echo -n "Convex Hull Test on ${x}... ";
    bash runTest.sh ${x};
done
echo

echo "This doesn't actually work because the polygon may be self intersecting..."
echo -n "Attempting to Compile Point in Polygon... ";
bash runTest.sh compile pointpoly
for x in ${RAND_PTS_INT} ${RAND_PTS_REAL}; do
    echo -n "Point in Polygon Test on ${x}... ";
    bash runTest.sh ${x};
done
echo

echo -n "Attempting to Compile Intersect Infinite Lines... ";
bash runTest.sh compile intersect_iline
for x in ${RAND_PTS_INT} ${RAND_PTS_REAL}; do
    echo -n "Intersect Infinite Line Test on ${x}... ";
    bash runTest.sh ${x};
done
echo

echo -n "Attempting to Compile Intersect Finite Lines... ";
bash runTest.sh compile intersect_line
for x in ${RAND_PTS_INT} ${RAND_PTS_REAL}; do
    echo -n "Intersect Finite Line Test on ${x}... ";
    bash runTest.sh ${x};
done
echo

echo -n "Attempting to Compile Intersect Two circles... ";
bash runTest.sh compile intersect_circle_circle
for x in ${RAND_PTS_INT} ${RAND_PTS_REAL}; do
    echo -n "Intersect Two Circles Test on ${x}... ";
    bash runTest.sh ${x};
done
echo

echo -n "Attempting to Compile Intersect Circle and Line... ";
bash runTest.sh compile intersect_iline_circle
for x in ${RAND_PTS_INT} ${RAND_PTS_REAL}; do
    echo -n "Intersect Circle and Line Test on ${x}... ";
    bash runTest.sh ${x};
done
echo

fi; # Geometry

if [ ${BIG_INT} == "TRUE" ]; then
echo -n "Attempting to Compile BigInteger... ";
bash runTest.sh compile bigint
for x in ${RAND_PTS_INT} ${RAND_BIG_INTS}; do
    echo -n "BigInteger Test on ${x}... ";
    bash runTest.sh ${x};
done
echo
fi; # Big Integer

if [ ${MATCHING} == "TRUE" ]; then
echo -n "Attempting to Compile Matching... ";
bash runTest.sh compile matching
for x in ${RAND_BIP_MATS}; do
    echo -n "Matching (and Vertex Cover) Test on ${x}... ";
    bash runTest.sh ${x};
done
echo

echo -n "Attempting to Compile Hungarian... ";
bash runTest.sh compile hungarian
for x in ${RAND_MAT_INT}; do
    echo -n "Hungarian Test on ${x}... ";
    bash runTest.sh ${x};
done
echo

fi;

bash runTest.sh remove
