using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace ImagePreviewer
{
    public partial class FormPreviewer : Form
    {
        private int mIndex = 0;
        private System.Timers.Timer mTimer = new System.Timers.Timer(2000);
        private List<string> mImagePath = new List<string>();

        public FormPreviewer()
        {
            InitializeComponent();
            mTimer.Elapsed += new System.Timers.ElapsedEventHandler(showImage);
            mTimer.AutoReset = true;
        }

        private void button_start_Click(object sender, EventArgs e)
        {
            mTimer.Enabled = true;
        }

        private void button_stop_Click(object sender, EventArgs e)
        {
            mTimer.Enabled = false;
        }

        public void showImage(object source, System.Timers.ElapsedEventArgs e)
        {
            if (mImagePath.Count < 1)
                return;

            mIndex++;
            if (mIndex == mImagePath.Count)
            {
                mIndex = 0;
            }
            this.pictureBox.Load(mImagePath[mIndex]);
        }

        private void button_choose_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "请选择相册";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                string foldPath = dialog.SelectedPath;
                this.textBox.Text = foldPath;
                DirectoryInfo theFolder = new DirectoryInfo(foldPath);
                FileInfo[] dirInfo = theFolder.GetFiles("*.jpg");
                mImagePath.Clear();
                foreach (FileInfo file in dirInfo)
                {
                    mImagePath.Add(foldPath+"\\"+file.ToString());
                }
            }  
        }

    }
}
