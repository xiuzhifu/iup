#ifdef USE_OPENGL
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "iup.h"          
#include "iupgl.h"
#include "iupglcontrols.h"


static unsigned char img_close[16 * 16] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static unsigned char img_open[16 * 16] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static Ihandle* load_image_Tecgraf(void)
{
  unsigned char imgdata[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88, 95, 108, 1, 90, 100, 117, 99, 123, 138, 166, 126, 137, 152, 181, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 95, 105, 123, 147, 122, 137, 165, 255, 136, 152, 183, 255, 132, 149, 179, 250, 133, 149, 178, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 92, 100, 115, 43, 111, 125, 150, 253, 140, 158, 190, 255, 135, 151, 182, 255, 132, 149, 179, 255, 131, 147, 177, 217, 153, 164, 188, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 113, 134, 148, 134, 151, 182, 255, 137, 154, 185, 255, 115, 129, 154, 252, 114, 128, 155, 255, 130, 146, 175, 255, 132, 147, 175, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 158, 159, 162, 3, 108, 121, 145, 230, 144, 162, 195, 255, 137, 154, 185, 197, 74, 79, 86, 45, 41, 46, 55, 246, 120, 134, 162, 255, 129, 145, 174, 156, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 95, 101, 113, 48, 124, 139, 167, 255, 144, 161, 194, 255, 138, 155, 186, 67, 0, 0, 0, 0, 49, 54, 62, 150, 87, 98, 118, 255, 128, 144, 173, 223, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 113, 132, 105, 137, 154, 185, 255, 139, 156, 188, 231, 143, 159, 187, 3, 0, 0, 0, 0, 64, 68, 76, 61, 70, 79, 95, 255, 127, 143, 172, 254, 134, 149, 175, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 121, 142, 153, 141, 159, 191, 255, 139, 156, 188, 164, 0, 0, 0, 0, 0, 0, 0, 0, 79, 82, 87, 3, 69, 77, 92, 241, 122, 137, 165, 255, 127, 142, 170, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 110, 122, 146, 191, 145, 163, 196, 255, 139, 156, 188, 107, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 77, 86, 101, 190, 115, 129, 156, 255, 126, 141, 170, 113, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 125, 149, 227, 150, 168, 201, 255, 141, 157, 188, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 91, 107, 144, 113, 127, 153, 255, 125, 140, 169, 144, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 163, 165, 167, 2, 112, 125, 150, 252, 155, 173, 203, 255, 143, 159, 189, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 94, 110, 109, 114, 128, 155, 255, 125, 140, 168, 175, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 160, 167, 181, 1, 120, 130, 149, 33, 48, 53, 59, 69, 43, 46, 52, 100, 50, 54, 59, 137, 116, 130, 156, 255, 155, 171, 201, 255, 105, 118, 142, 155, 104, 117, 141, 151, 105, 118, 141, 151, 105, 118, 142, 151, 101, 113, 136, 185, 111, 124, 150, 255, 116, 130, 156, 220, 112, 125, 148, 95, 115, 127, 150, 67, 123, 134, 156, 33, 168, 176, 190, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 119, 129, 147, 5, 109, 121, 142, 71, 106, 118, 140, 140, 105, 117, 140, 197, 107, 120, 144, 242, 120, 135, 162, 255, 123, 137, 163, 255, 44, 49, 58, 255, 28, 32, 39, 255, 125, 139, 164, 255, 150, 167, 197, 255, 138, 155, 186, 255, 131, 148, 178, 255, 125, 141, 170, 255, 119, 134, 162, 255, 114, 128, 154, 255, 108, 122, 147, 255, 104, 117, 141, 255, 102, 115, 138, 255, 103, 116, 139, 255, 107, 120, 145, 255, 111, 124, 149, 245, 113, 126, 151, 200, 113, 127, 152, 140, 116, 129, 154, 71, 122, 135, 158, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 118, 128, 145, 14, 106, 118, 140, 130, 104, 116, 139, 234, 105, 118, 142, 255, 128, 144, 173, 255, 147, 165, 199, 255, 157, 177, 213, 255, 150, 168, 202, 255, 140, 156, 187, 229, 41, 45, 52, 196, 39, 43, 51, 183, 130, 143, 168, 255, 144, 161, 192, 233, 109, 122, 145, 109, 105, 116, 138, 109, 99, 110, 130, 109, 92, 103, 123, 109, 91, 100, 117, 145, 97, 109, 131, 255, 95, 106, 128, 248, 74, 83, 97, 193, 64, 72, 85, 227, 56, 63, 75, 255, 55, 62, 75, 255, 65, 73, 88, 255, 90, 101, 121, 255, 111, 125, 150, 255, 114, 128, 154, 236, 116, 129, 155, 130, 127, 140, 165, 16, 0, 0, 0, 0,
    95, 101, 113, 22, 103, 115, 137, 220, 103, 116, 140, 255, 110, 123, 148, 255, 146, 165, 198, 255, 147, 165, 197, 232, 142, 158, 188, 147, 131, 144, 169, 78, 115, 123, 139, 20, 0, 0, 0, 0, 0, 0, 0, 0, 91, 97, 108, 68, 128, 142, 167, 255, 144, 162, 193, 212, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 95, 100, 107, 31, 120, 135, 163, 255, 133, 150, 180, 231, 0, 0, 0, 0, 0, 0, 0, 0, 86, 89, 93, 20, 50, 54, 61, 73, 37, 40, 46, 141, 33, 36, 42, 230, 46, 52, 63, 255, 107, 120, 144, 255, 116, 130, 157, 255, 118, 133, 159, 223, 132, 147, 174, 24,
    76, 83, 95, 114, 104, 117, 140, 255, 105, 117, 141, 255, 118, 133, 160, 253, 139, 155, 184, 116, 134, 143, 161, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 93, 100, 110, 74, 122, 137, 163, 255, 143, 160, 191, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 110, 114, 120, 31, 123, 138, 166, 255, 136, 153, 183, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 90, 93, 97, 5, 42, 45, 51, 111, 86, 97, 117, 253, 118, 133, 160, 255, 119, 133, 161, 255, 133, 149, 180, 116,
    46, 50, 56, 109, 67, 76, 91, 255, 105, 118, 142, 255, 107, 120, 145, 254, 112, 125, 149, 131, 127, 139, 161, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 93, 98, 109, 74, 116, 130, 156, 255, 142, 159, 190, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 115, 122, 31, 128, 143, 172, 255, 141, 157, 185, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 124, 137, 163, 7, 122, 136, 162, 122, 120, 135, 162, 254, 121, 136, 164, 255, 136, 152, 184, 255, 126, 141, 168, 116,
    71, 74, 79, 17, 31, 35, 41, 206, 42, 47, 57, 255, 77, 87, 105, 255, 103, 116, 140, 255, 110, 124, 149, 239, 112, 125, 150, 157, 115, 128, 153, 89, 122, 134, 158, 30, 147, 158, 177, 2, 0, 0, 0, 0, 81, 87, 96, 65, 109, 123, 148, 255, 141, 158, 190, 212, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 98, 103, 112, 31, 135, 151, 180, 255, 141, 156, 183, 231, 0, 0, 0, 0, 153, 164, 183, 1, 134, 147, 171, 30, 124, 138, 165, 84, 123, 138, 165, 154, 122, 137, 164, 235, 127, 143, 172, 255, 140, 157, 189, 255, 144, 162, 195, 255, 129, 144, 172, 218, 126, 138, 161, 22,
    0, 0, 0, 0, 64, 68, 73, 7, 39, 43, 49, 118, 32, 36, 42, 225, 30, 35, 42, 255, 50, 57, 68, 255, 72, 81, 97, 255, 91, 102, 123, 255, 105, 118, 142, 255, 113, 127, 152, 240, 115, 129, 155, 204, 111, 124, 149, 196, 111, 125, 150, 255, 126, 141, 170, 234, 119, 133, 159, 120, 120, 134, 160, 116, 121, 135, 161, 117, 121, 135, 162, 119, 116, 130, 155, 152, 127, 142, 170, 255, 125, 140, 168, 248, 123, 138, 166, 199, 130, 145, 173, 235, 140, 155, 183, 255, 143, 160, 190, 255, 143, 161, 193, 255, 147, 165, 199, 255, 145, 164, 197, 255, 132, 148, 177, 230, 127, 140, 166, 126, 124, 134, 151, 12, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 138, 141, 144, 3, 55, 58, 63, 58, 37, 40, 46, 126, 34, 38, 44, 184, 34, 38, 44, 235, 35, 39, 47, 254, 49, 55, 66, 255, 64, 72, 87, 255, 77, 87, 104, 255, 88, 98, 118, 255, 96, 108, 130, 255, 103, 116, 139, 255, 108, 122, 147, 255, 113, 127, 153, 255, 118, 133, 160, 255, 124, 140, 168, 255, 133, 148, 176, 255, 141, 156, 183, 255, 146, 161, 187, 255, 144, 159, 186, 255, 131, 146, 174, 254, 127, 141, 168, 237, 126, 141, 168, 188, 123, 137, 162, 131, 112, 123, 143, 61, 128, 132, 140, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 91, 94, 98, 19, 64, 68, 73, 56, 53, 57, 65, 82, 62, 67, 76, 116, 66, 74, 89, 255, 95, 107, 129, 255, 80, 88, 103, 155, 81, 90, 105, 151, 86, 95, 112, 151, 95, 104, 122, 151, 98, 109, 128, 180, 124, 139, 166, 255, 109, 122, 146, 218, 100, 110, 128, 84, 96, 104, 118, 56, 105, 109, 117, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 173, 174, 176, 2, 74, 83, 98, 252, 131, 147, 178, 255, 140, 155, 184, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122, 134, 157, 114, 151, 169, 203, 255, 123, 138, 165, 174, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 76, 90, 224, 122, 137, 165, 255, 136, 152, 182, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 125, 139, 165, 147, 146, 164, 198, 255, 122, 137, 165, 144, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 69, 81, 191, 110, 124, 149, 255, 134, 151, 181, 113, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 143, 170, 193, 142, 160, 192, 255, 122, 137, 164, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 58, 63, 74, 150, 94, 105, 127, 255, 133, 149, 179, 166, 0, 0, 0, 0, 0, 0, 0, 0, 115, 119, 128, 5, 130, 145, 174, 242, 137, 154, 186, 255, 125, 139, 166, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 54, 58, 66, 102, 72, 81, 97, 255, 132, 148, 178, 236, 148, 161, 187, 5, 0, 0, 0, 0, 110, 121, 140, 64, 140, 157, 189, 255, 127, 142, 171, 254, 131, 144, 169, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 65, 71, 43, 47, 52, 63, 255, 127, 143, 172, 255, 132, 148, 177, 75, 0, 0, 0, 0, 121, 134, 158, 160, 139, 156, 188, 255, 123, 138, 165, 223, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 154, 156, 158, 1, 36, 39, 46, 227, 106, 119, 143, 255, 130, 145, 175, 203, 114, 125, 147, 51, 123, 138, 166, 247, 131, 147, 177, 255, 123, 138, 165, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 37, 41, 47, 143, 68, 77, 93, 255, 128, 144, 174, 255, 126, 141, 170, 252, 129, 145, 174, 255, 123, 138, 166, 255, 127, 141, 167, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65, 68, 73, 40, 34, 38, 46, 250, 117, 131, 158, 255, 126, 142, 171, 255, 124, 140, 168, 255, 125, 139, 166, 214, 140, 152, 172, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 44, 50, 134, 58, 66, 79, 255, 123, 138, 166, 255, 123, 138, 166, 250, 127, 140, 165, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 154, 156, 158, 1, 46, 50, 55, 83, 82, 89, 102, 123, 106, 116, 136, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  Ihandle* image = IupImageRGBA(32, 32, imgdata);
  return image;
}

static Ihandle* load_image_FileSave(void)
{
  unsigned char imgdata[] = {
    255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 183, 182, 245, 255, 183, 182, 245, 255, 179, 178, 243, 255, 174, 173, 241, 255, 168, 167, 238, 255, 162, 161, 234, 255, 155, 154, 231, 255, 148, 147, 228, 255, 143, 142, 224, 255, 136, 135, 221, 255, 129, 128, 218, 255, 123, 122, 214, 255, 117, 116, 211, 255, 112, 111, 209, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 179, 178, 243, 255, 190, 189, 255, 255, 147, 146, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 75, 88, 190, 255, 89, 88, 176, 255, 89, 88, 176, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 173, 172, 240, 255, 190, 189, 255, 255, 138, 137, 239, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 234, 238, 246, 255, 63, 82, 184, 255, 51, 51, 103, 255, 86, 85, 170, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 167, 166, 237, 255, 190, 189, 255, 255, 129, 128, 230, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 234, 238, 246, 255, 222, 229, 240, 255, 52, 77, 179, 255, 122, 121, 223, 255, 83, 82, 164, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 159, 158, 233, 255, 190, 189, 255, 255, 119, 118, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 234, 238, 246, 255, 222, 229, 240, 255, 210, 219, 234, 255, 40, 71, 173, 255, 114, 113, 215, 255, 80, 79, 159, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 152, 151, 229, 255, 190, 189, 255, 255, 110, 109, 211, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 234, 238, 246, 255, 222, 229, 240, 255, 210, 219, 234, 255, 198, 209, 229, 255, 28, 65, 167, 255, 103, 103, 204, 255, 77, 77, 154, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 146, 145, 226, 255, 190, 189, 255, 255, 103, 102, 204, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 234, 238, 246, 255, 222, 229, 240, 255, 210, 219, 234, 255, 198, 209, 229, 255, 189, 202, 225, 255, 17, 59, 161, 255, 92, 93, 194, 255, 74, 74, 148, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 139, 138, 223, 255, 188, 187, 255, 255, 183, 182, 255, 255, 96, 99, 201, 255, 86, 94, 196, 255, 75, 88, 190, 255, 63, 82, 184, 255, 52, 77, 179, 255, 40, 71, 173, 255, 28, 65, 167, 255, 17, 59, 161, 255, 92, 93, 193, 255, 84, 86, 186, 255, 71, 71, 143, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 132, 131, 219, 255, 180, 179, 255, 255, 174, 173, 255, 255, 164, 163, 252, 255, 143, 142, 244, 255, 135, 134, 236, 255, 129, 128, 230, 255, 122, 121, 223, 255, 114, 113, 215, 255, 108, 107, 209, 255, 92, 93, 193, 255, 84, 86, 186, 255, 76, 80, 178, 255, 68, 68, 137, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 124, 123, 215, 255, 170, 169, 255, 255, 160, 159, 251, 255, 148, 147, 245, 255, 75, 91, 113, 255, 75, 91, 113, 255, 75, 91, 113, 255, 75, 91, 113, 255, 82, 98, 118, 255, 91, 106, 125, 255, 84, 86, 186, 255, 76, 79, 178, 255, 68, 73, 170, 255, 65, 65, 131, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 118, 117, 212, 255, 160, 159, 255, 255, 145, 144, 245, 255, 135, 134, 236, 255, 75, 91, 113, 255, 0, 0, 0, 255, 52, 60, 71, 255, 206, 217, 233, 255, 212, 221, 236, 255, 103, 116, 133, 255, 67, 75, 174, 255, 68, 73, 170, 255, 60, 66, 163, 255, 62, 62, 125, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 112, 111, 209, 255, 154, 153, 255, 255, 135, 134, 236, 255, 129, 128, 230, 255, 75, 91, 113, 255, 52, 60, 71, 255, 104, 120, 141, 255, 216, 224, 237, 255, 224, 231, 241, 255, 115, 127, 143, 255, 53, 65, 163, 255, 60, 66, 162, 255, 53, 61, 156, 255, 60, 59, 120, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 108, 107, 207, 255, 143, 142, 243, 255, 129, 128, 230, 255, 36, 68, 170, 255, 33, 50, 71, 255, 171, 180, 195, 255, 179, 187, 198, 255, 188, 193, 202, 255, 196, 200, 206, 255, 72, 77, 86, 255, 51, 62, 158, 255, 54, 61, 156, 255, 49, 57, 152, 255, 57, 57, 114, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 108, 107, 207, 84, 101, 100, 195, 255, 86, 85, 170, 255, 83, 82, 164, 255, 80, 79, 159, 255, 77, 77, 154, 255, 74, 74, 148, 255, 71, 71, 143, 255, 68, 68, 137, 255, 65, 65, 131, 255, 60, 59, 120, 255, 60, 59, 120, 255, 57, 57, 114, 255, 55, 54, 110, 255, 255, 0, 255, 255,
    255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255 };

  Ihandle* image = IupImageRGBA(16, 16, imgdata);
  return image;
}

static Ihandle* load_image_Test(void)
{
  Ihandle *image;
  unsigned char image_data_32[16 * 16 * 4] = {
  255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0,
  255, 0, 255, 0, 183, 182, 245, 255, 183, 182, 245, 255, 179, 178, 243, 255, 174, 173, 241, 255, 168, 167, 238, 255, 162, 161, 234, 255, 155, 154, 231, 255, 148, 147, 228, 255, 143, 142, 224, 255, 136, 135, 221, 255, 129, 128, 218, 255, 123, 122, 214, 255, 117, 116, 211, 255, 112, 111, 209, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 179, 178, 243, 255, 190, 189, 255, 255, 147, 146, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 75, 88, 190, 255, 89, 88, 176, 255, 89, 88, 176, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 173, 172, 240, 255, 190, 189, 255, 255, 138, 137, 239, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 234, 238, 246, 255, 63, 82, 184, 255, 51, 51, 103, 255, 86, 85, 170, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 167, 166, 237, 255, 190, 189, 255, 255, 129, 128, 230, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 234, 238, 246, 255, 222, 229, 240, 255, 52, 77, 179, 255, 122, 121, 223, 255, 83, 82, 164, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 159, 158, 233, 255, 190, 189, 255, 255, 119, 118, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 234, 238, 246, 255, 222, 229, 240, 255, 210, 219, 234, 255, 40, 71, 173, 255, 114, 113, 215, 255, 80, 79, 159, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 152, 151, 229, 255, 190, 189, 255, 255, 110, 109, 211, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 234, 238, 246, 255, 222, 229, 240, 255, 210, 219, 234, 255, 198, 209, 229, 255, 28, 65, 167, 255, 103, 103, 204, 255, 77, 77, 154, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 146, 145, 226, 255, 190, 189, 255, 255, 103, 102, 204, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 248, 251, 255, 234, 238, 246, 255, 222, 229, 240, 255, 210, 219, 234, 255, 198, 209, 229, 255, 189, 202, 225, 255, 17, 59, 161, 255, 92, 93, 194, 255, 74, 74, 148, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 139, 138, 223, 255, 188, 187, 255, 255, 183, 182, 255, 255, 96, 99, 201, 255, 86, 94, 196, 255, 75, 88, 190, 255, 63, 82, 184, 255, 52, 77, 179, 255, 40, 71, 173, 255, 28, 65, 167, 255, 17, 59, 161, 255, 92, 93, 193, 255, 84, 86, 186, 255, 71, 71, 143, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 132, 131, 219, 255, 180, 179, 255, 255, 174, 173, 255, 255, 164, 163, 252, 255, 143, 142, 244, 255, 135, 134, 236, 255, 129, 128, 230, 255, 122, 121, 223, 255, 114, 113, 215, 255, 108, 107, 209, 255, 92, 93, 193, 255, 84, 86, 186, 255, 76, 80, 178, 255, 68, 68, 137, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 124, 123, 215, 255, 170, 169, 255, 255, 160, 159, 251, 255, 148, 147, 245, 255, 75, 91, 113, 255, 75, 91, 113, 255, 75, 91, 113, 255, 75, 91, 113, 255, 82, 98, 118, 255, 91, 106, 125, 255, 84, 86, 186, 255, 76, 79, 178, 255, 68, 73, 170, 255, 65, 65, 131, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 118, 117, 212, 255, 160, 159, 255, 255, 145, 144, 245, 255, 135, 134, 236, 255, 75, 91, 113, 255, 0, 0, 0, 255, 52, 60, 71, 255, 206, 217, 233, 255, 212, 221, 236, 255, 103, 116, 133, 255, 67, 75, 174, 255, 68, 73, 170, 255, 60, 66, 163, 255, 62, 62, 125, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 112, 111, 209, 255, 154, 153, 255, 255, 135, 134, 236, 255, 129, 128, 230, 255, 75, 91, 113, 255, 52, 60, 71, 255, 104, 120, 141, 255, 216, 224, 237, 255, 224, 231, 241, 255, 115, 127, 143, 255, 53, 65, 163, 255, 60, 66, 162, 255, 53, 61, 156, 255, 60, 59, 120, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 108, 107, 207, 255, 143, 142, 243, 255, 129, 128, 230, 255, 36, 68, 170, 255, 33, 50, 71, 255, 171, 180, 195, 255, 179, 187, 198, 255, 188, 193, 202, 255, 196, 200, 206, 255, 72, 77, 86, 255, 51, 62, 158, 255, 54, 61, 156, 255, 49, 57, 152, 255, 57, 57, 114, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 108, 107, 207, 84, 101, 100, 195, 255, 86, 85, 170, 255, 83, 82, 164, 255, 80, 79, 159, 255, 77, 77, 154, 255, 74, 74, 148, 255, 71, 71, 143, 255, 68, 68, 137, 255, 65, 65, 131, 255, 60, 59, 120, 255, 60, 59, 120, 255, 57, 57, 114, 255, 55, 54, 110, 255, 255, 0, 255, 0,
  255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0 };

  image = IupImageRGBA(16, 16, image_data_32);
  return image;
}

typedef struct _color3D
{
  double r;
  double g;
  double b;
} color3D;

static void add_color_vertex(const color3D* c)
{
  glColor3d(c->r, c->g, c->b);
  glVertex3d(c->r, c->g, c->b);
}

static void draw_cube_face(const color3D* c1, const color3D* c2, const color3D* c3, const color3D* c4)
{
  color3D cm;
  cm.r = (c1->r + c2->r + c3->r + c4->r)/4;
  cm.g = (c1->g + c2->g + c3->g + c4->g)/4;
  cm.b = (c1->b + c2->b + c3->b + c4->b)/4;

  add_color_vertex(&cm);
  add_color_vertex(c1);
  add_color_vertex(c2);

  add_color_vertex(&cm);
  add_color_vertex(c2);
  add_color_vertex(c3);

  add_color_vertex(&cm);
  add_color_vertex(c3);
  add_color_vertex(c4);

  add_color_vertex(&cm);
  add_color_vertex(c4);
  add_color_vertex(c1);
}

const color3D c3D_black = {0.0, 0.0, 0.0};
const color3D c3D_red = {1.0, 0.0, 0.0};
const color3D c3D_green = {0.0, 1.0, 0.0};
const color3D c3D_blue = {0.0, 0.0, 1.0};
const color3D c3D_cian = {0.0, 1.0, 1.0};
const color3D c3D_magenta = {1.0, 0.0, 1.0};
const color3D c3D_yellow = {1.0, 1.0, 0.0};
const color3D c3D_white = {1.0, 1.0, 1.0};

static void draw_cube(void)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBegin(GL_TRIANGLES);

  draw_cube_face(&c3D_black, &c3D_blue, &c3D_magenta, &c3D_red);
  draw_cube_face(&c3D_black, &c3D_green, &c3D_cian, &c3D_blue);
  draw_cube_face(&c3D_black, &c3D_red, &c3D_yellow, &c3D_green);
  draw_cube_face(&c3D_white, &c3D_cian, &c3D_blue, &c3D_magenta);
  draw_cube_face(&c3D_white, &c3D_cian, &c3D_green, &c3D_yellow);
  draw_cube_face(&c3D_white, &c3D_magenta, &c3D_red, &c3D_yellow);

  glEnd();
}

