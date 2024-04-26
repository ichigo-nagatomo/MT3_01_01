#include <Novice.h>
#include <cmath>
#include <assert.h>

struct Vec3 {
	float x;
	float y;
	float z;
};

struct Matrix4x4 {
	float m[4][4];
};

//単位行列の作成
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == j) {
				result.m[i][j] = 1.0f;
			} else {
				result.m[i][j] = 0.0f;
			}
		}
	}

	return result;
}

//行列の積
Matrix4x4 Multiply(const Matrix4x4 &matrix1 , const Matrix4x4 &matrix2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += matrix1.m[i][k] * matrix2.m[k][j];
			}
		}
	}

	return result;
}

//Scale
Matrix4x4 MakeScaleMatrix(const Vec3 &scale) {
	Matrix4x4 matrix;

	matrix = MakeIdentity4x4();

	matrix.m[0][0] = scale.x;
	matrix.m[1][1] = scale.y;
	matrix.m[2][2] = scale.z;

	return matrix;
}

//Rotate
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result;

	result = MakeIdentity4x4();

	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);

	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result;

	result = MakeIdentity4x4();

	result.m[0][0] = std::cos(radian);
	result.m[0][2] = -std::sin(radian);
	result.m[2][0] = std::sin(radian);
	result.m[2][2] = std::cos(radian);

	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result;

	result = MakeIdentity4x4();

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);

	return result;
}

Matrix4x4 MakeRotateMatrix(const Vec3 &rotate) {
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 matrix = Multiply(rotateXMatrix , Multiply(rotateYMatrix , rotateZMatrix));
	return matrix;
}

//Translate
Matrix4x4 MakeTranslateMatrix(const Vec3 &translate) {
	Matrix4x4 matrix;
	matrix = MakeIdentity4x4();

	matrix.m[3][0] = translate.x;
	matrix.m[3][1] = translate.y;
	matrix.m[3][2] = translate.z;

	return matrix;
}

//Transform
Vec3 Transform(const Vec3& vector, const Matrix4x4 &matrix) {
	Vec3 result;
	// 各成分を計算
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];

	/*assert(w != 0.0f);*/
	result.x /= w;
	result.y /= w;
	result.z /= w;


	return result;
}

//Affine
Matrix4x4 MakeAffineMatrix(const Vec3 &scale , const Vec3 &rotate , const Vec3 &translate) {
	Matrix4x4 scaleMa = MakeScaleMatrix(scale);
	Matrix4x4 rotateMa = MakeRotateMatrix(rotate);
	Matrix4x4 translateMa = MakeTranslateMatrix(translate);

	Matrix4x4 matrix = Multiply(scaleMa , Multiply(rotateMa , translateMa));
	return matrix;
}

//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY , float aspectRatio , float nearClip , float farClip) {
	Matrix4x4 matrix;
	matrix.m[0][0] = 1.0f / aspectRatio * (1.0f / std::tan(fovY / 2.0f));
	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;
	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 1.0f / std::tan(fovY / 2.0f);
	matrix.m[1][2] = 0.0f;
	matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[2][2] = farClip / (farClip - nearClip);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	matrix.m[3][3] = 0.0f;

	return matrix;
}

//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left , float top , float right , float bottom , float nearClip , float farClip) {
	Matrix4x4 matrix;
	matrix.m[0][0] = 2.0f / (right - left);
	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;
	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 2.0f / (top - bottom);
	matrix.m[1][2] = 0.0f;
	matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[2][2] = 1.0f / (farClip - nearClip);
	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = (left + right) / (left - right);
	matrix.m[3][1] = (top + bottom) / (bottom - top);
	matrix.m[3][2] = nearClip / (nearClip - farClip);
	matrix.m[3][3] = 1.0f;

	return matrix;
}

//ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left , float top , float width , float height , float minDepth , float maxDepth) {
	Matrix4x4 matrix;
	matrix.m[0][0] = width / 2.0f;
	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;
	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = -(height / 2.0f);
	matrix.m[1][2] = 0.0f;
	matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[2][2] = maxDepth - minDepth;
	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = left + (width / 2.0f);
	matrix.m[3][1] = top + (height / 2.0f);
	matrix.m[3][2] = minDepth;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// 行列式を計算する関数
float Determinant(const Matrix4x4 &matrix) {
	float det =
		matrix.m[0][0] * (matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
						  matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
						  matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
						  matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
						  matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2] -
						  matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3]) -
		matrix.m[0][1] * (matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] +
						  matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] +
						  matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] -
						  matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] -
						  matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2] -
						  matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3]) +
		matrix.m[0][2] * (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
						  matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
						  matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] -
						  matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
						  matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] -
						  matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3]) -
		matrix.m[0][3] * (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] +
						  matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] +
						  matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] -
						  matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] -
						  matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] -
						  matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2]);

	return det;
}

