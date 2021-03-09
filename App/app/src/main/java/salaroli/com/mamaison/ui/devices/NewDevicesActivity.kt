package salaroli.com.mamaison.ui.devices

import android.content.Context
import android.content.Intent
import android.net.wifi.WifiManager
import android.os.Build
import android.os.Bundle
import android.provider.Settings
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModelProvider
import androidx.navigation.NavController
import androidx.navigation.findNavController
import salaroli.com.mamaison.R
import salaroli.com.mamaison.databinding.ActivityDevicesBinding
import salaroli.com.mamaison.ui.devices.fragments.AddDeviceViewModel

class NewDevicesActivity : AppCompatActivity(), View.OnClickListener {
    private lateinit var binding: ActivityDevicesBinding
    private lateinit var wifiManager: WifiManager
    private lateinit var navController: NavController
    private lateinit var viewModel: NewDevicesActivityViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityDevicesBinding.inflate(layoutInflater)
        setContentView(binding.root)
        wifiManager = applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
        navController = findNavController(R.id.nav_device_fragment)
        viewModel = ViewModelProvider(this).get(NewDevicesActivityViewModel::class.java)

        binding.fabAddRoom.setOnClickListener(this)
        binding.fabShowHome.setOnClickListener(this)
        binding.fabSearchDevice.setOnClickListener(this)
    }

    override fun onBackPressed() {
        super.onBackPressed()
        overridePendingTransition(R.anim.slide_in_right, R.anim.slide_out_left)
    }

    override fun onClick(view: View) {
        when(view.id) {
            R.id.fab_search_device -> addDeviceProcess()
            R.id.fab_add_room -> addRoomProcess()
            R.id.fab_show_home -> showAllProcess()
        }
    }

    private fun addDeviceProcess() {
        navController.navigate(R.id.navigation_adddevice)
        if (!wifiManager.isWifiEnabled) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) startActivityForResult(Intent(Settings.Panel.ACTION_WIFI), 545)
            else wifiManager.setWifiEnabled(true)
        }
    }

    private fun addRoomProcess() {
        navController.navigate(R.id.navigation_addroom)
    }

    private fun showAllProcess() {
        navController.navigate(R.id.navigation_showhome)
    }
}