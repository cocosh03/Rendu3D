#include "model.h"
#include <>

Model::Model(const char *filename): verts_(), faces_(){

  int Model::nverts() {
    return (int)verts_.size();
  }

  int Model::nfaces() {
    return (int)faces_.size();
  }

  Vec3f Model::vert(int i){
    return verts_[i];
  }

  Model::~Model(){
  }

  std::vector<int> Model::face(int idx) {
    return faces_[idx];
  }
}
