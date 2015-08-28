#pragma once

#include "opencv\highgui.h"

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "../ResCan.h"
#include <Windows.h>
#include <process.h> 

namespace ResCanCppGUI {

#pragma unmanaged
	ref class Form1; //! forward declaration 

	/*! Declare an unmanaged function type that takes two arguments
	 *	to be used by processing thread for showing result
	 *	Note the use of __stdcall for compatibility with managed code
	*/
	typedef int(__stdcall *FRAME_PROCESSED_CALLBACK_FUNCTION)(cv::Mat, cv::string); 
	

	/*!
	 *	struct used for passing parameter and sharing data 
	 *	between main thread and processing thread
	 *	Note: shared variables must declared as volatile.
	 */
	typedef volatile struct PROCCESSING_THREAD_PARAMETERS{
		FRAME_PROCESSED_CALLBACK_FUNCTION callBackFun;	/**< Callback fuction used in processing thread to show result */
		volatile bool shouldExit;
	};

	//! processing thread main function
	unsigned int __stdcall thread_func(void* param){

		cv::Mat frame, org;	//! matrices for working and captured images
		ResCan resCan;	//! processing class object

		//! cast parameters (void) pointer to parameters struct
		PROCCESSING_THREAD_PARAMETERS *args = (PROCCESSING_THREAD_PARAMETERS*)param; 

		cv::string valuestring;	//! result value string

		try
		{
			cv::VideoCapture camera; 
			camera.open(0); //! Open first camera for video capturing
			if (!camera.isOpened()) {
				std::cerr << ("Camera Error");
				return -1;
			}
			else {
				std::cout << ("Camera OK?");
			}
			while ( camera.isOpened()) {
				
				//! capture frame from camera
				camera.retrieve(org);

				//! flip original fram and put it in working matrix
				cv::flip(org, frame, -1);

				try
				{
					//! process frame and get result string
					valuestring = resCan.doMagic(frame);
				}
				catch (...)
				{
					continue; //! just ignore this frame and continue
				}


				//! check if application closing.
				if (args->shouldExit)
					return (0);

				//! show result callback
					args->callBackFun(frame, valuestring);


				Sleep(10); //! may enhance application responsiveness


			}
		return 0;
		}
		catch (const cv::Exception& ex)
		{
			std::cerr << "Error: " << ex.what() << std::endl;

		}
		catch (...){
			std::cerr << "Error: Unhandled Exception";
		}
		return -1;
	}



#pragma managed
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::Runtime::InteropServices;
	using namespace System::Threading;

	/// <summary>
	/// Application's main form class.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}

