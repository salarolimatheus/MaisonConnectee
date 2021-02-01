package salaroli.com.mamaison.repository

import salaroli.com.expandablecardview.RecyclerView.Space
import salaroli.com.mamaison.data.FakeDeviceDAO

class SpaceRepository private constructor(private val deviceDAO: FakeDeviceDAO) {

    companion object {
        @Volatile private var instance: SpaceRepository? = null
        fun getInstance(deviceDAO: FakeDeviceDAO)
            = instance ?: synchronized(this) {
                instance ?: SpaceRepository(deviceDAO).also { instance = it }
            }
    }

    fun addSpace(space: Space) {
        deviceDAO.addSpace(space)
    }

    fun getSpaces() = deviceDAO.getSpaces()
}
