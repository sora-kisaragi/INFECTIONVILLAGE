//  @File Reader_Wav.h

#ifndef FILEREADERWAV_H_INCLUCDED
#define FILEREADERWAV_H_INCLUCDED

/*
* wav データを取り出して配列に格納する.
*
*/

#include <vector>

namespace FileReader
{
	class Wave
	{
	public:
		/*waveのフォーマット,とりあえずALと同じ感じでフォーマットは定義したがAL_enumの定義はないので使用側で設定する*/
		enum e_FORMAT
		{
			e_FORMAT_MONO8,		//8bit モノラル
			e_FORMAT_MONO16,	//16bit モノラル
			e_FORMAT_STEREO8,	//8bit ステレオ
			e_FORMAT_STEREO16,	//16bit ステレオ
			e_FORMAT_NULL,		//チャンネル数が上記に相当しない場合.
		};
		
		/*関数群*/
		bool ReadFile(const char* importFileName);
		void DiscardData();
		void PrintData()const;
		bool ComvertToMono();

		/*以下要素のゲッター*/
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


