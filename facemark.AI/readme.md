# MachineLearning Project using TensorFlow and ... to detect Face Landmarks

- landmarks.py
  * Requirements
    + python cv2, dlib modules.
    + shape_predictor_68_face_landmarks.dat file in same directory.
  * Input : A frontal face image file path.
  * Run as a program. (Output : a json file)
    ```
    $ python landmarks.py image.jpg     # write landmarks to landmarks.json
      {"0": [283, 404], "1": [281, 460], "2": [283, 515], ...
    ```
  * Import as a module. (Output : a python dict)
    ```
    > import landmarks
    > 
    > filename = 'image.jpg'
    > points = landmarks.get_landmarks(filename)    # return landmarks as a dict
    >
    > 44x, 13y = points[44].x, points[13].y
    ```
