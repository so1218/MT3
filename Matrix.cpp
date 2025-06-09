#include "Matrix.h"

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result.m[row][col] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.m[row][col] += m1.m[row][k] * m2.m[k][col];
			}
		}
	}

	return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
	return Multiply(*this, other);
}

Matrix4x4 MakeIdentityMatrix() {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m)
{
	Matrix4x4 result{};
	float inv[16], det;
	const float* src = &m.m[0][0];

	inv[0] = src[5] * src[10] * src[15] -
		src[5] * src[11] * src[14] -
		src[9] * src[6] * src[15] +
		src[9] * src[7] * src[14] +
		src[13] * src[6] * src[11] -
		src[13] * src[7] * src[10];

	inv[4] = -src[4] * src[10] * src[15] +
		src[4] * src[11] * src[14] +
		src[8] * src[6] * src[15] -
		src[8] * src[7] * src[14] -
		src[12] * src[6] * src[11] +
		src[12] * src[7] * src[10];

	inv[8] = src[4] * src[9] * src[15] -
		src[4] * src[11] * src[13] -
		src[8] * src[5] * src[15] +
		src[8] * src[7] * src[13] +
		src[12] * src[5] * src[11] -
		src[12] * src[7] * src[9];

	inv[12] = -src[4] * src[9] * src[14] +
		src[4] * src[10] * src[13] +
		src[8] * src[5] * src[14] -
		src[8] * src[6] * src[13] -
		src[12] * src[5] * src[10] +
		src[12] * src[6] * src[9];

	inv[1] = -src[1] * src[10] * src[15] +
		src[1] * src[11] * src[14] +
		src[9] * src[2] * src[15] -
		src[9] * src[3] * src[14] -
		src[13] * src[2] * src[11] +
		src[13] * src[3] * src[10];

	inv[5] = src[0] * src[10] * src[15] -
		src[0] * src[11] * src[14] -
		src[8] * src[2] * src[15] +
		src[8] * src[3] * src[14] +
		src[12] * src[2] * src[11] -
		src[12] * src[3] * src[10];

	inv[9] = -src[0] * src[9] * src[15] +
		src[0] * src[11] * src[13] +
		src[8] * src[1] * src[15] -
		src[8] * src[3] * src[13] -
		src[12] * src[1] * src[11] +
		src[12] * src[3] * src[9];

	inv[13] = src[0] * src[9] * src[14] -
		src[0] * src[10] * src[13] -
		src[8] * src[1] * src[14] +
		src[8] * src[2] * src[13] +
		src[12] * src[1] * src[10] -
		src[12] * src[2] * src[9];

	inv[2] = src[1] * src[6] * src[15] -
		src[1] * src[7] * src[14] -
		src[5] * src[2] * src[15] +
		src[5] * src[3] * src[14] +
		src[13] * src[2] * src[7] -
		src[13] * src[3] * src[6];

	inv[6] = -src[0] * src[6] * src[15] +
		src[0] * src[7] * src[14] +
		src[4] * src[2] * src[15] -
		src[4] * src[3] * src[14] -
		src[12] * src[2] * src[7] +
		src[12] * src[3] * src[6];

	inv[10] = src[0] * src[5] * src[15] -
		src[0] * src[7] * src[13] -
		src[4] * src[1] * src[15] +
		src[4] * src[3] * src[13] +
		src[12] * src[1] * src[7] -
		src[12] * src[3] * src[5];

	inv[14] = -src[0] * src[5] * src[14] +
		src[0] * src[6] * src[13] +
		src[4] * src[1] * src[14] -
		src[4] * src[2] * src[13] -
		src[12] * src[1] * src[6] +
		src[12] * src[2] * src[5];

	inv[3] = -src[1] * src[6] * src[11] +
		src[1] * src[7] * src[10] +
		src[5] * src[2] * src[11] -
		src[5] * src[3] * src[10] -
		src[9] * src[2] * src[7] +
		src[9] * src[3] * src[6];

	inv[7] = src[0] * src[6] * src[11] -
		src[0] * src[7] * src[10] -
		src[4] * src[2] * src[11] +
		src[4] * src[3] * src[10] +
		src[8] * src[2] * src[7] -
		src[8] * src[3] * src[6];

	inv[11] = -src[0] * src[5] * src[11] +
		src[0] * src[7] * src[9] +
		src[4] * src[1] * src[11] -
		src[4] * src[3] * src[9] -
		src[8] * src[1] * src[7] +
		src[8] * src[3] * src[5];

	inv[15] = src[0] * src[5] * src[10] -
		src[0] * src[6] * src[9] -
		src[4] * src[1] * src[10] +
		src[4] * src[2] * src[9] +
		src[8] * src[1] * src[6] -
		src[8] * src[2] * src[5];

	det = src[0] * inv[0] + src[1] * inv[4] + src[2] * inv[8] + src[3] * inv[12];

	det = 1.0f / det;

	Matrix4x4 resultMatrix;
	for (int i = 0; i < 16; ++i)
	{
		((float*)resultMatrix.m)[i] = inv[i] * det;
	}

	return resultMatrix;
}


// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 matrix = {};

	// 単位行列
	for (int i = 0; i < 4; ++i)
	{
		matrix.m[i][i] = 1.0f;
	}

	// 平行移動成分を設定
	matrix.m[3][0] = translate.x;
	matrix.m[3][1] = translate.y;
	matrix.m[3][2] = translate.z;

	return matrix;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 matrix = {};

	// 拡大縮小成分を設定
	matrix.m[0][0] = scale.x;
	matrix.m[1][1] = scale.y;
	matrix.m[2][2] = scale.z;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian)
{
	Matrix4x4 matrix = {};

	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = std::cosf(radian);
	matrix.m[1][2] = std::sinf(radian);
	matrix.m[2][1] = -std::sinf(radian);
	matrix.m[2][2] = std::cosf(radian);
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian)
{
	Matrix4x4 matrix = {};

	matrix.m[0][0] = std::cosf(radian);
	matrix.m[0][2] = -std::sinf(radian);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][0] = std::sinf(radian);
	matrix.m[2][2] = std::cosf(radian);
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian)
{
	Matrix4x4 matrix = {};

	matrix.m[0][0] = std::cosf(radian);
	matrix.m[0][1] = std::sinf(radian);
	matrix.m[1][0] = -std::sinf(radian);
	matrix.m[1][1] = std::cosf(radian);
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	float x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	float y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	float z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];

	if (w != 0.0f && w != 1.0f) {
		x /= w;
		y /= w;
		z /= w;
	}

	return { x, y, z };
}

// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	// 回転行列を合成
	Matrix4x4 rotateMatrix = Multiply(Multiply(rotateXMatrix, rotateYMatrix), rotateZMatrix);

	// 最終的なアフィン行列
	Matrix4x4 affineMatrix = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return affineMatrix;
}

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 matrix = {};

	matrix.m[0][0] = 2.0f / (right - left);
	matrix.m[1][1] = 2.0f / (top - bottom);
	matrix.m[2][2] = 1.0f / (farClip - nearClip);
	matrix.m[3][0] = (right + left) / (left - right);
	matrix.m[3][1] = (top + bottom) / (bottom - top);
	matrix.m[3][2] = nearClip / (nearClip - farClip);
	matrix.m[3][3] = 1.0f;

	return matrix;
}

//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 matrix = {};
	float f = 1.0f / std::tanf(fovY * 0.5f);
	matrix.m[0][0] = f / aspectRatio;
	matrix.m[1][1] = f;
	matrix.m[2][2] = farClip / (farClip - nearClip);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][2] = (-farClip * nearClip) / (farClip - nearClip);
	return matrix;
}

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 matrix = {};

	matrix.m[0][0] = width / 2.0f;
	matrix.m[1][1] = -height / 2.0f;
	matrix.m[2][2] = maxDepth - minDepth;
	matrix.m[3][0] = left + width / 2.0f;
	matrix.m[3][1] = top + height / 2.0f;
	matrix.m[3][2] = minDepth;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

