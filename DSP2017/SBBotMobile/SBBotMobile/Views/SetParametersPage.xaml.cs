using System;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace SBBotMobile.Views
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SetParametersPage : ContentPage
    {
        public SetParametersPage()
        {
            InitializeComponent();
        }

        private async void Button_OnClicked(object sender, EventArgs e)
        {
            await Navigation.PopModalAsync();
        }
    }
}
