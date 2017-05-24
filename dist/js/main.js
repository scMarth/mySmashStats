var matchesDiv = $('#matches-div')[0];
var statsObject = {};

// Show / Hide Matches Button
var matchesBtn = $('#matches-btn')[0];
var matchesBtnState = 1;
matchesBtn.addEventListener("click", function(){
   if (matchesBtnState == 0){
      $('#matches-div').removeClass('hidden');
      matchesBtnState = 1;
      matchesBtn.innerHTML = "Hide Matches";
   }else{
      $('#matches-div').addClass('hidden');
      matchesBtnState = 0;
      matchesBtn.innerHTML = "Show Matches";
   }
   console.log(matchesBtnState);
});

// Show / Hide Stats Button
var statsBtn = $('#stats-btn')[0];
var statsBtnState = 1;
statsBtn.addEventListener("click", function(){
   if (statsBtnState == 0){
      $('#stats-div').removeClass('hidden');
      statsBtnState = 1;
      statsBtn.innerHTML = "Hide Stats";
   }else{
      $('#stats-div').addClass('hidden');
      statsBtnState = 0;
      statsBtn.innerHTML = "Show Stats";
   }
});



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

   //console.log(data);

   for (i=0; i<data.length; i++){
      htmlString += "<div class='match-div clearfix'>";

      /* Left Column */
      htmlString += "<div class='left-column'>";
      if (data[i].match_type == "RANKED"){ // Ranked Matches
         htmlString += "<div class='opponents-container'>";

         htmlString += "<div class='opponent-info-div'>";
         if (data[i].winner.player == "1"){ // you won
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
   }

   matchesDiv.insertAdjacentHTML('beforeend', htmlString);
}



function getStageShorthand(stageStr){
   switch (stageStr){
      case "Battlefield": return "BF";
      case "Final Destination": return "FD";
      case "Pokemon Stadium": return "PS";
      case "Yoshi's Story": return "YS";
      case "Fountain of Dreams": return "FoD";
      case "Dream Land": return "DL";
      default: return "";
   }
}

function insertIntoStatsObject(yourCharacter, opponentCharacter, opponentMedal, opponentDivision, stage, youWon){
   var matchup = yourCharacter + " vs. " + opponentCharacter;

   if (!(statsObject.hasOwnProperty(matchup))){ //matchup not found
      statsObject[matchup] = new matchupStatInit();
   }

   if (youWon){
      statsObject[matchup].wins["total"]++;
      statsObject[matchup].stages[getStageShorthand(stage)].wins[opponentMedal][opponentDivision]++;
   }else{
      statsObject[matchup].losses["total"]++;
      statsObject[matchup].stages[getStageShorthand(stage)].losses[opponentMedal][opponentDivision]++;
   }

}

function getStatsFromJSON(data){
   for (i=0; i<data.length; i++){
      if (data[i].match_type != "RANKED") continue; // skip friendlies

      var yourMedal = "";
      var opponentMedal = "";
      var youWon = false;
      var yourDivision = "";
      var opponentDivision = "";
      var yourCharacter = "";
      var stage = "";
      var opponentCharacter = "";

      var you;
      var opponent;

      if (data[i].winner.player == "1"){ // you won
         you = data[i].winner;
         opponent = data[i].loser;
      }else{
         you = data[i].loser;
         opponent = data[i].winner;
      }

      yourMedal = you.medal;
      opponentMedal = opponent.medal;

      yourDivision = you.division;
      opponentDivision = opponent.division;

      // iterate through games
      for (j=0; j<data[i].stages.length; j++){
         yourCharacter = data[i].p1_characters[j][0];
         opponentCharacter = data[i].p2_characters[j][0];
         stage = data[i].stages[j];
         if (data[i].p1_characters[j][1] == 1) youWon = true;
         else youWon = false;

         insertIntoStatsObject(yourCharacter, opponentCharacter, opponentMedal, opponentDivision, stage, youWon);
      }
   }
}

function renderStats(statsObject){
   for (var matchup in statsObject){
      if (statsObject.hasOwnProperty(matchup)){
         console.log("matchup object:", statsObject[matchup]);
      }
   }
}

function main(){
   renderHTML(HistoryJSON);
   getStatsFromJSON(HistoryJSON);
   console.log("this is the statsObject:", statsObject);
   renderStats(statsObject);
}