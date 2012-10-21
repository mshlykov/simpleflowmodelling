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
            m_connection = new OleDbConnection(@"Provider=SQLOLEDB;Server=.\SQLExpress;AttachDbFilename=D:\Maks\TFSCodeplex\modelling\Lab1\Lab1\bin\Debug\AIS.mdf;Database=AIS;Trusted_Connection=Yes;");
        }

        public static void Open()
        {
            m_connection.Open();
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
    }
}
