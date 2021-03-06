/*
pbrt source code is Copyright(c) 1998-2016
Matt Pharr, Greg Humphreys, and Wenzel Jakob.

This file is part of pbrt.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

- Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef RAINY_CORE_SHAPE_H
#define RAINY_CORE_SHAPE_H

#include"rainy.h"
#include"geometry.h"
#include"interaction.h"
#include"transform.h"

namespace rainy
{

class Shape 
{
public:
	Shape(const Transform* ObjectToWorld, const Transform* WorldToObject,
		bool reverseOrientation);
	virtual ~Shape();

	virtual Bounds3f object_bound() const = 0;
	virtual Bounds3f world_bound() const;

	virtual bool intersect(const Ray &ray, 
						   SurfaceInteraction* isect /*bool testAlphaTexture*/) const = 0;
	virtual bool intersectP(const Ray &ray,
						    bool testAlphaTexture = true) const;

	virtual Float area() const = 0;
	virtual Float pdf(const Interaction &) const { return 1 / area(); }

	//传入一个[0,1]范围的采样点，在几何体表面计算一个Interaction并返回相应的pdf(1/area())
	virtual Interaction sample(const Point2f &u, Float *pdf) const = 0;

	// Sample a point on the shape given a reference point |ref| and
	// return the PDF with respect to solid angle from |ref|.
	virtual Interaction sample(const Interaction &ref, const Point2f &u, Float *pdf) const;

	//传入一个isect和isect上的方向wi，若isect->wi->shape，则计算相应的pdf
	virtual Float pdf(const Interaction &ref, const Vector3f &wi) const;

	// Returns the solid angle subtended by the shape w.r.t. the reference
	// point p, given in world space. Some shapes compute this value in
	// closed-form, while the default implementation uses Monte Carlo
	// integration; the nSamples parameter determines how many samples are
	// used in this case.
	virtual Float solid_angle(const Point3f &p, int nSamples = 512) const;

public:
	std::unique_ptr<const Transform> ObjectToWorld, WorldToObject;
	const bool reverseOrientation;			//是否翻转（法线）方向
	const bool transformSwapsHandedness;    //转换坐标系（左手or右手？）
};

}	//namespace rainy


#endif //RAINY_CORE_SHAPE_H
