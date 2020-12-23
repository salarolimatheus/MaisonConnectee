package salaroli.com.mamaison.ui.maison

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import salaroli.com.expandablecardview.ExpandableCardView
import salaroli.com.mamaison.R

class RoomAdapter(private val room: List<Room>):RecyclerView.Adapter<RoomAdapter.RoomViewHolder>() {

    class RoomViewHolder(itemView: View):RecyclerView.ViewHolder(itemView) {
        val cardView: ExpandableCardView = itemView.findViewById(R.id.cardview)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RoomViewHolder {
        val itemView = LayoutInflater.from(parent.context).inflate(R.layout.cardview, parent, false)
        return RoomViewHolder(itemView)
    }

    override fun onBindViewHolder(holder: RoomViewHolder, position: Int) {
        val currentItem = room[position]
        holder.cardView.setText(currentItem.nameRoom, currentItem.infoRoom)
        holder.cardView.setDevices(currentItem.devices)
        if (currentItem.firstAction) {
            holder.cardView.enableFirstAction()
            holder.cardView.setFirstAction(currentItem.resourceFirstAction)
        } else holder.cardView.disableFirstAction()
        if (currentItem.secondAction) {
            holder.cardView.enableSecondAction()
            holder.cardView.setSecondAction(currentItem.resourceSecondAction)
        } else holder.cardView.disableSecondAction()
    }

    override fun getItemCount() = room.size
}