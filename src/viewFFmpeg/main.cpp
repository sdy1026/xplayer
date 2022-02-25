#include <iostream>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}


#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")

int main(int argc, int* arg[])
{
	//��ʾffmpeg��������
	//std::cout << avcodec_configuration() << std::endl;
	const char* path = "E:/git_path/XPlayer/src/viewFFmpeg/test.mp4";

	//���װ������
	AVFormatContext* ic = NULL;

	//��������
	AVDictionary* opt = NULL;
	//����rtsp��tcpЭ���
	av_dict_set(&opt, "rtsp_transport", "tcp", 0);
	//������ʱʱ��
	av_dict_set(&opt, "max_delay", "500", 0);
	int ret = avformat_open_input(
		&ic,
		path,
		0, //�Զ�ѡ����װ��
		&opt);

	if (ret != 0)
	{
		char buf[1024];
		av_strerror(ret, buf, sizeof buf -1);
		printf("open path:%s, failed:%s", path, buf);
		getchar();
		return -1;
	}
	printf("open success!\n");
	//��ȡ����Ϣ
	ret = avformat_find_stream_info(ic, 0);
	//��ʱ�� ����
	int totalMS = ic->duration / (AV_TIME_BASE / 100);
	std::cout << "totalMS:" << totalMS << std::endl;
	//��ӡ��ϸ��Ϣ
	av_dump_format(ic,0,NULL,0);
	//��ȡ����Ƶ����Ϣ
	//������ȡ
	int videoIndex = -1;
	int audioIndex = -1;
	for (int i = 0; i < ic->nb_streams; i++)
	{
		AVStream* as = ic->streams[i];
		if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoIndex = i;
			std::cout << "**************��Ƶ��Ϣ**************" << std::endl;
			std::cout << "width: " << as->codecpar->width << std::endl;
			std::cout << "height: " << as->codecpar->height << std::endl;
			std::cout << "fps: " << (as->avg_frame_rate.num / as->avg_frame_rate.den) << std::endl;
		}

		else if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioIndex = -1;
			std::cout << "**************��Ƶ��Ϣ**************" << std::endl;
			std::cout << "sample_rate: " << as->codecpar->sample_rate << std::endl;
			std::cout << "format: " << as->codecpar->format << std::endl;
			std::cout << "channels: " << as->codecpar->channels << std::endl;
			std::cout << "codec_id: " << as->codecpar->codec_id << std::endl;
			//һ֡���ݣ��� һ������������ ��ͨ��
			std::cout << "frame_size: " << as->codecpar->frame_size;
			//1024 * 2 * 2 =  4096 �ֽ� fps  = sample_rate / frame_size =  43.06
		}

	}

	videoIndex = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	audioIndex = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);


	if (ic)
	{
		avformat_close_input(&ic);
	}
	
	getchar();
	return 0;
}