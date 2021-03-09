package salaroli.com.mamaison.ui.main.maison

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import salaroli.com.mamaison.repository.SpaceRepository

class
HomeViewModelFactory(private val spaceRepository: SpaceRepository)
    : ViewModelProvider.NewInstanceFactory() {

    @Suppress("UNCHECKED_CAST")
    override fun <T : ViewModel?> create(modelClass: Class<T>): T {
        return HomeViewModel(spaceRepository) as T
    }
}