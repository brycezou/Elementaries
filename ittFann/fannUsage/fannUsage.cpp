#include <Windows.h>
#include <iostream>
#include "fann.h"

using namespace std;


class ittFann
{
private:
	unsigned int num_input;
	unsigned int num_output;
	unsigned int num_layers;
	unsigned int num_hidden;
public:
	struct fann *m_ann;
	ittFann()
	{
		m_ann = NULL;
	}
	ittFann(unsigned int nInput, unsigned int nOutput, unsigned int nLayers, unsigned int nHidden)
	{
		this->num_input = nInput;
		this->num_output = nOutput;
		this->num_layers = nLayers;
		this->num_hidden = nHidden;
		this->m_ann = NULL;
	}
	void fannInitial(fann_activationfunc_enum fun_hidden, fann_activationfunc_enum fun_output)
	{
		m_ann = fann_create_standard(num_layers, num_input, num_hidden, num_output);
		fann_set_activation_function_hidden(m_ann, fun_hidden);
		fann_set_activation_function_output(m_ann, fun_output);
		printf("network created!\n");
	}
	void fannTrain(char *filename, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error)
	{
		fann_train_on_file(m_ann, filename, max_epochs, epochs_between_reports, desired_error);
		printf("network trained!\n");
	}
	void fannSaveModel(char *filename)
	{
		fann_save(m_ann, filename);
		printf("network saved!\n");
	}
	void fannRelease()
	{
		fann_destroy(m_ann);
	}
	bool fannCreateFromFile(char *filename, bool bShow = true)
	{
		if (m_ann)
			fannRelease();
		m_ann = fann_create_from_file(filename);
		if(!m_ann)
		{
			printf("creating ann from file failed!\n");
			return false;
		}
		if (bShow)
		{
			fann_print_connections(m_ann);
			fann_print_parameters(m_ann);
		}
		printf("network created!\n");
		return true;
	}
	fann_type* fannPredict(fann_type *input, int nInput)
	{
		if (nInput == m_ann->num_input)
			return fann_run(m_ann, input);
		return NULL;
	}
};

int main()
{
	ittFann ittfann(2, 1, 3, 10);
	ittfann.fannInitial(FANN_SIGMOID_SYMMETRIC, FANN_SIGMOID_SYMMETRIC);
	ittfann.fannTrain("xor.txt", 100, 5, 0.001f);
	ittfann.fannSaveModel("model.txt");
	ittfann.fannRelease();

	ittFann ittfann2;
	ittfann2.fannCreateFromFile("model.txt", false);
	fann_type input[2] = {1, -1};
	fann_type *output = ittfann2.fannPredict(input, 2);
	for (unsigned int i = 0; i < ittfann2.m_ann->num_output; i++)
	{
		cout<<output[i]<<endl;
	}
	ittfann2.fannRelease();

	Sleep(10000);
	return 0;
}

