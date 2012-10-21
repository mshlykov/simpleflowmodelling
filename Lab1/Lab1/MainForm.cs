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
            FormController.GetAuthrizationForm().ShowDialog();
            DBStorage.Open();
            DBStorage.ExecuteQuery("SELECT * FROM Працівник", dataGridView1);
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            DBStorage.Close();
        }
    }
}
