#include "webots/kinect.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


#include <stdio.h>
#include <OpenNI.h>

#include "OniSampleUtilities.h"

#define SAMPLE_READ_WAIT_TIMEOUT 2000 //2000ms

using namespace openni;

Status rc;
Device device;
VideoFrameRef frame;
VideoStream depth;


static bool kinectenable = false;
static bool isInit = false;
static int height_pixels = 0;
static int width_pixels = 0;

float rangeImage [480*640];


/*************       initialization      *************/

void kinect_init() {
  kinectenable = false;
  isInit = true;
}


/*************      public API     *************/

WbDeviceTag wb_kinect_get(const char *name) {
  if(isInit == false) {
     kinect_init();
  }
  WbDeviceTag tag = 1;
  return tag;
}

void wb_kinect_enable(WbDeviceTag tag, int colorMS, int rangeMS, bool advancedModel) {
  rc = OpenNI::initialize();
  if (rc != STATUS_OK)
  {
    printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
    return;
  }

  rc = device.open(ANY_DEVICE);
  if (rc != STATUS_OK)
  {
    printf("Couldn't open device\n%s\n", OpenNI::getExtendedError());
    return;
  }

  if (device.getSensorInfo(SENSOR_DEPTH) != NULL)
  {
    rc = depth.create(device, SENSOR_DEPTH);
    if (rc != STATUS_OK)
    {
      printf("Couldn't create depth stream\n%s\n", OpenNI::getExtendedError());
      return;
    }
  }

  rc = depth.start();
  if (rc != STATUS_OK)
  {
    printf("Couldn't start the depth stream\n%s\n", OpenNI::getExtendedError());
    return;
  }

  // Do one read to update the withd and height.
  kinectenable = true;
  wb_kinect_get_range_image_mm(tag);
}

void wb_kinect_disable(WbDeviceTag tag) {
  depth.stop();
  depth.destroy();
  device.close();
  OpenNI::shutdown();
}

const float *wb_kinect_get_range_image(WbDeviceTag tag) {
  // int i;
  if(kinectenable) {
    // const int * rangeImageMm = wb_kinect_get_range_image_mm(tag);

    // int pixelNumber = wb_kinect_get_range_width(tag)*wb_kinect_get_range_height(tag);
    // for(i=0;i<pixelNumber;i++){
    //     kinectArray[tag].rangeImageMm[i] = (int) 1000*rangeImage[i];  
    // }
    // return kinectArray[tag].rangeImageMm;

  }

  // // kinect not enable
  fprintf(stderr, "1.Please enable the kinect before to use wb_kinect_get_range_image() - TO IMPLEMENT\n");
  return NULL;
}

const short unsigned int *wb_kinect_get_range_image_mm(WbDeviceTag tag) {
  if(kinectenable) {
    int i;
    int changedStreamDummy;
    VideoStream* pStream = &depth;

    rc = OpenNI::waitForAnyStream(&pStream, 1, &changedStreamDummy, SAMPLE_READ_WAIT_TIMEOUT);

    if (rc != STATUS_OK)
    {
      printf("Wait failed! (timeout is %d ms)\n%s\n", SAMPLE_READ_WAIT_TIMEOUT, OpenNI::getExtendedError());
      return NULL;

    }

    rc = depth.readFrame(&frame);

    if (rc != STATUS_OK)
    {
      printf("Read failed!\n%s\n", OpenNI::getExtendedError());
      return NULL;

    }

    if (frame.getVideoMode().getPixelFormat() != PIXEL_FORMAT_DEPTH_1_MM && frame.getVideoMode().getPixelFormat() != PIXEL_FORMAT_DEPTH_100_UM)
    {
      printf("Unexpected frame format\n");
      return NULL;
    }

    DepthPixel* pDepth = (DepthPixel*)frame.getData();
    height_pixels = frame.getHeight();
    width_pixels = frame.getWidth();

    if (width_pixels>160){
    //Fix blind column
    	for(i=0;i<height_pixels;i++){
          pDepth[i*width_pixels + 0] = pDepth[i*width_pixels + 3]; 
          pDepth[i*width_pixels + 1] = pDepth[i*width_pixels + 3];  
          pDepth[i*width_pixels + 2] = pDepth[i*width_pixels + 3];  
    	}
    } else {
        for(i=0;i<height_pixels;i++){
          pDepth[i*width_pixels + 0] = pDepth[i*width_pixels + 1];
        }
    }

    return pDepth;
  }
  // kinect not enable
  fprintf(stderr, "2.Please enable the kinect before to use wb_kinect_get_range_image()\n");
  return NULL;
}


