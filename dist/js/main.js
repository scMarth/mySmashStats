// var statsDiv = $('#stats-div')[0];
var matchesDiv = $('#matches-div')[0];


$(window).on("load", function(){
   main();
});

function getMedalImgString(medalStr){
   var output = "";

   switch (medalStr){
      case "bronze": output += "<div class='medal-bronze'></div>"; break;
      case "silver": output += "<div class='medal-silver'></div>"; break;
      case "gold": output += "<div class='medal-gold'></div>"; break;
      case "platinum": output += "<div class='medal-platinum'></div>"; break;
      case "master": output += "<div class='medal-master'></div>"; break;
      case "grandsmasher": output += "<div class='medal-grandsmasher'></div>"; break;
      case "unranked": output += "<div class='medal-unranked'></div>"; break;
   }

   return output;
}

function getStageImgString(stageStr){
   var output = "<div class='stage-img-div'>";

   switch (stageStr){
      case "Battlefield": output += "<div class='stage-BF'></div>"; break;
      case "Final Destination": output += "<div class='stage-FD'></div>"; break;
      case "Pokemon Stadium": output += "<div class='stage-PS'></div>"; break;
      case "Yoshi's Story": output += "<div class='stage-YS'></div>"; break;
      case "Fountain of Dreams": output += "<div class='stage-FoD'></div>"; break;
      case "Dream Land": output += "<div class='stage-DL'></div>"; break;
      default: break;
   }

   output += "</div>";
   return output;
}

function getCharImgString(dataArray){
   var charStr = dataArray[0];

   var output = "<div class='char-img-div'>";

   switch (charStr){
      case "Bowser": output += "<div class='char-Bowser'></div>"; break;
      case "Captain Falcon": output += "<div class='char-Falcon'></div>"; break;
      case "Donkey Kong": output += "<div class='char-DK'></div>"; break;
      case "Dr. Mario": output += "<div class='char-Doc'></div>"; break;
      case "Falco": output += "<div class='char-Falco'></div>"; break;
      case "Fox": output += "<div class='char-Fox'></div>"; break;
      case "Ganondorf": output += "<div class='char-Ganondorf'></div>"; break;
      case "Ice Climbers": output += "<div class='char-ICs'></div>"; break;
      case "Jigglypuff": output += "<div class='char-Jiggs'></div>"; break;
      case "Kirby": output += "<div class='char-Kirby'></div>"; break;
      case "Link": output += "<div class='char-Link'></div>"; break;
      case "Luigi": output += "<div class='char-Luigi'></div>"; break;
      case "Mario": output += "<div class='char-Mario'></div>"; break;
      case "Marth": output += "<div class='char-Marth'></div>"; break;
      case "Mewtwo": output += "<div class='char-Mewtwo'></div>"; break;
      case "Mr. Game & Watch": output += "<div class='char-GW'></div>"; break;
      case "Ness": output += "<div class='char-Ness'></div>"; break;
      case "Peach": output += "<div class='char-Peach'></div>"; break;
      case "Pikachu": output += "<div class='char-Pika'></div>"; break;
      case "Pichu": output += "<div class='char-Pichu'></div>"; break;
      case "Random": output += "<div class='char-Random'></div>"; break;
      case "Roy": output += "<div class='char-Roy'></div>"; break;
      case "Samus": output += "<div class='char-Samus'></div>"; break;
      case "Sheik": output += "<div class='char-Sheik'></div>"; break;               
      case "Yoshi": output += "<div class='char-Yoshi'></div>"; break;
      case "Young Link": output += "<div class='char-YLink'></div>"; break;
      case "Zelda": output += "<div class='char-Zelda'></div>"; break;
      default: break;
   }

   if (dataArray[1] == 0){
      output += "<div class='tint-div'></div>";
   }
   output += "</div>";
   return output;

}


function getUserDetails(player){
   output = "";

   output += "<span class='username'>" + player.username + "</span>" + "<br>";
   output += getMedalImgString(player.medal);
   output += "<br>";
   output += "<span class='division'>" + player.division + "</span>" + "<br>";

   output += "<br>";
   output += "<div class='opponent-misc-info'>";
   output += "<span class='user-id'>User ID: " + player.user_id + "</span>" + "<br>";
   output += "<span class='points'>Points: " + player.points + "</span>" + "<br>";
   output += "<span class='raw-points'>Raw Points: " + player.raw_points + "</span>" + "<br>";   
   output += "</div>"

   return output;
}

