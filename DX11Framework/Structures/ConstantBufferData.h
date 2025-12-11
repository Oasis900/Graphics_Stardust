#pragma once
#ifndef SINGLE_CONSTBUFFER_H
#define SINGLE_CONSTBUFFER_H
#include <DirectXMath.h>
using DirectX::XMMATRIX;
using DirectX::XMFLOAT4;
using DirectX::XMFLOAT3;

// --- Constant Buffer Data as a Singleton --- //
/* - Modified Constant Buffer Data to become a Singleton,
 * this was done to allow for one persistant copy of the data
 * to exist throughout the program, for going the need to constantly
 * instantiate new copies where it required use */

class ConstantBufferData
{
private:
	ConstantBufferData() = default;

    // --- Constant Buffer Format | Comments to demonstrate how bytes/bits are passed to GPU --- // 
    XMMATRIX projection_;				// 64  | 16
    XMMATRIX view_;						// 128 | 16
    XMMATRIX world_;					// 160 | 16
	//--------------------------------------------------//
	XMFLOAT4 diffuse_light_;			// 176 | 4
	XMFLOAT4 diffuse_mat_;				// 192 | 4
	//--------------------------------------------------//
	XMFLOAT4 ambient_light_;			// 208 | 4
	XMFLOAT4 ambient_mat_;				// 224 | 4
	//--------------------------------------------------//
	XMFLOAT4 specular_light_;			// 240 | 4
	XMFLOAT4 specular_mat_;				// 256 | 4
	XMFLOAT3 camera_position_;			// 268 | 3
	float specular_power_;				// 272 | 1
	//--------------------------------------------------//
    XMFLOAT3 point_pos_;				// 284 | 3
    float point_range_;					// 288 | 1
    XMFLOAT4 point_color_;				// 304 | 4
	//--------------------------------------------------//
    XMFLOAT3 light_direction_;			// 316 | 3
    float simple_count_;				// 320 | 1
	//--------------------------------------------------//
    XMFLOAT4 has_texture_;				// 336 | 4 | X = has texture | Y = Object for texturing
public:
	static ConstantBufferData& GetInstance()
	{
		static ConstantBufferData instance;
		return instance;
	};
	
	ConstantBufferData(const ConstantBufferData&) = delete;
	ConstantBufferData(ConstantBufferData&&) = delete;
	ConstantBufferData& operator=(const ConstantBufferData&) = delete;
	ConstantBufferData& operator=(ConstantBufferData&&) = delete;
	//--------------------------------------------------//
	XMMATRIX GetProjection() const {return projection_;}
	XMMATRIX GetView() const {return view_;}
	XMMATRIX GetWorld() const {return world_;}
	//--------------------------------------------------//
	XMFLOAT4 GetDiffuseLight() const {return diffuse_light_;}
	XMFLOAT4 GetDiffuseMaterial() const {return diffuse_mat_;}
	//--------------------------------------------------//
	XMFLOAT4 GetAmbientLight() const {return ambient_light_;}
	XMFLOAT4 GetAmbientMaterial() const {return ambient_mat_;}
	//--------------------------------------------------//
	XMFLOAT4 GetSpecularLight() const {return specular_light_;}
	XMFLOAT4 GetSpecularMaterial() const {return specular_mat_;}
	XMFLOAT3 GetCameraPosition() const {return camera_position_;}
	float GetSpecularPower() const {return specular_power_;}
	//--------------------------------------------------//
	XMFLOAT3 GetPointPosition() const {return point_pos_;}
	float GetPointRange() const {return point_range_;}
	XMFLOAT4 GetPointColor() const {return point_color_;}
	//--------------------------------------------------//
	XMFLOAT3 GetLightDirection() const {return light_direction_;}
	float GetSimpleCount() const {return simple_count_;}
	bool GetHasTexture() const {return static_cast<bool>(has_texture_.x);}
	int GetObjectType() const {return static_cast<int>(has_texture_.y);}
	//--------------------------------------------------//
	//--------------------------------------------------//
	void SetProjection(const XMMATRIX& projection) {this->projection_ = projection;}
	void SetView(const XMMATRIX& view) {this->view_ = view;}
	void SetWorld(const XMMATRIX& world) {this->world_ = world;}
	//--------------------------------------------------//
	void SetDiffuseLight(const XMFLOAT4& diffuse_light) {this->diffuse_light_ = diffuse_light;}
	void SetDiffuseMaterial(const XMFLOAT4& diffuse_material) {this->diffuse_mat_ = diffuse_material;}
	//--------------------------------------------------//
	void SetAmbientLight(const XMFLOAT4& ambient_light) {this->ambient_light_ = ambient_light;}
	void SetAmbientMaterial(const XMFLOAT4& ambient_material) {this->ambient_mat_ = ambient_material;}
	//--------------------------------------------------//
	void SetSpecularLight(const XMFLOAT4& specular_light) {this->specular_light_ = specular_light;}
	void SetSpecularMaterial(const XMFLOAT4& specular_material) {this->specular_mat_ = specular_material;}
	void SetCameraPosition(const XMFLOAT3& camera_position) {this->camera_position_ = camera_position;}
	void SetSpecularPower(float power) {this->specular_power_ = power;}
	//--------------------------------------------------//
	void SetPointPosition(const XMFLOAT3& point_position) {this->point_pos_ = point_position;}
	void SetPointRange(float point_range) {this->point_range_ = point_range;}
	void SetPointColor(const XMFLOAT4& point_color) {this->point_color_ = point_color;}
	//--------------------------------------------------//
	void SetLightDirection(const XMFLOAT3& light_direction) {this->light_direction_ = light_direction;}
	void SetSimpleCount(float simple_count) {this->simple_count_ = simple_count;}
	void SetHasTexture(bool has_texture) {this->has_texture_.x = static_cast<float>(abs(has_texture));}
	void SetObject(int object_type) {this->has_texture_.y = static_cast<float>(abs(object_type));}
};
#endif
