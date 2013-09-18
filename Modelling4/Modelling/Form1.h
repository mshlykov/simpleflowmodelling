#pragma once

#include "Drawer.h"
#include "Model.h"
#include "PollutionProblem.h"
namespace Modelling {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
  using namespace System::Globalization;

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
		  m_drawer = gcnew Drawer(pictureBox1->Size.Width, pictureBox1->Size.Height, -1.5, -1.5, 1.5, 1.5);
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
           bool to_draw, first_time;

  private: System::Windows::Forms::TextBox^  textBox2;

  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Button^  button1;
  private: System::Windows::Forms::TextBox^  textBox3;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::Timer^  timer1;
  private: System::Windows::Forms::Button^  button2;
  private: System::Windows::Forms::RadioButton^  radioButton1;
  private: System::Windows::Forms::RadioButton^  radioButton2;
  private: System::Windows::Forms::RadioButton^  radioButton3;
  private: System::Windows::Forms::RadioButton^  radioButton4;
  private: System::Windows::Forms::TextBox^  textBox1;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  textBox4;
  private: System::Windows::Forms::TextBox^  textBox5;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::Label^  label5;
  private: System::Windows::Forms::TextBox^  textBox6;
  private: System::Windows::Forms::Label^  label6;
  private: System::ComponentModel::IContainer^  components;
  protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


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
      
      double maxx = m_drawer->GetMaxX(), maxy = m_drawer->GetMaxY(), 
        minx = m_drawer->GetMinX(), miny = m_drawer->GetMinY();

      //Axes
      m_drawer->DrawLine(pen, e, m_drawer->GetMinX(), 0, m_drawer->GetMaxX(), 0);
      m_drawer->DrawLine(pen, e, 0, m_drawer->GetMinY(), 0, m_drawer->GetMaxY());
      
      pen->EndCap = System::Drawing::Drawing2D::LineCap::Flat;
      
      //XScale
      for(double i = minx; i < maxx; i += 0.5)
        m_drawer->DrawLine(pen, e, i, -0.01, i, 0.01);

      m_drawer->DrawText(e, L"0.5", 0.5, 0);

      //YScale
      for(double i = minx; i < maxx; i += 0.5)
        m_drawer->DrawLine(pen, e, -0.01, i, 0.01, i);

