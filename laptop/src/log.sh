python ui.py

cd ../images
DIR_NAME="thermalImages_$MISSION_NUMBER"
mkdir $DIR_NAME
TMP=$MISSION_NUMBER
MISSION_NUMBER=$((TMP+1))
export MISSION_NUMBER
echo $MISSION_NUMBER
mv *.jpg $DIR_NAME

cd ../src