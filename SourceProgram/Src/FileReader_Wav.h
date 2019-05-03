//  @File Reader_Wav.h

#ifndef FILEREADERWAV_H_INCLUCDED
#define FILEREADERWAV_H_INCLUCDED

/*
* wav �f�[�^�����o���Ĕz��Ɋi�[����.
*
*/

#include <vector>

namespace FileReader
{
	class Wave
	{
	public:
		/*wave�̃t�H�[�}�b�g,�Ƃ肠����AL�Ɠ��������Ńt�H�[�}�b�g�͒�`������AL_enum�̒�`�͂Ȃ��̂Ŏg�p���Őݒ肷��*/
		enum e_FORMAT
		{
			e_FORMAT_MONO8,		//8bit ���m����
			e_FORMAT_MONO16,	//16bit ���m����
			e_FORMAT_STEREO8,	//8bit �X�e���I
			e_FORMAT_STEREO16,	//16bit �X�e���I
			e_FORMAT_NULL,		//�`�����l��������L�ɑ������Ȃ��ꍇ.
		};
		
		/*�֐��Q*/
		bool ReadFile(const char* importFileName);
		void DiscardData();
		void PrintData()const;
		bool ComvertToMono();

		/*�ȉ��v�f�̃Q�b�^�[*/
		e_FORMAT GetFormat()const{return format;}
		const std::vector<uint8_t>& GetData()const { return waveData; }
		unsigned int GetSampleRate()const { return samplingRate; }
		const size_t GetSize()const { return dataSize; }
	private:
		e_FORMAT format = e_FORMAT_NULL;
		std::vector <uint8_t> waveData;
		unsigned int samplingRate=0;
		size_t dataSize=0;
	};


}

#endif // !FILEREADERWAV_H_INCLUCDED


