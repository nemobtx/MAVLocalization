//
//  RTAB_bag_of_words.hpp
//  LoopClosure
//
//  Created by jimmy on 2016-03-05.
//  Copyright © 2016 jimmy. All rights reserved.
//

#ifndef RTAB_bag_of_words_cpp
#define RTAB_bag_of_words_cpp

// Video Google: A text retrieval approach to object matching in videos ICCV 2013
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <vector>
#include "RTAB_kdtree.hpp"

using std::vector;

class RTAB_bag_of_words
{
    cv::Mat cluster_centers_;   // k-mean cluster center
    cv::Mat partition_;         // partition in the original data
    
    // section 6.1, remove most frequent and lest frequent words
    vector<size_t> lest_frequent_words_index_;
    vector<size_t> most_frequent_words_index_;
    vector<vector<int>> image_index_; // inversed index file
    
    RTAB_kdtree<float> kdtree_;
    
    // tf_idf parameter
    bool has_tf_idf_;
    int num_images_;
    vector<size_t> num_image_contain_term_; // the number of images containing the term. index: word index; value: number of images what it occurs
    vector<double> log_N_ni_;          // cache of "inverse document frequence"
    
public:
    RTAB_bag_of_words()
    {
        has_tf_idf_ = false;
    }
    ~RTAB_bag_of_words(){;}
    
    // feature_data_base: visual features
    // k: word vector length
    bool generate_visual_words(const cv::Mat & feature_database, int num_k_mean);
    
    // quantize all features (descriptors) in an image to a visual word
    // word: normalized word frequency, a row vector
    bool quantize_features(const cv::Mat & features, cv::Mat & word_frequency) const;
    
    // database_features: features from multiple images
    // tf_idf: term frequency–inverse document frequency .a text retrieval method
    bool generate_tf_idf_visual_words(const vector<cv::Mat> & database_features, int num_k_mean);
    
    // quantize all descriptors in an image to a visual word
    // tf_idf: term frequency–inverse document frequency .a text retrieval method
    bool quantize_features_it_idf(const cv::Mat & descriptors, cv::Mat & word) const;
    
    // only for cluster center, partition, and kdtree
    bool write_vocabulary(const char *xml_file) const;
    bool read_vocabulary(const char *xml_file);
    
    
private:
    
  //  bool set_stop_list(const double top_ratio = 0.05, const double bottom_ratio = 0.1);
    
   
    
};

// sort with index
// from http://stackoverflow.com/questions/1577475/c-sorting-and-keeping-track-of-indexes
template <typename T>
vector<size_t> sort_indices(const vector<T> & v)
{
    vector<size_t> idx(v.size());
    for (size_t i = 0; i < idx.size(); i++) {
        idx[i] = i;
    }
    std::sort(idx.begin(), idx.end(),
              [&v](size_t i1, size_t i2){return v[i1] < v[i2];});
    return idx;
}




#endif /* RTAB_bag_of_words_cpp */
