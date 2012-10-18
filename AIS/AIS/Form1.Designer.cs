namespace AIS
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.bindingSource1 = new System.Windows.Forms.BindingSource(this.components);
            this.aISDataSet = new AIS.AISDataSet();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.працкафBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.прац_кафTableAdapter = new AIS.AISDataSetTableAdapters.Прац_кафTableAdapter();
            this.пІПDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.назваDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.факультетDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.aISDataSet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.працкафBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // bindingSource1
            // 
            this.bindingSource1.DataSource = this.aISDataSet;
            this.bindingSource1.Position = 0;
            this.bindingSource1.CurrentChanged += new System.EventHandler(this.bindingSource1_CurrentChanged);
            // 
            // aISDataSet
            // 
            this.aISDataSet.DataSetName = "AISDataSet";
            this.aISDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // dataGridView1
            // 
            this.dataGridView1.AutoGenerateColumns = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.пІПDataGridViewTextBoxColumn,
            this.назваDataGridViewTextBoxColumn,
            this.факультетDataGridViewTextBoxColumn});
            this.dataGridView1.DataSource = this.працкафBindingSource;
            this.dataGridView1.Location = new System.Drawing.Point(16, 40);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(431, 178);
            this.dataGridView1.TabIndex = 0;
            // 
            // працкафBindingSource
            // 
            this.працкафBindingSource.DataMember = "Прац_каф";
            this.працкафBindingSource.DataSource = this.bindingSource1;
            // 
            // прац_кафTableAdapter
            // 
            this.прац_кафTableAdapter.ClearBeforeFill = true;
            // 
            // пІПDataGridViewTextBoxColumn
            // 
            this.пІПDataGridViewTextBoxColumn.DataPropertyName = "ПІП";
            this.пІПDataGridViewTextBoxColumn.HeaderText = "ПІП";
            this.пІПDataGridViewTextBoxColumn.Name = "пІПDataGridViewTextBoxColumn";
            // 
            // назваDataGridViewTextBoxColumn
            // 
            this.назваDataGridViewTextBoxColumn.DataPropertyName = "назва";
            this.назваDataGridViewTextBoxColumn.HeaderText = "назва";
            this.назваDataGridViewTextBoxColumn.Name = "назваDataGridViewTextBoxColumn";
            // 
            // факультетDataGridViewTextBoxColumn
            // 
            this.факультетDataGridViewTextBoxColumn.DataPropertyName = "факультет";
            this.факультетDataGridViewTextBoxColumn.HeaderText = "факультет";
            this.факультетDataGridViewTextBoxColumn.Name = "факультетDataGridViewTextBoxColumn";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(652, 262);
            this.Controls.Add(this.dataGridView1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.aISDataSet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.працкафBindingSource)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.BindingSource bindingSource1;
        private AISDataSet aISDataSet;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.BindingSource працкафBindingSource;
        private AISDataSetTableAdapters.Прац_кафTableAdapter прац_кафTableAdapter;
        private System.Windows.Forms.DataGridViewTextBoxColumn пІПDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn назваDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn факультетDataGridViewTextBoxColumn;
    }
}

