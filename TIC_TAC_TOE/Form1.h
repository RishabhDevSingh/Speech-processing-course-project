#pragma once
#include "HMM.h"
#include "Form2.h"
#include "Form3.h"

namespace TIC_TAC_TOE {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	namespace Win32 {
		[DllImport("kernel32.dll", CallingConvention = CallingConvention::StdCall)]
		int AllocConsole();
		[DllImport("kernel32.dll", CallingConvention = CallingConvention::StdCall)]
		int FreeConsole();
		[DllImport("kernel32.dll", CallingConvention = CallingConvention::StdCall)]
		int HideConsole();
	}
	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
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
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;










	protected: 

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::SystemColors::InactiveCaption;
			this->label1->Location = System::Drawing::Point(282, 18);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(73, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"TIC TAC TOE";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::SystemColors::Highlight;
			this->label2->Location = System::Drawing::Point(282, 309);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(52, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"WINNER";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->label3->Location = System::Drawing::Point(282, 352);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(38, 13);
			this->label3->TabIndex = 2;
			this->label3->Text = L"NONE";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->BackColor = System::Drawing::SystemColors::MenuBar;
			this->label5->Location = System::Drawing::Point(-1, 51);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(155, 13);
			this->label5->TabIndex = 4;
			this->label5->Text = L"select one of the below buttons";
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::SystemColors::HotTrack;
			this->button1->Location = System::Drawing::Point(256, 110);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(99, 38);
			this->button1->TabIndex = 5;
			this->button1->Text = L"INSTRUCTION";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::SystemColors::HotTrack;
			this->button2->Location = System::Drawing::Point(12, 194);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 47);
			this->button2->TabIndex = 6;
			this->button2->Text = L"TRAINING";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::SystemColors::HotTrack;
			this->button3->Location = System::Drawing::Point(150, 197);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 46);
			this->button3->TabIndex = 7;
			this->button3->Text = L"PLAY";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::SystemColors::HotTrack;
			this->button4->Location = System::Drawing::Point(285, 194);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(111, 53);
			this->button4->TabIndex = 8;
			this->button4->Text = L"TEST THE DIGITS";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// button5
			// 
			this->button5->BackColor = System::Drawing::SystemColors::HotTrack;
			this->button5->Location = System::Drawing::Point(495, 209);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 38);
			this->button5->TabIndex = 9;
			this->button5->Text = L"ABOUT";
			this->button5->UseVisualStyleBackColor = false;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->ClientSize = System::Drawing::Size(659, 458);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				  Form^ f2=gcnew Form2;
			 f2->Show();
			 }
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
			  Form^ f3=gcnew Form3;
			 f3->Show();
		 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			  int rc=Win32::AllocConsole();

				 int w=start();
				 //int rp=Win32::FreeConsole();
				 end_print();
				 if(w==1)
					 label3->Text="player1";
				 else if(w==2)
					 label3->Text="player2";
				 else
					  label3->Text="Draw";
			
		 }
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
			 int rc=Win32::AllocConsole();
			int z= speack_digit();
			 end_print();
			// int rp=Win32::FreeConsole();
			  // int rp=Win32::HideConsole();
			 

		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			  int rc=Win32::AllocConsole();
				 training();
				  end_print();
				// int rp=Win32::FreeConsole();
			
		 }
private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

