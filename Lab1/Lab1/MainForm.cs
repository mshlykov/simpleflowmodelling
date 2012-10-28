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
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            this.Visible = false;
            ChangeUser();
        }

        private void ChangeUser()
        {
            FormController.GetAuthrizationForm().ShowDialog();
            if (!Users.GetCurrPermission().Equals("Administrator"))
            {
                button1.Enabled = false;
                button2.Enabled = false;
                button3.Enabled = false;
                button4.Enabled = false;
            }
            DBStorage.Open();
            DBStorage.ExecuteQuery("SELECT * FROM Працівник", dataGridView1);
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            DBStorage.Close();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            DBStorage.BackUp();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            DBStorage.Restore();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            Users.DropCurrUser();
            DBStorage.Close();
            this.Hide();
            ChangeUser();
        }
    }
}
