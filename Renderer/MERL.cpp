// Copyright 2005 Mitsubishi Electric Research Laboratories All Rights Reserved.

// Permission to use, copy and modify this software and its documentation without
// fee for educational, research and non-profit purposes, is hereby granted, provided
// that the above copyright notice and the following three paragraphs appear in all copies.

// To request permission to incorporate this software into commercial products contact:
// Vice President of Marketing and Business Development;
// Mitsubishi Electric Research Laboratories (MERL), 201 Broadway, Cambridge, MA 02139 or 
// <license@merl.com>.

// IN NO EVENT SHALL MERL BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL,
// OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND
// ITS DOCUMENTATION, EVEN IF MERL HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

// MERL SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED
// HEREUNDER IS ON AN "AS IS" BASIS, AND MERL HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT,
// UPDATES, ENHANCEMENTS OR MODIFICATIONS.

#include "MERL.h"

MERL::MERL()
{
	isLoaded = 0;
}

MERL::~MERL()
{

}

// cross product of two vectors
void MERL::cross_product (double* v1, double* v2, double* out)
{
	out[0] = v1[1]*v2[2] - v1[2]*v2[1];
	out[1] = v1[2]*v2[0] - v1[0]*v2[2];
	out[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

// normalize vector
void MERL::normalize(double* v)
{
	// normalize
	double len = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	v[0] = v[0] / len;
	v[1] = v[1] / len;
	v[2] = v[2] / len;
}

// rotate vector along one axis
void MERL::rotate_vector(double* vector, double* axis, double angle, double* out)
{
	double temp;
	double cross[3];
	double cos_ang = cos(angle);
	double sin_ang = sin(angle);

	out[0] = vector[0] * cos_ang;
	out[1] = vector[1] * cos_ang;
	out[2] = vector[2] * cos_ang;

	temp = axis[0]*vector[0]+axis[1]*vector[1]+axis[2]*vector[2];
	temp = temp*(1.0-cos_ang);

	out[0] += axis[0] * temp;
	out[1] += axis[1] * temp;
	out[2] += axis[2] * temp;

	cross_product (axis,vector,cross);
	
	out[0] += cross[0] * sin_ang;
	out[1] += cross[1] * sin_ang;
	out[2] += cross[2] * sin_ang;
}


// convert standard coordinates to half vector/difference vector coordinates
void MERL::std_coords_to_half_diff_coords(double theta_in, double fi_in, double theta_out, double fi_out,
								double& theta_half,double& fi_half,double& theta_diff,double& fi_diff )
{

	// compute in vector
	double in_vec_z = cos(theta_in);
	double proj_in_vec = sin(theta_in);
	double in_vec_x = proj_in_vec*cos(fi_in);
	double in_vec_y = proj_in_vec*sin(fi_in);
	double in[3]= {in_vec_x,in_vec_y,in_vec_z};
	normalize(in);


	// compute out vector
	double out_vec_z = cos(theta_out);
	double proj_out_vec = sin(theta_out);
	double out_vec_x = proj_out_vec*cos(fi_out);
	double out_vec_y = proj_out_vec*sin(fi_out);
	double out[3]= {out_vec_x,out_vec_y,out_vec_z};
	normalize(out);


	// compute halfway vector
	double half_x = (in_vec_x + out_vec_x)/2.0f;
	double half_y = (in_vec_y + out_vec_y)/2.0f;
	double half_z = (in_vec_z + out_vec_z)/2.0f;
	double half[3] = {half_x,half_y,half_z};
	normalize(half);

	// compute  theta_half, fi_half
	theta_half = acos(half[2]);
	fi_half = atan2(half[1], half[0]);


	double bi_normal[3] = {0.0, 1.0, 0.0};
	double normal[3] = { 0.0, 0.0, 1.0 };
	double temp[3];
	double diff[3];

	// compute diff vector
	rotate_vector(in, normal , -fi_half, temp);
	rotate_vector(temp, bi_normal, -theta_half, diff);
	
	// compute  theta_diff, fi_diff	
	theta_diff = acos(diff[2]);
	fi_diff = atan2(diff[1], diff[0]);

}


// Given a pair of incoming/outgoing angles, look up the BRDF.
void MERL::lookup_brdf_val(double* brdf, double theta_in, double fi_in,
			  double theta_out, double fi_out, 
			  double& red_val,double& green_val,double& blue_val)
{
	// Convert to halfangle / difference angle coordinates
	double theta_half, fi_half, theta_diff, fi_diff;
	
	std_coords_to_half_diff_coords(theta_in, fi_in, theta_out, fi_out,
		       theta_half, fi_half, theta_diff, fi_diff);


	// Find index.
	// Note that phi_half is ignored, since isotropic BRDFs are assumed
	int ind = phi_diff_index(fi_diff) +
		  theta_diff_index(theta_diff) * BRDF_SAMPLING_RES_PHI_D / 2 +
		  theta_half_index(theta_half) * BRDF_SAMPLING_RES_PHI_D / 2 *
					         BRDF_SAMPLING_RES_THETA_D;

	red_val = brdf[ind] * RED_SCALE;
	green_val = brdf[ind + BRDF_SAMPLING_RES_THETA_H*BRDF_SAMPLING_RES_THETA_D*BRDF_SAMPLING_RES_PHI_D/2] * GREEN_SCALE;
	blue_val = brdf[ind + BRDF_SAMPLING_RES_THETA_H*BRDF_SAMPLING_RES_THETA_D*BRDF_SAMPLING_RES_PHI_D] * BLUE_SCALE;

	
	if (red_val < 0.0 || green_val < 0.0 || blue_val < 0.0)
	{
		//std::cout << "Below horizon.  " << theta_in << ", " << fi_in << ", " << theta_out	<< ", " << fi_out;
		red_val = 0.0;
		green_val = 0.0;
		blue_val = 0.0;
	}
}



// Read BRDF data
bool MERL::read_brdf(const char *filename, double* &brdf)
{
	FILE *f = fopen(filename, "rb");
	if (!f)
		return false;

	int dims[3];
	fread(dims, sizeof(int), 3, f);
	int n = dims[0] * dims[1] * dims[2];
	if (n != BRDF_SAMPLING_RES_THETA_H *
		 BRDF_SAMPLING_RES_THETA_D *
		 BRDF_SAMPLING_RES_PHI_D / 2) 
	{
		fprintf(stderr, "Dimensions don't match\n");
		fclose(f);
		return false;
	}

	brdf = (double*) malloc (sizeof(double)*3*n);
	fread(brdf, sizeof(double), 3*n, f);

	fclose(f);
	return true;
}



void MERL::run(const char* filename)
{
	// read brdf
	if (!read_brdf(filename, brdf)) 
	{
		fprintf(stderr, "Error reading %s\n", filename);
		exit(1);
	}
	isLoaded = 1;
}

void MERL::Init()
{

}

void MERL::Start()
{
	std::cout << "MERL start\n";
	SetUniform();
	isUpdatable = 1;
}

void MERL::Update()
{
	if (isUpdatable == 1)
	{
		glUniform1i(MaterialTypeID, type);
	}
}

void MERL::SetUniform()
{
	MaterialTypeID = glGetUniformLocation(ShaderID, "MaterialType");
}

std::vector<glm::vec3> MERL::SetColorBuffer(std::vector<glm::vec3> vertex, std::vector<glm::vec3> normal, std::vector<glm::vec3> color, glm::vec3 light, glm::vec3 view)
{
	color.clear();
	if (!isLoaded)
	{
		std::cout << "brdf data is not loaded.\n";
		return color;
	}

	float theta, theta_in, phi_in, theta_out, phi_out;
	glm::vec3 axis_in, axis_out;
	for (int i = 0; i < vertex.size(); i++)
	{
		glm::vec3 in_vec = glm::normalize(light - vertex[i]);
		glm::vec3 out_vec = glm::normalize(view - vertex[i]);
		glm::vec3 n = glm::normalize(normal[i]);
		glm::vec3 y = glm::normalize(glm::vec3(0.0, 1.0, 0.0));
		theta = acos(glm::dot(n, y));
		glm::vec3 axis = glm::cross(n, y);
		glm::mat3 rotmat = glm::rotate(theta, axis);
		glm::vec3 new_n = rotmat * n;
		in_vec = rotmat * in_vec;
		out_vec = rotmat * out_vec;
				
		theta_in = acos(in_vec.y);
		theta_out = acos(out_vec.y);
		phi_in = atan2(in_vec.z, in_vec.x);
		phi_out = atan2(out_vec.z, out_vec.x);
		double red, green, blue;
		lookup_brdf_val(brdf, theta_in, phi_in, theta_out, phi_out, red, green, blue);
		color.push_back(glm::vec3(red, green, blue));
	}

	isColorBuffered = 1;
	return color;
}