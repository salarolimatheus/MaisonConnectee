package salaroli.com.mamaison.ui.main.maison

import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.animation.AnimationUtils
import android.widget.Toast
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.Observer
import androidx.recyclerview.widget.LinearLayoutManager
import salaroli.com.expandablecardview.IotDevice
import salaroli.com.expandablecardview.RecyclerView.Space
import salaroli.com.expandablecardview.RecyclerView.SpaceAdapter
import salaroli.com.expandablecardview.RecyclerView.SpaceAdapter.InterfaceRecyclerCardView
import salaroli.com.mamaison.ui.devices.NewDevicesActivity
import salaroli.com.mamaison.ui.preferences.PreferencesActivity
import salaroli.com.mamaison.R
import salaroli.com.mamaison.R.color.*
import salaroli.com.mamaison.databinding.FragmentHomeBinding
import salaroli.com.mamaison.utilities.InjectorUtils

class HomeFragment : Fragment(), View.OnClickListener {

    private lateinit var homeBinding: FragmentHomeBinding
    private lateinit var homeViewModel: HomeViewModel
    private lateinit var spaceAdapter: SpaceAdapter

    override fun onCreateView (
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val factory = InjectorUtils.provideHomeViewModelFactory()
        homeViewModel = ViewModelProvider(this, factory).get(HomeViewModel::class.java)
        homeBinding = FragmentHomeBinding.inflate(inflater, container, false)
        val view = homeBinding.root
        spaceAdapter = SpaceAdapter(context)

        homeBinding.listHome.apply {
            adapter = spaceAdapter
            layoutManager = LinearLayoutManager(context)
            setHasFixedSize(true)
        }

        homeBinding.fabHome.setOnClickListener(this)
        homeBinding.addHome.setOnClickListener(this)
        homeBinding.preferencesHome.setOnClickListener(this)

        initializeUi()

        return view
    }

    private fun initializeUi() {
        homeViewModel.getSpaces().observe(viewLifecycleOwner, Observer { listSpaces ->
            spaceAdapter.setSpaces(listSpaces)
        })

        spaceAdapter.setInterfaceListener(object: InterfaceRecyclerCardView {
            override fun onItemSelect(space: Space, device: IotDevice) {
                Toast.makeText(context,space.nameSpace + "  " + device.textDevice,Toast.LENGTH_SHORT).show()
                //TODO: AQUI VAI ABRIR ALGUMA COISA
            }
            override fun noDevice(space: Space) {
                Toast.makeText(context, space.nameSpace + ": no devices", Toast.LENGTH_SHORT).show()
                //TODO: AQUI VAI ABRIR ALGUMA COISA
            }
        })
    }

    override fun onClick(view: View) {
        when(view.id) {
            R.id.add_home -> addHomeAction()
            R.id.preferences_home -> preferencesHomeAction()
            R.id.fab_home -> fabHomeAction()
        }
    }
    private fun addHomeAction() {
        startActivity(Intent(context, NewDevicesActivity::class.java))
        requireActivity().overridePendingTransition(R.anim.slide_in_left, R.anim.slide_out_right)
    }
    private fun preferencesHomeAction() {
        startActivity(Intent(context, PreferencesActivity::class.java))
        requireActivity().overridePendingTransition(R.anim.slide_in_right, R.anim.slide_out_left)
    }

    private fun fabHomeAction() {
        val animationUp = AnimationUtils.loadAnimation(context, R.anim.jump_from_down)
        val animationDown = AnimationUtils.loadAnimation(context, R.anim.jump_to_down)
        val rotation = AnimationUtils.loadAnimation(context, R.anim.rotate)
        if(homeBinding.fabLamp.visibility == View.INVISIBLE) {
            homeBinding.fabHome.startAnimation(rotation)
            homeBinding.fabHome.backgroundTintList = ContextCompat.getColorStateList(requireContext(), black)
            homeBinding.fabHome.imageTintList = ContextCompat.getColorStateList(requireContext(), white)
            homeBinding.fabLamp.startAnimation(animationUp)
            homeBinding.fabTemperature.startAnimation(animationUp)
            homeBinding.fabWindow.startAnimation(animationUp)
            homeBinding.fabLamp.visibility = View.VISIBLE
            homeBinding.fabTemperature.visibility = View.VISIBLE
            homeBinding.fabWindow.visibility = View.VISIBLE
        } else {
            homeBinding.fabLamp.startAnimation(animationDown)
            homeBinding.fabTemperature.startAnimation(animationDown)
            homeBinding.fabWindow.startAnimation(animationDown)
            homeBinding.fabLamp.visibility = View.INVISIBLE
            homeBinding.fabTemperature.visibility = View.INVISIBLE
            homeBinding.fabWindow.visibility = View.INVISIBLE
            homeBinding.fabHome.startAnimation(rotation)
            homeBinding.fabHome.backgroundTintList = ContextCompat.getColorStateList(
                    requireContext(),
                    secondaryColor
            )
            homeBinding.fabHome.imageTintList = ContextCompat.getColorStateList(requireContext(), black)
        }
    } //TODO: ARRUMAR AQUI DPS
}