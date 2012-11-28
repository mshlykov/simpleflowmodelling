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

        private void button1_Click(object sender, EventArgs e)
        {
            double[] a = new double[chart1.Series["Series1"].Points.Count - 1];
            double summ = 0;
            for (int i = 0; i < a.Length; ++i)
            {
                summ += chart1.Series["Series1"].Points[i].YValues[0] * chart1.Series["Series1"].Points[i].YValues[0];
                a[i] = chart1.Series["Series1"].Points[i].YValues[0];
            }
            for (int i = 0; i < a.Length; ++i)
            {
                a[i] *= chart1.Series["Series1"].Points[a.Length].YValues[0];
                a[i] /= summ;
            }
            summ = 0;
            for (int i = 0; i < a.Length; ++i)
                summ += a[i] * chart1.Series["Series1"].Points[i + 1].YValues[0];
            chart1.Series["Series1"].Points.AddY(summ);
        }
    }
}
