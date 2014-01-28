﻿using System;
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
using System.ComponentModel;
using System.Collections.ObjectModel;
using System.Collections;

using CtrlCmdCLI;
using CtrlCmdCLI.Def;

namespace EpgTimer
{
    /// <summary>
    /// ReserveView.xaml の相互作用ロジック
    /// </summary>
    public partial class ReserveView : UserControl
    {
        private bool RedrawReserve = true;
        private List<ReserveItem> reserveList = new List<ReserveItem>();
        private Dictionary<String, GridViewColumn> columnList = new Dictionary<String, GridViewColumn>();

        string _lastHeaderClicked = null;
        ListSortDirection _lastDirection = ListSortDirection.Ascending;
        string _lastHeaderClicked2 = null;
        ListSortDirection _lastDirection2 = ListSortDirection.Ascending;

        private CtrlCmdUtil cmd = CommonManager.Instance.CtrlCmd;

        MainWindow _mainWindow;
        PopupWindow _popupWindow;

        public ReserveView()
        {
            InitializeComponent();

            try
            {
                if (Settings.Instance.NoStyle == 1)
                {
                    button_del.Style = null;
                    button_change.Style = null;
                    button_no.Style = null;
                    button_add_manual.Style = null;
                    button_timeShiftPlay.Style = null;
                }

                foreach (GridViewColumn info in gridView_reserve.Columns)
                {
                    GridViewColumnHeader header = info.Header as GridViewColumnHeader;
                    columnList.Add((string)header.Tag, info);
                }
                gridView_reserve.Columns.Clear();

                foreach (ListColumnInfo info in Settings.Instance.ReserveListColumn)
                {
                    columnList[info.Tag].Width = info.Width;
                    gridView_reserve.Columns.Add(columnList[info.Tag]);
                }
                /*
                if (Settings.Instance.ResColumnWidth0 != 0)
                {
                    gridView_reserve.Columns[0].Width = Settings.Instance.ResColumnWidth0;
                }
                if (Settings.Instance.ResColumnWidth1 != 0)
                {
                    gridView_reserve.Columns[1].Width = Settings.Instance.ResColumnWidth1;
                }
                if (Settings.Instance.ResColumnWidth2 != 0)
                {
                    gridView_reserve.Columns[2].Width = Settings.Instance.ResColumnWidth2;
                }
                if (Settings.Instance.ResColumnWidth3 != 0)
                {
                    gridView_reserve.Columns[3].Width = Settings.Instance.ResColumnWidth3;
                }
                if (Settings.Instance.ResColumnWidth4 != 0)
                {
                    gridView_reserve.Columns[4].Width = Settings.Instance.ResColumnWidth4;
                }
                if (Settings.Instance.ResColumnWidth5 != 0)
                {
                    gridView_reserve.Columns[5].Width = Settings.Instance.ResColumnWidth5;
                }
                if (Settings.Instance.ResColumnWidth6 != 0)
                {
                    gridView_reserve.Columns[6].Width = Settings.Instance.ResColumnWidth6;
                }
                if (Settings.Instance.ResColumnWidth7 != 0)
                {
                    gridView_reserve.Columns[7].Width = Settings.Instance.ResColumnWidth7;
                }
                */
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }


        public void SaveSize()
        {
            try
            {
                /*
                Settings.Instance.ResColumnWidth0 = gridView_reserve.Columns[0].Width;
                Settings.Instance.ResColumnWidth1 = gridView_reserve.Columns[1].Width;
                Settings.Instance.ResColumnWidth2 = gridView_reserve.Columns[2].Width;
                Settings.Instance.ResColumnWidth3 = gridView_reserve.Columns[3].Width;
                Settings.Instance.ResColumnWidth4 = gridView_reserve.Columns[4].Width;
                Settings.Instance.ResColumnWidth5 = gridView_reserve.Columns[5].Width;
                Settings.Instance.ResColumnWidth6 = gridView_reserve.Columns[6].Width;
                Settings.Instance.ResColumnWidth7 = gridView_reserve.Columns[7].Width;
                */
                Settings.Instance.ReserveListColumn.Clear();
                foreach (GridViewColumn info in gridView_reserve.Columns)
                {
                    GridViewColumnHeader header = info.Header as GridViewColumnHeader;

                    Settings.Instance.ReserveListColumn.Add(new ListColumnInfo((String)header.Tag, info.Width));
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            if (RedrawReserve == true && this.IsVisible == true)
            {
                if (ReDrawReserveData() == true)
                {
                    RedrawReserve = false;
                }
            }
            this._mainWindow = (MainWindow)Window.GetWindow(this);
            this._popupWindow = new PopupWindow(Window.GetWindow(this));
        }

        /// <summary>
        /// 予約情報の更新通知
        /// </summary>
        public void UpdateReserveData()
        {
            RedrawReserve = true;
            if (this.IsVisible == true)
            {
                if (ReDrawReserveData() == true)
                {
                    RedrawReserve = false;
                }
            }
        }

        private bool ReDrawReserveData()
        {
            try
            {
                if (CommonManager.Instance.NWMode == true)
                {
                    if (CommonManager.Instance.NW.IsConnected == false)
                    {
                        return false;
                    }
                }
                if (!isInDesignMode)
                {
                    ErrCode err = CommonManager.Instance.DB.ReloadReserveInfo();
                    if (err == ErrCode.CMD_ERR_CONNECT)
                    {
                        this.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            MessageBox.Show("サーバー または EpgTimerSrv に接続できませんでした。");
                        }), null);
                        return false;
                    }
                    if (err == ErrCode.CMD_ERR_TIMEOUT)
                    {
                        this.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            MessageBox.Show("EpgTimerSrvとの接続にタイムアウトしました。031");
                        }), null);
                        return false;
                    }
                    if (err != ErrCode.CMD_SUCCESS)
                    {
                        this.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            MessageBox.Show("情報の取得でエラーが発生しました。");
                        }), null);
                        return false;
                    }
                }

                ICollectionView dataView = CollectionViewSource.GetDefaultView(listView_reserve.DataContext);
                if (dataView != null)
                {
                    dataView.SortDescriptions.Clear();
                    dataView.Refresh();
                }
                listView_reserve.DataContext = null;
                reserveList.Clear();

                foreach (ReserveData info in CommonManager.Instance.DB.ReserveList.Values)
                {
                    ReserveItem item = new ReserveItem(info);
                    reserveList.Add(item);
                }

                listView_reserve.DataContext = reserveList;
                if (_lastHeaderClicked != null)
                {
                    //string header = ((Binding)_lastHeaderClicked.DisplayMemberBinding).Path.Path;
                    if (String.Compare(_lastHeaderClicked, "RecFileName") != 0)
                    {
                        Sort(_lastHeaderClicked, _lastDirection);
                    }

                }
                else
                {
                    bool sort = false;
                    foreach (GridViewColumn info in gridView_reserve.Columns)
                    {
                        GridViewColumnHeader columnHeader = info.Header as GridViewColumnHeader;
                        string header = columnHeader.Tag as string;
                        if (String.Compare(header, Settings.Instance.ResColumnHead, true) == 0)
                        {
                            if (String.Compare(header, "RecFileName") != 0)
                            {
                                Sort(header, Settings.Instance.ResSortDirection);

                                _lastHeaderClicked = header;
                                _lastDirection = Settings.Instance.ResSortDirection;

                            }
                            sort = true;
                            break;
                        }
                    }
                    if (gridView_reserve.Columns.Count > 0 && sort == false)
                    {
                        GridViewColumnHeader columnHeader = gridView_reserve.Columns[0].Header as GridViewColumnHeader;
                        string header = columnHeader.Tag as string;
                        if (String.Compare(header, "RecFileName") != 0)
                        {
                            Sort(header, _lastDirection);
                            _lastHeaderClicked = header;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                this.Dispatcher.BeginInvoke(new Action(() =>
                {
                    MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
                }), null);
                return false;
            }

            // 枠線表示用
            CommonManager.Instance.DB.ReloadEpgData();

            return true;
        }

        private void Sort(string sortBy, ListSortDirection direction)
        {
            try
            {
                ListCollectionView dataView = (ListCollectionView)CollectionViewSource.GetDefaultView(listView_reserve.DataContext);

                Func<String, Func<ReserveItem, Object>> h2d = header =>
                {
                    switch (header)
                    {
                        case "StartTime":
                            return x => x.StartTime;
                        case "NetworkName":
                            return x => x.NetworkName;
                        case "ServiceName":
                            return x => x.ServiceName;
                        case "EventName":
                            return x => x.EventName;
                        case "RecMode":
                            return x => x.RecMode;
                        case "Priority":
                            return x => x.Priority;
                        case "Tuijyu":
                            return x => x.Tuijyu;
                        case "Comment":
                            return x => x.Comment;
                        case "RecFileName":
                            return x => x.RecFileName;
                        case "Pittari":
                            return x => x.Pittari;
                    }
                    return x => x.EventName;
                };

                var sortby = new List<NumericSortClass<ReserveItem>.SortDesc>();
                sortby.Add(new NumericSortClass<ReserveItem>.SortDesc(h2d(sortBy), direction));
                if (_lastHeaderClicked2 != null)
                {
                    if (String.Compare(sortBy, _lastHeaderClicked2) != 0)
                    {
                        sortby.Add(new NumericSortClass<ReserveItem>.SortDesc(h2d(_lastHeaderClicked2), _lastDirection2));
                    }
                }
                dataView.CustomSort = new NumericSortClass<ReserveItem>(sortby);

                Settings.Instance.ResColumnHead = sortBy;
                Settings.Instance.ResSortDirection = direction;

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        private void GridViewColumnHeader_Click(object sender, RoutedEventArgs e)
        {
            GridViewColumnHeader headerClicked = e.OriginalSource as GridViewColumnHeader;
            ListSortDirection direction;

            if (headerClicked != null)
            {
                if (headerClicked.Role != GridViewColumnHeaderRole.Padding)
                {
                    string header = headerClicked.Tag as string;
                    if (String.Compare(header, "RecFileName") == 0)
                    {
                        return;
                    }

                    if (String.Compare(header, _lastHeaderClicked) != 0)
                    {
                        direction = ListSortDirection.Ascending;
                        _lastHeaderClicked2 = _lastHeaderClicked;
                        _lastDirection2 = _lastDirection;
                    }
                    else
                    {
                        if (_lastDirection == ListSortDirection.Ascending)
                        {
                            direction = ListSortDirection.Descending;
                        }
                        else
                        {
                            direction = ListSortDirection.Ascending;
                        }
                    }

                    Sort(header, direction);

                    _lastHeaderClicked = header;
                    _lastDirection = direction;
                }
            }
        }

        private void listView_reserve_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            ChangeReserve();
        }

        private void button_change_Click(object sender, RoutedEventArgs e)
        {
            ChangeReserve();
        }

        private void ChangeReserve()
        {
            if (listView_reserve.SelectedItem != null)
            {
                ReserveItem item = listView_reserve.SelectedItem as ReserveItem;
                ChgReserveWindow dlg = new ChgReserveWindow();
                dlg.Owner = (Window)PresentationSource.FromVisual(this).RootVisual;
                dlg.SetReserveInfo(item.ReserveInfo);
                if (dlg.ShowDialog() == true)
                {
                }
            }
        }

        private void recmode_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                MenuItem menuItem = sender as MenuItem;
                List<ReserveData> list = new List<ReserveData>();
                foreach (ReserveItem item in listView_reserve.SelectedItems)
                {
                    ReserveData reserveInfo = item.ReserveInfo;

                    byte recMode = 0;
                    if (menuItem.Name.CompareTo("recmode_all") == 0)
                    {
                        recMode = 0;
                    }
                    else if (menuItem.Name.CompareTo("recmode_only") == 0)
                    {
                        recMode = 1;
                    }
                    else if (menuItem.Name.CompareTo("recmode_all_nodec") == 0)
                    {
                        recMode = 2;
                    }
                    else if (menuItem.Name.CompareTo("recmode_only_nodec") == 0)
                    {
                        recMode = 3;
                    }
                    else if (menuItem.Name.CompareTo("recmode_view") == 0)
                    {
                        recMode = 4;
                    }
                    else if (menuItem.Name.CompareTo("recmode_no") == 0)
                    {
                        recMode = 5;
                    }
                    else
                    {
                        return;
                    }
                    reserveInfo.RecSetting.RecMode = recMode;

                    list.Add(reserveInfo);
                }
                if (list.Count > 0)
                {
                    ErrCode err = (ErrCode)cmd.SendChgReserve(list);
                    if (err == ErrCode.CMD_ERR_CONNECT)
                    {
                        MessageBox.Show("サーバー または EpgTimerSrv に接続できませんでした。");
                    }
                    if (err == ErrCode.CMD_ERR_TIMEOUT)
                    {
                        MessageBox.Show("EpgTimerSrvとの接続にタイムアウトしました。032");
                    }
                    if (err != ErrCode.CMD_SUCCESS)
                    {
                        MessageBox.Show("チューナー一覧の取得でエラーが発生しました。");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        private void button_no_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                List<ReserveData> list = new List<ReserveData>();
                foreach (ReserveItem item in listView_reserve.SelectedItems)
                {
                    ReserveData reserveInfo = item.ReserveInfo;

                    reserveInfo.RecSetting.RecMode = 5;

                    list.Add(reserveInfo);
                }
                if (list.Count > 0)
                {
                    ErrCode err = (ErrCode)cmd.SendChgReserve(list);
                    if (err == ErrCode.CMD_ERR_CONNECT)
                    {
                        MessageBox.Show("サーバー または EpgTimerSrv に接続できませんでした。");
                    }
                    if (err == ErrCode.CMD_ERR_TIMEOUT)
                    {
                        MessageBox.Show("EpgTimerSrvとの接続にタイムアウトしました。033");
                    }
                    if (err != ErrCode.CMD_SUCCESS)
                    {
                        MessageBox.Show("チューナー一覧の取得でエラーが発生しました。");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        private void priority_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                MenuItem menuItem = sender as MenuItem;
                List<ReserveData> list = new List<ReserveData>();
                foreach (ReserveItem item in listView_reserve.SelectedItems)
                {
                    ReserveData reserveInfo = item.ReserveInfo;

                    byte priority = 1;
                    if (menuItem.Name.CompareTo("priority_1") == 0)
                    {
                        priority = 1;
                    }
                    else if (menuItem.Name.CompareTo("priority_2") == 0)
                    {
                        priority = 2;
                    }
                    else if (menuItem.Name.CompareTo("priority_3") == 0)
                    {
                        priority = 3;
                    }
                    else if (menuItem.Name.CompareTo("priority_4") == 0)
                    {
                        priority = 4;
                    }
                    else if (menuItem.Name.CompareTo("priority_5") == 0)
                    {
                        priority = 5;
                    }
                    else
                    {
                        return;
                    }
                    reserveInfo.RecSetting.Priority = priority;

                    list.Add(reserveInfo);
                }
                if (list.Count > 0)
                {
                    ErrCode err = (ErrCode)cmd.SendChgReserve(list);
                    if (err == ErrCode.CMD_ERR_CONNECT)
                    {
                        MessageBox.Show("サーバー または EpgTimerSrv に接続できませんでした。");
                    }
                    if (err == ErrCode.CMD_ERR_TIMEOUT)
                    {
                        MessageBox.Show("EpgTimerSrvとの接続にタイムアウトしました。034");
                    }
                    if (err != ErrCode.CMD_SUCCESS)
                    {
                        MessageBox.Show("チューナー一覧の取得でエラーが発生しました。");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        private void autoadd_Click(object sender, RoutedEventArgs e)
        {
            if (listView_reserve.SelectedItem != null)
            {
                SearchWindow dlg = new SearchWindow();
                dlg.Owner = (Window)PresentationSource.FromVisual(this).RootVisual;
                dlg.SetViewMode(1);

                EpgSearchKeyInfo key = new EpgSearchKeyInfo();

                ReserveItem item = listView_reserve.SelectedItem as ReserveItem;

                key.andKey = item.ReserveInfo.Title;
                Int64 sidKey = ((Int64)item.ReserveInfo.OriginalNetworkID) << 32 | ((Int64)item.ReserveInfo.TransportStreamID) << 16 | ((Int64)item.ReserveInfo.ServiceID);
                key.serviceList.Add(sidKey);

                dlg.SetSearchDefKey(key);
                dlg.ShowDialog();
            }
        }

        private void timeShiftPlay_Click(object sender, RoutedEventArgs e)
        {
            if (listView_reserve.SelectedItem != null)
            {
                ReserveItem info = listView_reserve.SelectedItem as ReserveItem;
                CommonManager.Instance.TVTestCtrl.StartTimeShift(info.ReserveInfo.ReserveID);
            }
        }

        private void button_del_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                List<UInt32> list = new List<UInt32>();
                foreach (ReserveItem item in listView_reserve.SelectedItems)
                {
                    ReserveData reserveInfo = item.ReserveInfo;

                    list.Add(reserveInfo.ReserveID);
                }
                if (list.Count > 0)
                {
                    ErrCode err = (ErrCode)cmd.SendDelReserve(list);
                    if (err == ErrCode.CMD_ERR_CONNECT)
                    {
                        MessageBox.Show("サーバー または EpgTimerSrv に接続できませんでした。");
                    }
                    if (err == ErrCode.CMD_ERR_TIMEOUT)
                    {
                        MessageBox.Show("EpgTimerSrvとの接続にタイムアウトしました。035");
                    }
                    if (err != ErrCode.CMD_SUCCESS)
                    {
                        MessageBox.Show("チューナー一覧の取得でエラーが発生しました。");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        private void button_add_manual_Click(object sender, RoutedEventArgs e)
        {
            ChgReserveWindow dlg = new ChgReserveWindow();
            dlg.Owner = (Window)PresentationSource.FromVisual(this).RootVisual;
            dlg.AddReserveMode(true);
            dlg.ShowDialog();
        }

        private void UserControl_IsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (RedrawReserve == true && this.IsVisible == true)
            {
                if (ReDrawReserveData() == true)
                {
                    RedrawReserve = false;
                }
            }
        }

        private void ContextMenu_Header_ContextMenuOpening(object sender, ContextMenuEventArgs e)
        {
            try
            {
                foreach (MenuItem item in listView_reserve.ContextMenu.Items)
                {
                    item.IsChecked = false;
                    foreach (ListColumnInfo info in Settings.Instance.ReserveListColumn)
                    {
                        if (info.Tag.CompareTo(item.Name) == 0)
                        {
                            item.IsChecked = true;
                            break;
                        }
                    }
                }


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        private void headerSelect_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                MenuItem menuItem = sender as MenuItem;
                if (menuItem.IsChecked == true)
                {

                    Settings.Instance.ReserveListColumn.Add(new ListColumnInfo(menuItem.Name, Double.NaN));
                    gridView_reserve.Columns.Add(columnList[menuItem.Name]);
                }
                else
                {
                    foreach (ListColumnInfo info in Settings.Instance.ReserveListColumn)
                    {
                        if (info.Tag.CompareTo(menuItem.Name) == 0)
                        {
                            Settings.Instance.ReserveListColumn.Remove(info);
                            gridView_reserve.Columns.Remove(columnList[menuItem.Name]);
                            break;
                        }
                    }
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        void listView_reserve_KeyDown(object sender, KeyEventArgs e)
        {
            if (Keyboard.Modifiers.HasFlag(ModifierKeys.Control) && Keyboard.Modifiers.HasFlag(ModifierKeys.Alt))
            {
                switch (e.SystemKey)
                {
                    case Key.D1: this.setPriority(1); break;
                    case Key.D2: this.setPriority(2); break;
                    case Key.D3: this.setPriority(3); break;
                    case Key.D4: this.setPriority(4); break;
                    case Key.D5: this.setPriority(5); break;
                }
            }
            else if (Keyboard.Modifiers.HasFlag(ModifierKeys.Alt))
            {
                switch (e.SystemKey)
                {
                    case Key.D0: this.setRecMode(0); break;
                    case Key.D1: this.setRecMode(1); break;
                    case Key.D2: this.setRecMode(2); break;
                    case Key.D3: this.setRecMode(3); break;
                    case Key.D4: this.setRecMode(4); break;
                    case Key.D5: this.setRecMode(5); break;
                }
            }
            else if (Keyboard.Modifiers.HasFlag(ModifierKeys.Control))
            {
                switch (e.Key)
                {
                    case Key.A:
                        this.autoadd_Click(this.listView_reserve.SelectedItem, new RoutedEventArgs(Button.ClickEvent));
                        break;
                    case Key.P:
                        this.timeShiftPlay_Click(this.listView_reserve.SelectedItem, new RoutedEventArgs(Button.ClickEvent));
                        break;
                }
            }
            else
            {
                switch (e.Key)
                {
                    case Key.F2:
                        this.MenuItem_Click_Google(this, new RoutedEventArgs(Button.ClickEvent));
                        break;
                    case Key.F3:
                        this.MenuItem_Click_ProgramTable(this, new RoutedEventArgs(Button.ClickEvent));
                        break;
                    case Key.Back:
                        new BlackoutWindow(Window.GetWindow(this)).showWindow("予約←→無効");
                        this.MenuItem_Click_ChangeRecMode(this.listView_reserve.SelectedItem, new RoutedEventArgs(Button.ClickEvent));
                        break;
                    case Key.Enter:
                        this.button_change_Click(this.listView_reserve.SelectedItem, new RoutedEventArgs(Button.ClickEvent));
                        break;
                    case Key.Delete:
                        this.deleteItem();
                        break;
                }
            }
        }

        void deleteItem()
        {
            if (listView_reserve.SelectedItems.Count == 0) { return; }
            //
            try
            {
                string text1 = "削除しますか？" + "　[削除アイテム数: " + listView_reserve.SelectedItems.Count + "]" + "\n\n";
                List<UInt32> dataIDList = new List<uint>();
                foreach (ReserveItem info in listView_reserve.SelectedItems)
                {
                    text1 += " ・ " + info.ReserveInfo.Title + "\n";
                }
                string caption1 = "登録項目削除の確認";
                if (MessageBox.Show(text1, caption1, MessageBoxButton.OKCancel, MessageBoxImage.Exclamation, MessageBoxResult.OK) == MessageBoxResult.OK)
                {
                    this.button_del_Click(this.listView_reserve.SelectedItem, new RoutedEventArgs(Button.ClickEvent));
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        void setPriority(int priority0)
        {
            try
            {
                List<ReserveData> list = new List<ReserveData>();
                foreach (ReserveItem item in listView_reserve.SelectedItems)
                {
                    ReserveData reserveInfo = item.ReserveInfo;
                    reserveInfo.RecSetting.Priority = BitConverter.GetBytes(priority0)[0]; ;
                    list.Add(reserveInfo);
                }
                if (list.Count > 0)
                {
                    ErrCode err = (ErrCode)cmd.SendChgReserve(list);
                    if (err == ErrCode.CMD_ERR_CONNECT)
                    {
                        MessageBox.Show("サーバー または EpgTimerSrv に接続できませんでした。");
                    }
                    if (err == ErrCode.CMD_ERR_TIMEOUT)
                    {
                        MessageBox.Show("EpgTimerSrvとの接続にタイムアウトしました。036");
                    }
                    if (err != ErrCode.CMD_SUCCESS)
                    {
                        MessageBox.Show("チューナー一覧の取得でエラーが発生しました。");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        void setRecMode(int redMode0)
        {
            try
            {
                List<ReserveData> list = new List<ReserveData>();
                foreach (ReserveItem item in listView_reserve.SelectedItems)
                {
                    ReserveData reserveInfo = item.ReserveInfo;
                    reserveInfo.RecSetting.RecMode = BitConverter.GetBytes(redMode0)[0];
                    list.Add(reserveInfo);
                }
                if (list.Count > 0)
                {
                    ErrCode err = (ErrCode)cmd.SendChgReserve(list);
                    if (err == ErrCode.CMD_ERR_CONNECT)
                    {
                        MessageBox.Show("サーバー または EpgTimerSrv に接続できませんでした。");
                    }
                    if (err == ErrCode.CMD_ERR_TIMEOUT)
                    {
                        MessageBox.Show("EpgTimerSrvとの接続にタイムアウトしました。037");
                    }
                    if (err != ErrCode.CMD_SUCCESS)
                    {
                        MessageBox.Show("チューナー一覧の取得でエラーが発生しました。");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        private void MenuItem_Click_ChangeRecMode(object sender, RoutedEventArgs e)
        {
            try
            {
                List<ReserveData> list = new List<ReserveData>();
                foreach (ReserveItem item in listView_reserve.SelectedItems)
                {
                    ReserveData reserveInfo = item.ReserveInfo;

                    if (item.ReserveInfo.RecSetting.RecMode == 5)
                    {
                        // 無効 => 予約
                        RecSettingData defSet = new RecSettingData();
                        Settings.GetDefRecSetting(0, ref defSet);
                        item.ReserveInfo.RecSetting.RecMode = defSet.RecMode;
                    }
                    else
                    {
                        //予約 => 無効
                        item.ReserveInfo.RecSetting.RecMode = 5;
                    }

                    list.Add(reserveInfo);
                }
                if (list.Count > 0)
                {
                    ErrCode err = (ErrCode)cmd.SendChgReserve(list);
                    if (err == ErrCode.CMD_ERR_CONNECT)
                    {
                        MessageBox.Show("サーバー または EpgTimerSrv に接続できませんでした。");
                    }
                    if (err == ErrCode.CMD_ERR_TIMEOUT)
                    {
                        MessageBox.Show("EpgTimerSrvとの接続にタイムアウトしました。038");
                    }
                    if (err != ErrCode.CMD_SUCCESS)
                    {
                        MessageBox.Show("チューナー一覧の取得でエラーが発生しました。");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        private void MenuItem_Click_ReserveDetail(object sender, RoutedEventArgs e)
        {
            ReserveItem item1 = this.listView_reserve.SelectedItem as ReserveItem;
            if (item1 != null)
            {
                this._popupWindow.show(
                    CommonManager.Instance.ConvertReserveText(item1.ReserveInfo));
            }
        }

        private void MenuItem_Click_PogramDetail(object sender, RoutedEventArgs e)
        {
            ReserveItem item1 = this.listView_reserve.SelectedItem as ReserveItem;
            if (item1 != null)
            {
                this._popupWindow.show(item1.ProgramDetail);
            }
        }

        private void MenuItem_Click_Google(object sender, RoutedEventArgs e)
        {
            ReserveItem item1 = this.listView_reserve.SelectedItem as ReserveItem;
            if (item1 != null)
            {
                this._popupWindow.google(item1.EventName);
            }
        }

        private void MenuItem_Click_ProgramTable(object sender, RoutedEventArgs e)
        {
            ReserveItem item1 = this.listView_reserve.SelectedItem as ReserveItem;
            if (item1 != null)
            {
                BlackoutWindow.selectedReserveItem = item1;
                this._mainWindow.moveTo_tabItem_epg();
            }
        }

        public bool isInDesignMode
        {
            get
            {
                return (System.Diagnostics.Process.GetCurrentProcess().ProcessName == "devenv");
            }
        }


    }
}
