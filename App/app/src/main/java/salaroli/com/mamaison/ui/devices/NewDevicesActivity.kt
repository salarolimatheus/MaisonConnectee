package salaroli.com.mamaison.ui.devices

import android.content.Context
import android.content.Intent
import android.net.wifi.WifiManager
import android.os.Build
import android.os.Bundle
import android.provider.Settings
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import salaroli.com.mamaison.R
import salaroli.com.mamaison.databinding.ActivityDevicesBinding

class NewDevicesActivity : AppCompatActivity(), View.OnClickListener {
    private lateinit var binding: ActivityDevicesBinding
    private lateinit var wifiManager: WifiManager

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityDevicesBinding.inflate(layoutInflater)
        setContentView(binding.root)
        wifiManager = applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
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
            R.id.fab_show_all -> showAllProcess()
        }
    }

    private fun addDeviceProcess() {
        if (!wifiManager.isWifiEnabled) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) startActivityForResult(Intent(Settings.Panel.ACTION_WIFI), 545)
            else wifiManager.setWifiEnabled(true)
        }
    }

    private fun addRoomProcess() {
        TODO("Not yet implemented")
    }

    private fun showAllProcess() {
        TODO("Not yet implemented")
    }
}