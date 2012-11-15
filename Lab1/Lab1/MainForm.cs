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
                button2.Enabled = false;
                button3.Enabled = false;
                
                dataGridView1.ReadOnly = true;
                this.dataGridView1.AllowUserToAddRows = false;
                this.dataGridView1.AllowUserToDeleteRows = false;
            
            }
            else
            {
                button2.Enabled = true;
                button3.Enabled = true;
                
                dataGridView1.ReadOnly = false;
                this.dataGridView1.AllowUserToAddRows = true;
                this.dataGridView1.AllowUserToDeleteRows = true;
            }
            this.listBox1.Items.Clear();
            this.Show();
            DBStorage.Open();
            DBStorage.ExecuteQuery("SELECT * FROM Працівник", dataGridView1);
            DBStorage.ExecuteQuery("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_CATALOG='AIS' AND TABLE_NAME!='sysdiagrams'", listBox1);
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            DBStorage.Close();
            DBStorage.SaveQueries();
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

        private void button6_Click(object sender, EventArgs e)
        {
            FormController.GetSearchForm().ShowDialog();
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            DBStorage.ExecuteQuery("SELECT * FROM " + (string)listBox1.SelectedItem, dataGridView1);
        }

        public DataGridView GetDataView()
        {
            return dataGridView1;
        }

        private void button7_Click(object sender, EventArgs e)
        {
            FormController.GetStatsForm().ShowDialog();
        }
    }
}
