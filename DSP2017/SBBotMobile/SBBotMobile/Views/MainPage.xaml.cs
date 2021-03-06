﻿using System;
using Xamarin.Forms;

namespace SBBotMobile.Views
{
    public partial class MainPage : ContentPage
    {
        public MainPage()
        {
            InitializeComponent();
            BindingContext = App.Locator.Main;
        }

        private async void Button_OnClicked(object sender, EventArgs e)
        {
            await Navigation.PushModalAsync(new SetParametersPage());
        }
    }
}
