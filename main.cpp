#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "imgui.h"

#include "Structures.h"
#include "Matrix.h"
#include "MathUtils.h"
#include "Collision.h"
#include "Draw.h"

const char kWindowTitle[] = "LE2A_12_ホリ_ソウヘイ_タイトル";


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	float fovY = 0.45f;
	float aspectRatio = 1280.0f / 720.0f;
	float nearClip = 0.1f;
	float farClip = 100.0f;

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };
	Vector3 cameraScale = { 1.0f,1.0f,1.0f };
	Vector3 cameraRotate = { 0.2f,0.0f,0.0f };
	Vector3 cameraTranslate = { 0.0f,2.0f,-10.0f };

	Segment segment;
	segment.origin = { -1.12f, 0.12f, -1.33f };
	segment.diff = { -1.12f, 0.58f, 0.14f };
	Vector3 center1{ 0.0f,0.0f,0.0f };
	Vector3 center2{ 0.0f,0.0f,0.0f };
	Vector3 center3{ 0.0f,0.0f,0.0f };

	Vector3 project = Project(Subtract(center1, segment.origin), segment.diff);
	Vector3 closestPoint = CrosestPoint(center1, segment);

	// 速度
	const float moveSpeed = 0.1f;

	Sphere sphere1{ center1,0.1f };
	Sphere sphere2{ center2,0.1f };
	Sphere sphere3{ center3,0.1f };

	Plane plane{ {-0.2f,0.9f,-0.3f},0 };
	plane.normal = Normalize(plane.normal); 

	Triangle triangle;
	triangle.vertices[0] = { 0, 0, 1 };
	triangle.vertices[1] = { 0, 1, 0 };
	triangle.vertices[2] = { 0, 0, -1 };

	AABB aabb1
	{
		.min{-2.25f,-0.14f,-0.99f},
		.max{-0.70f,0.910f,1.54f}
	};

	/*AABB aabb2
	{
		.min{0.2f,0.2f,0.2f},
		.max{1.0f,1.0f,1.0f}
	};*/
	Vector3 controlPoints[3]
	{
		{ -0.8f, 0.58f, 1.0f },
		{ 1.76f, 1.0f, -0.3f },
		{ 0.94f, -0.7f, 2.3f }
	};

	Vector3 translates[3] =
	{
		{0.2f,1.0f,0.0f} ,
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f},
	};
	Vector3 rotates[3] =
	{
		{0.0f,0.0f,-0.8f} ,
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f},
	};
	Vector3 scales[3] =
	{
		{1.0f,1.0f,1.0f} ,
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
	};

	Vector3 a{ 0.2f,1.0f,0.0f };
	Vector3 b{ 2.4f,3.1f,1.2f };
	Vector3 c = a + b;
	Vector3 d = a - b;
	Vector3 rotateVec{ 0.4f,1.43f,-0.8f };
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotateVec.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotateVec.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotateVec.z);
	Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

	bool isSpring = false;
	Spring spring{};
	spring.anchor = { 0.0f,0.0f,0.0f };
	spring.naturalLength = 1.0f; // 自然長
	spring.stiffness = 100.0f; // ばね定数
	spring.dampingCoefficient = 2.0f;

	Ball ball{};
	ball.position = { 1.2f, 0.0f, 0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;

	Sphere SphereBall{ ball.position,0.1f };

	Sphere sphereCircle{ {0,0,0},0.1f};
	Vector3 circleCenter{ 0.0f,0.0f,0.0f };
	
	float deltaTime = 1.0f / 60.0f;

	Pendulum pendulum;
	pendulum.anchor = { 0.0f,1.0f,0.0f };
	pendulum.length = 0.8f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.0f;
	pendulum.angularAcceleration = 0.0f;
	bool isPendulum = false;

	ConicalPendulum conicalPendulum;
	conicalPendulum.anchor = { 0.0f,1.0f,0.0f };
	conicalPendulum.length = 0.8f;
	conicalPendulum.halfApexAngle = 0.7f; 
	conicalPendulum.angle = 0.0f;
	conicalPendulum.angularVelocity = 0.0f;

	Ball reflectBall{};
	reflectBall.position = { 0.8f, 1.2f, 0.3f };
	reflectBall.acceleration = { 0.0f, -9.8f, 0.0f };
	reflectBall.mass = 2.0f;
	reflectBall.radius = 0.05f;
	reflectBall.color = WHITE;
	float e = 0.6f;
	Sphere reflectSphere{ reflectBall.position,reflectBall.radius };
	bool isReflect = false;
	Capsule capsule;
	capsule.segment.origin = { -5.0f, 0.0f, 0.0f };     
	capsule.segment.diff = { 10.0f, 0.0f, 0.0f };       
	capsule.segment.color = RED;
	capsule.radius = 0.5f;

	Vector3 axis = Normalize({ 1.0f,1.0f,1.0f });
	float angle = 0.44f;
	Matrix4x4 rotationMatrix = MakeRotateAxisAngle(axis, angle);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// キー入力による移動
		if (keys[DIK_W]) {
			cameraTranslate.y += moveSpeed;
		}
		if (keys[DIK_S]) {
			cameraTranslate.y -= moveSpeed;
		}
		if (keys[DIK_A]) {
			cameraTranslate.x -= moveSpeed;
		}
		if (keys[DIK_D]) {
			cameraTranslate.x += moveSpeed;
		}

		if (keys[DIK_Q]) {
			cameraTranslate.z += moveSpeed;
		}

		if (keys[DIK_E]) {
			cameraTranslate.z -= moveSpeed;
		}
		Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraScale, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nearClip, farClip);
		// WVPMatrixを作る
		Matrix4x4 worldViewProjectionMatrix = Multiply(Multiply(worldMatrix, viewMatrix), projectionMatrix);

		// ViewportMatrixを作る
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, 1280, 720, 0.0f, 1.0f);

		// 肩のローカル変形
		Matrix4x4 localMatrixShoulder = MakeAffineMatrix(scales[0], rotates[0], translates[0]);
		// 肩のワールド行列
		Matrix4x4 worldMatrixShoulder = localMatrixShoulder;
		// ひじのローカル変形
		Matrix4x4 localMatrixElbow = MakeAffineMatrix(scales[1], rotates[1], translates[1]);
		// ひじのワールド行列
		Matrix4x4 worldMatrixElbow = Multiply(localMatrixElbow, worldMatrixShoulder);
		// 手のローカル変形
		Matrix4x4 localMatrixHand = MakeAffineMatrix(scales[2], rotates[2], translates[2]);
		// 手のワールド行列
		Matrix4x4 worldMatrixHand = Multiply(localMatrixHand, worldMatrixElbow);

		// WVPMatrixを作る
		Matrix4x4 WVPMatrixShoulder = Multiply(Multiply(worldMatrixShoulder, viewMatrix), projectionMatrix);
		Matrix4x4 WVPMatrixElbow = Multiply(Multiply(worldMatrixElbow, viewMatrix), projectionMatrix);
		Matrix4x4 WVPMatrixHand = Multiply(Multiply(worldMatrixHand, viewMatrix), projectionMatrix);
		
		// 線の終点を計算する
		Vector3 segmentTrueEnd = Add(segment.origin, segment.diff);

		// start と end をワールド座標系からスクリーン座標系へ変換
		Vector3 start = Transform(Transform(segment.origin, worldViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(segmentTrueEnd, worldViewProjectionMatrix), viewportMatrix); // 修正後

		// スクリーン座標に変換
		Vector3 shoulderScreen = WorldToScreen(sphere1.center, WVPMatrixShoulder, 1280.0f, 720.0f);
		Vector3 elbowScreen = WorldToScreen(sphere2.center, WVPMatrixElbow, 1280.0f, 720.0f);
		Vector3 handScreen = WorldToScreen(sphere3.center, WVPMatrixHand, 1280.0f, 720.0f);
		
		if (isSpring)
		{
			// ばねの更新処理
			Vector3 diff = ball.position - spring.anchor;
			float length = Length(diff);
			if (length != 0.0f)
			{
				Vector3 direction = Normalize(diff);
				Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
				Vector3 displacement = length * (ball.position - restPosition);
				Vector3 restoringForce = -spring.stiffness * displacement;
				// 減衰力の計算
				Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
				Vector3 force = restoringForce + dampingForce;
				ball.acceleration = force / ball.mass;
			}

			// 加速度も速度もどちらも秒が基準
			ball.velocity += ball.acceleration * deltaTime;
			ball.position += ball.velocity * deltaTime;
		}

		SphereBall.center = ball.position;

		Vector3 ballScreen = WorldToScreen(SphereBall.center, worldViewProjectionMatrix, 1280.0f, 720.0f);
		Vector3 anchorScreen = WorldToScreen(spring.anchor, worldViewProjectionMatrix, 1280.0f, 720.0f);

		// 球の円運動の更新処理
		if (isPendulum)
		{
			/*pendulum.angularAcceleration =
				-9.8f / pendulum.length * std::sin(pendulum.angle);
			pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
			pendulum.angle += pendulum.angularVelocity * deltaTime;*/

			conicalPendulum.angularVelocity = std::sqrt(9.8f / (pendulum.length * std::cos(conicalPendulum.halfApexAngle)));
			conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;
		}
		/*sphereCircle.center.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
		sphereCircle.center.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
		sphereCircle.center.z = pendulum.anchor.z;*/

		float radius = conicalPendulum.length * std::sin(conicalPendulum.halfApexAngle);
		float height = conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle);
		sphereCircle.center.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * radius;
		sphereCircle.center.y = conicalPendulum.anchor.y - height;
		sphereCircle.center.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;

		Vector3 sphereCircleScreen = WorldToScreen(sphereCircle.center, worldViewProjectionMatrix, 1280.0f, 720.0f);
		Vector3 pendulumAnchorScreen = WorldToScreen(pendulum.anchor, worldViewProjectionMatrix, 1280.0f, 720.0f);

		if (isReflect)
		{
			reflectBall.velocity += reflectBall.acceleration * deltaTime;
			reflectBall.position += reflectBall.velocity * deltaTime;
		}
		if (IsCollision(Sphere{ reflectBall.position,reflectBall.radius,WHITE }, plane))
		{
			Vector3 closest = ClosestPointOnSegment(reflectBall.position, capsule.segment);
			Vector3 normal = Normalize(plane.normal);
			Vector3 reflected = Reflect(reflectBall.velocity, normal);
			Vector3 projectToNormal = Project(reflected, normal);
			Vector3 movingDirection = reflected - projectToNormal;
			reflectBall.velocity = projectToNormal * e + movingDirection;
		}

		reflectSphere.center = reflectBall.position;
		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		/*DrawSphere(sphere1, WVPMatrixShoulder, viewportMatrix, RED);
		DrawSphere(sphere2, WVPMatrixElbow, viewportMatrix, GREEN);
		DrawSphere(sphere3, WVPMatrixHand, viewportMatrix, BLUE);*/
		/*DrawSphere(SphereBall, worldViewProjectionMatrix, viewportMatrix, RED);
		Novice::DrawLine((int)anchorScreen.x, (int)anchorScreen.y,
			(int)ballScreen.x, (int)ballScreen.y, WHITE);*/
		/*DrawSphere(sphereCircle, worldViewProjectionMatrix, viewportMatrix, RED);
		Novice::DrawLine((int)pendulumAnchorScreen.x, (int)pendulumAnchorScreen.y,
			(int)sphereCircleScreen.x, (int)sphereCircleScreen.y, WHITE);*/
		/*Novice::DrawLine((int)shoulderScreen.x, (int)shoulderScreen.y,
			(int)elbowScreen.x, (int)elbowScreen.y, WHITE);
		Novice::DrawLine((int)elbowScreen.x, (int)elbowScreen.y,
			(int)handScreen.x, (int)handScreen.y, WHITE);*/
		/*Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), segment.color);*/
	/*	DrawSphere(reflectSphere, worldViewProjectionMatrix, viewportMatrix, RED);
		DrawPlane(plane, worldViewProjectionMatrix, viewportMatrix, BLACK);*/
		/*DrawTriangle(triangle, worldViewProjectionMatrix, viewportMatrix, WHITE);*/
		/*DrawAABB(aabb1, worldViewProjectionMatrix, viewportMatrix, aabb1.color);*/
		/*DrawAABB(aabb2, worldViewProjectionMatrix, viewportMatrix, WHITE);*/
		/*DrawBezierCurve(controlPoints[0], controlPoints[1], controlPoints[2],
			worldViewProjectionMatrix, viewportMatrix, WHITE);*/
		ImGui::Begin("MyWindow");
		/*ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.07f, -1280, 1280);*/
		/*aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);*/
		/*ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.07f, -1280, 1280);*/
		/*aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
		aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);
		aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
		aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);
		aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
		aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);*/
		/*ImGui::DragFloat3("triangle.v0", &triangle.vertices[0].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("triangle.v1", &triangle.vertices[1].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("triangle.v2", &triangle.vertices[2].x, 0.07f, -1280, 1280);*/
		/*ImGui::DragFloat3("sphere1.position", &sphere1.center.x, 0.07f, -1280, 1280);*/
		/*ImGui::DragFloat3("sphere2.position", &sphere2.center.x, 0.07f, 0, 1280);*/
		/*ImGui::DragFloat3("Segment origin", &segment.origin.x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("Segment diff", &segment.diff.x, 0.07f, -1280, 1280);*/
		/*ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::DragFloat3("normal", &plane.normal.x, 0.07f, -1, 1);
		ImGui::DragFloat("distance", &plane.distance, 0.07f, 0, 1280);*/
		/*ImGui::DragFloat3("controlPoints[0]", &controlPoints[0].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("controlPoints[1]", &controlPoints[1].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("controlPoints[2]", &controlPoints[2].x, 0.07f, -1280, 1280);*/
		/*ImGui::DragFloat3("translates[0]", &translates[0].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("rotates[0]", &rotates[0].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("scales[0]", &scales[0].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("translates[1]", &translates[1].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("rotates[1]", &rotates[1].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("scales[1]", &scales[1].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("translates[2]", &translates[2].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("rotates[2]", &rotates[2].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("scales[2]", &scales[2].x, 0.07f, -1280, 1280);*/
		/*ImGui::DragFloat3("cameraScale", &cameraScale.x, 0.07f, 0, 10);
		ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.07f, -10, 10);
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.07f, -1280, 1280);*/

		/*ImGui::Text("c:%f, %f, %f", c.x, c.y, c.z);
		ImGui::Text("d:%f, %f, %f", d.x, d.y, d.z);
		ImGui::Text("e:%f, %f, %f", e.x, e.y, e.z);
		ImGui::Text(
			"matrix:\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n",
			rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3],
			rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3],
			rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3],
			rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]
			);*/

		ImGui::Checkbox("Start", &isReflect);

		ImGui::End();

		MatrixScreenPrintf(0, 0, rotationMatrix, "rotateMatrix");

		plane.normal = Normalize(plane.normal);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
