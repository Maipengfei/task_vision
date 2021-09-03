;; Auto-generated. Do not edit!


(when (boundp 'node_communication::person)
  (if (not (find-package "NODE_COMMUNICATION"))
    (make-package "NODE_COMMUNICATION"))
  (shadow 'person (find-package "NODE_COMMUNICATION")))
(unless (find-package "NODE_COMMUNICATION::PERSON")
  (make-package "NODE_COMMUNICATION::PERSON"))

(in-package "ROS")
;;//! \htmlinclude person.msg.html


(defclass node_communication::person
  :super ros::object
  :slots (_a _b _sum ))

(defmethod node_communication::person
  (:init
   (&key
    ((:a __a) 0)
    ((:b __b) 0)
    ((:sum __sum) 0)
    )
   (send-super :init)
   (setq _a (round __a))
   (setq _b (round __b))
   (setq _sum (round __sum))
   self)
  (:a
   (&optional __a)
   (if __a (setq _a __a)) _a)
  (:b
   (&optional __b)
   (if __b (setq _b __b)) _b)
  (:sum
   (&optional __sum)
   (if __sum (setq _sum __sum)) _sum)
  (:serialization-length
   ()
   (+
    ;; uint8 _a
    1
    ;; uint8 _b
    1
    ;; uint8 _sum
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _a
       (write-byte _a s)
     ;; uint8 _b
       (write-byte _b s)
     ;; uint8 _sum
       (write-byte _sum s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _a
     (setq _a (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _b
     (setq _b (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _sum
     (setq _sum (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get node_communication::person :md5sum-) "8f32a6e30f428448f61ee5e7099ed0bd")
(setf (get node_communication::person :datatype-) "node_communication/person")
(setf (get node_communication::person :definition-)
      "uint8 a
uint8 b
uint8 sum

")



(provide :node_communication/person "8f32a6e30f428448f61ee5e7099ed0bd")


