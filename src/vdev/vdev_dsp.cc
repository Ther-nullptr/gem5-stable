//
// Created by lf-z on 4/24/17.
//

#include "vdev/vdev_dsp.hh"
#include "debug/EnergyMgmt.hh"
#include "debug/VirtualDeviceDSP.hh"
#include "debug/MemoryAccess.hh"

VirtualDeviceDSP::DevicePort::DevicePort(const std::string &_name, VirtualDeviceDSP *_vdev)
    : SlavePort(_name, _vdev), vdev(_vdev)
{

}

Tick
VirtualDeviceDSP::DevicePort::recvAtomic(PacketPtr pkt)
{
    assert(vdev);
    return vdev->recvAtomic(pkt);
}

void
VirtualDeviceDSP::DevicePort::recvFunctional(PacketPtr pkt)
{
    assert(vdev);
    vdev->recvFunctional(pkt);
}

bool
VirtualDeviceDSP::DevicePort::recvTimingReq(PacketPtr pkt)
{
    assert(vdev);
    return vdev->recvTimingReq(pkt);
}

void
VirtualDeviceDSP::DevicePort::recvRespRetry()
{
    assert(vdev);
    vdev->recvRespRetry();
}

AddrRangeList
VirtualDeviceDSP::DevicePort::getAddrRanges() const
{
    assert(vdev);

    AddrRangeList rangeList;
    rangeList.push_back(vdev->getAddrRange());

    return rangeList;
}

VirtualDeviceDSP::VirtualDeviceDSP(const Params *p)
    : MemObject(p),
      id(0),
      port(name() + ".port", this),
      cpu(p->cpu),
      range(p->range),
      delay_set(p->delay_set),
      delay_cpu_interrupt(p->delay_cpu_interrupt),
      event_interrupt(this, false, Event::Virtual_Interrupt),
      data_bandwidth(p->data_bandwidth)
{
    trace.resize(0);
    
    dsp.tickPerCycle = clockPeriod();
}

void
VirtualDeviceDSP::init()
{
    MemObject::init();

    if (port.isConnected()) {
        port.sendRangeChange();
    }

    //cpu->registerVDev(delay_recover, id);
    cpu->registerVDev(1, id);
    DPRINTF(VirtualDeviceDSP, "Virtual Device started with range: %#lx - %#lx\n",
            range.start(), range.end());

    execution_state = STATE_POWEROFF;
}

void
VirtualDeviceDSP::triggerInterrupt()
{
   DPRINTF(VirtualDeviceDSP, "Virtual device triggers an interrupt.\n");
   execution_state = STATE_IDLE; 
   assert(pmem & VDEV_WORK);
   /* Change register byte. */
   pmem |= VDEV_READY;
   pmem &= ~VDEV_WORK;

   if (dsp.finished)
   {
      pmem |= VDEV_FINISH;
   }
   /* Tell cpu. */
   cpu->virtualDeviceInterrupt(delay_cpu_interrupt);
   cpu->virtualDeviceEnd(id);         
}

Tick VirtualDeviceDSP::access(PacketPtr pkt)
{
    DPRINTF(MemoryAccess, "Virtual Device accessed at %#lx.\n", pkt->getAddr());
    Addr offset = pkt->getAddr() - range.start();
    if (pkt->isRead())
    {
        if (offset == 0)
        {
            *(pkt->getPtr<uint8_t>()) = pmem;
        }
        else if (offset == 1)
        {
            *(pkt->getPtr<uint8_t>()) = dsp.counter;
        }
        else
        {
            //normal read
            if (offset < 1024 * 512 + 2)
            {
                memcpy(pkt->getPtr<uint8_t>(), (uint8_t *)dsp.mem1 + offset - 2, pkt->getSize());
            }
            else
            {
                memcpy(pkt->getPtr<uint8_t>(), (uint8_t *)dsp.mem2 + offset - 1024 * 512 - 2, pkt->getSize());
            }
        }
        cpu->virtualDeviceSet(data_bandwidth * pkt->getSize());
    }
    else if (pkt->isWrite())
    {
        const uint8_t *pkt_addr = pkt->getConstPtr<uint8_t>();
        if (offset == 0)
        {
            /* Might be a request. */
            if (*pkt_addr & VDEV_SET)
            {
                /* Request */
                if (pmem & VDEV_WORK)
                {
                    /* Request fails because the vdev is working. */
                    DPRINTF(VirtualDeviceDSP, "Request discarded!\n");
                }
                else
                {
                    /* Request succeeds. */
                    execution_state = STATE_ACTIVE; // The virtual device enter/keep in the active status.
                    DPRINTF(VirtualDeviceDSP, "Virtual Device starts working.\n");

                    /* Set the virtual device to working mode */
                    pmem |= VDEV_WORK;
                    pmem &= ~VDEV_READY;
                    pmem &= ~VDEV_FINISH;

                    /* Schedule interrupt. */
                    delay_self = dsp.run();
                    DPRINTF(VirtualDeviceDSP, "Need Ticks:%i, Cycles:%i.\n",
                            delay_self,
                            ticksToCycles(delay_self));
                    schedule(event_interrupt, curTick() + delay_set + delay_self);
                    cpu->virtualDeviceSet(delay_set);
                    cpu->virtualDeviceStart(id);
                }
            }
            else
            {
                /* Not a request, but the first byte cannot be written. */
            }
        }
        else if (offset == 1)
        {
            dsp.counter = *(pkt->getConstPtr<uint8_t>());
        }
        else
        {
            /* Normal write. */
            if (offset < 1024 * 512 + 2)
            {
                memcpy((uint8_t *)dsp.mem1 + offset - 2, pkt_addr, pkt->getSize());
            }
            else 
            {
                memcpy((uint8_t *)dsp.mem2 + offset - 1024 * 512 - 2, pkt_addr, pkt->getSize());
            }
            cpu->virtualDeviceSet(data_bandwidth * pkt->getSize());
        }
    }
    return 0;
}

void 
VirtualDeviceDSP::tick()
{
}


int
VirtualDeviceDSP::handleMsg(const EnergyMsg &msg)
{
    DPRINTF(EnergyMgmt, "Device handleMsg called at %lu, msg.type=%d\n", curTick(), msg.type);
    return 0;
}

BaseSlavePort&
VirtualDeviceDSP::getSlavePort(const std::string &if_name, PortID idx)
{
    if (if_name == "port") {
        return port;
    } else {
        return MemObject::getSlavePort(if_name, idx);
    }
}

AddrRange
VirtualDeviceDSP::getAddrRange() const
{
    return range;
}

Tick
VirtualDeviceDSP::recvAtomic(PacketPtr pkt)
{
    return access(pkt);
}

void
VirtualDeviceDSP::recvFunctional(PacketPtr pkt)
{
    fatal("Functional access is not supported now.");
}

bool
VirtualDeviceDSP::recvTimingReq(PacketPtr pkt)
{
    fatal("Timing access is not supported now.");
}

void
VirtualDeviceDSP::recvRespRetry()
{
    fatal("Timing access is not supported now.");
}

VirtualDeviceDSP *
VirtualDeviceDSPParams::create()
{
    return new VirtualDeviceDSP(this);
}