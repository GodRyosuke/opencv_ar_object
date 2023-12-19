#include "Actor/ARMarker.hpp"

#define _USE_MATH_DEFINES
#include "math.h"

#include <opencv2/opencv.hpp>
#include <opencv2/aruco/charuco.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/euler_angles.hpp"

#include "Manager.hpp"
// #include "Component/VideoComponent.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Definitions.hpp"
#include "Util.hpp"
#include "Actor/Capture.hpp"
#include "Component/SpriteComponent.hpp"


ARMarker::ARMarker(Manager* manager)
    :Actor(manager, "ARMarker")
{
    m_dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    m_DetectedImgTex = new Texture(GL_TEXTURE0);
    m_SpriteComp = new SpriteComponent(this, m_DetectedImgTex, 5);

    double screenWidth = manager->GetScreenSize().x;
    double screenHeight = manager->GetScreenSize().y;
    glm::vec2 texScale = glm::vec2(static_cast<float>(screenWidth) , static_cast<float>(screenHeight));
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(texScale.x, texScale.y, 1.f));
    SetScale(scaleMat);
}

void ARMarker::UpdateActor()
{
    // 最新の動画フレームを取得する
    const Capture* capture = static_cast<const Capture*>(m_Manager->GetActor("Capture"));
    cv::Mat currentFrame = capture->GetCurrentFrame();
    if (currentFrame.rows == 0) {
        return;
    }

    // 検出したARマーカー情報を追記
    cv::Mat detected_img = currentFrame;
    std::vector<std::vector<cv::Point2f>> corners, rejectedCandidates;
    std::vector<int> ids;
    cv::Ptr<cv::aruco::DetectorParameters> params = cv::aruco::DetectorParameters::create();
    cv::aruco::detectMarkers(currentFrame, m_dictionary, corners, ids, params, rejectedCandidates);

#ifdef _REALSENSE
    // realsense d435f内部パラメータ
    cv::Mat cameraMat = (cv::Mat_<double>(3,3) << 1362.086753996991, 0, 959.5, 0, 1364.712560504054, 539.5, 0, 0, 1);
    cv::Mat distCoeffs = (cv::Mat_<double>(1,5) << 0.1406400877743553, -0.4505503566180014, 0, 0, 0.4179603222152956);
#else
    // pcカメラ
    cv::Mat cameraMat = (cv::Mat_<double>(3,3) << 852.8839555848483, 0, 639.5, 0, 852.215118770151, 359.5, 0, 0, 1);
    cv::Mat distCoeffs = (cv::Mat_<double>(1,5) << 0.08005910250868746, -0.3113160415419927, 0, 0, 0.196456176823346);
#endif
    std::vector<cv::Vec3d> marker_rots;
    std::vector<cv::Vec3d> marker_trans;

    if (ids.size() > 0) {
    // if ((ids.size() > 0) && (corners.size() > 0)) {
        cv::aruco::drawDetectedMarkers(detected_img, corners, ids);
        cv::aruco::estimatePoseSingleMarkers(corners, 0.055, cameraMat, distCoeffs, marker_rots, marker_trans);
        // Util::Print(marker_trans[0], "\n");
        cv::Mat rotMat;
        cv::Rodrigues(marker_rots[0], rotMat);
        
        // Actor* targetActor = m_Manager->GetActor("marker_axis");
        Actor* targetActor = m_Manager->GetActor("UnityChan");
        targetActor->SetPosition(glm::vec3(marker_trans[0][0], marker_trans[0][1], marker_trans[0][2]));
        glm::mat3 axisRot(
            rotMat.at<double>(0, 0), rotMat.at<double>(1, 0), rotMat.at<double>(2, 0),
            rotMat.at<double>(0, 1), rotMat.at<double>(1, 1), rotMat.at<double>(2, 1),
            rotMat.at<double>(0, 2), rotMat.at<double>(1, 2), rotMat.at<double>(2, 2)
        );
        // axisRot =  glm::inverse(axisRot);
        // glm::quat rot_quat = axisRot;
        // glm::vec3 euler = glm::eulerAngles(rot_quat);
        // axisRot = glm::eulerAngleXYZ(euler.z, euler.y, euler.x);
        // axisRot = glm::eulerAngleXYZ(-euler.x, -euler.y, -euler.z);
        // rollを計算
        // double roll = 0.0;
        // {
        //     double cos_theta = std::sqrt(1 - rotMat.at<double>(2, 0)*rotMat.at<double>(2, 0));
        //     if (cos_theta < 0.0000001) {    // theta == M_PI/2
        //         roll = std::atan2(rotMat.at<double>(0, 1), rotMat.at<double>(0, 2));
        //     } else {
        //         roll = std::atan2(rotMat.at<double>(2, 1)/cos_theta, rotMat.at<double>(2, 2)/cos_theta);
        //     }
        // }
        targetActor->SetRotation(axisRot);
        targetActor->SetScale(0.001);
        glm::mat4 camView = glm::lookAt(glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f));
        glm::vec2 screenSize = GetManager()->GetScreenSize();
        // const double fov = 45; // degree
        const double nearP = 0.1f;
        const double farP = 200;
        // glm::mat4 camProj;
        // camProj = glm::perspective<float>(glm::radians(fov), screenSize.x / screenSize.y, nearP, farP);

        // OpenGLのproj matを作る
        // camProj = glm::mat4(
        //     2*cameraMat.at<double>(0, 0)/screenSize.x, 0.0, 2*cameraMat.at<double>(0, 2)/screenSize.x - 1, 0.0,
        //     0.0, -2.0*cameraMat.at<double>(1, 1)/screenSize.y, -2.0*cameraMat.at<double>(1, 2)/screenSize.y + 1, 0.0,
        //     0.0, 0.0, (farP + nearP)/(farP - nearP), -2*farP*nearP/(farP - nearP),
        //     0.0, 0.0, 1.0, 0.0
        // );
        glm::mat4 camProj = glm::mat4(
            2*cameraMat.at<double>(0, 0)/screenSize.x, 0.0, -2*cameraMat.at<double>(0, 2)/screenSize.x + 1, 0.0,
            0.0, 2.0*cameraMat.at<double>(1, 1)/screenSize.y, 2.0*cameraMat.at<double>(1, 2)/screenSize.y - 1, 0.0,
            0.0, 0.0, -(farP + nearP)/(farP - nearP), -2*farP*nearP/(farP - nearP),
            0.0, 0.0, -1.0, 0.0
        );
        camProj = glm::transpose(camProj);
        // m_Manager->m_Renderer->SpecificShaderProcess("AxisShader", [camView, camProj](Shader* shader)->void {
        GetManager()->m_Renderer->AllShaderProcess([this, camProj, camView](Shader* shader) {
            shader->UseProgram();
            shader->SetMatrixUniform("CameraView", camView);
            shader->SetMatrixUniform("CameraProj", camProj);
        });
        // Util::Print(rotMat, "\n");
    } else {
        GetManager()->m_Renderer->AllShaderProcess([this](Shader* shader) {
        shader->UseProgram();
        shader->SetMatrixUniform("CameraView", glm::mat4(0.f));
        shader->SetMatrixUniform("CameraProj", glm::mat4(0.f));
    });
    }
    for (int i = 0; i < ids.size(); i++) {
        cv::aruco::drawAxis(detected_img, cameraMat, distCoeffs, marker_rots[i], marker_trans[i], 0.05);
    }

    m_DetectedImgTex->Update(detected_img);

    // textureの幅を現在の画面サイズの幅のm_imageRatio倍に合わせる
    double screenRatio = m_Manager->GetScreenSize().x / m_Manager->GetScreenSize().y;
    double videoRatio = static_cast<double>(m_DetectedImgTex->GetWidth()) / static_cast<double>(m_DetectedImgTex->GetHeight());
    double videoWidth = 0.0;
    double videoHeight = 0.0;
    if (screenRatio < videoRatio) {
        videoWidth = m_Manager->GetScreenSize().x;
        videoHeight = videoWidth / videoRatio;
    } else {
        videoHeight = m_Manager->GetScreenSize().y;
        videoWidth = videoHeight * videoRatio;
    }
    
    glm::vec2 texScale = glm::vec2(static_cast<float>(videoWidth) , static_cast<float>(videoHeight));
    // texScale *= m_Manager->GetScreenSize().x * m_imageRatio / tex->GetWidth();
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(texScale.x, texScale.y, 1.f));
    SetScale(scaleMat);


    
}