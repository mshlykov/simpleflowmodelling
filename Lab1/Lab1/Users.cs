using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Lab1
{
    class Users
    {
        public static string m_curr_user, m_permission;
        
        static Users()
        {
            m_users = new Dictionary<string, System.Tuple<string, string>>();
            m_successful_authorization = false;
            LoadUsers("Users.txt");
        }
        
        private static Dictionary<string, System.Tuple<string, string>> m_users;
        
        private static bool m_successful_authorization;

        public static bool IsAuthorized(String i_login, String i_password)
        {
            if (i_login.Length == 0 || i_password.Length == 0)
                return false;
            if (!m_users.ContainsKey(i_login))
                return false;
            if (!m_users[i_login].Item1.Equals(i_password))
                return false;

            m_successful_authorization = true;
            m_curr_user = i_login;
            m_permission = m_users[m_curr_user].Item2;
            return true;
        }

        public static bool AuthSuccess()
        {
            return m_successful_authorization;
        }

        public static void DropCurrUser()
        {
            m_successful_authorization = false;
            m_curr_user = null;
            m_permission = null;
        }

        public static string GetCurrPermission()
        {
            return m_permission;
        }

        private static void LoadUsers(String i_file_path)
        {
            System.IO.StreamReader sr = new System.IO.StreamReader(i_file_path);
            String str;
            String[] words;
            while (!sr.EndOfStream)
            {
                str = sr.ReadLine();
                words = str.Split(' ');
                m_users.Add(words[0], System.Tuple.Create(words[1],words[2]));
            }
            sr.Close();
        }
    }
}
