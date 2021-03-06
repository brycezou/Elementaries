#include "ittFann.h"


ittFann::ittFann()
{
	m_ann = NULL;
}
ittFann::ittFann(unsigned int nInput, unsigned int nOutput, unsigned int nLayers, unsigned int nHidden)
{
	this->num_input = nInput;
	this->num_output = nOutput;
	this->num_layers = nLayers;
	this->num_hidden = nHidden;
	this->m_ann = NULL;
}
void ittFann::fannInitial(fann_activationfunc_enum fun_hidden, fann_activationfunc_enum fun_output)
{
	//m_ann = fann_create_standard(num_layers, num_input, num_hidden, num_output);
	m_ann = fann_create_standard(num_layers, num_input, num_hidden, num_hidden, num_output);
	fann_set_activation_function_hidden(m_ann, fun_hidden);
	fann_set_activation_function_output(m_ann, fun_output);
	printf("network created!\n");
}
void ittFann::fannTrain(char *filename, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error)
{
	fann_train_on_file(m_ann, filename, max_epochs, epochs_between_reports, desired_error);
	printf("network trained!\n");
}
void ittFann::fannSaveModel(char *filename)
{
	fann_save(m_ann, filename);
	printf("network saved!\n");
}
void ittFann::fannRelease()
{
	fann_destroy(m_ann);
}
bool ittFann::fannCreateFromFile(char *filename, bool bShow)
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
fann_type* ittFann::fannPredict(fann_type *input, int nInput)
{
	if (nInput == m_ann->num_input)
		return fann_run(m_ann, input);
	return NULL;
}

//class ittFann
//{
//private:
//	unsigned int num_input;
//	unsigned int num_output;
//	unsigned int num_layers;
//	unsigned int num_hidden;
//public:
//	struct fann *m_ann;
//	ittFann()
//	{
//		m_ann = NULL;
//	}
//	ittFann(unsigned int nInput, unsigned int nOutput, unsigned int nLayers, unsigned int nHidden)
//	{
//		this->num_input = nInput;
//		this->num_output = nOutput;
//		this->num_layers = nLayers;
//		this->num_hidden = nHidden;
//		this->m_ann = NULL;
//	}
//	void fannInitial(fann_activationfunc_enum fun_hidden, fann_activationfunc_enum fun_output)
//	{
//		//m_ann = fann_create_standard(num_layers, num_input, num_hidden, num_output);
//		m_ann = fann_create_standard(num_layers, num_input, num_hidden, num_hidden, num_output);
//		fann_set_activation_function_hidden(m_ann, fun_hidden);
//		fann_set_activation_function_output(m_ann, fun_output);
//		printf("network created!\n");
//	}
//	void fannTrain(char *filename, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error)
//	{
//		fann_train_on_file(m_ann, filename, max_epochs, epochs_between_reports, desired_error);
//		printf("network trained!\n");
//	}
//	void fannSaveModel(char *filename)
//	{
//		fann_save(m_ann, filename);
//		printf("network saved!\n");
//	}
//	void fannRelease()
//	{
//		fann_destroy(m_ann);
//	}
//	bool fannCreateFromFile(char *filename, bool bShow = true)
//	{
//		if (m_ann)
//			fannRelease();
//		m_ann = fann_create_from_file(filename);
//		if(!m_ann)
//		{
//			printf("creating ann from file failed!\n");
//			return false;
//		}
//		if (bShow)
//		{
//			fann_print_connections(m_ann);
//			fann_print_parameters(m_ann);
//		}
//		printf("network created!\n");
//		return true;
//	}
//	fann_type* fannPredict(fann_type *input, int nInput)
//	{
//		if (nInput == m_ann->num_input)
//			return fann_run(m_ann, input);
//		return NULL;
//	}
//};