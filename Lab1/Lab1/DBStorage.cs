using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.OleDb;
using System.Data.SqlClient;
using System.IO;
using System.Windows.Forms;

namespace Lab1
{
    class DBStorage
    {
        private static OleDbConnection m_connection;
        public static Dictionary<string, string> m_user_queries;

        static DBStorage()
        {
            m_connection = new OleDbConnection(@"Provider=SQLOLEDB;Server=.\SQLExpress;Trusted_Connection=Yes;");
            m_user_queries = new Dictionary<string, string>();

            System.IO.StreamReader sr = new System.IO.StreamReader("UserQueries.txt");
            String name_str, query_str;
            while (!sr.EndOfStream)
            {
                name_str = sr.ReadLine();
                query_str = sr.ReadLine();
                m_user_queries.Add(name_str, query_str);
            }
        }

        public static void Open()
        {
            m_connection.Open();
            m_connection.ChangeDatabase("AIS");
        }

        public static void Close()
        {
            m_connection.Close();
        }

        public static OleDbDataReader ExecuteQuery(string i_query_text)
        {
            OleDbCommand comm = new OleDbCommand(i_query_text, m_connection);
            return comm.ExecuteReader();
        }

        public static void ExecuteQuery(string i_query_text, System.Windows.Forms.DataGridView i_view)
        {
            OleDbCommand comm = new OleDbCommand(i_query_text, m_connection);
            OleDbDataReader rdr = comm.ExecuteReader();
            DataTable tbl = new DataTable();
            tbl.Load(rdr);
            i_view.DataSource = tbl;
        }

        public static void ExecuteQuery(string i_query_text, System.Windows.Forms.ListBox i_box)
        {
            OleDbCommand comm = new OleDbCommand(i_query_text, m_connection);
            OleDbDataReader rdr = comm.ExecuteReader();


            if (rdr.HasRows)
            {
                while (rdr.Read())
                {
                    i_box.Items.Add(rdr.GetString(0));
                }
            }
        }

        public static void ExecuteNonQuery(String i_query)
        {
            OleDbCommand comm = new OleDbCommand(i_query, m_connection);
            comm.ExecuteNonQuery();
            
        }

        public static OleDbDataAdapter GetAdapter(String i_query)
        {
            OleDbDataAdapter ad = new OleDbDataAdapter(i_query, m_connection);
            return ad;
        }

        public static void BackUp()
        {
            OleDbCommand comm = new OleDbCommand(@"BACKUP DATABASE AIS TO DISK='" + Path.GetDirectoryName(Application.ExecutablePath) + @"\temp.bak" + '\'', m_connection);
            comm.ExecuteNonQuery();
            System.Windows.Forms.MessageBox.Show("Архівування завершене.");
        }

        public static void Restore()
        {
            m_connection.ChangeDatabase("master");
            OleDbCommand comm1 = new OleDbCommand(@"sp_detach_db @dbname='AIS'", m_connection),
                comm2 = new OleDbCommand(@"RESTORE DATABASE AIS FROM DISK = '" + Path.GetDirectoryName(Application.ExecutablePath) + @"\temp.bak" + "\' WITH REPLACE", m_connection);
            comm1.ExecuteNonQuery();
            comm2.ExecuteNonQuery();
            System.Windows.Forms.MessageBox.Show("Відновлення завершене.");
        }

        public static void SaveQueries()
        {
            StreamWriter wrtr = new StreamWriter("UserQueries.txt");

            foreach (string key in m_user_queries.Keys)
            {
                wrtr.WriteLine(key);
                wrtr.WriteLine(m_user_queries[key]);
            }
            wrtr.Flush();
        }
    }
}
