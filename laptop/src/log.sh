python ui.py

cd ../images
DIR_NAME="thermalImages_$MISSION_NUMBER"
mkdir $DIR_NAME

mv *.jpg $DIR_NAME
mv ../src/*.txt ./$DIR_NAME

cd ../src