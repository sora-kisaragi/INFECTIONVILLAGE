#define _CRT_SECURE_NO_WARNINGS
#include "FileReader_Wav.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/stat.h>
#include <string>

namespace FileReader
{
	void ErrorMessage(std::string str)
	{
		std::cerr << "ERROR>> Wave::FOpen>>" << str << std::endl;

	}
	/**
	* バイト列から数値を復元する.
	*
	* @param p      バイト列へのポインタ.
	* @param offset 数値のオフセット.
	* @param size   数値のバイト数(1～4).
	*
	* @return 復元した数値.
	*/
	uint32_t Get(const uint8_t* p, size_t offset, size_t size)
	{
		uint32_t n = 0;
		p += offset;
		for (size_t i = 0; i < size; ++i) {
			n += p[i] << (i * 8);
		}
		return n;
	}

	/*
	* バッファーからデータを読み取る.
	*
	* @param data	:受け取る配列(vector型)
	* @param buf	:読み取るバッファ
	* @param offset	:バッファのオフセット.
	*
	* @note <<読み取っただけオフセットの数は増やす使用にしている
	* @note <<注意事項,先に読み取るバッファのサイズをresizeしておく.
	*/
	void ReadBuffer(std::vector<uint8_t>& data, std::vector<uint8_t>& buf, size_t& offset)
	{
		for (size_t i = 0; i < data.size(); ++i)
		{
			data[i] = buf[offset];
			++offset;
		}
	}
	/*
	* std::vector<uint8_t> と std::string の比較.
	*
	* @param data 比較したいバイトデータ.
	* @param str 比較したい文字列.
	*
	* @retren  ture << 完全に一致 : false << 不一致 またはstringの長さが足りない;
	*
	* @note strの長さが足りない場合は問答無用でfalseを返すので注意.
	*/
	bool CheckString(const std::vector<uint8_t>& data, const std::string& str)
	{
		if (data.size() > str.size()) {
			ErrorMessage("CheckString関数で文字が足りません");
			return false;
		}
		for (int i = 0; i < data.size(); ++i) {
			if (data[i] != str[i])
			{
				return false;
			}
		}
		return true;
	}

	bool Wave::ReadFile(const char* filename)
	{

		//ファイルのサイズを取得する.
		struct stat st;
		if (stat(filename, &st)) {
			ErrorMessage("stat でファイルサイズの取得に失敗");
			return false;
		}
		//ファイルを開く.
		FILE *fp;
		fp = fopen(filename, "rb");
		if (!fp) {
			ErrorMessage("ファイルの読み込みに失敗");
			return false;
		}
		//ファイルのデータを一時保持する.
		std::vector<uint8_t> buf;
		//受け取るバッファのサイズをファイルに合わせる.
		buf.resize(st.st_size);
		//std::cout << buf.size() << std::endl;
		//ファイルからバイナリデータをバイト単位で受け取り受け取ったサイズを記録する
		const size_t readSize = fread(buf.data(), 1, st.st_size, fp);
		//ファイルを閉じる.
		fclose(fp);
		//もしファイルのサイズが最初に取得したファイルと違った場合終了する.
		if (readSize != st.st_size) {
			ErrorMessage("ファイルのサイズが合いません");
			return false;
		}

		//オフセットindexを用意.
		size_t offsetBuffer = 0;
		//バッファから一時的にデータを読み取るTempdataを用意する.
		std::vector<uint8_t> tmpData;

		//bufから4バイト分を受け取る.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//WAVEかどうかをチェックする.
		//if(tmpData[0] != 'R' || tmpData[1] != 'I' || tmpData[2] != 'F' || tmpData[3] != 'F' )
		//いちいち打ちたくなかったので関数にした.
		//if(CheckString(tmpData))
		if (!CheckString(tmpData, "RIFF")) {
			ErrorMessage("not RIFF");
			return false;
		}
		//次の4バイトを読み取る.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//チャンク"RIFF"の次の4バイトは総ファイル数なので読み飛ばす

		//次の4バイトを読み取る.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//"WAVE"でなければ終了.
		if (!CheckString(tmpData, "WAVE")) {
			ErrorMessage("not WAVE");
		}

		//次の4バイトを読み取る.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//"fmt "でなければ終了.
		if (!CheckString(tmpData, "fmt ")) {
			ErrorMessage("not fmt ");
		}
		uint32_t chunkSize, sampleRate, avgBytesPerSec, dataSize;
		uint16_t formatType, channels, bytesPerSample, bitsPerSample;

		//次の4バイトを読み取る.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//chunkSize

		chunkSize = Get(tmpData.data(), 0, 4);

		//次のバイトを読み取る.
		tmpData.resize(2);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//formatType
		formatType = Get(tmpData.data(), 0, 2);

		//次のバイトを読み取る.
		tmpData.resize(2);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//channels
		channels = Get(tmpData.data(), 0, 2);
		//次のバイトを読み取る.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//smplerate
		sampleRate = Get(tmpData.data(), 0, 4);
		//次のバイトを読み取る.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//avgBytesPerSec
		avgBytesPerSec = Get(tmpData.data(), 0, 4);
		//次のバイトを読み取る.
		tmpData.resize(2);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//bytesPerSample
		bytesPerSample = Get(tmpData.data(), 0, 2);
		//次のバイトを読み取る.
		tmpData.resize(2);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//bitPerSample
		bitsPerSample = Get(tmpData.data(), 0, 2);

		for (int i = 0; i < 256; ++i) {
			//次のバイトを読み取る.
			tmpData.resize(1);
			ReadBuffer(tmpData, buf, offsetBuffer);
			if (tmpData[0] == 'd') {
				//次のバイトを読み取る.
				tmpData.resize(1);
				ReadBuffer(tmpData, buf, offsetBuffer);
				if (tmpData[0] == 'a') {
					//次のバイトを読み取る.
					tmpData.resize(1);
					ReadBuffer(tmpData, buf, offsetBuffer);
					if (tmpData[0] == 't') {
						//次のバイトを読み取る.
						tmpData.resize(1);
						ReadBuffer(tmpData, buf, offsetBuffer);
						if (tmpData[0] == 'a') {
							break;
						}

					}

				}

			}

			if (i == 255) {
				ErrorMessage("データがありません");
				return false;
			}
		}
		//std::cout << "データを読み取る前のおふせっと" << offsetBuffer << std::endl;
		//次のバイトを読み取る.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//dataSize
		dataSize = Get(tmpData.data(), 0, 4);
		//ここから計算

		//size_t memory = bitsPerSample / 8;
		size_t length = dataSize;// / memory;
		waveData.resize(length);
		ReadBuffer(waveData, buf, offsetBuffer);
		//std::cout << "データを読み取った後のおふせっと" << offsetBuffer << std::endl;

		//ここまででデータの抜き取りは終了.
		buf.resize(0);

		if (channels == 1) {//モノラル
			format = (bitsPerSample == 8) ? e_FORMAT_MONO8 : e_FORMAT_MONO16;
		}
		else if (channels == 2) {//ステレオ
			format = (bitsPerSample == 8) ? e_FORMAT_STEREO8 : e_FORMAT_STEREO16;
		}
		else {
			ErrorMessage("読み込んだファイルのチャンネル数がモノラル又はステレオでない");
			format = e_FORMAT_NULL;
		}

		samplingRate = sampleRate;
		this->dataSize = waveData.size();
		return true;
	}