			gchframeProcessed.Free();
			delete processingThreadParameters;
		}

	private: 

		//! Shows processing or captured stream
		System::Windows::Forms::PictureBox^  pictureBox;
		
		//! Shows processing result string
		System::Windows::Forms::TextBox^  resultTextBox;
		
		System::Windows::Forms::Label^  label1;
		System::Windows::Forms::Button^  pauseButton; //! for pausing processing thread
		System::Windows::Forms::Button^  resumeButton;//! for resuming processing thread
		
		//! for swiches between captured and processing streams
		System::Windows::Forms::Button^  exitButton; //! exit the application

				
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->resultTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->pauseButton = (gcnew System::Windows::Forms::Button());
			this->resumeButton = (gcnew System::Windows::Forms::Button());
			this->exitButton = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox))->BeginInit();
			this->SuspendLayout();
			this->pictureBox->Location = System::Drawing::Point(13, 13);
			this->pictureBox->Name = L"pictureBox";
			this->pictureBox->Size = System::Drawing::Size(800, 600);
			this->pictureBox->TabIndex = 2;
			this->pictureBox->TabStop = false;
			this->resultTextBox->Enabled = false;
			this->resultTextBox->Location = System::Drawing::Point(900, 78);
			this->resultTextBox->Name = L"resultTextBox";
			this->resultTextBox->Size = System::Drawing::Size(100, 20);
			this->resultTextBox->TabIndex = 3;
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(819, 81);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(75, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Resistor Value";
			this->pauseButton->Location = System::Drawing::Point(822, 124);
			this->pauseButton->Name = L"pauseButton";
			this->pauseButton->Size = System::Drawing::Size(75, 23);
			this->pauseButton->TabIndex = 5;
			this->pauseButton->Text = L"Pause";
			this->pauseButton->UseVisualStyleBackColor = true;
			this->pauseButton->Click += gcnew System::EventHandler(this, &Form1::pauseButton_Click);
			this->resumeButton->Location = System::Drawing::Point(925, 124);
			this->resumeButton->Name = L"resumeButton";
			this->resumeButton->Size = System::Drawing::Size(75, 23);
			this->resumeButton->TabIndex = 6;
			this->resumeButton->Text = L"Resume";
			this->resumeButton->UseVisualStyleBackColor = true;
			this->resumeButton->Click += gcnew System::EventHandler(this, &Form1::resumeButton_Click);
			this->exitButton->Location = System::Drawing::Point(820, 588);
			this->exitButton->Name = L"exitButton";
			this->exitButton->Size = System::Drawing::Size(184, 23);
			this->exitButton->TabIndex = 9;
			this->exitButton->Text = L"Exit";
			this->exitButton->UseVisualStyleBackColor = true;
			this->exitButton->Click += gcnew System::EventHandler(this, &Form1::exitButton_Click);
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1016, 623);
			this->Controls->Add(this->exitButton);
			this->Controls->Add(this->resumeButton);
			this->Controls->Add(this->pauseButton);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->resultTextBox);
			this->Controls->Add(this->pictureBox);
			this->Name = L"Form1";
			this->ShowIcon = false;
			this->Text = L"ResCan";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: 
		//! delegates for accessing UI controls from other thread (safely) 
		delegate void SetImageDelegate(System::Drawing::Bitmap^);
		delegate void SetResultTextDelegate(System::String^);

		//! delegate for processing callback function
		delegate void FrameProcessedDelegate(cv::Mat, cv::string);

		//! processing callback function
		void FrameProcessed(cv::Mat frame, cv::string strResult){
			IplImage img = frame; // no data copy

			//! create bitmap to be shown
			System::Drawing::Bitmap^ bmp = gcnew  
				System::Drawing::Bitmap(img.width, img.height, img.widthStep,
				System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) img.imageData);

			//! if resistor value detected update text box.
			if (!strResult.empty()){
				//! copy result string from native CV::string to managed String
				System::String^ resultText = gcnew System::String(strResult.c_str());

				//! update result text box in UI (main) thread "asynchronously"
				SetResultTextDelegate^ setResultTextDelegate = gcnew SetResultTextDelegate(this, &Form1::setResultText);
				BeginInvoke(setResultTextDelegate, gcnew array<Object^> { resultText });

			}

			//! update picture box in UI (main) thread "asynchronously" 
			SetImageDelegate^ setImageDelegate = gcnew SetImageDelegate(this, &Form1::setImage);
			BeginInvoke(setImageDelegate, gcnew array<Object^> { bmp });
		}

		//! To avoid (unsafe) access of UI control from another thread
		void setImage(System::Drawing::Bitmap^ bitmap){
			pictureBox->Image = bitmap;
			pictureBox->Refresh();
		}

		//! To avoid (unsafe) access of UI control from another thread
		void setResultText(System::String^ result){
			//if (!result->Empty)  caller checked
				resultTextBox->Text = result;
		}


		FrameProcessedDelegate^ frameProcessedDelegate; //! delegate object for processing callback function
		IntPtr pFrameProcessed; //! managed pointer to callback function
		FRAME_PROCESSED_CALLBACK_FUNCTION frameProcessedCallBack; //! native pointer to callback function
		PROCCESSING_THREAD_PARAMETERS* processingThreadParameters; //! pointer to processing function parameter struct

		GCHandle gchframeProcessed; //! Garpage collector handl, used to prevent GC 
		HANDLE hProccessingThread; //! windows handle to processing thread

		System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {

			/*! 
			 *	Create callback fuction pointer 
			 *	to be used in processing thread for reporting result
			 */
			frameProcessedDelegate = gcnew FrameProcessedDelegate(this, &Form1::FrameProcessed);
			pFrameProcessed = Marshal::GetFunctionPointerForDelegate(frameProcessedDelegate);
			gchframeProcessed = GCHandle::Alloc(pFrameProcessed);
			frameProcessedCallBack = static_cast<FRAME_PROCESSED_CALLBACK_FUNCTION>(pFrameProcessed.ToPointer());

			//! Create and fill processing thread parameters struct  
			processingThreadParameters = new PROCCESSING_THREAD_PARAMETERS();
			processingThreadParameters->callBackFun = frameProcessedCallBack;
			processingThreadParameters->shouldExit = false;

			//! Create and start processing thread
			hProccessingThread = (HANDLE)_beginthreadex(0, 0, thread_func, (void*)processingThreadParameters, 0, 0);
		}
		
		//! pause button clicked
		private: System::Void pauseButton_Click(System::Object^  sender, System::EventArgs^  e) {
			//! suspend (pause) processing thread
			SuspendThread(hProccessingThread);
			this->pauseButton->Enabled = false;
			this->resumeButton->Enabled = true;
		}

		//! resume button clicked
		private: System::Void resumeButton_Click(System::Object^  sender, System::EventArgs^  e) {
			//! resume processing thread
			 ResumeThread(hProccessingThread);
			this->pauseButton->Enabled = true;
			this->resumeButton->Enabled = false;
		 }

private: 

private:
		 void stopProcessingThread(){
			 //! tell thread to exit through shared variable
			 processingThreadParameters->shouldExit = true;

			 //! Wait for thread completion
			 if (WaitForSingleObject(hProccessingThread, 1000) != WAIT_OBJECT_0){
				 TerminateThread(hProccessingThread, 0); //! force terminate processing thread
			 }
			 CloseHandle(hProccessingThread);	//! release windows resource			
			 hProccessingThread = INVALID_HANDLE_VALUE;
		 }
private: 
		System::Void exitButton_Click(System::Object^  sender, System::EventArgs^  e) {
			stopProcessingThread();
			Application::Exit();
		}
private:
		System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				//! stop processing thread before closing
				 stopProcessingThread();
		}
	};

}

 