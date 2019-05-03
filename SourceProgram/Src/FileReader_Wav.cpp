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
	* �o�C�g�񂩂琔�l�𕜌�����.
	*
	* @param p      �o�C�g��ւ̃|�C���^.
	* @param offset ���l�̃I�t�Z�b�g.
	* @param size   ���l�̃o�C�g��(1�`4).
	*
	* @return �����������l.
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
	* �o�b�t�@�[����f�[�^��ǂݎ��.
	*
	* @param data	:�󂯎��z��(vector�^)
	* @param buf	:�ǂݎ��o�b�t�@
	* @param offset	:�o�b�t�@�̃I�t�Z�b�g.
	*
	* @note <<�ǂݎ���������I�t�Z�b�g�̐��͑��₷�g�p�ɂ��Ă���
	* @note <<���ӎ���,��ɓǂݎ��o�b�t�@�̃T�C�Y��resize���Ă���.
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
	* std::vector<uint8_t> �� std::string �̔�r.
	*
	* @param data ��r�������o�C�g�f�[�^.
	* @param str ��r������������.
	*
	* @retren  ture << ���S�Ɉ�v : false << �s��v �܂���string�̒���������Ȃ�;
	*
	* @note str�̒���������Ȃ��ꍇ�͖ⓚ���p��false��Ԃ��̂Œ���.
	*/
	bool CheckString(const std::vector<uint8_t>& data, const std::string& str)
	{
		if (data.size() > str.size()) {
			ErrorMessage("CheckString�֐��ŕ���������܂���");
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

		//�t�@�C���̃T�C�Y���擾����.
		struct stat st;
		if (stat(filename, &st)) {
			ErrorMessage("stat �Ńt�@�C���T�C�Y�̎擾�Ɏ��s");
			return false;
		}
		//�t�@�C�����J��.
		FILE *fp;
		fp = fopen(filename, "rb");
		if (!fp) {
			ErrorMessage("�t�@�C���̓ǂݍ��݂Ɏ��s");
			return false;
		}
		//�t�@�C���̃f�[�^���ꎞ�ێ�����.
		std::vector<uint8_t> buf;
		//�󂯎��o�b�t�@�̃T�C�Y���t�@�C���ɍ��킹��.
		buf.resize(st.st_size);
		//std::cout << buf.size() << std::endl;
		//�t�@�C������o�C�i���f�[�^���o�C�g�P�ʂŎ󂯎��󂯎�����T�C�Y���L�^����
		const size_t readSize = fread(buf.data(), 1, st.st_size, fp);
		//�t�@�C�������.
		fclose(fp);
		//�����t�@�C���̃T�C�Y���ŏ��Ɏ擾�����t�@�C���ƈ�����ꍇ�I������.
		if (readSize != st.st_size) {
			ErrorMessage("�t�@�C���̃T�C�Y�������܂���");
			return false;
		}

		//�I�t�Z�b�gindex��p��.
		size_t offsetBuffer = 0;
		//�o�b�t�@����ꎞ�I�Ƀf�[�^��ǂݎ��Tempdata��p�ӂ���.
		std::vector<uint8_t> tmpData;

		//buf����4�o�C�g�����󂯎��.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//WAVE���ǂ������`�F�b�N����.
		//if(tmpData[0] != 'R' || tmpData[1] != 'I' || tmpData[2] != 'F' || tmpData[3] != 'F' )
		//���������ł������Ȃ������̂Ŋ֐��ɂ���.
		//if(CheckString(tmpData))
		if (!CheckString(tmpData, "RIFF")) {
			ErrorMessage("not RIFF");
			return false;
		}
		//����4�o�C�g��ǂݎ��.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//�`�����N"RIFF"�̎���4�o�C�g�͑��t�@�C�����Ȃ̂œǂݔ�΂�

		//����4�o�C�g��ǂݎ��.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//"WAVE"�łȂ���ΏI��.
		if (!CheckString(tmpData, "WAVE")) {
			ErrorMessage("not WAVE");
		}

		//����4�o�C�g��ǂݎ��.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//"fmt "�łȂ���ΏI��.
		if (!CheckString(tmpData, "fmt ")) {
			ErrorMessage("not fmt ");
		}
		uint32_t chunkSize, sampleRate, avgBytesPerSec, dataSize;
		uint16_t formatType, channels, bytesPerSample, bitsPerSample;

		//����4�o�C�g��ǂݎ��.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//chunkSize

		chunkSize = Get(tmpData.data(), 0, 4);

		//���̃o�C�g��ǂݎ��.
		tmpData.resize(2);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//formatType
		formatType = Get(tmpData.data(), 0, 2);

		//���̃o�C�g��ǂݎ��.
		tmpData.resize(2);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//channels
		channels = Get(tmpData.data(), 0, 2);
		//���̃o�C�g��ǂݎ��.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//smplerate
		sampleRate = Get(tmpData.data(), 0, 4);
		//���̃o�C�g��ǂݎ��.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//avgBytesPerSec
		avgBytesPerSec = Get(tmpData.data(), 0, 4);
		//���̃o�C�g��ǂݎ��.
		tmpData.resize(2);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//bytesPerSample
		bytesPerSample = Get(tmpData.data(), 0, 2);
		//���̃o�C�g��ǂݎ��.
		tmpData.resize(2);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//bitPerSample
		bitsPerSample = Get(tmpData.data(), 0, 2);

		for (int i = 0; i < 256; ++i) {
			//���̃o�C�g��ǂݎ��.
			tmpData.resize(1);
			ReadBuffer(tmpData, buf, offsetBuffer);
			if (tmpData[0] == 'd') {
				//���̃o�C�g��ǂݎ��.
				tmpData.resize(1);
				ReadBuffer(tmpData, buf, offsetBuffer);
				if (tmpData[0] == 'a') {
					//���̃o�C�g��ǂݎ��.
					tmpData.resize(1);
					ReadBuffer(tmpData, buf, offsetBuffer);
					if (tmpData[0] == 't') {
						//���̃o�C�g��ǂݎ��.
						tmpData.resize(1);
						ReadBuffer(tmpData, buf, offsetBuffer);
						if (tmpData[0] == 'a') {
							break;
						}

					}

				}

			}

			if (i == 255) {
				ErrorMessage("�f�[�^������܂���");
				return false;
			}
		}
		//std::cout << "�f�[�^��ǂݎ��O�̂��ӂ�����" << offsetBuffer << std::endl;
		//���̃o�C�g��ǂݎ��.
		tmpData.resize(4);
		ReadBuffer(tmpData, buf, offsetBuffer);
		//dataSize
		dataSize = Get(tmpData.data(), 0, 4);
		//��������v�Z

		//size_t memory = bitsPerSample / 8;
		size_t length = dataSize;// / memory;
		waveData.resize(length);
		ReadBuffer(waveData, buf, offsetBuffer);
		//std::cout << "�f�[�^��ǂݎ������̂��ӂ�����" << offsetBuffer << std::endl;

		//�����܂łŃf�[�^�̔������͏I��.
		buf.resize(0);

		if (channels == 1) {//���m����
			format = (bitsPerSample == 8) ? e_FORMAT_MONO8 : e_FORMAT_MONO16;
		}
		else if (channels == 2) {//�X�e���I
			format = (bitsPerSample == 8) ? e_FORMAT_STEREO8 : e_FORMAT_STEREO16;
		}
		else {
			ErrorMessage("�ǂݍ��񂾃t�@�C���̃`�����l���������m�������̓X�e���I�łȂ�");
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
	* �������Ă�������R���\�[���ɕ\������.
	*
	*
	*/
	void Wave::PrintData()const
	{
		//std::cout << "<FileReader::Wave::PrintData()>�ǂݍ��񂾃t�@�C������\�����܂�" << std::endl;
		//std::cout << "�f�[�^�̃T�C�Y:" << dataSize << std::endl;
		//std::cout << "�T���v�����O���[�g:" << this->samplingRate << std::endl;
		//std::cout << "�r�b�g���[�g�ƃT���v�����O���[�g:";
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
		//	std::cout << "�����Ȃ�(���s)" << std::endl;
		//	break;
		//}
		//std::cout << "" << std::endl;
	}
	/*
	* �X�e���I�t�@�C�������m�����ɕϊ�����,
	*
	* return true �ϊ����� :false �ϊ����Ȃ�����
	*
	* @note �ϊ����@�̓X�e���I������ R �� L �̃f�[�^�̕��ς�
	*		���m�����f�[�^�Ƃ��č쐬
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
			//std::cout << "FileReader::Wave::ComvertToMomo():���Ƀ��m�����`���̃t�@�C���ł�" << std::endl;
		}
		else {
			std::cout << "FileReader::Wave::ComvertToMomo():�ϊ��ɑΉ����Ă��܂���" << std::endl;
		}
		return false;
	}

}