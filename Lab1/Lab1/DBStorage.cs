using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.OleDb;
using System.Data.SqlClient;

namespace Lab1
{
    class DBStorage
    {
        private static OleDbConnection m_connection;
        
        static DBStorage()
        {
            m_connection = new OleDbConnection(@"Provider=SQLOLEDB;Server=.\SQLExpress;Trusted_Connection=Yes;");
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

        public static void ExecuteQuery(string i_query_text, System.Windows.Forms.DataGridView i_view)
        {
            OleDbCommand comm = new OleDbCommand(i_query_text, m_connection);
            OleDbDataReader rdr = comm.ExecuteReader();
            DataTable tbl = new DataTable();
            tbl.Load(rdr);
            i_view.DataSource = tbl;
        }

        public static void BackUp()
        {
            OleDbCommand comm = new OleDbCommand(@"BACKUP DATABASE AIS TO DISK='D:\Maks\TFSCodeplex\modelling\Lab1\Lab1\bin\Debug\Backup\temp.bak'", m_connection);
            comm.ExecuteNonQuery();
            System.Windows.Forms.MessageBox.Show("Архівування завершене.");
        }

        public static void Restore()
        {
            m_connection.ChangeDatabase("master");
            OleDbCommand comm1 = new OleDbCommand(@"sp_detach_db @dbname='AIS'", m_connection),
                comm2 = new OleDbCommand(@"RESTORE DATABASE AIS FROM DISK = 'D:\Maks\TFSCodeplex\modelling\Lab1\Lab1\bin\Debug\Backup\temp.bak' WITH REPLACE", m_connection);
            comm1.ExecuteNonQuery();
            comm2.ExecuteNonQuery();
            System.Windows.Forms.MessageBox.Show("Відновлення завершене.");
        }
    }
}
