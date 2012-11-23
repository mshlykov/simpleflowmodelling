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
           bool to_draw;

  private: System::Windows::Forms::TextBox^  textBox2;

  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Button^  button1;
  private: System::Windows::Forms::TextBox^  textBox3;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::Timer^  timer1;
  private: System::Windows::Forms::Button^  button2;
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
      int N = 20, M = model.GetSize();
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
      for(std::size_t i = 0; i < Model::off_points.size(); ++i)
        for(std::size_t j = 1; j < Model::off_points[i].size(); ++j)
          if(Model::off_gamma[i][j] > 0)
            m_drawer->DrawPoint(e, Model::off_points[i][j].X(), Model::off_points[i][j].Y(), 0xFF00F0FF);
          else
            m_drawer->DrawPoint(e, Model::off_points[i][j].X(), Model::off_points[i][j].Y(), 0xFF000FFF);

      const Contours& cont = model.GetContours();
      int t = 0;
      for(std::size_t i = 0; i < cont.size(); ++i)
        for(std::size_t j = 0; j < cont[i].size(); ++j, ++t)
          if(Model::curr_gamma[t] > 0)
            m_drawer->DrawPoint(e, cont[i][j].X(), cont[i][j].Y(), 0xFF00F0FF);
          else
            m_drawer->DrawPoint(e, cont[i][j].X(), cont[i][j].Y(), 0xFF000FFF);
      }

    void CalcColors(std::vector<std::vector<int>>& o_colors_by_phi, std::vector<std::vector<int>>& o_colors_by_psi, 
      const std::vector<double>& i_gamma, const Model& i_model, bool i_mode)
      {
      o_colors_by_phi.clear();
      int N = Math::Max(Width / 3, Height / 3) + 1, number_of_colors = 16, red_color = 0xFFFF0000, yellow_color = 0xFFFFFF00;
      double max_phi =i_mode ? i_model.CalcPhi(Vector2D(), i_gamma) : i_model.CalcPsi(Vector2D(), i_gamma), 
        min_phi = i_mode ? i_model.CalcPhi(Vector2D(), i_gamma) : i_model.CalcPsi(Vector2D(), i_gamma) , 
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
          phi_matr[i].push_back(i_mode ? i_model.CalcPhi(curr_point, i_gamma) : i_model.CalcPsi(curr_point, i_gamma));
          
          if(min_phi > phi_matr[i][j])
            min_phi = phi_matr[i][j];

          if(max_phi < phi_matr[i][j])
            max_phi = phi_matr[i][j];
          }
        }

      for(int i = 0; i < N; ++i)
        for(int j = 0; j < N; ++j)
          {
          int phi_color = static_cast<int>(number_of_colors * (phi_matr[i][j] - min_phi) / (max_phi - min_phi));
          
          o_colors_by_phi[i].push_back(yellow_color - phi_color * 0x1000);
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
    this->textBox2->Location = System::Drawing::Point(648, 65);
    this->textBox2->Name = L"textBox2";
    this->textBox2->Size = System::Drawing::Size(86, 20);
    this->textBox2->TabIndex = 2;
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
    this->button1->TabIndex = 6;
    this->button1->Text = L"Solve";
    this->button1->UseVisualStyleBackColor = true;
    this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
    // 
    // textBox3
    // 
    this->textBox3->Location = System::Drawing::Point(648, 91);
    this->textBox3->Name = L"textBox3";
    this->textBox3->Size = System::Drawing::Size(86, 20);
    this->textBox3->TabIndex = 5;
    // 
    // label3
    // 
    this->label3->Location = System::Drawing::Point(603, 91);
    this->label3->Name = L"label3";
    this->label3->Size = System::Drawing::Size(46, 20);
    this->label3->TabIndex = 7;
    this->label3->Text = L"Gamma";
    // 
    // timer1
    // 
    this->timer1->Enabled = true;
    this->timer1->Interval = 1000;
    this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
    // 
    // button2
    // 
    this->button2->Location = System::Drawing::Point(648, 144);
    this->button2->Name = L"button2";
    this->button2->Size = System::Drawing::Size(86, 21);
    this->button2->TabIndex = 8;
    this->button2->Text = L"Stop";
    this->button2->UseVisualStyleBackColor = true;
    this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
    // 
    // Form1
    // 
    this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
    this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
    this->ClientSize = System::Drawing::Size(754, 662);
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

               textBox2->Location = System::Drawing::Point(pictureBox1->Width + label2->Width, label2->Location.Y);
               textBox3->Location = System::Drawing::Point(pictureBox1->Width + label3->Width, label3->Location.Y);

               button1->Location = System::Drawing::Point(textBox2->Location.X, button1->Location.Y);
               button2->Location = System::Drawing::Point(textBox2->Location.X, button2->Location.Y);
               m_drawer->SetTargetResolution(pictureBox1->Size.Width, pictureBox1->Size.Height);

               ClearPicture();
               if(!Model::curr_gamma.empty())
                 {
                 FillColors(Model::colors_by_phi);
                 DrawPoints();
                 }
               Invalidate(true);
               }

             //-------------------------------------

    private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
               {

               double angle = 2 * Math::PI * System::Double::Parse(textBox2->Text) / 360,
                 gamma = System::Double::Parse(textBox3->Text);
               model.SetParams(Vector2D(Math::Cos(angle), Math::Sin(angle)), gamma);
               to_draw = true;
               
               for(std::size_t i = 0; i < Model::off_points.size(); ++i)
                 {
                 Model::off_points[i].clear();
                 Model::off_gamma[i].clear();
                 }
               Model::off_points[0].push_back(model.GetContours()[0][0]);
               Model::off_points[1].push_back(model.GetContours()[0][30]);
               Model::off_points[2].push_back(model.GetContours()[0][60]);
               Model::off_points[3].push_back(model.GetContours()[0][90]);
               Model::off_points[4].push_back(model.GetContours()[1][0]);
               Model::off_gamma[0].push_back(0);
               Model::off_gamma[1].push_back(0);
               Model::off_gamma[2].push_back(0);
               Model::off_gamma[3].push_back(0);
               Model::off_gamma[4].push_back(0);
               Model::colors_by_phi.clear();

               }

             //-------------------------------------

    private: System::Void Form1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
               {
               //DrawAxes();
               //DrawContours();
               }

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
           {
           ClearPicture();
           if(to_draw)
             model.CalcGamma(Model::curr_gamma);
           if(!Model::curr_gamma.empty())
               {
               FillColors(Model::colors_by_phi);
               DrawPoints();
               }
           if(to_draw)
             model.UpdatePoints();
           Invalidate(true);
           }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
           {
           to_draw = false;
           CalcColors(Model::colors_by_phi, Model::colors_by_psi, Model::curr_gamma, model, false);
           FillColors(Model::colors_by_phi);
           }
};
}

