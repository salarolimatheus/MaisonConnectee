package salaroli.com.mamaison.ui.maison

import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.animation.AnimationUtils
import android.widget.ImageButton
import android.widget.Toast
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.google.android.material.floatingactionbutton.FloatingActionButton
import salaroli.com.expandablecardview.ExpandableCardView
import salaroli.com.expandablecardview.ExpandableCardView.InterfaceExpandableCardView
import salaroli.com.expandablecardview.IotDevice
import salaroli.com.mamaison.DevicesActivity
import salaroli.com.mamaison.PreferencesActivity
import salaroli.com.mamaison.R
import salaroli.com.mamaison.R.color.*
import java.util.*

class HomeFragment : Fragment(), View.OnClickListener {

    private lateinit var homeViewModel: HomeViewModel
    private lateinit var fabHome: FloatingActionButton
    private lateinit var fabLamp: FloatingActionButton
    private lateinit var fabTemp: FloatingActionButton
    private lateinit var fabWindow: FloatingActionButton
    private lateinit var addButton: ImageButton
    private lateinit var preferencesButton: ImageButton
    private lateinit var cardView1: ExpandableCardView
    private lateinit var cardView2: ExpandableCardView

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
        //recycleView = view.findViewById(R.id.list_home)
        cardView1 = view.findViewById(R.id.cardview_principal)
        cardView2 = view.findViewById(R.id.cardview_secundario)

        fabHome.setOnClickListener(this)
        addButton.setOnClickListener(this)
        preferencesButton.setOnClickListener(this)
        updateCards()
        return view
    }

    private fun updateCards() {
        val devices: MutableList<IotDevice> = ArrayList()
        devices.add(IotDevice(R.drawable.lamp, "Objeto 1"))
        devices.add(IotDevice(R.drawable.window_open, "Objeto 2"))
        devices.add(IotDevice(R.drawable.lamp, "Objeto 3"))
        devices.add(IotDevice(R.drawable.window_open, "Objeto 4"))

        cardView1.setText("Room", "22.18 °C")
        cardView1.enableFirstAction()
        cardView1.enableSecondAction()

        cardView1.setInterfaceListener(object : InterfaceExpandableCardView {
            override fun onItemSelect(device: IotDevice) {
                Toast.makeText(context, device.textDevice, Toast.LENGTH_SHORT).show()
            }

            override fun noDevice() {
                Toast.makeText(context, "No Device", Toast.LENGTH_SHORT).show()
            }
        })

        cardView1.setFirstAction(R.drawable.lamp)
        cardView1.setSecondAction(R.drawable.window_open)
        cardView1.setDevices(devices)

        cardView2.setPrimaryText("Kitchen")
        cardView2.setSecondaryText("")
        cardView2.disableFirstAction()
        cardView2.disableSecondAction()
        cardView2.setDevices(devices);

        cardView2.setInterfaceListener(object : InterfaceExpandableCardView {
            override fun onItemSelect(device: IotDevice) {
                Toast.makeText(context, device.textDevice, Toast.LENGTH_SHORT).show()
            }

            override fun noDevice() {
                Toast.makeText(context, "No Device", Toast.LENGTH_SHORT).show()
            }
        })
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
            fabHome.backgroundTintList = ContextCompat.getColorStateList(
                requireContext(),
                secondaryColor
            )
            fabHome.imageTintList = ContextCompat.getColorStateList(requireContext(), black)
        }
    }
}