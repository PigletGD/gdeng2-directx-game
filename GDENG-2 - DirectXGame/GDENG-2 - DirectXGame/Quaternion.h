#pragma once
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <cmath>

class AQuaternion : public Vector4D
{
public:
	//Iden Quaternion
	AQuaternion() : Vector4D(0, 0, 0, 1) {

	}

	//Generate quaterion based on static vals
	AQuaternion(float x, float y, float z, float w) : Vector4D(x, y, z, w) {

	}

	//Generate quaternion based on angle-axis
	AQuaternion(Vector3D axis, float angle) : Vector4D() { /*Blank instructor call will ensure no null fuckery happens*/

		//Generate angle halves
		float sinHalf = sin(angle / 2);
		float cosHalf = cos(angle / 2);

		//Set values of the axis of rotation via the normalized (by sin or cos) values of the "axis" passed as a vector3
		m_x = axis.getX() * sinHalf;
		m_y = axis.getY() * sinHalf;
		m_z = axis.getZ() * sinHalf;

		//Set the magnitude of the quaternion based on the "opposing angle metric" (whatever the normalization did not use)
		m_w = cosHalf;

		//Logic here is that when you yeet the object into the 4th dimension, the magnitude value will remain the same and will allow the
		//recalculation of the correct angle vals by virtue of the amount of difference from the old sinHalf and the new sinHalf (in ratio to cosHalf)
		//TL;DR cosHalf makes sure that sinHalf doesn't malfunction
	}

	//Normalizing a quaternion basically means reducing all values by the ratio of all values
	AQuaternion Normalized(AQuaternion Initial) {

		//Get initial quaternions' length
		float length = Initial.getLength();

		//use as rationalizer for all elements of the quaternion
		AQuaternion Normal = AQuaternion(Initial.m_x / length, Initial.m_y / length, Initial.m_z / length, Initial.m_w / length);

		return Normal;
	}

	float getLength()
	{
		return sqrt(pow(m_x,2) + pow(m_y,2) + pow(m_z,2) + pow(m_w,2));
	}

	
	static AQuaternion Conjugate(AQuaternion Initial) { /*Returns the inverse of the given quaternion with the same angle magnitude*/
		AQuaternion Conjugate = AQuaternion(-Initial.m_x, -Initial.m_y, -Initial.m_z, Initial.m_w);

		return Conjugate;
	}

	//Give new rotated position by quaternion
	static Vector3D Rotate(Vector3D* base, AQuaternion rotate) {

		Vector3D newVector((rotate.m_y * base->m_z) - (rotate.m_z * base->m_y), (rotate.m_z * base->m_x) - (rotate.m_x * base->m_z), (rotate.m_x * base->m_y) - (rotate.m_y * base->m_x));

		Vector3D FinalVector((rotate.m_y * newVector.m_z) - (rotate.m_z * newVector.m_y), (rotate.m_z * newVector.m_x) - (rotate.m_x * newVector.m_z), (rotate.m_x * newVector.m_y) - (rotate.m_y * newVector.m_x));

		Vector3D rebase = *base;

		Vector3D FinalDisplacement = rebase + (newVector * (2 * rotate.m_w) + FinalVector * 2);

		return FinalDisplacement;

		/*
		Quaternion conjugate = Quaternion::Conjugate(rotate);

		Quaternion finalRotation = rotate * base;

		finalRotation = finalRotation * conjugate;

		return Vector3D(finalRotation.x, finalRotation.y, finalRotation.z);
		*/
	}

	AQuaternion operator *(float num)
	{
		return AQuaternion(m_x * num, m_y * num, m_z * num, m_w * num);
	}
	//Other quaternion
	AQuaternion operator *(AQuaternion other)
	{
		//Behold the 4th dimension fuckery

		//Calculate new angle magnitude
		float newW = m_w * other.m_w - (m_x * other.m_x + m_y * other.m_y + m_z * other.m_z);

		//Calculate new unit vector vals
		float newX = (m_w * other.m_x) + (other.m_w * m_x) + (m_y * other.m_z - m_z * other.m_y);
		float newY = (m_w * other.m_y) + (other.m_w * m_y) + (m_z * other.m_x - m_x * other.m_z);
		float newZ = (m_w * other.m_z) + (other.m_w * m_z) + (m_x * other.m_y - m_y * other.m_x);

		return AQuaternion(newX, newY, newZ, newW);
	}