      m_drawer->DrawText(e, L"0.5", 0, 0.5);
      }

    //-------------------------------------

    void DrawContours()
      {
      Graphics^ e = Graphics::FromImage(pictureBox1->Image);
      Pen^ pen = gcnew Pen(Color::DarkBlue);
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
      int N = 20;
      std::size_t M = model.GetSize();
      double maxx = m_drawer->GetMaxX(), maxy = m_drawer->GetMaxY(), 
        minx = m_drawer->GetMinX(), miny = m_drawer->GetMinY(), scaler = 1;
      Vector2D summ;
      for(int i = 1; i < N - 1; ++i)
        for(int j = 1; j < N - 1; ++j)
          {
          double n_x = minx + i * (maxx - minx) / (N - 1), 
            n_y = miny + j * (maxy - miny) / (N - 1);
          summ = model.CalcSpeed(Vector2D(n_x, n_y));
          if(i == 1 && j == 1)
            scaler = 0.5 * (maxx - minx) / ((N - 1) * summ.Length());
          m_drawer->DrawLine(pen, e, n_x, n_y, n_x + summ.X() * scaler, n_y + summ.Y() * scaler);
          m_drawer->DrawPoint(e, n_x, n_y, 0xFF000000);
          }
      for(std::size_t i = 0; i < model.GetOffPoints().size(); ++i)
        for(std::size_t j = 0; j < model.GetOffPoints()[i].size(); ++j)
          if(model.GetOffGamma()[i][j] > 0)
            m_drawer->DrawPoint(e, model.GetOffPoints()[i][j].X(), model.GetOffPoints()[i][j].Y(), 0xFF00F0FF);
          else
            m_drawer->DrawPoint(e, model.GetOffPoints()[i][j].X(), model.GetOffPoints()[i][j].Y(), 0xFF000FFF);

      const Contours& cont = model.GetContours();
      int t = 0;
      for(std::size_t i = 0; i < cont.size(); ++i)
        for(std::size_t j = 0; j < cont[i].size(); ++j, ++t)
          if(model.GetCurrGamma()[t] > 0)
            m_drawer->DrawPoint(e, cont[i][j].X(), cont[i][j].Y(), 0xFF00F0FF);
          else
            m_drawer->DrawPoint(e, cont[i][j].X(), cont[i][j].Y(), 0xFF000FFF);
      }

    void CalcColors(std::vector<std::vector<int>>& o_colors_by_phi, const Model& i_model, int i_mode)
      {
      o_colors_by_phi.clear();
      int N = Math::Max(Width / 3, Height / 3) + 1, number_of_colors = 16, red_color = 0xFFFF0000, yellow_color = 0xFFFFFF00;
      double 
        max_phi = i_mode == 0 ? i_model.CalcPhi(Vector2D()) : (i_mode == 1 ? i_model.CalcPsi(Vector2D()) : i_model.CalcCp(Vector2D())), 
        min_phi = i_mode == 0 ? i_model.CalcPhi(Vector2D()) : (i_mode == 1 ? i_model.CalcPsi(Vector2D()) : i_model.CalcCp(Vector2D())) , 
        maxx = m_drawer->GetMaxX(), maxy = m_drawer->GetMaxY(), 
        minx = m_drawer->GetMinX(), miny = m_drawer->GetMinY();
      Vector2D curr_point;
      std::vector<std::vector<double>> psi_matr, phi_matr;
      o_colors_by_phi.resize(N);
      phi_matr.resize(N);
      for(int i = 0; i < N; ++i)
        {
        curr_point.X() = minx + i * (maxx - minx) / (N - 1);

        for(int j = 0; j < N; ++j)
          {
          curr_point.Y() = miny + j * (maxy - miny) / (N - 1);
          phi_matr[i].push_back(i_mode == 0 ? i_model.CalcPhiSec(curr_point) : (i_mode == 1 ? i_model.CalcPsi(curr_point) : i_model.CalcCp(curr_point)));
          
          if(min_phi > phi_matr[i][j])
            min_phi = phi_matr[i][j];

          if(max_phi < phi_matr[i][j])
            max_phi = phi_matr[i][j];
          if(i_mode == 2 && phi_matr[i][j] > 1)
            phi_matr[i][j] = 1;
          }
        }
      if (i_mode == 2 && max_phi > 1)
        max_phi = 1;
      for(int i = 0; i < N; ++i)
        for(int j = 0; j < N; ++j)
          {
          double coef = 2.0 * (phi_matr[i][j] - min_phi) / (max_phi - min_phi);

          int color = 0;

          if(coef >= 1.0)
            {
            coef -= 1.0;
            int col = 255 - static_cast<int>(static_cast<int>(number_of_colors * coef) * 255.0 / number_of_colors);
            color = 0xFF000000 + col + (col << 8) + (255 << 16);
            }
          else
            {
            int col = static_cast<int>(static_cast<int>(number_of_colors * coef) * 255.0 / number_of_colors);
            color = 0xFF000000 + 255 + (col << 8) + (col << 16);
            }

          //int diff = static_cast<int>(0xFF * coef); 
          o_colors_by_phi[i].push_back(color);
          }

      }

    //-------------------------------------

    void FillColors(const std::vector<std::vector<int>>& i_colors)
      {
      Graphics^ e = Graphics::FromImage(pictureBox1->Image);
      e->SmoothingMode = SmoothingMode::HighSpeed;
      double maxx = m_drawer->GetMaxX(), maxy = m_drawer->GetMaxY(), 
        minx = m_drawer->GetMinX(), miny = m_drawer->GetMinY();
      for(std::size_t i = 0; i < i_colors.size(); ++i)
        for(std::size_t j = 0; j < i_colors[i].size(); ++j)
          {
          m_drawer->DrawPoint(e, minx + i * (maxx - minx) / (i_colors[i].size() - 1), miny + j * (maxy - miny) / (i_colors.size() - 1), i_colors[i][j]);
          }
      }

    //-------------------------------------

    void DrawParticles()
      {
        Graphics^ e = Graphics::FromImage(pictureBox1->Image);
        std::vector<SulfurParticle> particles = probl.GetParticles();
        for(std::size_t i = 0; i < particles.size(); ++i)
          m_drawer->DrawPoint(e, particles[i].GetLocation().X(), particles[i].GetLocation().Y(), 0xFFFF0000);
      }
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
    this->components = (gcnew System::ComponentModel::Container());
    this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
    this->textBox2 = (gcnew System::Windows::Forms::TextBox());
    this->label2 = (gcnew System::Windows::Forms::Label());
    this->button1 = (gcnew System::Windows::Forms::Button());
    this->textBox3 = (gcnew System::Windows::Forms::TextBox());
    this->label3 = (gcnew System::Windows::Forms::Label());
    this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
    this->button2 = (gcnew System::Windows::Forms::Button());
    this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
    this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
    this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
    this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
    this->textBox1 = (gcnew System::Windows::Forms::TextBox());
    this->label1 = (gcnew System::Windows::Forms::Label());
    this->textBox4 = (gcnew System::Windows::Forms::TextBox());
    this->textBox5 = (gcnew System::Windows::Forms::TextBox());
    this->label4 = (gcnew System::Windows::Forms::Label());
    this->label5 = (gcnew System::Windows::Forms::Label());
    this->textBox6 = (gcnew System::Windows::Forms::TextBox());
    this->label6 = (gcnew System::Windows::Forms::Label());
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
    // textBox2
    // 
    this->textBox2->Location = System::Drawing::Point(644, 62);
    this->textBox2->Name = L"textBox2";
    this->textBox2->Size = System::Drawing::Size(86, 20);
    this->textBox2->TabIndex = 2;
    // 
    // label2
    // 
    this->label2->Location = System::Drawing::Point(597, 62);
    this->label2->Name = L"label2";
    this->label2->Size = System::Drawing::Size(41, 20);
    this->label2->TabIndex = 4;
    this->label2->Text = L"Angle";
    // 
    // button1
    // 
    this->button1->Location = System::Drawing::Point(644, 114);
    this->button1->Name = L"button1";
    this->button1->Size = System::Drawing::Size(86, 21);
    this->button1->TabIndex = 6;
    this->button1->Text = L"Solve";
    this->button1->UseVisualStyleBackColor = true;
    this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
    // 
    // textBox3
    // 
    this->textBox3->Location = System::Drawing::Point(644, 88);
    this->textBox3->Name = L"textBox3";
    this->textBox3->Size = System::Drawing::Size(86, 20);
    this->textBox3->TabIndex = 5;
    // 
    // label3
    // 
    this->label3->Location = System::Drawing::Point(597, 88);
    this->label3->Name = L"label3";
    this->label3->Size = System::Drawing::Size(46, 20);
    this->label3->TabIndex = 7;
    this->label3->Text = L"Gamma";
    // 
    // timer1
    // 
    this->timer1->Enabled = true;
    this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
    // 
    // button2
    // 
    this->button2->Location = System::Drawing::Point(644, 141);
    this->button2->Name = L"button2";
    this->button2->Size = System::Drawing::Size(86, 21);
    this->button2->TabIndex = 8;
    this->button2->Text = L"Calculate";
    this->button2->UseVisualStyleBackColor = true;
    this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
    // 
    // radioButton1
    // 
    this->radioButton1->AutoSize = true;
    this->radioButton1->Checked = true;
    this->radioButton1->Location = System::Drawing::Point(644, 168);
    this->radioButton1->Name = L"radioButton1";
    this->radioButton1->Size = System::Drawing::Size(87, 17);
    this->radioButton1->TabIndex = 9;
    this->radioButton1->TabStop = true;
    this->radioButton1->Text = L"Calculate Phi";
    this->radioButton1->UseVisualStyleBackColor = true;
    // 
    // radioButton2
    // 
    this->radioButton2->AutoSize = true;
    this->radioButton2->Location = System::Drawing::Point(644, 191);
    this->radioButton2->Name = L"radioButton2";
    this->radioButton2->Size = System::Drawing::Size(86, 17);
    this->radioButton2->TabIndex = 10;
    this->radioButton2->Text = L"Calculate Psi";
    this->radioButton2->UseVisualStyleBackColor = true;
    // 
    // radioButton3
    // 
    this->radioButton3->AutoSize = true;
    this->radioButton3->Location = System::Drawing::Point(644, 214);
    this->radioButton3->Name = L"radioButton3";
    this->radioButton3->Size = System::Drawing::Size(85, 17);
    this->radioButton3->TabIndex = 11;
    this->radioButton3->Text = L"Calculate Cp";
    this->radioButton3->UseVisualStyleBackColor = true;
    // 
    // radioButton4
    // 
    this->radioButton4->AutoSize = true;
    this->radioButton4->Location = System::Drawing::Point(644, 237);
    this->radioButton4->Name = L"radioButton4";
    this->radioButton4->Size = System::Drawing::Size(105, 17);
    this->radioButton4->TabIndex = 12;
    this->radioButton4->TabStop = true;
    this->radioButton4->Text = L"Pollution problem";
    this->radioButton4->UseVisualStyleBackColor = true;
    this->radioButton4->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton4_CheckedChanged);
    // 
    // textBox1
    // 
    this->textBox1->Enabled = false;
    this->textBox1->Location = System::Drawing::Point(655, 263);
    this->textBox1->Name = L"textBox1";
    this->textBox1->Size = System::Drawing::Size(86, 20);
    this->textBox1->TabIndex = 13;
    // 
    // label1
    // 
    this->label1->AutoSize = true;
    this->label1->Location = System::Drawing::Point(598, 263);
    this->label1->Name = L"label1";
    this->label1->Size = System::Drawing::Size(30, 13);
    this->label1->TabIndex = 14;
    this->label1->Text = L"Time";
    // 
    // textBox4
    // 
    this->textBox4->Enabled = false;
    this->textBox4->Location = System::Drawing::Point(655, 315);
    this->textBox4->Name = L"textBox4";
    this->textBox4->Size = System::Drawing::Size(86, 20);
    this->textBox4->TabIndex = 16;
    // 
    // textBox5
    // 
    this->textBox5->Enabled = false;
    this->textBox5->Location = System::Drawing::Point(655, 289);
    this->textBox5->Name = L"textBox5";
    this->textBox5->Size = System::Drawing::Size(86, 20);
    this->textBox5->TabIndex = 15;
    // 
    // label4
    // 
    this->label4->AutoSize = true;
    this->label4->Location = System::Drawing::Point(597, 289);
    this->label4->Name = L"label4";
    this->label4->Size = System::Drawing::Size(58, 13);
    this->label4->TabIndex = 17;
    this->label4->Text = L"Location X";
    // 
    // label5
    // 
    this->label5->AutoSize = true;
    this->label5->Location = System::Drawing::Point(597, 315);
    this->label5->Name = L"label5";
    this->label5->Size = System::Drawing::Size(58, 13);
    this->label5->TabIndex = 18;
    this->label5->Text = L"Location Y";
    // 
    // textBox6
    // 
    this->textBox6->Enabled = false;
    this->textBox6->Location = System::Drawing::Point(654, 341);
    this->textBox6->Name = L"textBox6";
    this->textBox6->Size = System::Drawing::Size(86, 20);
    this->textBox6->TabIndex = 19;
    // 
    // label6
    // 
    this->label6->AutoSize = true;
    this->label6->Location = System::Drawing::Point(598, 341);
    this->label6->Name = L"label6";
    this->label6->Size = System::Drawing::Size(40, 13);
    this->label6->TabIndex = 20;
    this->label6->Text = L"Radius";
    // 
    // Form1
    // 
    this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
    this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
    this->ClientSize = System::Drawing::Size(754, 662);
    this->Controls->Add(this->label6);
    this->Controls->Add(this->textBox6);
    this->Controls->Add(this->label5);
    this->Controls->Add(this->label4);
    this->Controls->Add(this->textBox5);
    this->Controls->Add(this->textBox4);
    this->Controls->Add(this->label1);
    this->Controls->Add(this->textBox1);
    this->Controls->Add(this->radioButton4);
    this->Controls->Add(this->radioButton3);
    this->Controls->Add(this->radioButton2);
    this->Controls->Add(this->radioButton1);
    this->Controls->Add(this->button2);
    this->Controls->Add(this->label3);
    this->Controls->Add(this->textBox3);
    this->Controls->Add(this->button1);
    this->Controls->Add(this->label2);
    this->Controls->Add(this->textBox2);
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
                 label2->Location = System::Drawing::Point(pictureBox1->Width, label2->Location.Y);
                 label3->Location = System::Drawing::Point(pictureBox1->Width, label3->Location.Y);
                 label1->Location = System::Drawing::Point(pictureBox1->Width, label1->Location.Y);
                 label4->Location = System::Drawing::Point(pictureBox1->Width, label4->Location.Y);
                 label5->Location = System::Drawing::Point(pictureBox1->Width, label5->Location.Y);
                 label6->Location = System::Drawing::Point(pictureBox1->Width, label6->Location.Y);

                 textBox2->Location = System::Drawing::Point(pictureBox1->Width + label3->Width, label2->Location.Y);
                 textBox3->Location = System::Drawing::Point(pictureBox1->Width + label3->Width, label3->Location.Y);
                 textBox1->Location = System::Drawing::Point(pictureBox1->Width + label4->Width, label1->Location.Y);
                 textBox4->Location = System::Drawing::Point(pictureBox1->Width + label4->Width, label4->Location.Y);
                 textBox5->Location = System::Drawing::Point(pictureBox1->Width + label4->Width, label5->Location.Y);
                 textBox6->Location = System::Drawing::Point(pictureBox1->Width + label4->Width, label6->Location.Y);

                 button1->Location = System::Drawing::Point(textBox2->Location.X, button1->Location.Y);
                 button2->Location = System::Drawing::Point(textBox2->Location.X, button2->Location.Y);
                 radioButton1->Location = System::Drawing::Point(textBox2->Location.X, radioButton1->Location.Y);
                 radioButton2->Location = System::Drawing::Point(textBox2->Location.X, radioButton2->Location.Y);
                 radioButton3->Location = System::Drawing::Point(textBox2->Location.X, radioButton3->Location.Y);
                 radioButton4->Location = System::Drawing::Point(textBox2->Location.X, radioButton4->Location.Y);
                 m_drawer->SetTargetResolution(pictureBox1->Size.Width, pictureBox1->Size.Height);

               }

             //-------------------------------------

    private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
               {
                 CultureInfo^ cf = gcnew CultureInfo( "en-US", false );
                 NumberFormatInfo^ nfi_e = cf->NumberFormat;
                 double angle = 2 * Math::PI * System::Double::Parse(textBox2->Text, nfi_e) / 360,
                   gamma = System::Double::Parse(textBox3->Text, nfi_e);
                 model.SetParams(Vector2D(Math::Cos(angle), Math::Sin(angle)), gamma);
                 if(radioButton4->Checked)
                   {
                     double rad = System::Double::Parse(textBox6->Text, nfi_e),
                       avg_ttl = System::Double::Parse(textBox1->Text, nfi_e), 
                       posx = System::Double::Parse(textBox5->Text, nfi_e), 
                       posy = System::Double::Parse(textBox4->Text, nfi_e);
                     probl.SetParams(rad, avg_ttl, Vector2D(posx, posy));
                   }
                 to_draw = true;
                 first_time = true;
                 model.ReInit();
                 Model::colors_by_phi.clear();
               }

             //-------------------------------------

    private: System::Void Form1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
               {

               }

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
           {
             if(to_draw)
               {
                 if(!first_time)
                   {
                     model.UpdatePoints();
                     if(radioButton4->Checked)
                       probl.UpdateTime(model);
                   }
                 model.CalcGamma();
                 if(radioButton4->Checked)
                   probl.MoveParticles(model);
               }

             if(!model.GetCurrGamma().empty() && to_draw)
               {
                 ClearPicture();
                 DrawAxes();
                 DrawPoints();
                 if(radioButton4->Checked)
                   {  
                     DrawParticles();
                     if(probl.GetAvgTTL() < probl.GetCurrTime())
                       to_draw = false;
                   }
                 Invalidate(true);
               }
              first_time = false;             
           }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
           {
             int mode = -1;
             if(radioButton1->Checked)
               mode = 0;
             if(radioButton2->Checked)
               mode = 1;
             if(radioButton3->Checked)
               mode = 2;
             to_draw = false;
             if(mode != -1)
               {
                 CalcColors(Model::colors_by_phi, model, mode);
                 FillColors(Model::colors_by_phi);
               }
             DrawAxes();
             DrawPoints();
             Invalidate(true);
           }
private: System::Void radioButton4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
           {
             if(radioButton4->Checked)
               {
                 textBox1->Enabled = true;
                 textBox4->Enabled = true;
                 textBox5->Enabled = true;
                 textBox6->Enabled = true;
               }
             else
               {
                 textBox1->Enabled = false;
                 textBox4->Enabled = false;
                 textBox5->Enabled = false;
                 textBox6->Enabled = false;
               }
           }
};
}