// const short unsigned int *wb_kinect_get_range_image_mm(WbDeviceTag tag) {
//   if(kinectenable) {
//     int changedStreamDummy;
//     VideoStream* pStream = &depth;

//     rc = OpenNI::waitForAnyStream(&pStream, 1, &changedStreamDummy, SAMPLE_READ_WAIT_TIMEOUT);

//     if (rc != STATUS_OK)
//     {
//       printf("Wait failed! (timeout is %d ms)\n%s\n", SAMPLE_READ_WAIT_TIMEOUT, OpenNI::getExtendedError());
//       return NULL;

//     }

//     rc = depth.readFrame(&frame);

//     if (rc != STATUS_OK)
//     {
//       printf("Read failed!\n%s\n", OpenNI::getExtendedError());
//       return NULL;

//     }

//     if (frame.getVideoMode().getPixelFormat() != PIXEL_FORMAT_DEPTH_1_MM && frame.getVideoMode().getPixelFormat() != PIXEL_FORMAT_DEPTH_100_UM)
//     {
//       printf("Unexpected frame format\n");
//       return NULL;
//     }

//     DepthPixel* pDepth = (DepthPixel*)frame.getData();
//     height_pixels = frame.getHeight();
//     width_pixels = frame.getWidth();
//     return pDepth;
//   }
//   // kinect not enable
//   fprintf(stderr, "2.Please enable the kinect before to use wb_kinect_get_range_image()\n");
//   return NULL;
// }



int wb_kinect_get_range_width(WbDeviceTag tag) {
  if(kinectenable)
    return width_pixels;
  // kinect not enable
  fprintf(stderr, "3.Please enable the kinect before to use wb_kinect_get_range_width()\n");
  return 0;
}

int wb_kinect_get_range_height(WbDeviceTag tag) {
  if(kinectenable)
    return height_pixels;
  // // kinect not enable
  fprintf(stderr, "4Please enable the kinect before to use wb_kinect_get_range_height()\n");
  return 0;
}



/**********************************************************************************************/
/**********************************************************************************************/

//FUNCTIONS TO BE DONE (depending on the hardware)
double wb_kinect_get_range_fov(WbDeviceTag tag) {
  return 0.0;
}

double wb_kinect_get_min_range(WbDeviceTag tag) {
  return 0.0;
}

double wb_kinect_get_max_range(WbDeviceTag tag) {
  return 0.0;
}

int wb_kinect_get_range_sampling_period(WbDeviceTag tag) {
    return 0;
}

//FUNCTIONS to be done for the camera part - Not priority now

int wb_kinect_get_color_width(WbDeviceTag tag) {
  return 0;
}

int wb_kinect_get_color_height(WbDeviceTag tag) {
  fprintf(stderr, "Not implemented yet\n");
  return 0;
}

double wb_kinect_get_color_fov(WbDeviceTag tag) {
  return 0.0;
}

const unsigned char *wb_kinect_get_image(WbDeviceTag tag) {
  fprintf(stderr, "Not implemented yet\n");
  return NULL;
}

int wb_kinect_get_color_sampling_period(WbDeviceTag tag) {
    return 0;
}


