package salaroli.com.mamaison.ui.maison

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.animation.AnimationUtils
import android.widget.ImageButton
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.google.android.material.floatingactionbutton.FloatingActionButton
import salaroli.com.mamaison.DevicesActivity
import salaroli.com.mamaison.PreferencesActivity
import salaroli.com.mamaison.R
import salaroli.com.mamaison.R.color.*


class HomeFragment : Fragment(), View.OnClickListener {

    private lateinit var homeViewModel: HomeViewModel
    private lateinit var fabHome: FloatingActionButton
    private lateinit var fabLamp: FloatingActionButton
    private lateinit var fabTemp: FloatingActionButton
    private lateinit var fabWindow: FloatingActionButton
    private lateinit var addButton: ImageButton
    private lateinit var preferencesButton: ImageButton

    override fun onCreateView(
            inflater: LayoutInflater,
            container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View? {
        homeViewModel = ViewModelProvider(this).get(HomeViewModel::class.java)
        val view = inflater.inflate(R.layout.fragment_home, container, false)
        fabHome = view.findViewById(R.id.fab_home)
        fabLamp = view.findViewById(R.id.fab_lamp)
        fabTemp = view.findViewById(R.id.fab_temperature)
        addButton = view.findViewById(R.id.add_home)
        fabWindow = view.findViewById(R.id.fab_window)
        preferencesButton = view.findViewById(R.id.preferences_home)

        fabHome.setOnClickListener(this)
        addButton.setOnClickListener(this)
        preferencesButton.setOnClickListener(this)
        return view
    }

    override fun onClick(view: View) {
        when(view.id) {
            R.id.fab_home -> fabHomeAction()
            R.id.add_home -> addHomeAction()
            R.id.preferences_home -> preferencesHomeAction()
        }
    }

    private fun addHomeAction() {
        val intentDevices = Intent(context, DevicesActivity::class.java)
        startActivity(intentDevices)
        requireActivity().overridePendingTransition(R.anim.slide_in_left, R.anim.slide_out_right)
    }

    private fun preferencesHomeAction() {
        val intentPreferences = Intent(context, PreferencesActivity::class.java)
        startActivity(intentPreferences)
        requireActivity().overridePendingTransition(R.anim.slide_in_right, R.anim.slide_out_left)
    }

    private fun fabHomeAction() {
        val animationUp = AnimationUtils.loadAnimation(context, R.anim.jump_from_down)
        val animationDown = AnimationUtils.loadAnimation(context, R.anim.jump_to_down)
        val rotation = AnimationUtils.loadAnimation(context, R.anim.rotate)
        if(fabLamp.visibility == View.INVISIBLE) {
            fabHome.startAnimation(rotation)
            fabHome.backgroundTintList = ContextCompat.getColorStateList(requireContext(), black)
            fabHome.imageTintList = ContextCompat.getColorStateList(requireContext(), white)
            fabLamp.startAnimation(animationUp)
            fabTemp.startAnimation(animationUp)
            fabWindow.startAnimation(animationUp)
            fabLamp.visibility = View.VISIBLE
            fabTemp.visibility = View.VISIBLE
            fabWindow.visibility = View.VISIBLE
        } else {
            fabLamp.startAnimation(animationDown)
            fabTemp.startAnimation(animationDown)
            fabWindow.startAnimation(animationDown)
            fabLamp.visibility = View.INVISIBLE
            fabTemp.visibility = View.INVISIBLE
            fabWindow.visibility = View.INVISIBLE
            fabHome.startAnimation(rotation)
            fabHome.backgroundTintList = ContextCompat.getColorStateList(requireContext(), secondaryColor)
            fabHome.imageTintList = ContextCompat.getColorStateList(requireContext(), black)
        }
    }
}