	void Wave::DiscardData()
	{
		format = e_FORMAT_NULL;
		waveData.clear();
		samplingRate = 0;
		dataSize = 0;
	}


	/*
	* 所持している情報をコンソールに表示する.
	*
	*
	*/
	void Wave::PrintData()const
	{
		//std::cout << "<FileReader::Wave::PrintData()>読み込んだファイル情報を表示します" << std::endl;
		//std::cout << "データのサイズ:" << dataSize << std::endl;
		//std::cout << "サンプリングレート:" << this->samplingRate << std::endl;
		//std::cout << "ビットレートとサンプリングレート:";
		//switch (format)
		//{
		//case e_FORMAT_MONO8:
		//	std::cout << "mono8" << std::endl;

		//	break;
		//case e_FORMAT_MONO16:
		//	std::cout << "mono16" << std::endl;

		//	break;
		//case e_FORMAT_STEREO8:
		//	std::cout << "stereo8" << std::endl;

		//	break;
		//case e_FORMAT_STEREO16:
		//	std::cout << "stereo16" << std::endl;

		//	break;

		//default:
		//	std::cout << "相当なし(失敗)" << std::endl;
		//	break;
		//}
		//std::cout << "" << std::endl;
	}
	/*
	* ステレオファイルをモノラルに変換する,
	*
	* return true 変換した :false 変換しなかった
	*
	* @note 変換方法はステレオ音源の R と L のデータの平均を
	*		モノラルデータとして作成
	*/
	bool Wave::ComvertToMono()
	{
		if (format == e_FORMAT_STEREO8) {
			union Conversion
			{
				uint8_t binaryData;
				char charData;
			};

			for (size_t i = 0; i < waveData.size() / 2; ++i) {
				size_t offset = i * 2;
				Conversion tmpDataL;
				tmpDataL.binaryData = waveData[offset];
				Conversion tmpDataR;
				tmpDataR.binaryData = waveData[offset];
				Conversion tmpDataMono;
				tmpDataMono.charData = (tmpDataL.charData + tmpDataR.charData) / 2;
				waveData[i] = tmpDataMono.binaryData;
			}
			waveData.resize(waveData.size() / 2);
			dataSize = waveData.size();
			format = e_FORMAT_MONO8;
			return true;
		}
		else if (format == e_FORMAT_STEREO16)
		{
			union Conversion
			{
			public:
				uint16_t binaryData;
				short shortData;
			};
			for (size_t i = 0; i < waveData.size() / 4; ++i) {
				size_t offset = i * 4;
				Conversion tmpDataL;
				tmpDataL.binaryData = waveData[offset] + (waveData[offset + 1] << 8);
				Conversion tmpDataR;
				tmpDataR.binaryData = waveData[offset + 2] + (waveData[offset + 3] << 8);
				Conversion tmpDataMono;
				tmpDataMono.shortData = (tmpDataL.shortData + tmpDataR.shortData) / 2;
				uint8_t dataFirst = static_cast<uint8_t>(tmpDataMono.binaryData);
				uint8_t dataSecond = static_cast<uint8_t>(tmpDataMono.binaryData >> 8);
				waveData[offset / 2] = dataFirst;
				waveData[offset / 2 + 1] = dataSecond;
			}
			waveData.resize(waveData.size() / 2);
			dataSize = waveData.size();
			format = e_FORMAT_MONO16;
			return true;

		}
		else if (format == e_FORMAT_MONO8 || format == e_FORMAT_MONO16) {
			//std::cout << "FileReader::Wave::ComvertToMomo():既にモノラル形式のファイルです" << std::endl;
		}
		else {
			std::cout << "FileReader::Wave::ComvertToMomo():変換に対応していません" << std::endl;
		}
		return false;
	}

}