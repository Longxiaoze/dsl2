/**
* This file is part of Direct Sparse Localization (DSL).
*
* Modified for compatibility with modern OpenGL standards.
*/

#ifndef DSL_GUI_H_
#define DSL_GUI_H_

#include <GL/glew.h>
#include <pangolin/pangolin.h>
#include <pangolin/gl/gl.h>
#include <pangolin/gl/gldraw.h>
#include <map>
#include "core/gpu_texture.h"
#include "util/global_calib.h"
#include "shaders/shaders.h"

namespace dsl {

class GUI {
 public:
  GUI(bool offscreen = false, bool no_panel = false);

  virtual ~GUI();

  void PreCall();
  void PostCall();

  void DisplayImg(const std::string &id, GPUTexture *img, bool flipy = false);

  inline void DrawFrustum(const Eigen::Matrix4f &pose, float scale = 0.1) {
    Eigen::Matrix3f K = Eigen::Matrix3f::Identity();
    K(0, 0) = fxG[0];
    K(1, 1) = fyG[0];
    K(0, 2) = cxG[0];
    K(1, 2) = cyG[0];

    Eigen::Matrix3f Kinv = K.inverse();

    pangolin::glDrawFrustum(Kinv.cast<GLfloat>(),
                            wG[0],
                            hG[0],
                            pose,
                            scale);
  }

  void DrawAxes(const Eigen::Matrix4f &pose, float scale) {
    glPushMatrix();
    glMultMatrixf(pose.data());

    glBegin(GL_LINES);
    // X-axis
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(scale, 0, 0);

    // Y-axis
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, scale, 0);

    // Z-axis
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, scale);
    glEnd();

    glPopMatrix();
  }

  void DrawWorldPoints(const std::vector<Eigen::Vector3f, Eigen::aligned_allocator<Eigen::Vector3f>> &world_points,
                       float point_size = 1.0,
                       const Eigen::Vector3f &color = Eigen::Vector3f(1, 0, 0)) {
    glPushMatrix();

    glColor3f(color.x(), color.y(), color.z());
    glPointSize(point_size);

    glBegin(GL_POINTS);
    for (const auto &pt : world_points) {
      glVertex3f(pt.x(), pt.y(), pt.z());
    }
    glEnd();

    glPointSize(1.0);
    glColor3f(0, 0, 0);

    glPopMatrix();
  }

  void DrawMeshes(const std::vector<std::vector<Eigen::Vector3f, Eigen::aligned_allocator<Eigen::Vector3f>>> &triangles,
                  const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 1)) {
    glPushMatrix();

    glColor3f(color.x(), color.y(), color.z());

    for (const auto &triangle : triangles) {
      glBegin(GL_LINE_LOOP);
      for (const auto &vertex : triangle) {
        glVertex3f(vertex.x(), vertex.y(), vertex.z());
      }
      glEnd();
    }

    glColor3f(0, 0, 0);

    glPopMatrix();
  }

  void SetFollowing(const Eigen::Matrix4f &currPose);
  void FollowPose(const Eigen::Matrix4f &currPose);
  void FollowAbsPose(const Eigen::Matrix4f &currPose);

  int width;
  int height;
  int panel;

  pangolin::Var<bool> *pause,
      *debug,
      *tracking_debug,
      *step,
      *followPose,
      *save,
      *run_opt,
      *draw_normals,
      *draw_colors,
      *draw_predict,
      *draw_global_model,
      *draw_trajectory;
  pangolin::Var<int> *gpuMem;
  pangolin::Var<int> *dataIdx;
  pangolin::Var<float> *accuracyIdx;
  pangolin::Var<float> *percentageIdx;
  pangolin::Var<std::string> *totalPoints;

  pangolin::DataLog resLog, inLog;
  pangolin::Plotter *resPlot,
      *inPlot;

  pangolin::OpenGlRenderState s_cam;
  pangolin::OpenGlMatrix delta_mv;

  pangolin::GlRenderBuffer *renderBuffer;
  pangolin::GlFramebuffer *colorFrameBuffer;
  GPUTexture *colorTexture;
  std::shared_ptr<Shader> colorProgram;
};

}

#endif  // DSL_GUI_H_
