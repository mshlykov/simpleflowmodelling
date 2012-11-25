using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.OleDb;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Lab1
{
    public partial class StatsForm : Form
    {
        public StatsForm()
        {
            InitializeComponent();
        }

        private void StatsForm_Load(object sender, EventArgs e)
        {
            listBox1.Items.Clear();
            DBStorage.ExecuteQuery("SELECT ПІП FROM Працівник", listBox1);
            listBox1.SelectedItem = listBox1.Items[0];
            FillDiag();
            //chart1.Series["Series1"].Points.AddXY(System.DateTime.Today, 5000);
            //chart1.Series["Series1"].Points.AddXY(System.DateTime.Today, 3000);
        }

        private void FillDiag()
        {
            chart1.Series["Series1"].Points.Clear();
            OleDbDataReader rdr = DBStorage.ExecuteQuery("SELECT * FROM Зміна_зарплатні WHERE ПІП = '" + listBox1.SelectedItem + "\'");
            if (rdr.HasRows)
            {
                while (rdr.Read())
                {
                    chart1.Series["Series1"].Points.AddY(rdr.GetInt32(3));
                }
            }
        }

        private void StatsForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Hide();
            FormController.GetMainForm().Show();
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            FillDiag();
        }
    }
}
