#ifndef DRIVE_DATA_H
#define DRIVE_DATA_H

#include <stdint.h>

typedef struct {
    float time_ms;
    float speed;
    float rpm;
} DriveData;

extern const DriveData driveLog[];
extern const int driveLogLength;

#endif // DRIVE_DATA_H