function renderHTML(data){
   var htmlString = "";

   console.log(data);

   for (i=0; i<data.length; i++){
      htmlString += "<div class='match-div clearfix'>";

      /* Left Column */
      htmlString += "<div class='left-column'>";
      if (data[i].match_type == "RANKED"){ // Ranked Matches
         htmlString += "<div class='opponents-container'>";

         htmlString += "<div class='opponent-info-div'>";
         if (data[i].winner.player == "1"){
            htmlString += getUserDetails(data[i].winner);
         }else{
            htmlString += getUserDetails(data[i].loser);
         }
         htmlString += "</div>";

         htmlString += "<div class='opponent-info-div'>";
         if (data[i].winner.player == "1"){
            htmlString += getUserDetails(data[i].loser);  
         }else{
            htmlString += getUserDetails(data[i].winner);
         }
         htmlString += "</div>";
         htmlString += "</div>";
      }else{ // Friendlies
         htmlString += "<div class='opponents-container'>";

         htmlString += "<div class='opponent-info-div'>";
         htmlString += getUserDetails(data[i].opponent1);
         htmlString += "</div>";

         htmlString += "<div class='opponent-info-div'>";
         htmlString += getUserDetails(data[i].opponent2);
         htmlString += "</div>";

         htmlString += "</div>";     
      }
      htmlString += "</div>"; /* End Left Column */

      /* Right Column */
      htmlString += "<div class='right-column'>";
      htmlString += "<span>" + "Match ID: " + data[i].match_id + "</span>" + "<br>";
      htmlString += "<span>" + "Season ID: " + data[i].season_id + "</span>" + "<br>";
      htmlString += "<span>" + "Ladder ID: " + data[i].ladder_num + "</span>" + "<br>";
      htmlString += "<span>" + "Match Type: " + data[i].match_type + "</span>" + "<br>";
      htmlString += "<span>" + "Date: " + data[i].date + "</span>" + "<br>";

      if (data[i].match_type == "RANKED"){ // Ranked Matches
         // Games
         htmlString += "<div class='games-div'>";
         htmlString += "<div class='stages-div'>";
         for (j=0; j<data[i].stages.length; j++){
            htmlString += getStageImgString(data[i].stages[j]);
         }
         htmlString += "</div>";

         htmlString += "<div class='p1chars-div'>";
         for (j=0; j<data[i].p1_characters.length; j++){
            htmlString += getCharImgString(data[i].p1_characters[j]);
         }
         htmlString += "</div>";

         htmlString += "<div class='p2chars-div'>";
         for (j=0; j<data[i].p2_characters.length; j++){
            htmlString += getCharImgString(data[i].p2_characters[j]);
         }
         htmlString += "</div>";

         htmlString += "</div>";
      }
      htmlString += "</div>"; /* End Right Column */

      htmlString += "</div>";

      /*
      htmlString += "<div class='match-div'>";
      htmlString += "<span>" + "Match ID: " + data[i].match_id + "</span>" + "<br>";
      htmlString += "<span>" + "Season ID: " + data[i].season_id + "</span>" + "<br>";
      htmlString += "<span>" + "Ladder ID: " + data[i].ladder_num + "</span>" + "<br>";
      htmlString += "<span>" + "Match Type: " + data[i].match_type + "</span>" + "<br>";
      htmlString += "<span>" + "Date: " + data[i].date + "</span>" + "<br>";

      if (data[i].match_type == "RANKED"){ // Ranked Matches
         htmlString += "<div class='opponents-container'>";

         htmlString += "<div class='opponent-info-div'>";
         if (data[i].winner.player == "1"){
            htmlString += getUserDetails(data[i].winner);
         }else{
            htmlString += getUserDetails(data[i].loser);
         }
         htmlString += "</div>";

         htmlString += "<div class='opponent-info-div'>";
         if (data[i].winner.player == "1"){
            htmlString += getUserDetails(data[i].loser);  
         }else{
            htmlString += getUserDetails(data[i].winner);
         }
         htmlString += "</div>";
         htmlString += "<br>";
         // Games
         htmlString += "<div class='games-div'>";
         htmlString += "<div class='stages-div'>";
         for (j=0; j<data[i].stages.length; j++){
            htmlString += getStageImgString(data[i].stages[j]);
         }
         htmlString += "</div>";

         htmlString += "<div class='p1chars-div'>";
         for (j=0; j<data[i].p1_characters.length; j++){
            htmlString += getCharImgString(data[i].p1_characters[j]);
         }
         htmlString += "</div>";

         htmlString += "<div class='p2chars-div'>";
         for (j=0; j<data[i].p2_characters.length; j++){
            htmlString += getCharImgString(data[i].p2_characters[j]);
         }
         htmlString += "</div>";

         htmlString += "</div>";

         htmlString += "</div>";
      }else{ // Unranked Matches
         htmlString += "<div class='opponents-container'>";

         htmlString += "<div class='opponent-info-div'>";
         htmlString += getUserDetails(data[i].opponent1);
         htmlString += "</div>";

         htmlString += "<div class='opponent-info-div'>";
         htmlString += getUserDetails(data[i].opponent2);
         htmlString += "</div>";

         htmlString += "</div>";         
      }

      htmlString += "</div>";
      */
   }

   matchesDiv.insertAdjacentHTML('beforeend', htmlString);
}


function main(){
   renderHTML(HistoryJSON);
}