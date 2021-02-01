package salaroli.com.mamaison.data

class FakeDatabase private constructor() {

    var deviceDAO = FakeDeviceDAO()
        private set

    companion object {
        @Volatile private var instance: FakeDatabase? = null
        fun getInstance() =
            instance ?: synchronized(this) {
                instance ?: FakeDatabase().also { instance = it }
            }
    }
}