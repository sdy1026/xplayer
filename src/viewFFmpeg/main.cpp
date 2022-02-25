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
	//显示ffmpeg编译配置
	//std::cout << avcodec_configuration() << std::endl;
	const char* path = "E:/git_path/XPlayer/src/viewFFmpeg/test.mp4";

	//解封装上下文
	AVFormatContext* ic = NULL;

	//参数设置
	AVDictionary* opt = NULL;
	//设置rtsp以tcp协议打开
	av_dict_set(&opt, "rtsp_transport", "tcp", 0);
	//网络延时时间
	av_dict_set(&opt, "max_delay", "500", 0);
	int ret = avformat_open_input(
		&ic,
		path,
		0, //自动选择解放装器
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
	//获取流信息
	ret = avformat_find_stream_info(ic, 0);
	//总时长 毫秒
	int totalMS = ic->duration / (AV_TIME_BASE / 100);
	std::cout << "totalMS:" << totalMS << std::endl;
	//打印详细信息
	av_dump_format(ic,0,NULL,0);
	//获取音视频流信息
	//遍历获取
	int videoIndex = -1;
	int audioIndex = -1;
	for (int i = 0; i < ic->nb_streams; i++)
	{
		AVStream* as = ic->streams[i];
		if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoIndex = i;
			std::cout << "**************视频信息**************" << std::endl;
			std::cout << "width: " << as->codecpar->width << std::endl;
			std::cout << "height: " << as->codecpar->height << std::endl;
			std::cout << "fps: " << (as->avg_frame_rate.num / as->avg_frame_rate.den) << std::endl;
		}

		else if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioIndex = -1;
			std::cout << "**************音频信息**************" << std::endl;
			std::cout << "sample_rate: " << as->codecpar->sample_rate << std::endl;
			std::cout << "format: " << as->codecpar->format << std::endl;
			std::cout << "channels: " << as->codecpar->channels << std::endl;
			std::cout << "codec_id: " << as->codecpar->codec_id << std::endl;
			//一帧数据？？ 一定量的样本数 单通道
			std::cout << "frame_size: " << as->codecpar->frame_size;
			//1024 * 2 * 2 =  4096 字节 fps  = sample_rate / frame_size =  43.06
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