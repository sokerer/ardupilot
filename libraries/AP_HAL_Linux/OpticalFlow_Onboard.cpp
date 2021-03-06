/*
    This class has been implemented based on
    yavta -- Yet Another V4L2 Test Application written by:
    Laurent Pinchart <laurent.pinchart@ideasonboard.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <AP_HAL/AP_HAL.h>
#if CONFIG_HAL_BOARD_SUBTYPE == HAL_BOARD_SUBTYPE_LINUX_BEBOP
#include "OpticalFlow_Onboard.h"

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/v4l2-mediabus.h>

#include "CameraSensor_Mt9v117.h"
#include "GPIO.h"
#include "PWM_Sysfs.h"

#define OPTICAL_FLOW_ONBOARD_RTPRIO 11

extern const AP_HAL::HAL& hal;

using namespace Linux;

void OpticalFlow_Onboard::init(AP_HAL::OpticalFlow::Gyro_Cb get_gyro)
{
    uint32_t top, left;
    uint32_t crop_width, crop_height;
    uint32_t memtype = V4L2_MEMORY_MMAP;
    unsigned int nbufs = 0;
    int ret;
    pthread_attr_t attr;
    struct sched_param param = {
        .sched_priority = OPTICAL_FLOW_ONBOARD_RTPRIO
    };

    if (_initialized) {
        return;
    }

    _get_gyro = get_gyro;

    _videoin = new VideoIn;
    _pwm = new PWM_Sysfs_Bebop(BEBOP_CAMV_PWM);
    _pwm->set_freq(BEBOP_CAMV_PWM_FREQ);
    _pwm->enable(true);

    _camerasensor = new CameraSensor_Mt9v117(HAL_OPTFLOW_ONBOARD_SUBDEV_PATH,
                                             hal.i2c, 0x5D, MT9V117_QVGA,
                                             BEBOP_GPIO_CAMV_NRST,
                                             BEBOP_CAMV_PWM_FREQ);
    if (!_camerasensor->set_format(HAL_OPTFLOW_ONBOARD_SENSOR_WIDTH,
                                   HAL_OPTFLOW_ONBOARD_SENSOR_HEIGHT,
                                   V4L2_MBUS_FMT_UYVY8_2X8)) {
        AP_HAL::panic("OpticalFlow_Onboard: couldn't set subdev fmt\n");
    }
    const char* device_path = HAL_OPTFLOW_ONBOARD_VDEV_PATH;
    memtype = V4L2_MEMORY_MMAP;
    nbufs = HAL_OPTFLOW_ONBOARD_NBUFS;
    _width = HAL_OPTFLOW_ONBOARD_OUTPUT_WIDTH;
    _height = HAL_OPTFLOW_ONBOARD_OUTPUT_HEIGHT;
    crop_width = HAL_OPTFLOW_ONBOARD_CROP_WIDTH;
    crop_height = HAL_OPTFLOW_ONBOARD_CROP_HEIGHT;
    top = 0;
    /* make the image square by cropping to YxY, removing the lateral edges */
    left = (HAL_OPTFLOW_ONBOARD_SENSOR_WIDTH -
            HAL_OPTFLOW_ONBOARD_SENSOR_HEIGHT) / 2;
    _format = V4L2_PIX_FMT_NV12;

    if (device_path == NULL ||
        !_videoin->open_device(device_path, memtype)) {
        AP_HAL::panic("OpticalFlow_Onboard: couldn't open "
                      "video device");
    }

    if (!_videoin->set_crop(left, top, crop_width, crop_height)) {
        AP_HAL::panic("OpticalFlow_Onboard: couldn't set video crop");
    }

    if (!_videoin->set_format(&_width, &_height, &_format, &_bytesperline,
                              &_sizeimage)) {
        AP_HAL::panic("OpticalFlow_Onboard: couldn't set video format");
        return;
    }

    if (_format != V4L2_PIX_FMT_NV12 && _format != V4L2_PIX_FMT_GREY) {
        AP_HAL::panic("OpticalFlow_Onboard: planar or monochrome format needed");
    }

    if (!_videoin->allocate_buffers(nbufs)) {
        AP_HAL::panic("OpticalFlow_Onboard: couldn't allocate video buffers");
    }

    _videoin->prepare_capture();

    /* Use px4 algorithm for optical flow */
    _flow = new Flow_PX4(_width, _bytesperline,
                         HAL_FLOW_PX4_MAX_FLOW_PIXEL,
                         HAL_FLOW_PX4_BOTTOM_FLOW_FEATURE_THRESHOLD,
                         HAL_FLOW_PX4_BOTTOM_FLOW_VALUE_THRESHOLD);

    /* Create the thread that will be waiting for frames
     * Initialize thread and mutex */
    ret = pthread_mutex_init(&_mutex, NULL);
    if (ret != 0) {
        AP_HAL::panic("OpticalFlow_Onboard: failed to init mutex");
    }

    ret = pthread_attr_init(&attr);
    if (ret != 0) {
        AP_HAL::panic("OpticalFlow_Onboard: failed to init attr");
    }
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    pthread_attr_setschedparam(&attr, &param);
    ret = pthread_create(&_thread, &attr, _read_thread, this);
    if (ret != 0) {
        AP_HAL::panic("OpticalFlow_Onboard: failed to create thread");
    }

    _initialized = true;
}

