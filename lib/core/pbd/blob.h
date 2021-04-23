#ifndef BLOB_HPP
#define BLOB_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <vector>
#include "Box2D/Box2D.h"

// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef BLOB_H
#define BLOB_H

#include <vector>
#include "Box2D/Box2D.h"

/// A solid circle shape
class blob : public b2Shape
{
public:
	blob(int x);

	blob* Clone(b2BlockAllocator* allocator) const override;

	int32 GetChildCount() const override;

	/// Implement b2Shape.
	bool TestPoint(const b2Transform& transform, const b2Vec2& p) const override;

	bool RayCast(b2RayCastOutput* output, const b2RayCastInput& input,
				const b2Transform& transform, int32 childIndex) const override;

	void ComputeAABB(b2AABB* aabb, const b2Transform& transform, int32 childIndex) const override;

	void ComputeMass(b2MassData* massData, float density) const override;

	void DrawParticle(int i);

	void Draw();

	b2Vec2 m_p;
private:
    int dims;
    std::vector<b2CircleShape> particles;
};

inline blob::blob(int x)
{
    dims = x;
	m_type = e_circle;
	m_radius = 0.0f;
	m_p.SetZero();
    particles.resize(pow(dims,2));
}

#endif