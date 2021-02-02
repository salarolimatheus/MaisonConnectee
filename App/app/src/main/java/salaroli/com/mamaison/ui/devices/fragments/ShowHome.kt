package salaroli.com.mamaison.ui.devices.fragments

import androidx.lifecycle.ViewModelProvider
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import salaroli.com.mamaison.R

class ShowHome : Fragment() {

    companion object {
        fun newInstance() = ShowHome()
    }

    private lateinit var viewModel: ShowHomeViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_show_home, container, false)
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        viewModel = ViewModelProvider(this).get(ShowHomeViewModel::class.java)
        // TODO: Use the ViewModel
    }

}