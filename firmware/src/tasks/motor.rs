use crate::types::SharedCan;
use common::types::RxEvent;
use defmt::*;
use drivers::vesc::Vesc;
use embassy_sync::{blocking_mutex::raw::CriticalSectionRawMutex, channel::Sender};

#[embassy_executor::task]
pub async fn run(can: SharedCan, sender: Sender<'static, CriticalSectionRawMutex, RxEvent, 128>) {
    // Init Driver
    let mut vesc = Vesc::new(can);
    info!("VESC Task Started");

    loop {
        // Read Status (this blocks until a valid status message arrives)
        match vesc.read_status().await {
            Ok(status) => {
                // Log and Send
                debug!("VESC: {} RPM, {} mA", status.speed(), status.current());

                let msg = RxEvent::Motor(status);
                if sender.try_send(msg).is_err() {
                    warn!("RxChannel full, dropping VESC status");
                }
            }
            Err(e) => {
                error!("VESC Read Error: {:?}", e);
            }
        }
    }
}