static double model_view_matrix[16];
static int use_model_matrix = 0;

static void init(void)
{
  glClearColor(1, 1, 1, 0.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, 1, 1, 10);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.5, 0.5, 3.0,  /* posicao do observador */
    0.5, 0.5, 0.0,  /* ponto de referencia   */
    0.0, 1.0, 0.0); /* vup                   */

  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);

  if (use_model_matrix)
    glLoadMatrixd(model_view_matrix);
}

static int pos_x, pos_y;
static int move = 0;

static int button_cb(Ihandle *ih,int but,int pressed,int x,int y,char* status)
{
  (void)ih;
  (void)status;

  if (but==IUP_BUTTON1)
  {
    if (pressed)
    {
      pos_x = x;
      pos_y = y;
      move = 1;
    }
    else
    {
      move = 0;
    }
  }
  return IUP_DEFAULT;
}

#define INVERT_Y(_y) (height-y)

static int motion_cb(Ihandle *ih,int x,int y,char* status)
{
  (void)status;

  if (move)
  {
    double dif_x, dif_y;
    double dx, dy, dz;
    double x1, y1, z1;
    double x2, y2, z2;
    double angle, norma;
    int height = IupGetInt2(ih, "RASTERSIZE");
    double mv[16];
    double pm[16];
    int    vp[4];

    IupGLMakeCurrent(ih);

    glGetDoublev(GL_MODELVIEW_MATRIX, mv);
    glGetDoublev(GL_PROJECTION_MATRIX, pm);
    glGetIntegerv(GL_VIEWPORT, vp);

    dif_x = x - pos_x;
    dif_y = y - pos_y;

    if (dif_x == 0 && dif_y == 0)
      return IUP_DEFAULT;

    pos_x = x;
    pos_y = y;

    angle = sqrt(dif_x*dif_x + dif_y*dif_y);

    gluUnProject(pos_x, INVERT_Y(pos_y), 0.0,
      mv, pm, vp,
      &x1, &y1, &z1);
    gluUnProject((double)(dif_y + pos_x), (double)(dif_x + INVERT_Y(pos_y)), 0.0,
      mv, pm, vp,
      &x2, &y2, &z2);
    dx = x2-x1; dy = y2-y1; dz = z2-z1;
    norma = sqrt(dx*dx + dy*dy + dz*dz);
    dx /= norma; dy /= norma; dz /= norma;

    glMatrixMode(GL_MODELVIEW);
    glTranslated(0.5, 0.5, 0.5);
    glRotated (angle, dx, dy, dz);
    glTranslated(-0.5, -0.5, -0.5);
    glGetDoublev(GL_MODELVIEW_MATRIX, model_view_matrix);
    use_model_matrix = 1;

    draw_cube();
  
    IupGLSwapBuffers(ih); 
  }
  return IUP_DEFAULT;
}

