#目前进度
一. 配置好openvino环境
二. 实现基于yolo模型的物体识别
    在Example/openvino/yolo3/build目录下
    执行 ./object_detection_yolo
    默认打开摄像头识别,也可以在后面加参数 --video=视频文件名以打开视频文件进行识别。
三. 转化Yolov3.weights文件转化.pb文件（详细转化步骤可参考官网）
3generate an IR of the YOLOv3-tiny TensorFlow model:
python3 mo_tf.py                                                        \
--input_model /path/to/yolo_v3_tiny.pb                                  \
--transformations_config $MO_ROOT/extensions/front/tf/yolo_v3_tiny.json \
--batch 1                                                               \
--output_dir <OUTPUT_MODEL_DIR>

四、用ov执行代码:
./yolo_opencv -i /dev/video0 -m path/to/yolo_v3_tiny.xml -d CPU