	AQuaternion operator *=(AQuaternion other)
	{
		
		float newW = m_w * other.m_w - (m_x * other.m_x + m_y * other.m_y + m_z * other.m_z);
		
		float newX = (m_w * other.m_x) + (other.m_w * m_x) + (m_y * other.m_z - m_z * other.m_y);
		float newY = (m_w * other.m_y) + (other.m_w * m_y) + (m_z * other.m_x - m_x * other.m_z);
		float newZ = (m_w * other.m_z) + (other.m_w * m_z) + (m_x * other.m_y - m_y * other.m_x);

		return AQuaternion(newX, newY, newZ, newW);
	}

	//Vector3D
	AQuaternion operator *(Vector3D* vector) {
		float newW = (-m_x * vector->m_x) - (m_y * vector->m_y) - (m_z * vector->m_z); //Using x as a forced Euler axis

		float newX = m_w * vector->m_x + (m_y * vector->m_z) - (m_z * vector->m_y); //use w as Euler axis

		float newY = m_w * vector->m_y + (m_z * vector->m_x) - (m_x * vector->m_z); //use w as Euler axis

		float newZ = m_w * vector->m_z + (m_x * vector->m_y) - (m_y * vector->m_x); //use w as Euler axis

		return AQuaternion(newX, newY, newZ, newW);
	}

	AQuaternion operator *=(Vector3D* vector) {
		float newW = (-m_x * vector->m_x) - (m_y * vector->m_y) - (m_z * vector->m_z); //Using x as a forced Euler axis

		float newX = m_w * vector->m_x + (m_y * vector->m_z) - (m_z * vector->m_y); //use w as Euler axis

		float newY = m_w * vector->m_y + (m_z * vector->m_x) - (m_x * vector->m_z); //use w as Euler axis

		float newZ = m_w * vector->m_z + (m_x * vector->m_y) - (m_y * vector->m_x); //use w as Euler axis

		return AQuaternion(newX, newY, newZ, newW);
	}

	//Subtract by ... other quaternion
	AQuaternion operator -(AQuaternion other)
	{
		//Simply subtract all relevant items
		return AQuaternion(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
	}

	//Add ... other quaternion
	AQuaternion operator +(AQuaternion other)
	{
		//Simply add all relevant items
		return AQuaternion(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
	}

	//The meat and potatoes
	//THIS Quaternion to Rotation Matrix 
	Matrix4x4 ToRotationMatrix() {
		Vector3D forwardRot; // Holds the rotation around Z
		Vector3D upwardRot; //Holds the rotation around the Y
		Vector3D rightwardRot; //Holds the rotation around the X

		//As influenced by the x - z plane --> Dir1
		float forwardX = 2 * (m_x * m_z) - (m_w * m_y);
		//As influenced by the y + z plane --> Dir2
		float forwardY = 2 * (m_y * m_z) + (m_w * m_x);
		//As influenced by the unit vector value --> Euler Mag
		float forwardZ = 1 - (2 * (m_x * m_x) + (m_y * m_y));

		forwardRot = Vector3D(forwardX, forwardY, forwardZ);

		//As influenced by the x - y plane --> Dir1
		float upwardX = 2 * ((m_x * m_y) + (m_w * m_z));
		//As influenced by the unit vector value --> Euler Mag
		float upwardY = 1 - (2 * ((m_x * m_x) + (m_z * m_z)));
		//As influenced by the y - z --> Dir2
		float upwardZ = 2 * ((m_y * m_z) - (m_w * m_x));

		upwardRot = Vector3D(upwardX, upwardY, upwardZ);

		//As influenced by the unit vector value --> Euler Mag
		float rightwardX = 1 - (2 * (m_y * m_y) + (m_z * m_z));
		//As influenced by the x - y plane --> Dir1
		float rightwardY = 2 * ((m_x * m_y) - (m_w * m_z));
		//As influenced by the x - z --> Dir2
		float rightwardZ = 2 * ((m_x * m_z) + (m_w * m_y));

		rightwardRot = Vector3D(rightwardX, rightwardY, rightwardZ);

		//Finally, return the matrix of rotation
		Matrix4x4 rotationMatrix;
		rotationMatrix.SetRotationMatrixByQuaternion(forwardRot, upwardRot, rightwardRot);
		return rotationMatrix;
	}
	//if the function above fails then this will all not work



private:
};
