using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Lab1
{
    class FormController
    {
        private static Form m_main_form, m_authorization_form, m_stats_form;

        static FormController()
        {
            m_main_form = new MainForm();
            m_authorization_form = new Form1();
            m_stats_form = new StatsForm();
        }

        public static Form GetMainForm()
        {
            return m_main_form;
        }

        public static Form GetAuthrizationForm()
        {
            return m_authorization_form;
        }
        
        public static Form GetStatsForm()
        {
            return m_stats_form;
        }

    }
}