static int action(Ihandle *ih)
{
  IupGLMakeCurrent(ih);

  init();

  draw_cube();

  IupGLSwapBuffers(ih); 

  return IUP_DEFAULT;
}

static int button_action_cb(Ihandle *ih)
{
  printf("ACTION_CB(%s) NAME=%s\n", IupGetClassName(ih), IupGetAttribute(ih, "NAME"));
  return IUP_DEFAULT;
}

static int toggle_action_cb(Ihandle *ih, int state)
{
  printf("ACTION_CB(%s, state=%d) NAME=%s\n", IupGetClassName(ih), state, IupGetAttribute(ih, "NAME"));
  return IUP_DEFAULT;
}

static int link_action_cb(Ihandle *ih, const char* url)
{
  printf("ACTION_CB(%s, url=%s)\n", IupGetClassName(ih), url);
  return IUP_DEFAULT;
}

static int val_action_cb(Ihandle *ih)
{
  Ihandle* pbar = (Ihandle*)IupGetAttribute(ih, "PROGRESSBAR");
  IupSetStrAttribute(pbar, "VALUE", IupGetAttribute(ih, "VALUE"));
  printf("ACTION_CB(%s, value=%0.1f) NAME=%s\n", IupGetClassName(ih), IupGetFloat(ih, "VALUE"), IupGetAttribute(ih, "NAME"));
  return IUP_DEFAULT;
}

