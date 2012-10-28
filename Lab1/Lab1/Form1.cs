using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Lab1
{
    public partial class Form1 : Form
    {   
        public Form1()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            FormController.GetMainForm().Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            System.String login = textBox1.Text, pass = textBox2.Text;

            if (Users.IsAuthorized(textBox1.Text, textBox2.Text))
            {
                this.Hide();
                return;
            }
            label3.Text = "Невірний логін або пароль!";        
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(!Users.AuthSuccess())
                FormController.GetMainForm().Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            textBox1.Text = "";
            textBox2.Text = "";
        }
    }
}
