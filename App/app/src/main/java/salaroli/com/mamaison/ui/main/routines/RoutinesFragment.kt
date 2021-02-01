package salaroli.com.mamaison.ui.main.routines

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import salaroli.com.mamaison.R

class RoutinesFragment : Fragment() {

    private lateinit var routinesViewModel: RoutinesViewModel

    override fun onCreateView(
            inflater: LayoutInflater,
            container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View? {
        routinesViewModel = ViewModelProvider(this).get(RoutinesViewModel::class.java)
        val root = inflater.inflate(R.layout.fragment_routines, container, false)
        val textView: TextView = root.findViewById(R.id.text_notifications)
        routinesViewModel.text.observe(viewLifecycleOwner, Observer {
            textView.text = it
        })
        return root
    }
}