static int expand_cb(Ihandle *ih)
{
  printf("ACTION(%s)\n", IupGetClassName(ih));
  return IUP_DEFAULT;
}

static int extrabutton_cb(Ihandle *ih, int button, int pressed)
{
  printf("EXTRABUTTON_CB(%s, but=%d, press=%d)\n", IupGetClassName(ih), button, pressed);
  return IUP_DEFAULT;
}

void GLCanvasCubeTest(void)
{
  Ihandle *dlg, *canvas, *box, *gtoggle, *gtoggle1, *gtoggle2,
    *hbox, *vbox, *glabel, *gsep1, *gsep2, *gbutton1, *gbutton2,
    *pbar1, *pbar2, *glink, *gval1, *gval2, *gframe1, *gframe2,
    *gexp1, *gexp2, *image_open, *image_close, *image_high,
    *gframe3, *vbox2, *gtoggle3, *gtoggle4, *gtoggle5, *gsbox;

  IupGLCanvasOpen();
  IupGLControlsOpen();

  glabel = IupGLLabel("Label");
//  IupSetAttribute(glabel, "FGCOLOR", "255 255 255");
  IupSetAttribute(glabel, "FONT", "Arial, 18");
  IupSetAttributeHandle(glabel, "IMAGE", load_image_Tecgraf());

  gbutton1 = IupGLButton("Button");
  IupSetAttribute(gbutton1, "PADDING", "5x5");
  //IupSetAttribute(gbutton1, "BGCOLOR", "245 245 245 92");
  IupSetCallback(gbutton1, "ACTION", button_action_cb);
  IupSetAttribute(gbutton1, "NAME", "button1");
  IupSetAttribute(gbutton1, "TIP", "Button Tip");
//  IupSetAttribute(gbutton1, "RASTERSIZE", "x100");

  gbutton2 = IupGLButton(NULL);
  IupSetAttribute(gbutton2, "PADDING", "5x5");
  IupSetAttributeHandle(gbutton2, "IMAGE", load_image_FileSave());
  IupSetCallback(gbutton2, "ACTION", button_action_cb);
  IupSetAttribute(gbutton2, "NAME", "button2");

  gtoggle = IupGLToggle("Toggle");
  IupSetAttribute(gtoggle, "PADDING", "5x5");
  IupSetCallback(gtoggle, "ACTION", (Icallback)toggle_action_cb);
  IupSetAttribute(gtoggle, "NAME", "toggle");

  gtoggle1 = IupGLToggle(NULL);
  IupSetAttribute(gtoggle1, "PADDING", "5x5");
  IupSetAttributeHandle(gtoggle1, "IMAGE", load_image_Test());
  IupSetCallback(gtoggle1, "ACTION", (Icallback)toggle_action_cb);
  IupSetAttribute(gtoggle1, "NAME", "toggle1");

  gtoggle2 = IupGLToggle(NULL);
  IupSetAttribute(gtoggle2, "PADDING", "5x5");
  IupSetAttributeHandle(gtoggle2, "IMAGE", load_image_Test());
  IupSetCallback(gtoggle2, "ACTION", (Icallback)toggle_action_cb);
  IupSetAttribute(gtoggle2, "NAME", "toggle2");

  gsep1 = IupGLSeparator();

  glink = IupGLLink("http://www.tecgraf.puc-rio.br/iup", "IUP Toolkit");
  IupSetCallback(glink, "ACTION", (Icallback)link_action_cb);

  pbar1 = IupGLProgressBar();
  IupSetAttribute(pbar1, "VALUE", "0.3");
  IupSetAttribute(pbar1, "SHOW_TEXT", "Yes");

  gval1 = IupGLVal();
  IupSetAttribute(gval1, "VALUE", "0.3");
  IupSetCallback(gval1, "VALUECHANGED_CB", val_action_cb);
  IupSetAttribute(gval1, "PROGRESSBAR", (char*)pbar1);
  IupSetAttribute(gval1, "NAME", "val1");
  IupSetAttribute(gval1, "TIP", "Val Tip");

  hbox = IupHbox(glabel, gsep1, gbutton1, gtoggle, glink, pbar1, gval1, NULL);
  IupSetAttribute(hbox, "ALIGNMENT", "ACENTER");
  IupSetAttribute(hbox, "MARGIN", "5x5");
  IupSetAttribute(hbox, "GAP", "5");

  pbar2 = IupGLProgressBar();
  IupSetAttribute(pbar2, "VALUE", "0.3");
  IupSetAttribute(pbar2, "ORIENTATION", "VERTICAL");

  gval2 = IupGLVal();
  IupSetAttribute(gval2, "VALUE", "0.3");
  IupSetAttribute(gval2, "ORIENTATION", "VERTICAL");
  IupSetCallback(gval2, "VALUECHANGED_CB", val_action_cb);
  IupSetAttribute(gval2, "PROGRESSBAR", (char*)pbar2);
  IupSetAttribute(gval2, "NAME", "val2");

  gsep2 = IupGLSeparator();
  IupSetAttribute(gsep2, "ORIENTATION", "HORIZONTAL");

  vbox = IupVbox(gbutton2, gsep2, 
    IupRadio(IupSetAttributes(IupVbox(gtoggle1, gtoggle2, NULL), "MARGIN=0x0")),
    pbar2,
    gval2,
    NULL);
  IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
  IupSetAttribute(vbox, "MARGIN", "5x5");
  IupSetAttribute(vbox, "GAP", "5");

  gtoggle5 = IupGLToggle("Radio Toggle");
  IupSetAttribute(gtoggle5, "PADDING", "5x5");
  IupSetCallback(gtoggle5, "ACTION", (Icallback)toggle_action_cb);
  IupSetAttribute(gtoggle5, "NAME", "toggle5");
  IupSetAttribute(gtoggle5, "CHECKMARK", "Yes");
//  IupSetAttribute(gtoggle5, "RIGHTBUTTON", "Yes");

  gtoggle3 = IupGLToggle("Radio Toggle");
  IupSetAttribute(gtoggle3, "PADDING", "5x5");
  IupSetCallback(gtoggle3, "ACTION", (Icallback)toggle_action_cb);
  IupSetAttribute(gtoggle3, "NAME", "toggle3");
  IupSetAttribute(gtoggle3, "CHECKMARK", "Yes");

  gtoggle4 = IupGLToggle("Radio Toggle");
  IupSetAttribute(gtoggle4, "PADDING", "5x5");
  IupSetAttributeHandle(gtoggle4, "IMAGE", load_image_Test());
  IupSetCallback(gtoggle4, "ACTION", (Icallback)toggle_action_cb);
  IupSetAttribute(gtoggle4, "NAME", "toggle4");
  IupSetAttribute(gtoggle4, "CHECKMARK", "Yes");
//  IupSetAttribute(gtoggle4, "RIGHTBUTTON", "Yes");

  vbox2 = IupVbox(
    IupRadio(IupSetAttributes(IupVbox(gtoggle3, gtoggle4, NULL), "MARGIN=0x0")),
    gtoggle5,
    NULL);

  gsbox = IupSetAttributes(IupGLScrollBox(vbox2), "RASTERSIZE=90x90");

  gframe1 = IupSetAttributes(IupGLFrame(hbox), "TITLE=Frame");
  gframe2 = IupSetAttributes(IupGLFrame(vbox), "BACKGROUND=\"250 250 160\", FRAMECOLOR=\"250 250 160\"");
  gframe3 = IupSetAttributes(IupGLFrame(gsbox), "TITLE=Frame, TITLEBOX=Yes");

  gexp1 = IupSetAttributes(IupGLExpander(gframe1), "TITLE=Expander");
  gexp2 = IupSetAttributes(IupGLExpander(gframe2), "BARPOSITION=LEFT");

  canvas = IupGLCanvasBox(
    IupSetAttributes(gexp1, "HORIZONTALALIGN=ACENTER, VERTICALALIGN=ATOP, MOVEABLE=Yes"),
    IupSetAttributes(gexp2, "HORIZONTALALIGN=ALEFT, VERTICALALIGN=ACENTER"),
    IupSetAttributes(gframe3, "MOVEABLE=Yes, POSITION=\"550,200\""),
    NULL);

  image_open = IupImage(16, 16, img_open);
  image_close = IupImage(16, 16, img_close);
  image_high = IupImage(16, 16, img_close);
  IupSetAttribute(image_open, "0", "BGCOLOR");
  IupSetAttribute(image_open, "1", "192 192 192");
  IupSetAttribute(image_close, "0", "BGCOLOR");
  IupSetAttribute(image_close, "1", "192 192 192");
  IupSetAttribute(image_high, "1", "192 192 192");

  //  IupSetAttribute(gexp1, "BARSIZE", "50");
  //  IupSetAttributeHandle(gexp1, "IMAGE", image_close);
  //  IupSetAttributeHandle(gexp1, "IMOPEN", image_open);
  //  IupSetAttribute(gexp1, "IMAGE", "img1");
  IupSetCallback(gexp1, "ACTION", (Icallback)expand_cb);
  IupSetAttribute(gexp1, "EXTRABUTTONS", "3");
  IupSetCallback(gexp1, "EXTRABUTTON_CB", (Icallback)extrabutton_cb);
  IupSetAttributeHandle(gexp1, "IMAGEEXTRA1", image_close);
  IupSetAttributeHandle(gexp1, "IMAGEEXTRAPRESS1", image_open);
  IupSetAttributeHandle(gexp1, "IMAGEEXTRAHIGHLIGHT1", image_high);
  IupSetAttributeHandle(gexp1, "IMAGEEXTRA2", image_close);
  IupSetAttributeHandle(gexp1, "IMAGEEXTRAPRESS2", image_open);
  IupSetAttributeHandle(gexp1, "IMAGEEXTRAHIGHLIGHT2", image_high);
  IupSetAttributeHandle(gexp1, "IMAGEEXTRA3", image_close);
  IupSetAttributeHandle(gexp1, "IMAGEEXTRAPRESS3", image_open);
  IupSetAttributeHandle(gexp1, "IMAGEEXTRAHIGHLIGHT3", image_high);
  //IupSetAttribute(gexp1, "REDRAWALL", "No");

  IupSetCallback(canvas, "ACTION", action);
  IupSetCallback(canvas, "BUTTON_CB", (Icallback)button_cb);
  IupSetCallback(canvas, "MOTION_CB", (Icallback)motion_cb);
  IupSetAttribute(canvas, "BUFFER", "DOUBLE");
  IupSetAttribute(canvas, "MARGIN", "10x10");

  box = IupVbox(canvas, NULL);
  IupSetAttribute(box, "MARGIN", "5x5");

  dlg = IupDialog(box);
  IupSetAttribute(dlg, "TITLE", "IupGLCanvas Test");
  IupSetAttribute(dlg, "RASTERSIZE", "800x600");

  IupMap(dlg);

  IupGLMakeCurrent(canvas);
  printf("Vendor: %s\n", glGetString(GL_VENDOR));
  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("Version: %s\n", glGetString(GL_VERSION));

  IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
  IupSetAttribute(dlg, "RASTERSIZE", NULL);
}

#ifndef BIG_TEST
int main(int argc, char* argv[])
{
  IupOpen(&argc, &argv);

  GLCanvasCubeTest();

  IupMainLoop();

  IupClose();

  return EXIT_SUCCESS;
}
#endif
#endif
