#pragma once

#include "Drawer.h"
#include "Model.h"
namespace Modelling {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
		  pictureBox1->Image = gcnew Bitmap(pictureBox1->Size.Width, pictureBox1->Size.Height);
		  m_drawer = gcnew Drawer(pictureBox1->Size.Width, pictureBox1->Size.Height, -2, -2, 2, 2);
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
  private: System::Windows::Forms::PictureBox^  pictureBox1;
           Drawer^ m_drawer;
  private: System::Windows::Forms::TextBox^  textBox1;
  private: System::Windows::Forms::TextBox^  textBox2;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Button^  button1;
  private: System::Windows::Forms::TextBox^  textBox3;
  private: System::Windows::Forms::Label^  label3;
  protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

    void ClearPicture()
      {
      Graphics^ e = Graphics::FromImage(pictureBox1->Image);
      e->Clear(Color::White);
      }

    //-------------------------------------

    void DrawAxes()
      {
      Graphics^ e = Graphics::FromImage(pictureBox1->Image);
      Pen^ pen = gcnew Pen(Color::Black);
      
      pen->EndCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;
      
      //Axes
      m_drawer->DrawLine(pen, e, m_drawer->GetMinX(), 0, m_drawer->GetMaxX(), 0);
      m_drawer->DrawLine(pen, e, 0, m_drawer->GetMinY(), 0, m_drawer->GetMaxY());
      
      pen->EndCap = System::Drawing::Drawing2D::LineCap::Flat;
      
      //XScale
      m_drawer->DrawLine(pen, e, 0.5, -0.01, 0.5, 0.01);
      m_drawer->DrawText(e, L"0.5", 0.5, 0);

      //YScale
      m_drawer->DrawLine(pen, e, -0.01, 0.5, 0.01, 0.5);
      m_drawer->DrawText(e, L"0.5", 0, 0.5);
      }

    //-------------------------------------

    void DrawContours()
      {
      Graphics^ e = Graphics::FromImage(pictureBox1->Image);
      Pen^ pen = gcnew Pen(Color::Red);
      pen->Width = 2;
      pen->EndCap = System::Drawing::Drawing2D::LineCap::RoundAnchor;
      pen->StartCap = System::Drawing::Drawing2D::LineCap::RoundAnchor;
      const Contours& cont = model.GetContours();
      for(std::size_t i = 0; i < cont.size(); ++i)
        for(std::size_t j = 0; j < cont[i].size() - 1; ++j)
          m_drawer->DrawLine(pen, e, cont[i][j].X(), cont[i][j].Y(), cont[i][j + 1].X(), cont[i][j + 1].Y());
      }

    //-------------------------------------

    void ReshapePictureBox1()
      {
      this->pictureBox1->Location = System::Drawing::Point(0, 0);
      this->pictureBox1->Size = System::Drawing::Size(static_cast<int>(this->ClientRectangle.Width * 0.8), this->ClientRectangle.Height);
      pictureBox1->Image = gcnew Bitmap(pictureBox1->Size.Width, pictureBox1->Size.Height);
      }

    //-------------------------------------

    void DrawPoints()
      {
      Graphics^ e = Graphics::FromImage(pictureBox1->Image);
      Pen^ pen = gcnew Pen(Color::Green);
      pen->EndCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;
      int N = 20, M = model.GetSize();
      double maxx = m_drawer->GetMaxX(), maxy = m_drawer->GetMaxY(), 
        minx = m_drawer->GetMinX(), miny = m_drawer->GetMinY();
      for(int i = 1; i < N - 1; ++i)
        for(int j = 1; j < N - 1; ++j)
          {
          Vector2D summ = model.GetVelocity();
          double n_x = minx + i * (maxx - minx) / (N - 1), 
            n_y = miny + j * (maxy - miny) / (N - 1);
          for(int m = 0; m < M; ++m)
            summ = summ + Model::curr_gamma[m] * model.V(m, Vector2D (n_x, n_y));
          m_drawer->DrawLine(pen, e, n_x, n_y, n_x + summ.X(), n_y + summ.Y());
          m_drawer->DrawPoint(e, n_x, n_y);
          }
      }

