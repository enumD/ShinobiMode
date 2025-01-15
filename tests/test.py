import cv2
import time

# Create HOG detector
hog = cv2.HOGDescriptor()
hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

# Open video stream
cap = cv2.VideoCapture(0)  # ls /dev/video* to get the devices attached
if not cap.isOpened():
    print("Errore: cannot find video src.")
else:
    print("Webcam found! Press q to exit.")
    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                print("Cannot read frame.")
                break

            # People Detection
            boxes, weights = hog.detectMultiScale(frame, winStride=(8, 8))
            for (x, y, w, h) in boxes:
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

            # Show the result
            cv2.imshow("People Detection", frame)

            # Sleep a litle to reduce cpu usage
            if cv2.waitKey(30) & 0xFF == ord('q'):  # wait 30ms or exit with q
                break
            time.sleep(0.03)

    except KeyboardInterrupt:
        print("\n User exit.")
    finally:
        cap.release()
        cv2.destroyAllWindows()
