package salaroli.com.mamaison.utilities

import salaroli.com.mamaison.data.FakeDatabase
import salaroli.com.mamaison.data.FakeDeviceDAO
import salaroli.com.mamaison.repository.SpaceRepository
import salaroli.com.mamaison.ui.main.maison.HomeViewModelFactory

object InjectorUtils {
    fun provideHomeViewModelFactory(): HomeViewModelFactory {
        val spaceRepository = SpaceRepository.getInstance(FakeDatabase.getInstance().deviceDAO)
        return HomeViewModelFactory(spaceRepository)
    }
}