    //-------------------------------------

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
    this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
    this->textBox1 = (gcnew System::Windows::Forms::TextBox());
    this->textBox2 = (gcnew System::Windows::Forms::TextBox());
    this->label1 = (gcnew System::Windows::Forms::Label());
    this->label2 = (gcnew System::Windows::Forms::Label());
    this->button1 = (gcnew System::Windows::Forms::Button());
    this->textBox3 = (gcnew System::Windows::Forms::TextBox());
    this->label3 = (gcnew System::Windows::Forms::Label());
    (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
    this->SuspendLayout();
    // 
    // pictureBox1
    // 
    this->pictureBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
      | System::Windows::Forms::AnchorStyles::Left));
    this->pictureBox1->BackColor = System::Drawing::Color::White;
    this->pictureBox1->Location = System::Drawing::Point(0, 0);
    this->pictureBox1->Name = L"pictureBox1";
    this->pictureBox1->Size = System::Drawing::Size(597, 664);
    this->pictureBox1->TabIndex = 0;
    this->pictureBox1->TabStop = false;
    // 
    // textBox1
    // 
    this->textBox1->Location = System::Drawing::Point(647, 39);
    this->textBox1->Name = L"textBox1";
    this->textBox1->Size = System::Drawing::Size(86, 20);
    this->textBox1->TabIndex = 1;
    // 
    // textBox2
    // 
    this->textBox2->Location = System::Drawing::Point(648, 65);
    this->textBox2->Name = L"textBox2";
    this->textBox2->Size = System::Drawing::Size(86, 20);
    this->textBox2->TabIndex = 2;
    // 
    // label1
    // 
    this->label1->Location = System::Drawing::Point(600, 39);
    this->label1->Name = L"label1";
    this->label1->Size = System::Drawing::Size(46, 20);
    this->label1->TabIndex = 3;
    this->label1->Text = L"Velocity";
    // 
    // label2
    // 
    this->label2->Location = System::Drawing::Point(600, 65);
    this->label2->Name = L"label2";
    this->label2->Size = System::Drawing::Size(46, 20);
    this->label2->TabIndex = 4;
    this->label2->Text = L"Angle";
    // 
    // button1
    // 
    this->button1->Location = System::Drawing::Point(648, 117);
    this->button1->Name = L"button1";
    this->button1->Size = System::Drawing::Size(86, 21);
    this->button1->TabIndex = 5;
    this->button1->Text = L"Solve";
    this->button1->UseVisualStyleBackColor = true;
    this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
    // 
    // textBox3
    // 
    this->textBox3->Location = System::Drawing::Point(648, 91);
    this->textBox3->Name = L"textBox3";
    this->textBox3->Size = System::Drawing::Size(86, 20);
    this->textBox3->TabIndex = 6;
    // 
    // label3
    // 
    this->label3->Location = System::Drawing::Point(603, 91);
    this->label3->Name = L"label3";
    this->label3->Size = System::Drawing::Size(46, 20);
    this->label3->TabIndex = 7;
    this->label3->Text = L"Gamma";
    // 
    // Form1
    // 
    this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
    this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
    this->ClientSize = System::Drawing::Size(754, 662);
    this->Controls->Add(this->label3);
    this->Controls->Add(this->textBox3);
    this->Controls->Add(this->button1);
    this->Controls->Add(this->label2);
    this->Controls->Add(this->label1);
    this->Controls->Add(this->textBox2);
    this->Controls->Add(this->textBox1);
    this->Controls->Add(this->pictureBox1);
    this->MinimumSize = System::Drawing::Size(100, 100);
    this->Name = L"Form1";
    this->Text = L"Form1";
    this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::Form1_Paint);
    this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
    (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
    this->ResumeLayout(false);
    this->PerformLayout();

      }
#pragma endregion

    private: System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e) 
               {
               ReshapePictureBox1();
               label1->Location = System::Drawing::Point(pictureBox1->Width, label1->Location.Y);
               label2->Location = System::Drawing::Point(pictureBox1->Width, label2->Location.Y);
               label3->Location = System::Drawing::Point(pictureBox1->Width, label3->Location.Y);
             
               textBox1->Location = System::Drawing::Point(pictureBox1->Width + label1->Width, label1->Location.Y);
               textBox2->Location = System::Drawing::Point(pictureBox1->Width + label2->Width, label2->Location.Y);
               textBox3->Location = System::Drawing::Point(pictureBox1->Width + label3->Width, label3->Location.Y);

               button1->Location = System::Drawing::Point(textBox1->Location.X, button1->Location.Y);
               m_drawer->SetTargetResolution(pictureBox1->Size.Width, pictureBox1->Size.Height);

               ClearPicture();
               if(!Model::curr_gamma.empty())
                 DrawPoints();
               Invalidate(true);
               }

             //-------------------------------------

    private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
               {
               double velocity = System::Double::Parse(textBox1->Text), 
                 angle = 2 * Math::PI * System::Double::Parse(textBox2->Text) / 360,
                 gamma = System::Double::Parse(textBox3->Text);
               model.CalcGamma(Model::curr_gamma, Vector2D(velocity * Math::Cos(angle), velocity * Math::Sin(angle)), gamma);

               ClearPicture();
               if(!Model::curr_gamma.empty())
                 DrawPoints();
               Invalidate(true);
               }

             //-------------------------------------

    private: System::Void Form1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
               {
               DrawAxes();
               DrawContours();
               }
};
}

