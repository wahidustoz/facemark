# -*- encoding: utf-8 -*-
import dlib
import cv2
import sys
import json

# create list for landmarks
ALL = list(range(0, 68))
RIGHT_EYEBROW = list(range(17, 22))
LEFT_EYEBROW = list(range(22, 27))
RIGHT_EYE = list(range(36, 42))
LEFT_EYE = list(range(42, 48))
NOSE = list(range(27, 36))
MOUTH_OUTLINE = list(range(48, 61))
MOUTH_INNER = list(range(61, 68))
JAWLINE = list(range(0, 17))

# return 68 facial landmarks of image
def get_landmarks(filename) :
    # create face detector, predictor
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor('shape_predictor_68_face_landmarks.dat')

    # read image
    image = cv2.imread(filename, cv2.IMREAD_COLOR)

    # gray scale conversion
    img_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # get only 1 face (up-sampling=1)
    face = detector(img_gray, 1)[0]

    # make prediction and transform to numpy array
    landmarks = predictor(img_gray, face)

    # get 68 points
    points = landmarks.parts()

    # print 68 points
    for i, p in enumerate(points) :
        print('[%d] (%d, %d)' % (i, p.x, p.y))
        
    return points
    
if __name__ == "__main__" :
    points = get_landmarks(sys.argv[1])

    # write landmarks to landmarks.json
    with open("landmarks.json", "w") as json_file :
        landmark_dict = { i : [p.x, p.y] for i, p in enumerate(points) }
        json_file.write(json.dumps(landmark_dict))
        json_file.write('\n')

    print("Landmarks successfully written to landmarks.json")