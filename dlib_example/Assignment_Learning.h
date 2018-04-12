#ifndef _ASSIGNMENT_LEARNING_H__
#define _ASSIGNMENT_LEARNING_H__


#include <iostream>
#include <dlib/svm_threaded.h>

typedef dlib::matrix<double, 0, 1> column_vector;
typedef std::pair<std::vector<column_vector>, std::vector<column_vector> > sample_type;
typedef std::vector<long> label_type;
const unsigned long num_dims = 3;

struct feature_extractor
{
	typedef column_vector feature_vector_type;
	typedef column_vector lhs_element;
	typedef column_vector rhs_element;
	unsigned long num_features() const
	{
		return num_dims;
	}
	void get_features(	const lhs_element& left,const rhs_element& right,feature_vector_type& feats) const
	{
		feats = squared(left - right);
	}
};

void serialize(const feature_extractor&, std::ostream&);
void deserialize(feature_extractor&, std::istream&);


//分配学习
int Assignment_Learnning();

//合成数据
void make_data(std::vector<sample_type>& samples, std::vector<label_type>& labels);
#endif