Vector3 WorldToScreen(const Vector3& worldPos, const Matrix4x4& WVPMatrix, float screenWidth, float screenHeight) {
	Vector3 clipPos = Transform(worldPos, WVPMatrix);

	// NDC → Screen
	Vector3 screenPos;
	screenPos.x = (clipPos.x + 1.0f) * 0.5f * screenWidth;
	screenPos.y = (1.0f - clipPos.y) * 0.5f * screenHeight;
	screenPos.z = clipPos.z;

	return screenPos;
}

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle)
{
	Matrix4x4 matrix = {};
	float cos = std::cos(angle);
	float sin = std::sin(angle);
	float t = 1.0f - cos;
	matrix.m[0][0] = t * axis.x * axis.x + cos;
	matrix.m[0][1] = t * axis.x * axis.y + sin * axis.z;
	matrix.m[0][2] = t * axis.x * axis.z - sin * axis.y;
	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = t * axis.x * axis.y - sin * axis.z;
	matrix.m[1][1] = t * axis.y * axis.y + cos;
	matrix.m[1][2] = t * axis.y * axis.z + sin * axis.x;
	matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = t * axis.x * axis.z + sin * axis.y;
	matrix.m[2][1] = t * axis.y * axis.z - sin * axis.x;
	matrix.m[2][2] = t * axis.z * axis.z + cos;
	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][2] = 0.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {
	// 1. 'from' と 'to' ベクトルを正規化します。
	Vector3 u = Normalize(from);
	Vector3 v = Normalize(to);

	// 2. 回転軸 n を計算します。
	Vector3 n = u.cross(v);
	float n_length = n.length();

	// 特殊なケースの処理：
	// a) u と v が同じ方向 (u == v) の場合
	//    回転は不要で、単位行列を返します。
	if (n_length < 1e-6) { // n がほぼゼロベクトル、つまり u と v が平行
		// u と v がほぼ等しい場合、単位行列を返します。
		// u と v が逆方向 (u = -v) の場合もこの条件に該当します。
		// 画像には「u=-vのとき、u x v = 0, u・v = -1なので、-Eとなり、反転する行列になってしまう」とあります。
		// これに対処するため、180度回転を明示的に処理します。
		if (Dot(u,v) < 0) { // u と v が逆方向 (ほぼ -1)
			// 180度回転
			// この場合、回転軸は任意（u に垂直な任意の軸）ですが、
			// n がゼロベクトルなので、軸を見つける必要があります。
			// 例えば、u と直交する任意のベクトルを軸として利用できます。
			// 簡略化のために、画像を参考に180度回転の行列を構築します。
			// 任意の軸 (u に垂直な軸) を見つける方法：
			// 例えば、(1,0,0) と u の外積がゼロでなければそれを使う。ゼロなら (0,1,0) との外積を使う。
			Vector3 axis;
			if (std::abs(u.x) < 0.9f) { // x成分が小さい場合、X軸との外積が有効な軸になる可能性が高い
				axis = Vector3(1.0f, 0.0f, 0.0f).cross(u).Normalize();
			}
			else { // x成分が大きい場合、Y軸との外積を使う
				axis = Vector3(0.0f, 1.0f, 0.0f).cross(u).Normalize();
			}

			// 180度回転行列 (ロドリゲスの回転公式などから)
			// R = I + 2 * (n n^T) - I = 2 * (n n^T) - I  (ただし、n は正規化された軸)
			// この場合、theta = PI (180度) なので、cos(PI) = -1, sin(PI) = 0
			// R = n n^T (1 - (-1)) + (-1)I = 2 * n n^T - I
			// 各要素は
			// R_ij = 2 * n_i * n_j (i != j)
			// R_ii = 2 * n_i^2 - 1
			float nx = axis.x;
			float ny = axis.y;
			float nz = axis.z;

			Matrix4x4 result;
			result.m[0][0] = 2.0f * nx * nx - 1.0f;
			result.m[0][1] = 2.0f * nx * ny;
			result.m[0][2] = 2.0f * nx * nz;
			result.m[1][0] = 2.0f * ny * nx;
			result.m[1][1] = 2.0f * ny * ny - 1.0f;
			result.m[1][2] = 2.0f * ny * nz;
			result.m[2][0] = 2.0f * nz * nx;
			result.m[2][1] = 2.0f * nz * ny;
			result.m[2][2] = 2.0f * nz * nz - 1.0f;

			// 4x4 行列なので、4列目はそのまま (移動成分はなし)
			result.m[0][3] = 0.0f;
			result.m[1][3] = 0.0f;
			result.m[2][3] = 0.0f;
			result.m[3][0] = 0.0f;
			result.m[3][1] = 0.0f;
			result.m[3][2] = 0.0f;
			result.m[3][3] = 1.0f;

			return result;
		}
		// u と v がほぼ同じ方向の場合 (n_length が小さいが dot(u,v) も 0 に近い)
		// 例えば、fromとtoがどちらもゼロベクトルのような異常なケース
		// 通常は from.normalize() と to.normalize() で対処されるはず
		return Matrix4x4(); // 単位行列を返す
	}

	// 3. 回転軸 n を正規化します。
	n = Normalize(n);

	// 4. cos_theta と sin_theta を計算します。
	float cos_theta = Dot(u,v);
	// sin_theta = n_length; // u,vが正規化されているので ||u x v|| = sin(theta)

	// 浮動小数点計算の誤差により、cos_theta が [-1, 1] の範囲をわずかに超えることがあります。
	// std::acos に渡す前にクランプします。
	if (cos_theta > 1.0f) cos_theta = 1.0f;
	if (cos_theta < -1.0f) cos_theta = -1.0f;

	float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta); // sin^2 + cos^2 = 1 より
	// ただし、u x v の方向と、theta の方向が一致するように sin_theta の符号を考慮する必要があります。
	// ||u x v|| は常に正なので、この計算で問題ありません。
	// 回転の方向は軸 n と右手系で決まります。
	// from から to への回転なので、外積 u x v はこの回転の方向を向いているはずです。

	// 5. 回転行列の要素を計算します。
	float nx = n.x;
	float ny = n.y;
	float nz = n.z;

	float one_minus_cos_theta = 1.0f - cos_theta;

	Matrix4x4 R;

	// 1行目
	R.m[0][0] = nx * nx * one_minus_cos_theta + cos_theta;
	R.m[0][1] = nx * ny * one_minus_cos_theta + nz * sin_theta;
	R.m[0][2] = nx * nz * one_minus_cos_theta - ny * sin_theta;

	// 2行目
	R.m[1][0] = nx * ny * one_minus_cos_theta - nz * sin_theta;
	R.m[1][1] = ny * ny * one_minus_cos_theta + cos_theta;
	R.m[1][2] = ny * nz * one_minus_cos_theta + nx * sin_theta;

	// 3行目
	R.m[2][0] = nx * nz * one_minus_cos_theta + ny * sin_theta;
	R.m[2][1] = ny * nz * one_minus_cos_theta - nx * sin_theta;
	R.m[2][2] = nz * nz * one_minus_cos_theta + cos_theta;

	// 4x4 行列なので、平行移動成分はなし (単位行列のまま)
	R.m[0][3] = 0.0f;
	R.m[1][3] = 0.0f;
	R.m[2][3] = 0.0f;
	R.m[3][0] = 0.0f;
	R.m[3][1] = 0.0f;
	R.m[3][2] = 0.0f;
	R.m[3][3] = 1.0f;

	return R;
}
