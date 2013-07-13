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

namespace EpgTimer
{
    /// <summary>
    /// EpgDataViewSetting.xaml の相互作用ロジック
    /// </summary>
    public partial class EpgDataViewSetting : UserControl
    {
        private EpgSearchKeyInfo searchKey = new EpgSearchKeyInfo();

        private ListBox templist = new ListBox();

        private void RefreshServiceButtons()
        {
            try
            {
                if (listBox_serviceView.Items.Count > 0)
                {
                    if (button_service_delAll.IsEnabled != true)
                    {
                        button_service_delAll.IsEnabled = true;
                    }
                }
                else
                {
                    if (button_service_delAll.IsEnabled != false)
                    {
                        button_service_delAll.IsEnabled = false;
                    }
                }

                if (listBox_serviceView.SelectedItems.Count > 0)
                {
                    if (button_service_del.IsEnabled != true)
                    {
                        button_service_del.IsEnabled = true;
                    }

                    int count = listBox_serviceView.Items.Count;
                    int topindex = count;
                    int bottomindex = -1;
                    foreach (ChSet5Item info in listBox_serviceView.SelectedItems)
                    {
                        int index = listBox_serviceView.Items.IndexOf(info);
                        if (index < topindex)
                        {
                            topindex = index;
                        }
                        if (index > bottomindex)
                        {
                            bottomindex = index;
                        }
                    }

                    if (topindex < 1)
                    {
                        if (button_service_up.IsEnabled != false)
                        {
                            button_service_up.IsEnabled = false;
                        }
                    }
                    else
                    {
                        if (button_service_up.IsEnabled != true)
                        {
                            button_service_up.IsEnabled = true;
                        }
                    }

                    if (bottomindex >= count - 1)
                    {
                        if (button_service_down.IsEnabled != false)
                        {
                            button_service_down.IsEnabled = false;
                        }
                    }
                    else
                    {
                        if (button_service_down.IsEnabled != true)
                        {
                            button_service_down.IsEnabled = true;
                        }
                    }
                }
                else
                {
                    if (button_service_del.IsEnabled != false)
                    {
                        button_service_del.IsEnabled = false;
                    }

                    if (button_service_up.IsEnabled != false)
                    {
                        button_service_up.IsEnabled = false;
                    }

                    if (button_service_down.IsEnabled != false)
                    {
                        button_service_down.IsEnabled = false;
                    }
                }

                ListBox list = null;
                if (tabItem_bs.IsSelected == true)
                {
                    list = listBox_serviceBS;
                }
                else if (tabItem_cs.IsSelected == true)
                {
                    list = listBox_serviceCS;
                }
                else if (tabItem_tere.IsSelected == true)
                {
                    list = listBox_serviceTere;
                }
                else if (tabItem_sphd.IsSelected == true)
                {
                    list = listBox_serviceSPHD;
                }
                else if (tabItem_spsd.IsSelected == true)
                {
                    list = listBox_serviceSPSD;
                }
                else if (tabItem_other.IsSelected == true)
                {
                    list = listBox_serviceOther;
                }

                if (list != null)
                {
                    if (list.Items.Count > 0)
                    {
                        if (button_service_addAll.IsEnabled != true)
                        {
                            button_service_addAll.IsEnabled = true;
                        }

                        if (button_service_addVideo.IsEnabled != true)
                        {
                            button_service_addVideo.IsEnabled = true;
                        }
                    }
                    else
                    {
                        if (button_service_addAll.IsEnabled != false)
                        {
                            button_service_addAll.IsEnabled = false;
                        }

                        if (button_service_addVideo.IsEnabled != false)
                        {
                            button_service_addVideo.IsEnabled = false;
                        }
                    }

                    if (list.SelectedItems.Count > 0)
                    {
                        if (button_service_add.IsEnabled != true)
                        {
                            button_service_add.IsEnabled = true;
                        }
                    }
                    else
                    {
                        if (button_service_add.IsEnabled != false)
                        {
                            button_service_add.IsEnabled = false;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
            return;
        }

        private void RefreshGenreButtons()
        {
            try
            {
                if (listBox_jyanruView.Items.Count > 0)
                {
                    if (button_jyanru_delAll.IsEnabled != true)
                    {
                        button_jyanru_delAll.IsEnabled = true;
                    }
                }
                else
                {
                    if (button_jyanru_delAll.IsEnabled != false)
                    {
                        button_jyanru_delAll.IsEnabled = false;
                    }
                }

                if (listBox_jyanruView.SelectedItems.Count > 0)
                {
                    if (button_jyanru_del.IsEnabled != true)
                    {
                        button_jyanru_del.IsEnabled = true;
                    }
                }
                else
                {
                    if (button_jyanru_del.IsEnabled != false)
                    {
                        button_jyanru_del.IsEnabled = false;
                    }
                }

                if (listBox_jyanru.Items.Count > 0)
                {
                    if (button_jyanru_addAll.IsEnabled != true)
                    {
                        button_jyanru_addAll.IsEnabled = true;
                    }
                }
                else
                {
                    if (button_jyanru_addAll.IsEnabled != false)
                    {
                        button_jyanru_addAll.IsEnabled = false;
                    }
                }

                if (listBox_jyanru.SelectedItems.Count > 0)
                {
                    if (button_jyanru_add.IsEnabled != true)
                    {
                        button_jyanru_add.IsEnabled = true;
                    }
                }
                else
                {
                    if (button_jyanru_add.IsEnabled != false)
                    {
                        button_jyanru_add.IsEnabled = false;
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
            return;
        }

        private ListBox GetDisabledServiceListboxFromONID(ushort ONID)
        {
            if (ONID == 0x0004)
            {
                return listBox_serviceBS;
            }

            if (ONID == 0x0006 || ONID == 0x0007)
            {
                return listBox_serviceCS;
            }

            if (0x7880 <= ONID && ONID <= 0x7FE8)
            {
                return listBox_serviceTere;
            }

            if (ONID == 0x000A)
            {
                return listBox_serviceSPHD;
            }

            if (ONID == 0x0001 || ONID == 0x0003)
            {
                return listBox_serviceSPSD;
            }

            return listBox_serviceOther;
        }

        private void AddToDisabledServiceList(ChSet5Item info)
        {
            ListBox lb = GetDisabledServiceListboxFromONID(info.ONID);
            if (!lb.Items.Contains(info))
            {
                int count = lb.Items.Count;
                int i;
                for (i = 0; i < count; i++)
                {
                    ChSet5Item info2 = lb.Items.GetItemAt(i) as ChSet5Item;
                    int order;
                    int order2;
                    if (info.ONID == 0x0001 || info.ONID == 0x0003 || info.ONID == 0x0004 || info.ONID == 0x0006 || info.ONID == 0x0007 || info.ONID == 0x000A)
                    {
                        order = info.SID & 0x03FF;
                        order2 = info2.SID & 0x03FF;
                    }
                    else if (0x7880 <= info.ONID && info.ONID <= 0x7FE8)
                    {
                        order = ((info.SID & 0x0180) << 7) | ((info.SID & 0x0078) << 7) | ((info.SID & 0xFE00) >> 6) | (info.SID & 0x0007);
                        order2 = ((info2.SID & 0x0180) << 7) | ((info2.SID & 0x0078) << 7) | ((info2.SID & 0xFE00) >> 6) | (info2.SID & 0x0007);
                    }
                    else
                    {
                        order = info.SID;
                        order2 = info2.SID;
                    }

                    if (order2 > order)
                    {
                        lb.Items.Insert(i, info);
                        break;
                    }
                }

                if (i >= count)
                {
                    lb.Items.Add(info);
                }
            }
            return;
        }

        private void RemoveFromDisabledServiceList(ChSet5Item info)
        {
            ListBox lb = GetDisabledServiceListboxFromONID(info.ONID);
            if (lb.Items.Contains(info))
            {
                int selectedindex = lb.SelectedIndex;
                int removedindex = lb.Items.IndexOf(info);
                lb.Items.Remove(info);
                if (selectedindex == removedindex)
                {
                    if (selectedindex >= lb.Items.Count)
                    {
                        lb.SelectedIndex = lb.Items.Count - 1;
                    }
                    else
                    {
                        lb.SelectedIndex = selectedindex;
                    }
                }
            }
            return;
        }

        private void AddService(bool IsAll, bool IsVideosOnly)
        {
            try
            {
                ListBox list;
                if (tabItem_bs.IsSelected == true)
                {
                    list = listBox_serviceBS;
                }
                else if (tabItem_cs.IsSelected == true)
                {
                    list = listBox_serviceCS;
                }
                else if (tabItem_tere.IsSelected == true)
                {
                    list = listBox_serviceTere;
                }
                else if (tabItem_sphd.IsSelected == true)
                {
                    list = listBox_serviceSPHD;
                }
                else if (tabItem_spsd.IsSelected == true)
                {
                    list = listBox_serviceSPSD;
                }
                else if (tabItem_other.IsSelected == true)
                {
                    list = listBox_serviceOther;
                }
                else
                {
                    return;
                }

                templist.Items.Clear();
                if (IsAll)
                {
                    if (list.Items.Count <= 0)
                    {
                        return;
                    }

                    foreach (ChSet5Item info in list.Items)
                    {
                        if (IsVideosOnly && info.ServiceType != 0x01 && info.ServiceType != 0xA5 && info.ServiceType != 0x81)
                        {
                            continue;
                        }
                        templist.Items.Add(info);
                    }
                }
                else
                {
                    if (list.SelectedItems.Count <= 0)
                    {
                        return;
                    }

                    foreach (ChSet5Item info in list.SelectedItems)
                    {
                        templist.Items.Add(info);
                    }
                }

                foreach (ChSet5Item info in templist.Items)
                {
                    if (!listBox_serviceView.Items.Contains(info))
                    {
                        int selectedindex = listBox_serviceView.SelectedIndex;
                        if (selectedindex >= 0)
                        {
                            listBox_serviceView.Items.Insert(selectedindex, info);
                        }
                        else
                        {
                            listBox_serviceView.Items.Add(info);
                        }
                        listBox_serviceView.ScrollIntoView(info);
                    }
                    RemoveFromDisabledServiceList(info);
                }
                templist.Items.Clear();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
            RefreshServiceButtons();
        }

        private void RemoveService(bool IsAll)
        {
            try
            {
                templist.Items.Clear();
                if (IsAll)
                {
                    if (listBox_serviceView.Items.Count <= 0)
                    {
                        return;
                    }

                    foreach (ChSet5Item info in listBox_serviceView.Items)
                    {
                        templist.Items.Add(info);
                    }
                }
                else
                {
                    if (listBox_serviceView.SelectedItems.Count <= 0)
                    {
                        return;
                    }

                    foreach (ChSet5Item info in listBox_serviceView.SelectedItems)
                    {
                        templist.Items.Add(info);
                    }
                }

                foreach (ChSet5Item info in templist.Items)
                {
                    AddToDisabledServiceList(info);
                    if (listBox_serviceView.Items.Contains(info))
                    {
                        int selectedindex = listBox_serviceView.SelectedIndex;
                        int removedindex = listBox_serviceView.Items.IndexOf(info);
                        listBox_serviceView.Items.Remove(info);
                        if (selectedindex == removedindex)
                        {
                            if (selectedindex >= listBox_serviceView.Items.Count)
                            {
                                listBox_serviceView.SelectedIndex = listBox_serviceView.Items.Count - 1;
                            }
                            else
                            {
                                listBox_serviceView.SelectedIndex = selectedindex;
                            }
                        }
                    }
                }
                templist.Items.Clear();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
            RefreshServiceButtons();
        }

        private void AddToGenreList(ListBox list, ContentKindInfo info)
        {
            if (!list.Items.Contains(info))
            {
                int count = list.Items.Count;
                int i;
                for (i = 0; i < count; i++)
                {
                    ContentKindInfo info2 = list.Items.GetItemAt(i) as ContentKindInfo;
                    if (info2.ID > info.ID)
                    {
                        list.Items.Insert(i, info);
                        break;
                    }
                }

                if (i >= count)
                {
                    list.Items.Add(info);
                }
            }
            return;
        }

        private void RemoveFromGenreList(ListBox list, ContentKindInfo info)
        {
            if (list.Items.Contains(info))
            {
                int selectedindex = list.SelectedIndex;
                int removedindex = list.Items.IndexOf(info);
                list.Items.Remove(info);
                if (selectedindex == removedindex)
                {
                    if (selectedindex >= list.Items.Count)
                    {
                        list.SelectedIndex = list.Items.Count - 1;
                    }
                    else
                    {
                        list.SelectedIndex = selectedindex;
                    }
                }
            }
            return;
        }

        private void AddGenre(bool IsAll)
        {
            try
            {
                templist.Items.Clear();
                if (IsAll)
                {
                    if (listBox_jyanru.Items.Count <= 0)
                    {
                        return;
                    }

                    foreach (ContentKindInfo info in listBox_jyanru.Items)
                    {
                        templist.Items.Add(info);
                    }
                }
                else
                {
                    if (listBox_jyanru.SelectedItems.Count <= 0)
                    {
                        return;
                    }

                    foreach (ContentKindInfo info in listBox_jyanru.SelectedItems)
                    {
                        templist.Items.Add(info);
                    }
                }

                foreach (ContentKindInfo info in templist.Items)
                {
                    AddToGenreList(listBox_jyanruView, info);
                    RemoveFromGenreList(listBox_jyanru, info);
                }
                templist.Items.Clear();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
            RefreshGenreButtons();
        }

        private void RemoveGenre(bool IsAll)
        {
            try
            {
                templist.Items.Clear();
                if (IsAll)
                {
                    if (listBox_jyanruView.Items.Count <= 0)
                    {
                        return;
                    }

                    foreach (ContentKindInfo info in listBox_jyanruView.Items)
                    {
                        templist.Items.Add(info);
                    }
                }
                else
                {
                    if (listBox_jyanruView.SelectedItems.Count <= 0)
                    {
                        return;
                    }

                    foreach (ContentKindInfo info in listBox_jyanruView.SelectedItems)
                    {
                        templist.Items.Add(info);
                    }
                }

                foreach (ContentKindInfo info in templist.Items)
                {
                    AddToGenreList(listBox_jyanru, info);
                    RemoveFromGenreList(listBox_jyanruView, info);
                }
                templist.Items.Clear();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
            RefreshGenreButtons();
        }

        public EpgDataViewSetting()
        {
            InitializeComponent();

            try
            {
                if (Settings.Instance.NoStyle == 1)
                {
                    button_searchKey.Style = null;
                    button_service_addAll.Style = null;
                    button_service_add.Style = null;
                    button_service_del.Style = null;
                    button_service_delAll.Style = null;
                    button_service_up.Style = null;
                    button_service_down.Style = null;
                    button_service_addVideo.Style = null;
                    button_jyanru_addAll.Style = null;
                    button_jyanru_add.Style = null;
                    button_jyanru_del.Style = null;
                    button_jyanru_delAll.Style = null;
                }

                comboBox_timeH_week.ItemsSource = CommonManager.Instance.HourDictionary.Values;
                comboBox_timeH_week.SelectedIndex = 4;


                foreach (ChSet5Item info in ChSet5.Instance.ChList.Values)
                {
                    AddToDisabledServiceList(info);
                }
                foreach (ContentKindInfo info in CommonManager.Instance.ContentKindDictionary.Values)
                {
                    AddToGenreList(listBox_jyanru, info);
                }

                radioButton_rate.IsChecked = true;
                radioButton_week.IsChecked = false;
                radioButton_list.IsChecked = false;

                // ListBox内が空なサービスTabを削除
                if (listBox_serviceBS.Items.Count < 1)
                {
                    tabControl2.Items.Remove(tabItem_bs);
                }

                if (listBox_serviceCS.Items.Count < 1)
                {
                    tabControl2.Items.Remove(tabItem_cs);
                }

                if (listBox_serviceTere.Items.Count < 1)
                {
                    tabControl2.Items.Remove(tabItem_tere);
                }

                if (listBox_serviceSPHD.Items.Count < 1)
                {
                    tabControl2.Items.Remove(tabItem_sphd);
                }

                if (listBox_serviceSPSD.Items.Count < 1)
                {
                    tabControl2.Items.Remove(tabItem_spsd);
                }

                if (listBox_serviceOther.Items.Count < 1)
                {
                    tabControl2.Items.Remove(tabItem_other);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
            RefreshServiceButtons();
            RefreshGenreButtons();
        }

        /// <summary>
        /// デフォルト表示の設定値
        /// </summary>
        /// <param name="setInfo"></param>
        public void SetSetting(CustomEpgTabInfo setInfo)
        {
            searchKey.aimaiFlag = setInfo.SearchKey.aimaiFlag;
            searchKey.andKey = setInfo.SearchKey.andKey;
            searchKey.audioList = setInfo.SearchKey.audioList.ToList();
            searchKey.contentList = setInfo.SearchKey.contentList.ToList();
            searchKey.dateList = setInfo.SearchKey.dateList.ToList();
            searchKey.freeCAFlag = setInfo.SearchKey.freeCAFlag;
            searchKey.notContetFlag = setInfo.SearchKey.notContetFlag;
            searchKey.notDateFlag = setInfo.SearchKey.notDateFlag;
            searchKey.notKey = setInfo.SearchKey.notKey;
            searchKey.regExpFlag = setInfo.SearchKey.regExpFlag;
            searchKey.serviceList = setInfo.SearchKey.serviceList.ToList();
            searchKey.titleOnlyFlag = setInfo.SearchKey.titleOnlyFlag;
            searchKey.videoList = setInfo.SearchKey.videoList.ToList();

            textBox_tabName.Text = setInfo.TabName;
            if (setInfo.ViewMode == 1)
            {
                radioButton_rate.IsChecked = false;
                radioButton_week.IsChecked = true;
                radioButton_list.IsChecked = false;
            }
            else if (setInfo.ViewMode == 2)
            {
                radioButton_rate.IsChecked = false;
                radioButton_week.IsChecked = false;
                radioButton_list.IsChecked = true;
            }
            else
            {
                radioButton_rate.IsChecked = true;
                radioButton_week.IsChecked = false;
                radioButton_list.IsChecked = false;
            }

            if (setInfo.NeedTimeOnlyBasic == true)
            {
                checkBox_noTimeView_rate.IsChecked = true;
            }
            else
            {
                checkBox_noTimeView_rate.IsChecked = false;
            }
            if (setInfo.NeedTimeOnlyWeek == true)
            {
                checkBox_noTimeView_week.IsChecked = true;
            }
            else
            {
                checkBox_noTimeView_week.IsChecked = false;
            }
            comboBox_timeH_week.SelectedIndex = setInfo.StartTimeWeek;

            if (setInfo.SearchMode == true)
            {
                checkBox_searchMode.IsChecked = true;
            }
            else
            {
                checkBox_searchMode.IsChecked = false;
            }

            foreach (UInt64 id in setInfo.ViewServiceList)
            {
                if (ChSet5.Instance.ChList.ContainsKey(id) == true)
                {
                    listBox_serviceView.Items.Add(ChSet5.Instance.ChList[id]);
                    RemoveFromDisabledServiceList(ChSet5.Instance.ChList[id]);
                }
            }
            foreach (UInt16 id in setInfo.ViewContentKindList)
            {
                if (CommonManager.Instance.ContentKindDictionary.ContainsKey(id) == true)
                {
                    AddToGenreList(listBox_jyanruView, CommonManager.Instance.ContentKindDictionary[id]);
                    RemoveFromGenreList(listBox_jyanru, CommonManager.Instance.ContentKindDictionary[id]);
                }
            }
            RefreshServiceButtons();
            RefreshGenreButtons();
        }

        /// <summary>
        /// 設定値の取得
        /// </summary>
        /// <param name="setInfo"></param>
        public void GetSetting(ref CustomEpgTabInfo info)
        {
            info.TabName = textBox_tabName.Text;
            if (radioButton_rate.IsChecked == true)
            {
                info.ViewMode = 0;
            }
            else if (radioButton_week.IsChecked == true)
            {
                info.ViewMode = 1;
            }
            else if (radioButton_list.IsChecked == true)
            {
                info.ViewMode = 2;
            }
            else
            {
                info.ViewMode = 0;
            }
            if (checkBox_noTimeView_rate.IsChecked == true)
            {
                info.NeedTimeOnlyBasic = true;
            }
            else
            {
                info.NeedTimeOnlyBasic = false;
            }
            if (checkBox_noTimeView_week.IsChecked == true)
            {
                info.NeedTimeOnlyWeek = true;
            } 
            else
            {
                info.NeedTimeOnlyWeek = false;
            }
            info.StartTimeWeek = comboBox_timeH_week.SelectedIndex;

            if (checkBox_searchMode.IsChecked == true)
            {
                info.SearchMode = true;
            }
            else
            {
                info.SearchMode = false;
            }

            info.SearchKey.aimaiFlag = searchKey.aimaiFlag;
            info.SearchKey.andKey = searchKey.andKey;
            info.SearchKey.audioList = searchKey.audioList.ToList();
            info.SearchKey.contentList = searchKey.contentList.ToList();
            info.SearchKey.dateList = searchKey.dateList.ToList();
            info.SearchKey.freeCAFlag = searchKey.freeCAFlag;
            info.SearchKey.notContetFlag = searchKey.notContetFlag;
            info.SearchKey.notDateFlag = searchKey.notDateFlag;
            info.SearchKey.notKey = searchKey.notKey;
            info.SearchKey.regExpFlag = searchKey.regExpFlag;
            info.SearchKey.serviceList = searchKey.serviceList.ToList();
            info.SearchKey.titleOnlyFlag = searchKey.titleOnlyFlag;
            info.SearchKey.videoList = searchKey.videoList.ToList();

            info.ViewServiceList.Clear();
            foreach (ChSet5Item item in listBox_serviceView.Items)
            {
                info.ViewServiceList.Add(item.Key);
            }

            info.ViewContentKindList.Clear();
            foreach (ContentKindInfo item in listBox_jyanruView.Items)
            {
                info.ViewContentKindList.Add(item.ID);
            }
        }

        /// <summary>
        /// サービス全追加
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_service_addAll_Click(object sender, RoutedEventArgs e)
        {
            AddService(true, false);
        }

        /// <summary>
        /// 映像のみ全追加
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_service_addVideo_Click(object sender, RoutedEventArgs e)
        {
            AddService(true, true);
        }

        /// <summary>
        /// 選択されている１つ以上のサービスを追加
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_service_add_Click(object sender, RoutedEventArgs e)
        {
            AddService(false, false);
        }

        /// <summary>
        /// 選択されている１つ以上のサービスを削除
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_service_del_Click(object sender, RoutedEventArgs e)
        {
            RemoveService(false);
        }

        /// <summary>
        /// サービス全削除
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_service_delAll_Click(object sender, RoutedEventArgs e)
        {
            RemoveService(true);
        }

        /// <summary>
        /// 1つ上に移動
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_service_up_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (listBox_serviceView.SelectedItem != null)
                {
                    templist.Items.Clear();
                    int count = listBox_serviceView.Items.Count;
                    int topindex = count;
                    foreach (ChSet5Item info in listBox_serviceView.SelectedItems)
                    {
                        templist.Items.Add(info);
                        int index = listBox_serviceView.Items.IndexOf(info);
                        if (index < topindex)
                        {
                            topindex = index;
                        }
                    }

                    if (topindex < 1)
                    {
                        // 上に移動できない
                        return;
                    }

                    for (int i = 0; i < count; i++)
                    {
                        if (templist.Items.Contains(listBox_serviceView.Items.GetItemAt(i)))
                        {
                            object temp = listBox_serviceView.Items.GetItemAt(i - 1);
                            listBox_serviceView.Items.RemoveAt(i - 1);
                            listBox_serviceView.Items.Insert(i, temp);
                        }
                    }
                    listBox_serviceView.ScrollIntoView(listBox_serviceView.Items.GetItemAt(topindex - 1));
                    templist.Items.Clear();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
            RefreshServiceButtons();
        }

        /// <summary>
        /// 1つ下に移動
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_service_down_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (listBox_serviceView.SelectedItem != null)
                {
                    templist.Items.Clear();
                    int count = listBox_serviceView.Items.Count;
                    int bottomindex = -1;
                    foreach (ChSet5Item info in listBox_serviceView.SelectedItems)
                    {
                        templist.Items.Add(info);
                        int index = listBox_serviceView.Items.IndexOf(info);
                        if (index > bottomindex)
                        {
                            bottomindex = index;
                        }
                    }

                    if (bottomindex >= count - 1)
                    {
                        // 下に移動できない
                        return;
                    }

                    for (int i = count - 2; i >= 0; i--)
                    {
                        if (templist.Items.Contains(listBox_serviceView.Items.GetItemAt(i)))
                        {
                            object temp = listBox_serviceView.Items.GetItemAt(i + 1);
                            listBox_serviceView.Items.RemoveAt(i + 1);
                            listBox_serviceView.Items.Insert(i, temp);
                        }
                    }
                    listBox_serviceView.ScrollIntoView(listBox_serviceView.Items.GetItemAt(bottomindex + 1));
                    templist.Items.Clear();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
            RefreshServiceButtons();
        }

        /// <summary>
        /// ジャンル全追加
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_jyanru_addAll_Click(object sender, RoutedEventArgs e)
        {
            AddGenre(true);
        }

        /// <summary>
        /// ジャンル１つ追加
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_jyanru_add_Click(object sender, RoutedEventArgs e)
        {
            AddGenre(false);
        }

        /// <summary>
        /// ジャンル１つ削除
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_jyanru_del_Click(object sender, RoutedEventArgs e)
        {
            RemoveGenre(false);
        }

        /// <summary>
        /// ジャンル全削除
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_jyanru_delAll_Click(object sender, RoutedEventArgs e)
        {
            RemoveGenre(true);
        }
        
        private void listBox_serviceView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            RefreshServiceButtons();
            textBox_serviceView1.Text = "";
            if (listBox_serviceView.SelectedItems.Count != 1)
            {
                return;
            }
            ChSet5Item info = listBox_serviceView.SelectedItem as ChSet5Item;

            textBox_serviceView1.Text = info.NetworkName + "\r\n";
            textBox_serviceView1.Text += "OriginalNetworkID : " + info.ONID.ToString() + " (0x" + info.ONID.ToString("X4") + ")\r\n";
            textBox_serviceView1.Text += "TransportStreamID : " + info.TSID.ToString() + " (0x" + info.TSID.ToString("X4") + ")\r\n";
            textBox_serviceView1.Text += "ServiceID : " + info.SID.ToString() + " (0x" + info.SID.ToString("X4") + ")\r\n";
        }

        private void listBox_serviceBS_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            RefreshServiceButtons();
            textBox_serviceView2.Text = "";
            if (tabItem_bs.IsSelected == true)
            {
                if (listBox_serviceBS.SelectedItems.Count != 1)
                {
                    return;
                }
                ChSet5Item info = listBox_serviceBS.SelectedItem as ChSet5Item;

                textBox_serviceView2.Text = info.NetworkName + "\r\n";
                textBox_serviceView2.Text += "OriginalNetworkID : " + info.ONID.ToString() + " (0x" + info.ONID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "TransportStreamID : " + info.TSID.ToString() + " (0x" + info.TSID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "ServiceID : " + info.SID.ToString() + " (0x" + info.SID.ToString("X4") + ")\r\n";
            }
        }

        private void listBox_serviceCS_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            RefreshServiceButtons();
            textBox_serviceView2.Text = "";
            if (tabItem_cs.IsSelected == true)
            {
                if (listBox_serviceCS.SelectedItems.Count != 1)
                {
                    return;
                }
                ChSet5Item info = listBox_serviceCS.SelectedItem as ChSet5Item;

                textBox_serviceView2.Text = info.NetworkName + "\r\n";
                textBox_serviceView2.Text += "OriginalNetworkID : " + info.ONID.ToString() + " (0x" + info.ONID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "TransportStreamID : " + info.TSID.ToString() + " (0x" + info.TSID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "ServiceID : " + info.SID.ToString() + " (0x" + info.SID.ToString("X4") + ")\r\n";
            }
        }

        private void listBox_serviceTere_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            RefreshServiceButtons();
            textBox_serviceView2.Text = "";
            if (tabItem_tere.IsSelected == true)
            {
                if (listBox_serviceTere.SelectedItems.Count != 1)
                {
                    return;
                }
                ChSet5Item info = listBox_serviceTere.SelectedItem as ChSet5Item;

                textBox_serviceView2.Text = info.NetworkName + "\r\n";
                textBox_serviceView2.Text += "OriginalNetworkID : " + info.ONID.ToString() + " (0x" + info.ONID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "TransportStreamID : " + info.TSID.ToString() + " (0x" + info.TSID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "ServiceID : " + info.SID.ToString() + " (0x" + info.SID.ToString("X4") + ")\r\n";
            }
        }

        private void listBox_serviceSPHD_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            RefreshServiceButtons();
            textBox_serviceView2.Text = "";
            if (tabItem_sphd.IsSelected == true)
            {
                if (listBox_serviceSPHD.SelectedItems.Count != 1)
                {
                    return;
                }
                ChSet5Item info = listBox_serviceSPHD.SelectedItem as ChSet5Item;

                textBox_serviceView2.Text = info.NetworkName + "\r\n";
                textBox_serviceView2.Text += "OriginalNetworkID : " + info.ONID.ToString() + " (0x" + info.ONID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "TransportStreamID : " + info.TSID.ToString() + " (0x" + info.TSID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "ServiceID : " + info.SID.ToString() + " (0x" + info.SID.ToString("X4") + ")\r\n";
            }
        }

        private void listBox_serviceSPSD_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            RefreshServiceButtons();
            textBox_serviceView2.Text = "";
            if (tabItem_spsd.IsSelected == true)
            {
                if (listBox_serviceSPSD.SelectedItems.Count != 1)
                {
                    return;
                }
                ChSet5Item info = listBox_serviceSPSD.SelectedItem as ChSet5Item;

                textBox_serviceView2.Text = info.NetworkName + "\r\n";
                textBox_serviceView2.Text += "OriginalNetworkID : " + info.ONID.ToString() + " (0x" + info.ONID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "TransportStreamID : " + info.TSID.ToString() + " (0x" + info.TSID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "ServiceID : " + info.SID.ToString() + " (0x" + info.SID.ToString("X4") + ")\r\n";
            }
        }

        private void listBox_serviceOther_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            RefreshServiceButtons();
            textBox_serviceView2.Text = "";
            if (tabItem_other.IsSelected == true)
            {
                if (listBox_serviceOther.SelectedItems.Count != 1)
                {
                    return;
                }
                ChSet5Item info = listBox_serviceOther.SelectedItem as ChSet5Item;

                textBox_serviceView2.Text = info.NetworkName + "\r\n";
                textBox_serviceView2.Text += "OriginalNetworkID : " + info.ONID.ToString() + " (0x" + info.ONID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "TransportStreamID : " + info.TSID.ToString() + " (0x" + info.TSID.ToString("X4") + ")\r\n";
                textBox_serviceView2.Text += "ServiceID : " + info.SID.ToString() + " (0x" + info.SID.ToString("X4") + ")\r\n";
            }
        }

        private void button_searchKey_Click(object sender, RoutedEventArgs e)
        {
            SetDefSearchSettingWindow dlg = new SetDefSearchSettingWindow();
            PresentationSource topWindow = PresentationSource.FromVisual(this);
            if (topWindow != null)
            {
                dlg.Owner = (Window)topWindow.RootVisual;
            }
            dlg.SetDefSetting(searchKey);
            if (dlg.ShowDialog() == true)
            {
                dlg.GetSetting(ref searchKey);
            }
        }

        private void listBox_serviceView_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string t = e.OriginalSource.GetType().Name;
            if (t == "TextBlock" || t == "Border")
            {
                RemoveService(false);
            }
        }

        private void listBox_serviceBS_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string t = e.OriginalSource.GetType().Name;
            if (t == "TextBlock" || t == "Border")
            {
                AddService(false, false);
            }
        }

        private void listBox_serviceCS_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string t = e.OriginalSource.GetType().Name;
            if (t == "TextBlock" || t == "Border")
            {
                AddService(false, false);
            }
        }

        private void listBox_serviceTere_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string t = e.OriginalSource.GetType().Name;
            if (t == "TextBlock" || t == "Border")
            {
                AddService(false, false);
            }
        }

        private void listBox_serviceSPHD_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string t = e.OriginalSource.GetType().Name;
            if (t == "TextBlock" || t == "Border")
            {
                AddService(false, false);
            }
        }

        private void listBox_serviceSPSD_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string t = e.OriginalSource.GetType().Name;
            if (t == "TextBlock" || t == "Border")
            {
                AddService(false, false);
            }
        }

        private void listBox_serviceOther_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string t = e.OriginalSource.GetType().Name;
            if (t == "TextBlock" || t == "Border")
            {
                AddService(false, false);
            }
        }

        private void tabControl2_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            RefreshServiceButtons();
        }

        private void listBox_jyanruView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            RefreshGenreButtons();
        }

        private void listBox_jyanru_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            RefreshGenreButtons();
        }

        private void listBox_jyanruView_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string t = e.OriginalSource.GetType().Name;
            if (t == "TextBlock" || t == "Border")
            {
                RemoveGenre(false);
            }
        }

        private void listBox_jyanru_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string t = e.OriginalSource.GetType().Name;
            if (t == "TextBlock" || t == "Border")
            {
                AddGenre(false);
            }
        }

    }
}