bool OpticalFlow_Onboard::read(AP_HAL::OpticalFlow::Data_Frame& frame)
{
    bool ret;

    pthread_mutex_lock(&_mutex);
    if (!_data_available) {
        ret = false;
        goto end;
    }
    frame.pixel_flow_x_integral = _pixel_flow_x_integral;
    frame.pixel_flow_y_integral = _pixel_flow_y_integral;
    frame.gyro_x_integral = _gyro_x_integral;
    frame.gyro_y_integral = _gyro_y_integral;
    frame.delta_time = _integration_timespan;
    frame.quality = _surface_quality;
    _integration_timespan = 0;
    _pixel_flow_x_integral = 0;
    _pixel_flow_y_integral = 0;
    _gyro_x_integral = 0;
    _gyro_y_integral = 0;
    _data_available = false;
    ret = true;
end:
    pthread_mutex_unlock(&_mutex);
    return ret;
}

void *OpticalFlow_Onboard::_read_thread(void *arg)
{
    OpticalFlow_Onboard *optflow_onboard = (OpticalFlow_Onboard *) arg;

    optflow_onboard->_run_optflow();
    return NULL;
}

void OpticalFlow_Onboard::_run_optflow()
{
    float rate_x, rate_y, rate_z;
    Vector3f gyro_rate;
    Vector2f flow_rate;
    VideoIn::Frame video_frame;
    uint8_t qual;

    while(true) {
        /* wait for next frame to come */
        if (!_videoin->get_frame(video_frame)) {
            AP_HAL::panic("OpticalFlow_Onboard: couldn't get frame\n");
        }
        /* if it is at least the second frame we receive
         * since we have to compare 2 frames */
        if (_last_video_frame.data == NULL) {
            _last_video_frame = video_frame;
            continue;
        }

        /* read gyro data from EKF via the opticalflow driver */
        _get_gyro(rate_x, rate_y, rate_z);
        gyro_rate.x = rate_x;
        gyro_rate.y = rate_y;
        gyro_rate.z = rate_z;

#ifdef OPTICALFLOW_ONBOARD_RECORD_VIDEO
        int fd = open(OPTICALFLOW_ONBOARD_VIDEO_FILE, O_CREAT | O_WRONLY
                | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP |
                S_IWGRP | S_IROTH | S_IWOTH);
	    if (fd != -1) {
	        write(fd, video_frame.data, _sizeimage);
#ifdef OPTICALFLOW_ONBOARD_RECORD_METADATAS
            struct PACKED {
                uint32_t timestamp;
                float x;
                float y;
                float z;
            } metas = { video_frame.timestamp, rate_x, rate_y, rate_z};
            write(fd, &metas, sizeof(metas));
#endif
	        close(fd);
        }
#endif

        /* compute gyro data and video frames
         * get flow rate to send it to the opticalflow driver
         */
        qual = _flow->compute_flow((uint8_t*)_last_video_frame.data,
                                   (uint8_t *)video_frame.data,
                                   video_frame.timestamp -
                                   _last_video_frame.timestamp,
                                   &flow_rate.x, &flow_rate.y);

        /* fill data frame for upper layers */
        pthread_mutex_lock(&_mutex);
        _pixel_flow_x_integral += flow_rate.x /
                                  HAL_FLOW_PX4_FOCAL_LENGTH_MILLIPX;
        _pixel_flow_y_integral += flow_rate.y /
                                  HAL_FLOW_PX4_FOCAL_LENGTH_MILLIPX;
        _integration_timespan += video_frame.timestamp -
                                 _last_video_frame.timestamp;
        _gyro_x_integral       += gyro_rate.x * (video_frame.timestamp -
                                 _last_video_frame.timestamp);
        _gyro_y_integral       += gyro_rate.y * (video_frame.timestamp -
                                 _last_video_frame.timestamp);
        _surface_quality = qual;
        _data_available = true;
        pthread_mutex_unlock(&_mutex);

        /* give the last frame back to the video input driver */
        _videoin->put_frame(_last_video_frame);
        _last_video_frame = video_frame;
    }
}
#endif
