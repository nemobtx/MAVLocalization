//
//  SCRF_tree.hpp
//  LoopClosure
//
//  Created by jimmy on 2016-03-19.
//  Copyright © 2016 jimmy. All rights reserved.
//

#ifndef SCRF_tree_cpp
#define SCRF_tree_cpp

// Scene Coordinate Regression Forests for Camera Relocalization in RGB-D Images
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"
#include <vector>
#include "SCRF_util.hpp"

using std::vector;

class SCRF_tree_node;

class SCRF_tree
{
    friend class SCRF_regressor;
    SCRF_tree_node *root_;
    cv::RNG rng_;   // random number generator
    
    SCRF_tree_parameter param_;
    
public:
    bool verbose_;
    
public:
    SCRF_tree()
    {
        root_ = NULL;
        verbose_ = true;
    }
    ~SCRF_tree(){;} //@todo release memory
    
    
    
    SCRF_tree_node * root_node()
    {
        return root_;
    }
    
    void setRootNode(SCRF_tree_node * node)
    {
        root_ = node;
    }
    
    void setTreeParameter(const SCRF_tree_parameter & param)
    {
        param_ = param;
    }
    // training random forest by build a decision tree
    // samples: sampled image pixel locations
    // rgbImages: same size, rgb, 8bit image
    // depthImages: same size, 16bit image
    bool build(const vector<SCRF_learning_sample> & samples,
               const vector<cv::Mat> & rgbImages,               
               const SCRF_tree_parameter & param);
    
    // indices: index of samples
    bool build(const vector<SCRF_learning_sample> & samples,
               const vector<unsigned int> & indices,
               const vector<cv::Mat> & rgbImages,
               const SCRF_tree_parameter & param);
    
    // output: depth in world coordinate (in testing_sample)
    bool predict(const SCRF_learning_sample & sample,
                 const cv::Mat & rgbImage,
                 SCRF_testing_result & predict) const;
    
private:
    bool configure_node(const vector<SCRF_learning_sample> & samples,
                        const vector<cv::Mat> & rgbImages,                        
                        const vector<unsigned int> & indices,
                        int depth,
                        SCRF_tree_node *node,
                        const SCRF_tree_parameter & param);
    
    bool predict(const SCRF_tree_node * const node,
                 const SCRF_learning_sample & sample,
                 const cv::Mat & rgbImage,
                 SCRF_testing_result & predict) const;
    
    
    
    
};

struct SCRF_split_parameter
{
    cv::Vec2d d1_;  // displacement in image coordinate
    cv::Vec2d d2_;
    int c1_;        // rgb image channel
    int c2_;
    double threhold_;  // threshold of splitting. store result
    
public:
    SCRF_split_parameter()
    {
        c1_ = 0;
        c2_ = 0;
        threhold_ = 0.0;
    }
};

class SCRF_tree_node
{
public:
    SCRF_tree_node *left_child_;
    SCRF_tree_node *right_child_;
    
    SCRF_split_parameter split_param_;    
    cv::Point3d p3d_; // world coordinate of the node, only available in leaf node
    
    int depth_;
    bool is_leaf_;
    
    SCRF_tree_node()
    {
        left_child_ = NULL;
        right_child_ = NULL;
        is_leaf_ = false;
        depth_ = 0;
    }
    SCRF_tree_node(int depth)
    {
        left_child_ = NULL;
        right_child_ = NULL;
        is_leaf_ = false;
        depth_ = depth;        
    }
    
    static bool write_tree(const char *fileName, SCRF_tree_node * root);
    
    static bool read_tree(const char *fileName, SCRF_tree_node * & root);

    
};



#endif /* SCRF_tree_cpp */
