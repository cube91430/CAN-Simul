 void bit_bit(){
/*    //HIGH BEAM - LOW BEAM - DOOR       -     0x0d000020
    //Battery Fault
     bitWrite(err, 7, butState5);      //HIGH Beam ON!
     bitWrite(err, 6, butState6);
     bitWrite(err, 5, 0);      //LOW Beam ON!
     bitWrite(err, 4, 0);
     bitWrite(err, 3, 0);
     bitWrite(err, 2, 0);
     bitWrite(err, 1, 0);      //SafetyBelt
     bitWrite(err, 0, 0);

     //Hardware Fault
     bitWrite(err_hrd, 7, butState8);      //HIGH Beam ON!
     bitWrite(err_hrd, 6, butState7);
     bitWrite(err_hrd, 5, 0);      //LOW Beam ON!
     bitWrite(err_hrd, 4, 0);
     bitWrite(err_hrd, 3, 0);
     bitWrite(err_hrd, 2, 0);
     bitWrite(err_hrd, 1, 0);      //SafetyBelt
     bitWrite(err_hrd, 0, 0);

     //Other Fault
     bitWrite(err_oth, 7, butState1);      //HIGH Beam ON!
     bitWrite(err_oth, 6, butState2);
     bitWrite(err_oth, 5, 0);      //LOW Beam ON!
     bitWrite(err_oth, 4, 0);
     bitWrite(err_oth, 3, 0);
     bitWrite(err_oth, 2, 0);
     bitWrite(err_oth, 1, 0);      //SafetyBelt
     bitWrite(err_oth, 0, 0);
     
     //Byte 0
     
     bitWrite(yu, 7, butState7);      //HIGH Beam ON!
     bitWrite(yu, 6, butState8);
     bitWrite(yu, 5, butState8);      //LOW Beam ON!
     bitWrite(yu, 4, 0);
     bitWrite(yu, 3, 0);
     bitWrite(yu, 2, 0);
     bitWrite(yu, 1, butState10);      //SafetyBelt
     bitWrite(yu, 0, 0);

     //Byte 1

     bitWrite(fr, 7, 0);    
     bitWrite(fr, 6, 0);
     bitWrite(fr, 5, butState9);      //Rear Door OPEN!
     bitWrite(fr, 4, butState9);      //Front Door OPEN!
     bitWrite(fr, 3, 0);
     bitWrite(fr, 2, 0);
     bitWrite(fr, 1, 0);      
     bitWrite(fr, 0, butState10);      //Safety bel

    //2nd System Status         -   0x0cf00500
   
    bitWrite(sen, 7, butState5);        //Hazard LEFT ON!
    bitWrite(sen, 6, butState6);        //Hazard RIGHT ON!
    bitWrite(sen, 5, 0);        
    bitWrite(sen, 4, 0);
    bitWrite(sen, 3, 0);
    bitWrite(sen, 2, 0);
    bitWrite(sen, 1, 0);
    bitWrite(sen, 0, 0);

    //1st System Status          - 0x0cf00500
    
     bitWrite(gear, 7, 0);
     bitWrite(gear, 6, 0);
     bitWrite(gear, 5, 0);
     bitWrite(gear, 4, butState11);      //Foot Brake OFF!
     bitWrite(gear, 3, butState12);      //Hand Brake OFF!
     bitWrite(gear, 2, 0);
     bitWrite(gear, 1, 1);      //gear NEUTRAL
     bitWrite(gear, 0, 0);    

    if (butState1 == 1){
       
     bitWrite(gear, 7, 0);
     bitWrite(gear, 6, 0);
     bitWrite(gear, 5, 0);
     bitWrite(gear, 4, butState11);      //Foot Brake OFF!
     bitWrite(gear, 3, butState12);      //Hand Brake OFF!
     bitWrite(gear, 2, 0);
     bitWrite(gear, 1, 1);      //gear DRIVE
     bitWrite(gear, 0, 0);
    }

    else if (butState2 == 1) {
      bitWrite(gear, 7, 0);
     bitWrite(gear, 6, 0);
     bitWrite(gear, 5, 0);
     bitWrite(gear, 4, butState11);      //Foot Brake OFF!
     bitWrite(gear, 3, butState12);      //Hand Brake OFF!
     bitWrite(gear, 2, 0);
     bitWrite(gear, 1, 1);      //gear REVERSE
     bitWrite(gear, 0, 1);
    }

    else if (butState3 == 1) {
      bitWrite(gear, 7, 0);
     bitWrite(gear, 6, 0);
     bitWrite(gear, 5, 0);
     bitWrite(gear, 4, butState11);      //Foot Brake OFF!
     bitWrite(gear, 3, butState12);      //Hand Brake OFF!
     bitWrite(gear, 2, 1);
     bitWrite(gear, 1, 0);      //gear NEUTRAL
     bitWrite(gear, 0, 0);
    }
    else if (butState1 == 0){
         
     bitWrite(gear, 7, 0);
     bitWrite(gear, 6, 0);
     bitWrite(gear, 5, 0);
     bitWrite(gear, 4, butState11);      //Foot Brake OFF!
     bitWrite(gear, 3, butState12);      //Hand Brake OFF!
     bitWrite(gear, 2, 0);
     bitWrite(gear, 1, 0);
     bitWrite(gear, 0, 0);
    }

    else if (butState2 == 0){
         
     bitWrite(gear, 7, 0);
     bitWrite(gear, 6, 0);
     bitWrite(gear, 5, 0);
     bitWrite(gear, 4, butState11);      //Foot Brake OFF!
     bitWrite(gear, 3, butState12);      //Hand Brake OFF!
     bitWrite(gear, 2, 0);
     bitWrite(gear, 1, 0);
     bitWrite(gear, 0, 0);
    }

    else if (butState3 == 0){
         
     bitWrite(gear, 7, 0);
     bitWrite(gear, 6, 0);
     bitWrite(gear, 5, 0);
     bitWrite(gear, 4, butState11);      //Foot Brake OFF!
     bitWrite(gear, 3, butState12);      //Hand Brake OFF!
     bitWrite(gear, 2, 0);
     bitWrite(gear, 1, 0);
     bitWrite(gear, 0, 0);
    }     


/*   switch (grind){
     case 0:
     bitWrite(gear, 7, 0);
     bitWrite(gear, 6, 0);
     bitWrite(gear, 5, 0);
     bitWrite(gear, 4, 0);      //Foot Brake OFF!
     bitWrite(gear, 3, 1);      //Hand Brake OFF!
     bitWrite(gear, 2, 0);
     bitWrite(gear, 1, 0);      //gear Park
     bitWrite(gear, 0, 0);
      break;

     case 1:
     bitWrite(gear, 7, 0);
     bitWrite(gear, 6, 0);
     bitWrite(gear, 5, 0);
     bitWrite(gear, 4, 0);      //Foot Brake OFF!
     bitWrite(gear, 3, 1);      //Hand Brake OFF!
     bitWrite(gear, 2, 0);
     bitWrite(gear, 1, 1);      //gear Reverse
     bitWrite(gear, 0, 1);
      break;
   }    */
 
}
