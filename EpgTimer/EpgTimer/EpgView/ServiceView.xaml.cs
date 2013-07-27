using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using CtrlCmdCLI;
using CtrlCmdCLI.Def;
using EpgTimer.EpgView;

namespace EpgTimer.EpgView
{
    /// <summary>
    /// ServiceView.xaml の相互作用ロジック
    /// </summary>
    public partial class ServiceView : UserControl
    {
        public ServiceView()
        {
            InitializeComponent();
        }

        public void ClearInfo()
        {
            stackPanel_service.Children.Clear();
        }

        public void SetService(Dictionary<UInt64, EpgServiceInfo> serviceList)
        {
            stackPanel_service.Children.Clear();
            foreach (EpgServiceInfo info in serviceList.Values)
            {
                TextBlock item = new TextBlock();
                item.Text = info.service_name;
                if (0x7880 <= info.ONID && info.ONID <= 0x7FE8)
                {
                    item.Text += "\r\n" + "地デジ ";
                }
                else if (info.ONID == 0x0004)
                {
                    item.Text += "\r\n" + "BS ";
                }
                else if (info.ONID == 0x0006 || info.ONID == 0x0007)
                {
                    item.Text += "\r\n" + "CS ";
                }
                else if (info.ONID == 0x000A)
                {
                    item.Text += "\r\n" + "スカパー! ";
                }
                else if (info.ONID == 0x0001 || info.ONID == 0x0003)
                {
                    item.Text += "\r\n" + "スカパー!SD ";
                }else
                {
                    item.Text += "\r\n" + " ";
                }
                
                if (info.remote_control_key_id != 0)
                {
                    switch (info.remote_control_key_id)
                    {
                        case 1:
                            item.Text += "①";
                            break;
                        case 2:
                            item.Text += "②";
                            break;
                        case 3:
                            item.Text += "③";
                            break;
                        case 4:
                            item.Text += "④";
                            break;
                        case 5:
                            item.Text += "⑤";
                            break;
                        case 6:
                            item.Text += "⑥";
                            break;
                        case 7:
                            item.Text += "⑦";
                            break;
                        case 8:
                            item.Text += "⑧";
                            break;
                        case 9:
                            item.Text += "⑨";
                            break;
                        case 10:
                            item.Text += "⑩";
                            break;
                        case 11:
                            item.Text += "⑪";
                            break;
                        case 12:
                            item.Text += "⑫";
                            break;
                    }
                }
                if (info.direct_tuning_number != 0)
                {
                    item.Text += info.direct_tuning_number.ToString("000");
                }
                else
                {
                    item.Text += (info.SID & 0x3FF).ToString("000");
                }
                item.Width = Settings.Instance.ServiceWidth - 4;
                item.Margin = new Thickness(2, 2, 2, 2);
                item.Background = Brushes.AliceBlue;
                item.Foreground = Brushes.Black;
                item.TextAlignment = TextAlignment.Center;
                item.FontSize = 12;
                item.MouseLeftButtonDown += new MouseButtonEventHandler(item_MouseLeftButtonDown);
                item.DataContext = info;
                stackPanel_service.Children.Add(item);
            }
        }

        void item_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount == 2)
            {
                if (sender.GetType() == typeof(TextBlock))
                {
                    TextBlock item = sender as TextBlock;
                    EpgServiceInfo serviceInfo = item.DataContext as EpgServiceInfo;
                    CommonManager.Instance.TVTestCtrl.SetLiveCh(serviceInfo.ONID, serviceInfo.TSID, serviceInfo.SID);
                }
            }
        }
    }
}
