using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data.OleDb;

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
                button1.Enabled = false;
                dataGridView1.ReadOnly = true;
                this.dataGridView1.AllowUserToAddRows = false;
                this.dataGridView1.AllowUserToDeleteRows = false;
            
            }
            else
            {
                button2.Enabled = true;
                button3.Enabled = true;
                button1.Enabled = true;
                dataGridView1.ReadOnly = false;
                this.dataGridView1.AllowUserToAddRows = true;
                this.dataGridView1.AllowUserToDeleteRows = true;
            }
            this.listBox1.Items.Clear();
            this.Show();
            DBStorage.Open();
            //DBStorage.ExecuteQuery("SELECT * FROM Працівник", dataGridView1);
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
            if (Users.GetCurrPermission().Equals("Administrator"))
            {
                dataGridView1.ReadOnly = false;
                this.dataGridView1.AllowUserToAddRows = true;
                this.dataGridView1.AllowUserToDeleteRows = true;
            }
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

        private void button1_Click(object sender, EventArgs e)
        {
            string curr_table = listBox1.SelectedItem.ToString(), tabledata = "", valuedata = "", 
                column_query = "SELECT COLUMN_NAME FROM AIS.INFORMATION_SCHEMA.COLUMNS WHERE table_name = '{0}'";
            //DBStorage.ExecuteNonQuery("DELETE FROM " + curr_table);
            List<string> columns = new List<string>();
            OleDbDataReader rdr = DBStorage.ExecuteQuery(
                String.Format(column_query, curr_table)
                );
            if (rdr.HasRows)
            {
                bool first = true;
                while (rdr.Read())
                {
                    if (first)
                    {
                        tabledata = '(' + rdr.GetString(0);
                        first = false;
                    }
                    else
                    {
                        tabledata += ',' + rdr.GetString(0);
                    }
                    columns.Add(rdr.GetString(0));
                }
                tabledata += ')';
            }

            foreach (DataGridViewRow row in dataGridView1.Rows)
            {
                if (!row.IsNewRow)
                {
                    string format = "SELECT COUNT(*) FROM {0} WHERE {1}", keydata = "";

                    if (curr_table == "Працівник")
                    {
                        keydata = String.Format("ПІП = \'{0}\'", row.Cells[0].FormattedValue.ToString());
                    }
                    else if (curr_table == "Посада")
                    {
                        keydata = String.Format("назва = \'{0}\'", row.Cells[0].FormattedValue.ToString());
                    }
                    else if (curr_table == "Зміна_зарплатні" || curr_table == "Зміна_посади")
                    {
                        keydata = String.Format("ID = {0}", row.Cells[0].FormattedValue.ToString());
                    }
                    else if (curr_table == "Кафедра")
                    {
                        keydata = String.Format("назва = \'{0}\' AND факультет = \'{1}\'", row.Cells[0].FormattedValue.ToString(), row.Cells[1].FormattedValue.ToString());
                    }
                    else if (curr_table == "Прац_каф")
                    {
                        keydata = String.Format("ПІП = \'{0}\' AND назва = \'{0}\' AND факультет = \'{1}\'", row.Cells[0].FormattedValue.ToString(), row.Cells[1].FormattedValue.ToString(), row.Cells[1].FormattedValue.ToString());
                    }

                    OleDbDataReader rdr1 = DBStorage.ExecuteQuery(String.Format(format, curr_table, keydata));
                    rdr1.Read();
                    int c = rdr1.GetInt32(0);
                    string setstring = "";
                    bool first = true;
                    if (c == 0)
                    {
                        foreach (DataGridViewCell cell in row.Cells)
                        {
                            if (first)
                            {
                                valuedata = '(' + "\'" + cell.FormattedValue.ToString() + "\'";
                                first = false;
                            }
                            else
                            {
                                valuedata += ',' + "\'" + cell.FormattedValue.ToString() + "\'";
                            }
                        }
                        valuedata += ')';
                        DBStorage.ExecuteNonQuery(System.String.Format("INSERT INTO {0} VALUES {1}", curr_table + tabledata, valuedata));
                    }
                    else
                    {
                        List<string>.Enumerator en = columns.GetEnumerator();
                        en.MoveNext();
                        foreach (DataGridViewCell cell in row.Cells)
                        {
                            if (first)
                            {
                                setstring = en.Current + " = \'" + cell.FormattedValue.ToString() + "\'";
                                first = false;
                            }
                            else
                            {
                                setstring += ',' + en.Current + " = \'" + cell.FormattedValue.ToString() + "\'";
                            }
                            en.MoveNext();
                        }

                        DBStorage.ExecuteNonQuery(String.Format("UPDATE {0} SET {1} WHERE {2}", curr_table, setstring, keydata));
                    }
                }
            }

            
            //System.String.Format("INSERT INTO {0} VALUES {1}", 1, 2, 3, 4);
        }
    }
}
