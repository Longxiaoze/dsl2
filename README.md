# DSL2

The Orginal repo is [DSL](https://github.com/hyye/dsl)

Project page: https://sites.google.com/view/dsl-ram-lab/

I update this code to **ubuntu20+cuda11.8+opencv4** and I wrote a **detailed installation document**.

# Install
## Packages Pangolin v0.6
you need to add `#include <limits>`  in `Pangolin-0.6/include/pangolin/gl/colour.h`

you need to change `return attributes.memoryType == cudaMemoryTypeDevice;` to `return attributes.type == cudaMemoryTypeDevice;`  in `Pangolin-0.6/include/pangolin/image/memcpy.h` line 55

``` bash
# some libs
sudo apt-get install cmake
sudo apt-get install libgoogle-glog-dev libgflags-dev
sudo apt-get install libatlas-base-dev
sudo apt-get install libeigen3-dev
sudo apt-get install libsuitesparse-dev

# Pangolin v0.6
wget https://github.com/stevenlovegrove/Pangolin/archive/refs/tags/v0.6.zip
unzip v0.6.zip
cd Pangolin-0.6
mkdir build
cd build
sudo apt-get install libgl1-mesa-dev libglew-dev libpython3-dev python3-numpy cmake ffmpeg libavcodec-dev libavutil-dev libavformat-dev libswscale-dev libjpeg-dev libpng-dev libtiff-dev libopenexr-dev
sudo apt install libusb-1.0-0-dev
cmake ..
make -j8
sudo make install
```

## OpenGL check
**You need to make sure that you run code in your terminal**
``` bash
glxinfo | grep "OpenGL"
```
**This needs to show that you are using a NVIDIA GPU driver**
``` html
OpenGL vendor string: NVIDIA Corporation
OpenGL renderer string: NVIDIA GeForce RTX 3090/PCIe/SSE2
OpenGL core profile version string: 4.6.0 NVIDIA 535.216.01
OpenGL core profile shading language version string: 4.60 NVIDIA
OpenGL core profile context flags: (none)
OpenGL core profile profile mask: core profile
OpenGL core profile extensions:
OpenGL version string: 4.6.0 NVIDIA 535.216.01
OpenGL shading language version string: 4.60 NVIDIA
OpenGL context flags: (none)
OpenGL profile mask: (none)
OpenGL extensions:
OpenGL ES profile version string: OpenGL ES 3.2 NVIDIA 535.216.01
OpenGL ES profile shading language version string: OpenGL ES GLSL ES 3.20
OpenGL ES profile extensions:

```

## pcl 1.10 + ceres
If you are using ubuntu20, you can use:
``` bash
sudo apt-get install libpcl-dev
```

If you are using ubuntu22, you need to insall pcl-1.10 from source:
``` bash
# del existing pcl, be careful with this
sudo rm -rf /usr/local/include/pcl-*
sudo rm -rf /usr/local/lib/libpcl_*
sudo rm -rf /usr/local/lib/pkgconfig/pcl_*
sudo rm -rf /usr/local/bin/pcl_*
sudo rm -rf /usr/local/share/pcl*

git clone https://github.com/PointCloudLibrary/pcl.git
cd pcl
git checkout pcl-1.10.0
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
make -j8
sudo make install
```
If you are using ubuntu22, you need to insall ceres 1.14 from source:
``` bash
# make sure if you need to install for ceres 1.14
# sudo rm -rf /usr/local/include/oneapi /usr/local/include/tbb /usr/local/lib/libtbb*
wget https://github.com/oneapi-src/oneTBB/archive/refs/tags/2020_U3.tar.gz
tar -xvf 2020_U3.tar.gz
cd oneTBB-2020_U3
make -j8
sudo make install
cd ../

sudo apt-get update
sudo apt-get install cmake libgoogle-glog-dev libgflags-dev libatlas-base-dev libeigen3-dev libsuitesparse-dev
wget http://ceres-solver.org/ceres-solver-1.14.0.tar.gz
tar zxf ceres-solver-1.14.0.tar.gz
mkdir build
cd build
cmake ..
make -j8
make test
sudo make install
```



## DSL2
``` bash
# git clone https://github.com/hyye/dsl
git clone https://github.com/Longxiaoze/dsl2
cd dsl2
git submodule update --init --recursive
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
make
```


# RUN
``` bash
cd ~/codes/slam_projects/dsl2/build
./src/dsl_main --path ../left_pinhole
```


## Monocular Direct Sparse Localization in a Prior 3D Surfel Map
#### Authors: Haoyang Ye, Huaiyang Huang, and Ming Liu from [RAM-LAB](https://ram-lab.com/).

## Paper and Video
Related publications:
```
@inproceedings{ye2020monocular,
  title={Monocular direct sparse localization in a prior 3d surfel map},
  author={Ye, Haoyang and Huang, Huaiyang and Liu, Ming},
  booktitle={2020 IEEE International Conference on Robotics and Automation (ICRA)},
  pages={8892--8898},
  year={2020},
  organization={IEEE}
}
@inproceedings{ye20213d,
  title={3D Surfel Map-Aided Visual Relocalization with Learned Descriptors},
  author={Ye, Haoyang and Huang, Huaiyang and Hutter, Marco and Sandy, Timothy and Liu, Ming},
  booktitle={2021 International Conference on Robotics and Automation (ICRA)},
  pages={5574-5581},
  year={2021},
  organization={IEEE}
}
```
Video:
https://www.youtube.com/watch?v=LTihCBGcURo

## Dependency
1. [Pangolin](https://github.com/stevenlovegrove/Pangolin).
2. [CUDA](https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&target_distro=Ubuntu&target_version=1804&target_type=deblocal).
3. [Ceres-solver](http://ceres-solver.org/installation.html#linux).
4. [PCL](http://www.pointclouds.org/downloads/), the default version accompanying by ROS.
5. [OpenCV](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html), the default version accompanying by ROS.

## Build
0. `git submodule update --init --recursive`
1. `mkdir build && cd build`
2. `cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo` 
3. `make -j8`

## Example
The sample config file can be downloaded from [this link](https://hkustconnect-my.sharepoint.com/:u:/g/personal/hyeab_connect_ust_hk/EXDlfYJgCclHuxvTiEcfpbIBJmNAwdE1soquwUeHGuaItw?e=63oiEP).

To run the example:
```shell
[path_to_build]/src/dsl_main --path "[path_to_dataset]/left_pinhole"
```

## Preparing Your Own Data
1. Collect LiDAR and camera data.
2. Build LiDAR map and obtain LiDAR poses (the poses are not necessary).
3. Pre-process LiDAR map to make the `[path_to_dataset]/*.pcd` map file contains `normal_x, normal_y, normal_z` fields (downsample & normal estimation).
4. Extract and undistort images into `[path_to_dataset]/images`.
5. Set the first camera pose to `initial_pose` and other camera parameters in `[path_to_dataset]/config.yaml`.

## Note
This implementation of DSL takes [Ceres Solver](http://ceres-solver.org/) as backend, which is different from the the implementation of the original paper with DSO-backend. This leads to different performance, i.e., speed and accuracy, compared to the reported results.

## Credits
This work is inspired from several open-source projects, such as [DSO](https://github.com/JakobEngel/dso), [DSM](https://github.com/jzubizarreta/dsm), [Elastic-Fusion](https://github.com/mp3guy/ElasticFusion), [SuperPoint](https://github.com/magicleap/SuperPointPretrainedNetwork), [DBoW2](https://github.com/dorian3d/DBoW2), [NetVlad](https://www.di.ens.fr/willow/research/netvlad/), [LIO-mapping](https://github.com/hyye/lio-mapping) and etc.

## Licence
The source code is released under [GPL-3.0](https://www.gnu.org/licenses/).
