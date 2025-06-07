#include "Draw.h"


void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

// グリッドを描画する関数
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	// グリッドの半分の幅
	const float kGridHalfWidth = 2.0f;
	// 分割数
	const float kSubdivision = 10.0f;
	// 1つ分の長さ
	const float kGridEvery = (kGridHalfWidth * 2.0f) / kSubdivision;
	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex)
	{
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		float x = -kGridHalfWidth + xIndex * kGridEvery;
		Vector3 start = { x, 0.0f, -kGridHalfWidth }; // 奥
		Vector3 end = { x, 0.0f, +kGridHalfWidth }; // 手前

		start = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		end = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), 0x555555ff);
	}

	// 左から右への線を順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex)
	{
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		float z = -kGridHalfWidth + zIndex * kGridEvery;
		Vector3 start = { -kGridHalfWidth, 0.0f, z }; // 左
		Vector3 end = { +kGridHalfWidth, 0.0f, z }; // 右

		start = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		end = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), 0x555555ff);
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// 分割数
	const uint32_t kSubdivision = 20;
	// 経度分割一つ分の角度
	const float kLonEvery = (2.0f * float(M_PI)) / float(kSubdivision);
	// 緯度分割一つ分の角度
	const float kLatEvery = (float(M_PI)) / float(kSubdivision);
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = kLonEvery * lonIndex;

			// 各点を計算
			Vector3 p1 = {
				sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon),
				sphere.center.y + sphere.radius * std::sinf(lat),
				sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon)
			};
			Vector3 p2 = {
				sphere.center.x + sphere.radius * std::cosf(lat + kLatEvery) * std::cosf(lon),
				sphere.center.y + sphere.radius * std::sinf(lat + kLatEvery),
				sphere.center.z + sphere.radius * std::cosf(lat + kLatEvery) * std::sinf(lon)
			};
			Vector3 p3 = {
				sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon + kLonEvery),
				sphere.center.y + sphere.radius * std::sinf(lat),
				sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon + kLonEvery)
			};

			p1 = Transform(Transform(p1, viewProjectionMatrix), viewportMatrix);
			p2 = Transform(Transform(p2, viewProjectionMatrix), viewportMatrix);
			p3 = Transform(Transform(p3, viewProjectionMatrix), viewportMatrix);

			// 経度方向の線
			Novice::DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, color);
			// 緯度方向の線
			Novice::DrawLine((int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, color);
		}
	}
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index)
	{
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	// 四角形として線を描画
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[2].x, (int)points[2].y, color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[3].x, (int)points[3].y, color);
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[3].x, (int)points[3].y, color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[2].x, (int)points[2].y, color);
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// 1番目の頂点を変換
	Vector3 p0 = Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	// 2番目の頂点を変換
	Vector3 p1 = Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	// 3番目の頂点を変換
	Vector3 p2 = Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);

	Novice::DrawTriangle(
		static_cast<int>(p0.x), static_cast<int>(p0.y),
		static_cast<int>(p1.x), static_cast<int>(p1.y),
		static_cast<int>(p2.x), static_cast<int>(p2.y),
		color, kFillModeWireFrame
	);
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// AABBの8つの頂点を計算
	Vector3 vertices[8];
	vertices[0] = { aabb.min.x, aabb.min.y, aabb.min.z };
	vertices[1] = { aabb.max.x, aabb.min.y, aabb.min.z };
	vertices[2] = { aabb.max.x, aabb.max.y, aabb.min.z };
	vertices[3] = { aabb.min.x, aabb.max.y, aabb.min.z };

	vertices[4] = { aabb.min.x, aabb.min.y, aabb.max.z };
	vertices[5] = { aabb.max.x, aabb.min.y, aabb.max.z };
	vertices[6] = { aabb.max.x, aabb.max.y, aabb.max.z };
	vertices[7] = { aabb.min.x, aabb.max.y, aabb.max.z };

	// AABBのエッジのインデックス 
	int edges[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0}, // 底面
		{4, 5}, {5, 6}, {6, 7}, {7, 4}, // 天面
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  // 側面
	};

	// 変換された頂点座標を格納する配列
	Vector2 projectedVertices[8];

	for (int i = 0; i < 8; ++i) {

		float x = vertices[i].x * viewProjectionMatrix.m[0][0] + vertices[i].y * viewProjectionMatrix.m[1][0] + vertices[i].z * viewProjectionMatrix.m[2][0] + viewProjectionMatrix.m[3][0];
		float y = vertices[i].x * viewProjectionMatrix.m[0][1] + vertices[i].y * viewProjectionMatrix.m[1][1] + vertices[i].z * viewProjectionMatrix.m[2][1] + viewProjectionMatrix.m[3][1];
		float z = vertices[i].x * viewProjectionMatrix.m[0][2] + vertices[i].y * viewProjectionMatrix.m[1][2] + vertices[i].z * viewProjectionMatrix.m[2][2] + viewProjectionMatrix.m[3][2];
		float w = vertices[i].x * viewProjectionMatrix.m[0][3] + vertices[i].y * viewProjectionMatrix.m[1][3] + vertices[i].z * viewProjectionMatrix.m[2][3] + viewProjectionMatrix.m[3][3];
		if (w <= 0.0001f)
		{
			projectedVertices[i] = { -9999, -9999 };
			continue;
		}
		Vector3 ndc_coord = { x / w, y / w, z / w };


		float screenX = ndc_coord.x * viewportMatrix.m[0][0] + ndc_coord.y * viewportMatrix.m[1][0] + ndc_coord.z * viewportMatrix.m[2][0] + viewportMatrix.m[3][0];
		float screenY = ndc_coord.x * viewportMatrix.m[0][1] + ndc_coord.y * viewportMatrix.m[1][1] + ndc_coord.z * viewportMatrix.m[2][1] + viewportMatrix.m[3][1];

		projectedVertices[i] = { screenX, screenY };
	}

	// 各エッジを2Dで描画
	for (int i = 0; i < 12; ++i) {
		int v1_idx = edges[i][0];
		int v2_idx = edges[i][1];

		// 画面外に変換された頂点を含む線は描画しない (簡易クリッピング)
		if (projectedVertices[v1_idx].x == -9999 || projectedVertices[v2_idx].x == -9999) {
			continue;
		}


		Novice::DrawLine((int)projectedVertices[v1_idx].x, (int)projectedVertices[v1_idx].y,
			(int)projectedVertices[v2_idx].x, (int)projectedVertices[v2_idx].y,
			color);
	}
}

void DrawBezierCurve(const Vector3& p0, const Vector3& p1, const Vector3& p2,
	const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	const int segments = 32; // 曲線を分割する数
	Vector3 prev = p0;

	for (int i = 1; i <= segments; ++i)
	{
		float t = static_cast<float>(i) / segments;

		// 2次ベジエ曲線の評価
		Vector3 a = Lerp(p0, p1, t);
		Vector3 b = Lerp(p1, p2, t);
		Vector3 point = Lerp(a, b, t);

		// 座標変換 (viewProjection → viewport)
		Vector3 screenPrev = Transform(prev, viewProjectionMatrix);
		screenPrev = Transform(screenPrev, viewportMatrix);

		Vector3 screenPoint = Transform(point, viewProjectionMatrix);
		screenPoint = Transform(screenPoint, viewportMatrix);

		// 描画
		Novice::DrawLine(
			static_cast<int>(screenPrev.x), static_cast<int>(screenPrev.y),
			static_cast<int>(screenPoint.x), static_cast<int>(screenPoint.y),
			color
		);

		prev = point;
	}
}