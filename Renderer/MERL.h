#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "Material.h"

#define BRDF_SAMPLING_RES_THETA_H       90
#define BRDF_SAMPLING_RES_THETA_D       90
#define BRDF_SAMPLING_RES_PHI_D         360

#define RED_SCALE (1.0/1500.0)
#define GREEN_SCALE (1.15/1500.0)
#define BLUE_SCALE (1.66/1500.0)
#define M_PI	3.1415926535897932384626433832795

////Workflow////////////
// 1. Readfile
// 2. lookup_brdf_val
// 3. std_coords_to_half_diff_coords
// 4. Find index
// 5. get RGB data
class MERL : public Material
{
public:
	MERL();
	~MERL();

	// cross product of two vectors
	void cross_product(double* v1, double* v2, double* out);
	// normalize vector
	void normalize(double* v);
	// rotate vector along one axis
	void rotate_vector(double* vector, double* axis, double angle, double* out);
	// convert standard coordinates to half vector/difference vector coordinates
	void std_coords_to_half_diff_coords(double theta_in, double fi_in, double theta_out, double fi_out,
		double& theta_half, double& fi_half, double& theta_diff, double& fi_diff);
	// Given a pair of incoming/outgoing angles, look up the BRDF.
	void lookup_brdf_val(double* brdf, double theta_in, double fi_in,
		double theta_out, double fi_out,
		double& red_val, double& green_val, double& blue_val);
	// Read BRDF data
	bool read_brdf(const char *filename, double* &brdf);

	inline int theta_half_index(double theta_half);
	inline int theta_diff_index(double theta_diff);
	inline int phi_diff_index(double phi_diff);

	void run(const char* filename);
	std::vector<glm::vec3> SetColorBuffer(std::vector<glm::vec3> vertex, std::vector<glm::vec3> normal, std::vector<glm::vec3> color, glm::vec3 light, glm::vec3 view);

	double* brdf;

	// Virtual methods inherited from Material
	virtual void Start();
	virtual void Init();
	virtual void Update();
	virtual void SetUniform();

	// uniform variable handles
	GLuint FresnelID;
	GLuint RoughnessID;
	GLuint DiffuseID;
	GLuint SpecularID;
	
private:
	int isLoaded;
	int isColorBuffered;
};

// Lookup theta_half index
// This is a non-linear mapping!
// In:  [0 .. pi/2]
// Out: [0 .. 89]
inline int MERL::theta_half_index(double theta_half)
{
	if (theta_half <= 0.0)
		return 0;
	double theta_half_deg = ((theta_half / (M_PI / 2.0))*BRDF_SAMPLING_RES_THETA_H);
	double temp = theta_half_deg * BRDF_SAMPLING_RES_THETA_H;
	temp = sqrt(temp);
	int ret_val = (int)temp;
	if (ret_val < 0) ret_val = 0;
	if (ret_val >= BRDF_SAMPLING_RES_THETA_H)
		ret_val = BRDF_SAMPLING_RES_THETA_H - 1;
	return ret_val;
}

// Lookup theta_diff index
// In:  [0 .. pi/2]
// Out: [0 .. 89]
inline int MERL::theta_diff_index(double theta_diff)
{
	int tmp = int(theta_diff / (M_PI * 0.5) * BRDF_SAMPLING_RES_THETA_D);
	if (tmp < 0)
		return 0;
	else if (tmp < BRDF_SAMPLING_RES_THETA_D - 1)
		return tmp;
	else
		return BRDF_SAMPLING_RES_THETA_D - 1;
}


// Lookup phi_diff index
inline int MERL::phi_diff_index(double phi_diff)
{
	// Because of reciprocity, the BRDF is unchanged under
	// phi_diff -> phi_diff + M_PI
	if (phi_diff < 0.0)
		phi_diff += M_PI;

	// In: phi_diff in [0 .. pi]
	// Out: tmp in [0 .. 179]
	int tmp = int(phi_diff / M_PI * BRDF_SAMPLING_RES_PHI_D / 2);
	if (tmp < 0)
		return 0;
	else if (tmp < BRDF_SAMPLING_RES_PHI_D / 2 - 1)
		return tmp;
	else
		return BRDF_SAMPLING_RES_PHI_D / 2 - 1;
}