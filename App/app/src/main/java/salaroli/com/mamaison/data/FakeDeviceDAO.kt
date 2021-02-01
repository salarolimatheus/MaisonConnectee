package salaroli.com.mamaison.data

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import salaroli.com.expandablecardview.IotDevice
import salaroli.com.expandablecardview.RecyclerView.Space
import salaroli.com.mamaison.R
import java.util.ArrayList

class FakeDeviceDAO {
    private val listSpaces: MutableList<Space> = ArrayList()
    private val spaces = MutableLiveData<List<Space>>()


    init {
        val devices: MutableList<IotDevice> = ArrayList()
        devices.add(IotDevice(R.drawable.lamp, "Object 1"))
        devices.add(IotDevice(R.drawable.window_open, "Object 2"))
        devices.add(IotDevice(R.drawable.lamp, "Object 3"))
        devices.add(IotDevice(R.drawable.window_open, "Object 4"))

        listSpaces.add(Space(1, "Bedroom", "", devices, true, true,R.drawable.lamp, R.drawable.window_open))
        listSpaces.add(Space(2, "Kitchen", "", devices, true, true,R.drawable.lamp, R.drawable.window_open))

        spaces.value = listSpaces
    }

    fun addSpace(space: Space) {
        listSpaces.add(space)
        spaces.value = listSpaces
    }

    fun getSpaces() = spaces as LiveData<List<Space>>
}