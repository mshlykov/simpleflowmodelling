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
    public partial class StatsForm : Form
    {
        public StatsForm()
        {
            InitializeComponent();
        }

        private void StatsForm_Load(object sender, EventArgs e)
        {
            DBStorage.ExecuteQuery("SELECT ПІП FROM Працівник", listBox1);
            chart1.Series["Series1"].Points.AddXY(System.DateTime.Today, 5000);
            chart1.Series["Series1"].Points.AddXY(System.DateTime.Today, 3000);
        }

        private void StatsForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Hide();
            FormController.GetMainForm().Show();
        }
    }
}
