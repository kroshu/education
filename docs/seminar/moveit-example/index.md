# Robot mozgatása MoveIt segítségével

A második részben két példán keresztül ismerkedünk meg a MoveIt alapjaival:

1. Egy objektumot helyezünk el a virtuális térben, amit a robotnak ki kell kerülnie.
2. Megvalósítunk egy egyszerű pick & place alkalmazást.

A kiinduláshoz szükséges kódot a `moveit_example` mappában találjátok. Először a `first_scenario.hpp` fájlt egészítjük ki, ahol egy téglatestet helyezünk el a világban, majd a robotot a túloldalára mozgatjuk. Ezután a `second_scenario.hpp` fájlban egy előre elkészített környezetben kell a lila hengert az egyik asztalról a másikra áthelyezni úgy, hogy az soha ne forduljon fejjel lefelé.
