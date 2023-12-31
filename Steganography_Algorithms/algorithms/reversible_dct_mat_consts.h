
#define mat_width 8
#define mat_height 8


static const long double s0[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000,    
    -1.448, -1.620, +2.041, -0.184, +1.351, +1.518, +0.914, +1.000 
};

static const long double i_s0[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000,    
    +1.448, +1.620, -2.041, +0.184, -1.351, -1.518, -0.914, +1.000 
};

static const long double s1[mat_height * mat_width] = {
    +1.000, +1.204, -1.697, +0.216, -1.291, -0.910, -1.202, +0.750,
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double i_s1[mat_height * mat_width] = {
    +1.000, -1.204, +1.697, -0.216, +1.291, +0.910, +1.202, -0.750,
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double s2[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000,
    -0.832, +1.000, -0.949, +0.476, -1.004, -0.753, -1.001, +0.624, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double i_s2[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000,
    +0.832, +1.000, +0.949, -0.476, +1.004, +0.753, +1.001, -0.624, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double s3[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.575, +0.692, +1.000, -0.757, +0.248, +0.377, +0.675, -0.432, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double i_s3[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    -0.575, -0.692, +1.000, +0.757, -0.248, -0.377, -0.675, +0.432, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double s4[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    -0.459, -0.986, +0.356, +1.000, -0.778, -0.792, -0.644, +0.340, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double i_s4[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.459, +0.986, -0.356, +1.000, +0.778, +0.792, +0.644, -0.340, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double s5[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.380, +0.632, +0.070, +0.242, +1.000, -0.024, +0.163, -0.319,
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double i_s5[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    -0.380, -0.632, -0.070, -0.242, +1.000, +0.024, -0.163, +0.319,
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double s6[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.129, -0.687, +0.522, +0.688, +1.438, +1.000, +0.720, +0.310, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double i_s6[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    -0.129, +0.687, -0.522, -0.688, -1.438, +1.000, -0.720, -0.310, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double s7[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    +0.489, -1.395, +0.764, +0.466, +1.682, -1.154, +1.000, +0.634,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double i_s7[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    -0.489, +1.395, -0.764, -0.466, -1.682, +1.154, +1.000, -0.634,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000   
};

static const long double s8[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000,
    -0.044, -2.200, +1.177, +0.575, +2.099, -1.263, -1.152, -1.000   
};

static const long double i_s8[mat_height * mat_width] = {
    +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000, +0.000, 
    +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000, +0.000,
    +0.000, +0.000, +0.000, +0.000, +0.000, +0.000, +1.000, +0.000,
    -0.044, -2.200, +1.177, +0.575, +2.099, -1.263, -1.152, -1.000   
};

static const long double p[mat_height * mat_width] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 
    0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 
};

static const long double i_p[mat_height * mat_width] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 
    0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0 
};

#undef mat_width
#undef mat_height