//逆行列
Matrix4x4 Inverse(const Matrix4x4& matrix) {
	Matrix4x4 result;

	float det = Determinant(matrix);
	if (det == 0) {
		return result;
	}

	float invDet = 1.0f / det;

	result.m[0][0] = (matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
					  matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
					  matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
					  matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
					  matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2] -
					  matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3]) * invDet;
	result.m[0][1] = (-matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3] -
					  matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] -
					  matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2] +
					  matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] +
					  matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2] +
					  matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3]) * invDet;
	result.m[0][2] = (matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3] +
					  matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] +
					  matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2] -
					  matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1] -
					  matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2] -
					  matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3]) * invDet;
	result.m[0][3] = (-matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] -
					  matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] -
					  matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] +
					  matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] +
					  matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] +
					  matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3]) * invDet;

	result.m[1][0] = (-matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] -
					  matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] -
					  matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] +
					  matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] +
					  matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2] +
					  matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3]) * invDet;
	result.m[1][1] = (matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3] +
					  matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] +
					  matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2] -
					  matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0] -
					  matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2] -
					  matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3]) * invDet;
	result.m[1][2] = (-matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3] -
					  matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] -
					  matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2] +
					  matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] +
					  matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2] +
					  matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3]) * invDet;
	result.m[1][3] = (matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] +
					  matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] +
					  matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] -
					  matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] -
					  matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] -
					  matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3]) * invDet;

	result.m[2][0] = (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
					  matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
					  matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] -
					  matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
					  matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] -
					  matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3]) * invDet;
	result.m[2][1] = (-matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3] -
					  matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] -
					  matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1] +
					  matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] +
					  matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1] +
					  matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3]) * invDet;
	result.m[2][2] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3] +
					  matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] +
					  matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1] -
					  matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0] -
					  matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1] -
					  matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3]) * invDet;
	result.m[2][3] = (-matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] -
					  matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] -
					  matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] +
					  matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] +
					  matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] +
					  matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3]) * invDet;

	result.m[3][0] = (-matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] -
					  matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] -
					  matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] +
					  matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] +
					  matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] +
					  matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2]) * invDet;
	result.m[3][1] = (matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2] +
					  matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] +
					  matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1] -
					  matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0] -
					  matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1] -
					  matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2]) * invDet;
	result.m[3][2] = (-matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2] -
					  matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] -
					  matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1] +
					  matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] +
					  matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1] +
					  matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2]) * invDet;
	result.m[3][3] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +
					  matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +
					  matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
					  matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
					  matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] -
					  matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2]) * invDet;

	return result;
}

//クロス積
Vec3 Cross(const Vec3 &v1 , const Vec3 &v2) {
	Vec3 result;
	result.x = (v1.y * v2.z) - (v1.z * v2.y);
	result.y = (v1.z * v2.x) - (v1.x * v2.z);
	result.z = (v1.x * v2.y) - (v1.y * v2.x);
	return result;
}

//
static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void VectorScreenPrintf(int x , int y , Vec3 vector) {
	Novice::ScreenPrintf(
		x * kColumnWidth , y * kRowHeight , "%.02f" , vector.x
	);
	Novice::ScreenPrintf(
		x + 1 * kColumnWidth , y * kRowHeight , "%.02f" , vector.y
	);
	Novice::ScreenPrintf(
		x + 2 * kColumnWidth , y * kRowHeight , "%.02f" , vector.z
	);
}

const char kWindowTitle[] = "LD2B_06_ナガトモイチゴ_MT3_01_01";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	//宣言・初期化
	Vec3 kLocalVerticse[3] = {
		{0.0f, 0.05f, 0.0f},
		{0.1f, -0.05f, 0.0f},
		{-0.1f, -0.05f, 0.0f}
	};

	Vec3 camaraPosition = {
		0.0f, 0.0f, -1.0f
	};

	Vec3 rotate {0.0f, 0.0f, 0.0f};
	Vec3 translate {0.0f, 0.0f, 0.0f};

	Matrix4x4 worldMatrix;
	Matrix4x4 camaraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;
	Matrix4x4 viewportMatrix;
	Vec3 screenVertices[3];
	Vec3 ndcVertex;

	Vec3 v1 {1.2f, -3.9f, 2.5f};
	Vec3 v2 {2.8f, 0.4f, -1.3f};
	Vec3 cross = Cross(v1 , v2);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

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

		if (keys[DIK_D]) {
			translate.x += 0.01f;
		} else if (keys[DIK_A]) {
			translate.x -= 0.01f;
		} else if (keys[DIK_W]) {
			translate.z += 0.01f;
		} else if (keys[DIK_S]) {
			translate.z -= 0.01f;
		}

		rotate.y += 0.01f;

		worldMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f} , rotate , translate);
		camaraMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f} , {0.0f, 0.0f, 0.0f} , camaraPosition);
		viewMatrix = Inverse(camaraMatrix);
		projectionMatrix = MakePerspectiveFovMatrix(0.45f , 1280.0f / 720.0f , 0.1f , 100.0f);
		worldViewProjectionMatrix = Multiply(worldMatrix , Multiply(viewMatrix , projectionMatrix));
		viewportMatrix = MakeViewportMatrix(0 , 0 , 1280.0f , 720.0f , 0.0f , 1.0f);
		for (int i = 0; i < 3; ++i) {
			ndcVertex = Transform(kLocalVerticse[i] , worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex , viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawTriangle(
			static_cast<int>(screenVertices[0].x) , static_cast<int>(screenVertices[0].y) ,
			static_cast<int>(screenVertices[1].x) , static_cast<int>(screenVertices[1].y) ,
			static_cast<int>(screenVertices[2].x) , static_cast<int>(screenVertices[2].y) ,
			RED ,
			kFillModeSolid
		);

		VectorScreenPrintf(0 , 0 , cross);

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
