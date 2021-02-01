package salaroli.com.mamaison.ui.main.maison

import salaroli.com.expandablecardview.RecyclerView.Space
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import salaroli.com.expandablecardview.IotDevice
import salaroli.com.mamaison.repository.SpaceRepository

class HomeViewModel(private val spaceReposity: SpaceRepository) : ViewModel() {

    fun getSpaces() = spaceReposity.getSpaces()

    fun addSpace(space: Space) = spaceReposity.addSpace(space)

}