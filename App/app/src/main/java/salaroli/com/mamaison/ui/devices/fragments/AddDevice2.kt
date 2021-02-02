package salaroli.com.mamaison.ui.devices.fragments

import androidx.lifecycle.ViewModelProvider
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import salaroli.com.mamaison.R

class AddDevice2 : Fragment() {

    companion object {
        fun newInstance() = AddDevice2()
    }

    private lateinit var viewModel: AddDevice2ViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_add_device2, container, false)
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        viewModel = ViewModelProvider(this).get(AddDevice2ViewModel::class.java)
        // TODO: Use the ViewModel
    }

}