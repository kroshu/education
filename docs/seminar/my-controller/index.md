# Robot vezérlése saját kontrollerrel

Ebben a feladatban egy olyan kontrollert kell készíteni, amely egy ROS topicon keresztül kapott csuklópozíciókat követve mozgatja a robotot. A kontrollernek képesnek kell lennie új célpozíció fogadására akkor is, ha az előző célt még nem érte el. További követelmény, hogy a kontroller legyen paraméterezhető egy konfigurációs fájlon keresztül. A kiinduló ROS csomagot a `my_controller` mappában találjátok.
