package salaroli.com.mamaison.ui.main.regles

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import salaroli.com.mamaison.R

class RulesFragment : Fragment() {

    private lateinit var rulesViewModel: RulesViewModel

    override fun onCreateView(
            inflater: LayoutInflater,
            container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View? {
        rulesViewModel =
                ViewModelProvider(this).get(RulesViewModel::class.java)
        val root = inflater.inflate(R.layout.fragment_rules, container, false)
        val textView: TextView = root.findViewById(R.id.text_home)
        rulesViewModel.text.observe(viewLifecycleOwner, Observer {
            textView.text = it
        })
        return root
    }
}