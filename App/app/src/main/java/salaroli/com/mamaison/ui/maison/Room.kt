package salaroli.com.mamaison.ui.maison

import salaroli.com.expandablecardview.IotDevice

data class Room(val nameRoom: String, val infoRoom: String, val devices: List<IotDevice>,
                val firstAction: Boolean, val secondAction: Boolean, val resourceFirstAction: Int, val resourceSecondAction: Int)

