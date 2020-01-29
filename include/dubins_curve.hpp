#pragma once

#include <iostream>
#include <fstream>
#include <math.h>
#ifndef DUBINS_H
#define DUBINS_H


#include "student_planning_interface.hpp"


void dubins_test();

typedef enum 
{
    LSL = 0,
    LSR = 1,
    RSL = 2,
    RSR = 3,
    RLR = 4,
    LRL = 5
} DubinsPathType;

typedef struct 
{
    /* the initial configuration */
    double qi[3];        
    /* the lengths of the three segments */
    double param[3];     
    /* model forward velocity / model angular velocity */
    double rho;          
    /* the path type described */
    DubinsPathType type; 
} DubinsPath;

struct arc_extract
{
    Point start_point;
    Point end_point;
    float radius;
    Point center;
    float length;
    int LSR;

};

// struct line_extract @Alvaro NOT USED ANYMORE
// {
//     Point start_point;
//     Point end_point;
//     float length;

// };

#define EDUBOK        (0)   /* No error */
#define EDUBCOCONFIGS (1)   /* Colocated configurations */
#define EDUBPARAM     (2)   /* Path parameterisitation error */
#define EDUBBADRHO    (3)   /* the rho value is invalid */
#define EDUBNOPATH    (4)   /* no connection between configurations with this word */

/**
 * Callback function for path sampling
 *
 * @note the q parameter is a configuration
 * @note the t parameter is the distance along the path
 * @note the user_data parameter is forwarded from the caller
 * @note return non-zero to denote sampling should be stopped
 */
typedef int (*DubinsPathSamplingCallback)(double q[3], double t, void* user_data,DubinsPath* path,double end_point_segments[6]);

/**
 * Generate a path from an initial configuration to
 * a target configuration, with a specified maximum turning
 * radii
 *
 * A configuration is (x, y, theta), where theta is in radians, with zero
 * along the line x = 0, and counter-clockwise is positive
 *
 * @param path  - the resultant path
 * @param q0    - a configuration specified as an array of x, y, theta
 * @param q1    - a configuration specified as an array of x, y, theta
 * @param rho   - turning radius of the vehicle (forward velocity divided by maximum angular velocity)
 * @return      - non-zero on error
 */
int dubins_shortest_path(DubinsPath* path, double q0[3], double q1[3], double rho);

/**
 * Generate a path with a specified word from an initial configuration to
 * a target configuration, with a specified turning radius 
 *
 * @param path     - the resultant path
 * @param q0       - a configuration specified as an array of x, y, theta
 * @param q1       - a configuration specified as an array of x, y, theta
 * @param rho      - turning radius of the vehicle (forward velocity divided by maximum angular velocity)
 * @param pathType - the specific path type to use
 * @return         - non-zero on error
 */
int dubins_path(DubinsPath* path, double q0[3], double q1[3], double rho, DubinsPathType pathType);

/**
 * Calculate the length of an initialised path
 *
 * @param path - the path to find the length of
 */
double dubins_path_length(DubinsPath* path);

/**
 * Return the length of a specific segment in an initialized path
 *
 * @param path - the path to find the length of
 * @param i    - the segment you to get the length of (0-2)
 */
double dubins_segment_length(DubinsPath* path, int i);

/**
 * Return the normalized length of a specific segment in an initialized path
 *
 * @param path - the path to find the length of
 * @param i    - the segment you to get the length of (0-2)
 */
double dubins_segment_length_normalized( DubinsPath* path, int i );

/**
 * Extract an integer that represents which path type was used
 *
 * @param path    - an initialised path
 * @return        - one of LSL, LSR, RSL, RSR, RLR or LRL 
 */
DubinsPathType dubins_path_type(DubinsPath* path);

/**
 * Calculate the configuration along the path, using the parameter t
 *
 * @param path - an initialised path
 * @param t    - a length measure, where 0 <= t < dubins_path_length(path)
 * @param q    - the configuration result
 * @returns    - non-zero if 't' is not in the correct range
 */
int dubins_path_sample(DubinsPath* path, double t, double q[3],double end_point_segments[6]);

/**
 * Walk along the path at a fixed sampling interval, calling the
 * callback function at each interval
 *
 * The sampling process continues until the whole path is sampled, or the callback returns a non-zero value
 *
 * @param path      - the path to sample
 * @param stepSize  - the distance along the path for subsequent samples
 * @param cb        - the callback function to call for each sample
 * @param user_data - optional information to pass on to the callback
 *
 * @returns - zero on successful completion, or the result of the callback
 */
int dubins_path_sample_many(DubinsPath* path, 
                            double stepSize, 
                            DubinsPathSamplingCallback cb, 
                            void* user_data,double end_point_segments[6]);

/**
 * Convenience function to identify the endpoint of a path
 *
 * @param path - an initialised path
 * @param q    - the configuration result
 */
int dubins_path_endpoint(DubinsPath* path, double q[3]);

/**
 * Convenience function to extract a subset of a path
 *
 * @param path    - an initialised path
 * @param t       - a length measure, where 0 < t < dubins_path_length(path)
 * @param newpath - the resultant path
 */
int dubins_extract_subpath(DubinsPath* path, double t, DubinsPath* newpath);



//Helper functions

bool dubins_wrapper_api(Path& path,struct arc_extract three_seg[3],double start_dubins[3],double end_dubins[3],double rho);

/**
 * Convenience function to extract a subset of a path
 *
 * @param path    - an DubinsPath path
 * @param end_point_segments       - end points of segment 1 & 2
 * @param three_seg - Each segment extracted and its data
 * @param goal - segmet 3 end point
 */
void dubins_segments_extract(DubinsPath *path, double *end_point_segments,double rho,struct arc_extract three_seg[3] ,double goal[3]);

Point find_center(Point start,Point end,float radius,float length,int LSR);

#endif