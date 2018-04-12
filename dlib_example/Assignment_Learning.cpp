#include "Assignment_Learning.h"

using namespace std;
using namespace dlib;

void serialize(const feature_extractor&, std::ostream&)
{

}
void deserialize(feature_extractor&, std::istream&)
{

}

int Assignment_Learnning()
{
	try
	{
		// Get a small bit of training data.
		std::vector<sample_type> samples;
		std::vector<label_type> labels;
		make_data(samples, labels);


		structural_assignment_trainer<feature_extractor> trainer;
		// This is the common SVM C parameter.  Larger values encourage the
		// trainer to attempt to fit the data exactly but might overfit. 
		// In general, you determine this parameter by cross-validation.
		trainer.set_c(10);
		// This trainer can use multiple CPU cores to speed up the training.  
		// So set this to the number of available CPU cores. 
		trainer.set_num_threads(4);

		// Do the training and save the results in assigner.
		assignment_function<feature_extractor> assigner = trainer.train(samples, labels);


		// Test the assigner on our data.  The output will indicate that it makes the
		// correct associations on all samples.
		cout << "Test the learned assignment function: " << endl;
		for (unsigned long i = 0; i < samples.size(); ++i)
		{
			// Predict the assignments for the LHS and RHS in samples[i].   
			std::vector<long> predicted_assignments = assigner(samples[i]);
			cout << "true labels:      " << trans(mat(labels[i]));
			cout << "predicted labels: " << trans(mat(predicted_assignments)) << endl;
		}

		// We can also use this tool to compute the percentage of assignments predicted correctly.
		cout << "training accuracy: " << test_assignment_function(assigner, samples, labels) << endl;


		// Since testing on your training data is a really bad idea, we can also do 5-fold cross validation.
		// Happily, this also indicates that all associations were made correctly.
		randomize_samples(samples, labels);
		cout << "cv accuracy:       " << cross_validate_assignment_trainer(trainer, samples, labels, 5) << endl;



		// Finally, the assigner can be serialized to disk just like most dlib objects.
		serialize("assigner.dat") << assigner;

		// recall from disk
		deserialize("assigner.dat") >> assigner;
	}
	catch (std::exception& e)
	{
		cout << "EXCEPTION THROWN" << endl;
		cout << e.what() << endl;
	}
	return 0;
}

void make_data(	std::vector<sample_type>& samples,	std::vector<label_type>& labels)
{
	// Make four different vectors.  We will use them to make example assignments.
	column_vector A(num_dims), B(num_dims), C(num_dims), D(num_dims);
	A = 1, 0, 0;
	B = 0, 1, 0;
	C = 0, 0, 1;
	D = 0, 1, 1;

	std::vector<column_vector> lhs;
	std::vector<column_vector> rhs;
	label_type mapping;

	// In all the assignments to follow, we will only say an element of the LHS 
	// matches an element of the RHS if the two are equal.  So A matches with A, 
	// B with B, etc.  But never A with C, for example.
	// ------------------------

	lhs.resize(3);
	lhs[0] = A;
	lhs[1] = B;
	lhs[2] = C;

	rhs.resize(3);
	rhs[0] = B;
	rhs[1] = A;
	rhs[2] = C;

	mapping.resize(3);
	mapping[0] = 1;  // lhs[0] matches rhs[1]
	mapping[1] = 0;  // lhs[1] matches rhs[0]
	mapping[2] = 2;  // lhs[2] matches rhs[2]

	samples.push_back(make_pair(lhs, rhs));
	labels.push_back(mapping);

	// ------------------------

	lhs[0] = C;
	lhs[1] = A;
	lhs[2] = B;

	rhs[0] = A;
	rhs[1] = B;
	rhs[2] = D;

	mapping[0] = -1;  // The -1 indicates that lhs[0] doesn't match anything in rhs.
	mapping[1] = 0;   // lhs[1] matches rhs[0]
	mapping[2] = 1;   // lhs[2] matches rhs[1]

	samples.push_back(make_pair(lhs, rhs));
	labels.push_back(mapping);

	// ------------------------

	lhs[0] = A;
	lhs[1] = B;
	lhs[2] = C;

	rhs.resize(4);
	rhs[0] = C;
	rhs[1] = B;
	rhs[2] = A;
	rhs[3] = D;

	mapping[0] = 2;
	mapping[1] = 1;
	mapping[2] = 0;

	samples.push_back(make_pair(lhs, rhs));
	labels.push_back(mapping);

	// ------------------------

	lhs.resize(2);
	lhs[0] = B;
	lhs[1] = C;

	rhs.resize(3);
	rhs[0] = C;
	rhs[1] = A;
	rhs[2] = D;

	mapping.resize(2);
	mapping[0] = -1;
	mapping[1] = 0;

	samples.push_back(make_pair(lhs, rhs));
	labels.push_back(mapping);

	// ------------------------

	lhs.resize(3);
	lhs[0] = D;
	lhs[1] = B;
	lhs[2] = C;

	// rhs will be empty.  So none of the items in lhs can match anything.
	rhs.resize(0);

	mapping.resize(3);
	mapping[0] = -1;
	mapping[1] = -1;
	mapping[2] = -1;

	samples.push_back(make_pair(lhs, rhs));
	labels.push_back(mapping);
}
