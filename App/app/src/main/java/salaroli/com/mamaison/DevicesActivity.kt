package salaroli.com.mamaison

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import salaroli.com.expandablecardview.IotDevice
import salaroli.com.mamaison.ui.maison.Room
import salaroli.com.mamaison.ui.maison.RoomAdapter
import java.util.ArrayList

class DevicesActivity : AppCompatActivity() {

    private lateinit var recyclerView : RecyclerView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_devices)
        recyclerView = findViewById(R.id.recycler_view)
        val roomCards = generateCards()

        recyclerView.apply {
            layoutManager = LinearLayoutManager(context)
            adapter = RoomAdapter(roomCards)
            setHasFixedSize(true)
        }
    }

    override fun onBackPressed() {
        super.onBackPressed()
        overridePendingTransition(R.anim.slide_in_right, R.anim.slide_out_left)
    }

    private fun generateCards(): List<Room> {
        val devices: MutableList<IotDevice> = ArrayList()
        devices.add(IotDevice(R.drawable.lamp, "Objeto 1"))
        devices.add(IotDevice(R.drawable.window_open, "Objeto 2"))
        devices.add(IotDevice(R.drawable.lamp, "Objeto 3"))
        devices.add(IotDevice(R.drawable.window_open, "Objeto 4"))
        devices.add(IotDevice(R.drawable.lamp, "Objeto 5"))
        devices.add(IotDevice(R.drawable.window_open, "Objeto 6"))
        devices.add(IotDevice(R.drawable.lamp, "Objeto 7"))
        devices.add(IotDevice(R.drawable.window_open, "Objeto 8"))

        val roomList = ArrayList<Room>()
        roomList += Room("Bedroom", "", devices, true, true, R.drawable.ic_baseline_scatter_plot_24, R.drawable.ic_baseline_sensor_window_24)
        roomList += Room("Kitchen", "", devices, false, false, 0, 0)
        return roomList
    }
}