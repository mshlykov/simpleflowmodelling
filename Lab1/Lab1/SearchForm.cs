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
    public partial class SearchForm : Form
    {
        public SearchForm()
        {
            InitializeComponent();
        }

        private void SearchForm_Load(object sender, EventArgs e)
        {
            if (!Users.GetCurrPermission().Equals("Administrator"))
            {
                label6.Enabled = false;
                textBox7.Enabled = false;
                textBox1.Enabled = false;
                listBox1.Enabled = false;
                button3.Enabled = false;
                button4.Enabled = false;
            }
            else
            {
                label6.Enabled = true;
                textBox7.Enabled = true;
                textBox1.Enabled = true;
                listBox1.Enabled = true;
                button3.Enabled = true;
                button4.Enabled = true;
            }
            foreach (string key in DBStorage.m_user_queries.Keys)
            {
                listBox1.Items.Add(key);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (textBox7.Text != "" && textBox1.Text != "" &&
                textBox7.Text != null && textBox1.Text != null)
            {
                DBStorage.m_user_queries.Add(textBox7.Text, textBox1.Text);
                listBox1.Items.Add(textBox7.Text);
                textBox7.Text = "";
                textBox1.Text = "";
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            DBStorage.m_user_queries.Remove((String)listBox1.SelectedItem);
            listBox1.Items.Remove(listBox1.SelectedItem);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            DBStorage.ExecuteQuery(DBStorage.m_user_queries[(String)listBox1.SelectedItem], ((MainForm)FormController.GetMainForm()).GetDataView());
            this.Close();
        }
    }
}
