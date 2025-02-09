
import cv2
import numpy
import time
import mss
from ultralytics import YOLO
from collections import Counter
import requests
import os
model = YOLO(os.path.join("yolo_client", "best.pt"))

with mss.mss() as sct:
    # Part of the screen to capturet
    monitor = {"top": 0, "left": 1580, "width": 340, "height": 100}
    imgs = []
    last_time = time.time()
    while "Screen capturing":
        img = numpy.array(sct.grab(monitor))

        # if time.time() - last_time > 1:
        #     results = model.predict(imgs)
        #     probs = [img.names[img.probs.top1] for img in results]
        #     item = Counter(probs).most_common(1)[0][0]
        #     print(item)
        #     imgs.clear()
        #     last_time = time.time()
        # else:
        #     imgs.append(img)
        
        cv2.imshow("OpenCV/Numpy normal", img)
        mss_img = sct.grab(monitor)
        img_tmp_path = "tmp.png"
        mss.tools.to_png(mss_img.rgb, mss_img.size, output=img_tmp_path)
        result = model.predict(img_tmp_path)
        
        predicted = result[0].names[result[0].probs.top1]
        if predicted == "death":
            predicted = 0
        elif predicted == "full_live":
            predicted = 5

        requests.get("")

       


        if cv2.waitKey(25) & 0xFF == ord("q"):
            cv2.destroyAllWindows()
            break