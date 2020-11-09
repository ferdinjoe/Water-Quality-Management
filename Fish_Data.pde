import processing.serial.*;
import java.io.FileWriter;
import java.io.*;
FileWriter fw;
BufferedWriter bw;
Serial myPort;  // The serial port

void setup() {
  // List all the available serial ports:
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[0], 230400);
}

void draw() {
  while (myPort.available() > 0) {
    String inBuffer = myPort.readString();
    delay(1000);
    String year=str(year());
    String month=str(month());
    String day=str(day());
    String hour=str(hour());
    String minute=str(minute());
    String second=str(second());
    String val=year+","+month+","+day+","+hour+","+minute+","+second+",";
    
    if (inBuffer != null) {
      
      String val1=val+inBuffer;
      
      File file =new File("fishdata.csv");
      try{
         
//chemin = dataPath;
// positions.txt== your file;
 
          if(!file.exists()){
            file.createNewFile();
          }
 
          FileWriter fw = new FileWriter(file,true);///true = append
          BufferedWriter bw = new BufferedWriter(fw);
          PrintWriter pw = new PrintWriter(bw);
          
          
          
          if(inBuffer.length()>30 && inBuffer.length()<55)
          {
          pw.write(val1);
          println(val1);
 
          pw.close();
          }
 
 
       }catch(IOException ioe){
           System.out.println("Exception ");
           ioe.printStackTrace();
      }
      
    }
  }
}
