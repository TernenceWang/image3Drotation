#include <opencv2/stitching.hpp>
#include <opencv.hpp>
//#include <opencv2\highgui\highgui.hpp>
//#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2\videoio\videoio.hpp>
#include <iostream>
#include <time.h>
#include<io.h>
#include <sstream>
using namespace cv;
using namespace std;






void getFiles( string path, vector<string>& files )  
{  
	//文件句柄  
	long   hFile   =   0;  
	//文件信息  
	struct _finddata_t fileinfo;  
	string p;  
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)  
	{  
		do  
		{  
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if((fileinfo.attrib &  _A_SUBDIR))  
			{  
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
					getFiles( p.assign(path).append("\\").append(fileinfo.name), files );  
			}  
			else  
			{  
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
			}  
		}while(_findnext(hFile, &fileinfo)  == 0);  
		_findclose(hFile);  
	}  
}  

int main()
{
	
	

    std::cout<<"please enter video pathname:"<<endl;
	string videopath;
	//cin>>videopath;
	
	vector<string> files;  

	////获取该路径下的所有文件  
	/*getFiles(videopath, files );  
	int size = files.size();  
	for (int i = 0;i < size;i++)  
	{  
		cout<<files[i].c_str()<<endl;  
	}  
	*/

//	system("del /s /q img\\");
	int count_tmp = 0;
	files.push_back("D:\\Record\\1.avi");
	for (int i=0;i<1;i++)
	{
	
	
		VideoCapture inputVideo(files[i]);
		if(!inputVideo.isOpened())
		{

			cout<<"-- Open error !"<<endl;
			return -1;
		}
		Size s=Size((int)inputVideo.get(CAP_PROP_FRAME_WIDTH),(int)inputVideo.get(CAP_PROP_FRAME_HEIGHT));
		int fps=inputVideo.get(CAP_PROP_FPS);
		double count=inputVideo.get(CAP_PROP_FRAME_COUNT);
		cout<<"fps   "<<fps<<endl;
		cout<<"count   "<<count<<endl;
		cout<<"Size   "<<s.width<<"    "<<s.height<<endl;


		while(true)  
		{  
			Mat frame;  
			inputVideo>>frame;  
			if (frame.empty())
			{
				break; 
			}


			if (count_tmp % (fps*3) == 0)  
			{  
				stringstream sout;
				sout<<count_tmp;
				imwrite("img\\"+sout.str()+".png",frame);
			}        
			++count_tmp;  

		}
	}
	cout<<"OK"<<endl;

	clock_t t_start, t_end;

	bool try_use_gpu = false;
	vector<Mat> Imgs;
	string result_name = "stitch333.png";
	vector<string> files1;  
	getFiles("img\\", files1 );  
	for (int i = 0;i <files1.size();i++)  
	{  
		Mat img = imread(files1[i],1);
		Imgs.push_back(img);
	}  
	Mat pano;
	t_start = clock();
	Stitcher stitcher = Stitcher::createDefault(try_use_gpu);

	stitcher.setRegistrationResol(0.6);//为了加速，我选0.1,默认是0.6,最大值1最慢，此方法用于特征点检测阶段，如果找不到特征点，调高吧
	stitcher.setWaveCorrection(false);//默认是true，为加速选false，表示跳过WaveCorrection步骤
	//stitcher.setRegistrationResol(0.1);//为了加速，我选0.1,默认是0.6,最大值1最慢，此方法用于特征点检测阶段，如果找不到特征点，调高吧
	//stitcher.setWaveCorrection(true);//默认是true，为加速选false，表示跳过WaveCorrection步骤
	

	Stitcher::Status status = stitcher.stitch(Imgs, pano);
	t_end = clock();
	cout<<"OK"<<endl;
	cout<<"The running time of algorithm is : "<<double(t_end-t_start)/CLOCKS_PER_SEC <<endl;

	if (status != Stitcher::OK)
	{
		cout << "Cannot stitch images,  error code = " << int(status) << endl;
		return -1;
	}
	
	imwrite(result_name, pano);
	imshow("sss",pano);
	waitKey(0);
	return 0;
}