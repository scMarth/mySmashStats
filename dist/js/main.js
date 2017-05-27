var matchesDiv = $('#matches-div')[0];
var statsDiv = $('#stats-div')[0];
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

function getCharImgString(charStr){
   switch (charStr){
      case "Bowser": return "<div class='char-Bowser'></div>";
      case "Captain Falcon": return "<div class='char-Falcon'></div>";
      case "Donkey Kong": return "<div class='char-DK'></div>";
      case "Dr. Mario": return "<div class='char-Doc'></div>";
      case "Falco": return "<div class='char-Falco'></div>";
      case "Fox": return "<div class='char-Fox'></div>";
      case "Ganondorf": return "<div class='char-Ganondorf'></div>";
      case "Ice Climbers": return "<div class='char-ICs'></div>";
      case "Jigglypuff": return "<div class='char-Jiggs'></div>";
      case "Kirby": return "<div class='char-Kirby'></div>";
      case "Link": return "<div class='char-Link'></div>";
      case "Luigi": return "<div class='char-Luigi'></div>";
      case "Mario": return "<div class='char-Mario'></div>";
      case "Marth": return "<div class='char-Marth'></div>";
      case "Mewtwo": return "<div class='char-Mewtwo'></div>";
      case "Mr. Game & Watch": return "<div class='char-GW'></div>";
      case "Ness": return "<div class='char-Ness'></div>";
      case "Peach": return "<div class='char-Peach'></div>";
      case "Pikachu": return "<div class='char-Pika'></div>";
      case "Pichu": return "<div class='char-Pichu'></div>";
      case "Random": return "<div class='char-Random'></div>";
      case "Roy": return "<div class='char-Roy'></div>";
      case "Samus": return "<div class='char-Samus'></div>";
      case "Sheik": return "<div class='char-Sheik'></div>";
      case "Yoshi": return "<div class='char-Yoshi'></div>";
      case "Young Link": return "<div class='char-YLink'></div>";
      case "Zelda": return "<div class='char-Zelda'></div>";
      default: break;
   }
}

function getCharImgStringFromArray(dataArray){
   var charStr = dataArray[0];
   var output = "<div class='char-img-div'>";
   var output = output + getCharImgString(charStr);
   if (dataArray[1] == 0) output += "<div class='char-tint-div'></div>";
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
            htmlString += getCharImgStringFromArray(data[i].p1_characters[j]);
         }
         htmlString += "</div>";

         htmlString += "<div class='p2chars-div'>";
         for (j=0; j<data[i].p2_characters.length; j++){
            htmlString += getCharImgStringFromArray(data[i].p2_characters[j]);
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
      statsObject[matchup].stages[getStageShorthand(stage)].wins['total']++;
   }else{
      statsObject[matchup].losses["total"]++;
      statsObject[matchup].stages[getStageShorthand(stage)].losses[opponentMedal][opponentDivision]++;
      statsObject[matchup].stages[getStageShorthand(stage)].losses['total']++;
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


function isInt(n){
   return n % 1 == 0;
}

// rounds floats to 3 decimal places but not full numbers
function roundFloatsOnly(x){
   if (isInt(x)) return x;
   else return x.toFixed(3);
}

function capitalizeString(str){
   return str.charAt(0).toUpperCase() + str.slice(1);
}

function getSmallMedalImgString(medalStr){
   var output = "";

   switch (medalStr){
      case "bronze": output += "<div class='medal-bronze-small'></div>"; break;
      case "silver": output += "<div class='medal-silver-small'></div>"; break;
      case "gold": output += "<div class='medal-gold-small'></div>"; break;
      case "platinum": output += "<div class='medal-platinum-small'></div>"; break;
      case "master": output += "<div class='medal-master-small'></div>"; break;
      case "grandsmasher": output += "<div class='medal-grandsmasher-small'></div>"; break;
      case "unranked": output += "<div class='medal-unranked-small'></div>"; break;
   }

   return output;
}


function getTableStringFromStatsChart(chart){
   var outHTML = "";

   outHTML += "<table class='stats-table'>";
      outHTML += "<thead>";
         outHTML += "<tr>";
            outHTML += "<th></th>";
            outHTML += "<th>Wins</th>";
            outHTML += "<th>Losses</th>";
            outHTML += "<th>Total Games</th>";
            outHTML += "<th>Winrate</th>";
         outHTML += "</tr>";
      outHTML += "</thead>";

      outHTML += "<tbody>";
         outHTML += "<tr class='highlight'>";
            outHTML += "<td>" + "Total" + "</td>";
            outHTML += "<td>" + chart.wins['total'] + "</td>";
            outHTML += "<td>" + chart.losses['total'] + "</td>";
            outHTML += "<td>" + chart.total_games.total + "</td>";
            outHTML += "<td>" + roundFloatsOnly(chart.winrates.total) + " % </td>";
         outHTML += "</tr>";

         for (i=0; i<medals.length; i++){
            outHTML += "<tr class='highlight'>";
            outHTML += "<td>" + "vs. " + capitalizeString(medals[i])
               + " " + getSmallMedalImgString(medals[i]) + "</td>";
            outHTML += "<td>" + chart.wins[medals[i]] + "</td>";
            outHTML += "<td>" + chart.losses[medals[i]] + "</td>";
            outHTML += "<td>" + chart.total_games[medals[i]] + "</td>";
            outHTML += "<td>" + roundFloatsOnly(chart.winrates[medals[i]]) + " % </td>";
            outHTML += "</tr>";
         }

      outHTML += "</tbody>";

   outHTML += "</table>";

   return outHTML;
}


function getLeftCharacterFromMatchup(matchupStr){
   var tokens = matchupStr.split(" ");
   return tokens[0];
}

function getRightCharacterFromMatchup(matchupStr){
   var tokens = matchupStr.split(" ");
   var ret = "";
   for (i=2; i<tokens.length - 1; i++){
      ret += tokens[i] + " ";
   }
   ret += tokens[tokens.length-1];
   return ret;
}


var stageTintDivCounter = 0;
var stageTintDivStates = [];

function addStageTintDivToggles(){
   for (i=0; i<stageTintDivCounter; i++){
      stageTintDivStates.push(0);

      var btnid = "#stage-tint-div"+String(i);

      var btn = $(btnid)[0];

      btnid = "#stats-stage-btn"+String(i);
      btn = $(btnid)[0];
      btn.addEventListener("click", function(){;
         var num = String(this.id).substring(15);
         
         if (stageTintDivStates[num] == 1){
            //console.log("case 1", this);
            var tmp = "#stage-tint-div"+String(num);
            $(tmp).addClass('hidden');
            stageTintDivStates[num] = 0;
         }else{
            //console.log("case 2", this);
            var tmp = "#stage-tint-div"+String(num);
            $(tmp).removeClass('hidden');
            stageTintDivStates[num] = 1;
         }

      });
   }



}



function renderStats(statsObject){
   var outHTML = "";

   for (var matchup in statsObject){
      if (statsObject.hasOwnProperty(matchup)){

         outHTML += "<div class='matchup-stats-div'>";
         console.log("matchup object:", statsObject[matchup]);

         outHTML += "<div class='matchup-stats-div-header'>"
            + getCharImgString(getLeftCharacterFromMatchup(matchup))
            + " " + matchup + " "
            + getCharImgString(getRightCharacterFromMatchup(matchup))
            + "</div>";

         outHTML += "<div class='matchup-stats-stages-div'>";
         outHTML += "<div class='stats-stage-btn' id='stats-stage-btn" + stageTintDivCounter + "'>" + "<div class='stage-BF'></div>"
            + "<div id='stage-tint-div" + stageTintDivCounter++ + "' "
            + "class='stage-tint-div hidden'></div>" + "</div>";
         outHTML += "<div class='stats-stage-btn' id='stats-stage-btn" + stageTintDivCounter + "'>" + "<div class='stage-FD'></div>"
            + "<div id='stage-tint-div" + stageTintDivCounter++ + "' "
            + "class='stage-tint-div hidden'></div>" + "</div>";
         outHTML += "<div class='stats-stage-btn' id='stats-stage-btn" + stageTintDivCounter + "'>" + "<div class='stage-FoD'></div>"
            + "<div id='stage-tint-div" + stageTintDivCounter++ + "' "
            + "class='stage-tint-div hidden'></div>" + "</div>";
         outHTML += "<div class='stats-stage-btn' id='stats-stage-btn" + stageTintDivCounter + "'>" + "<div class='stage-YS'></div>"
            + "<div id='stage-tint-div" + stageTintDivCounter++ + "' "
            + "class='stage-tint-div hidden'></div>" + "</div>";
         outHTML += "<div class='stats-stage-btn' id='stats-stage-btn" + stageTintDivCounter + "'>" + "<div class='stage-PS'></div>"
            + "<div id='stage-tint-div" + stageTintDivCounter++ + "' "
            + "class='stage-tint-div hidden'></div>" + "</div>";
         outHTML += "<div class='stats-stage-btn' id='stats-stage-btn" + stageTintDivCounter + "'>" + "<div class='stage-DL'></div>"
            + "<div id='stage-tint-div" + stageTintDivCounter++ + "' "
            + "class='stage-tint-div hidden'></div>" + "</div>";
         outHTML += "</div>";

         var sc_FD = new statsChart();
         var sc_BF = new statsChart();
         var sc_YS = new statsChart();
         var sc_PS = new statsChart();
         var sc_FoD = new statsChart();
         var sc_DL = new statsChart();

         populateStatsChart(statsObject[matchup].stages['FD'], sc_FD);
         populateStatsChart(statsObject[matchup].stages['BF'], sc_BF);
         populateStatsChart(statsObject[matchup].stages['YS'], sc_YS);
         populateStatsChart(statsObject[matchup].stages['PS'], sc_PS);
         populateStatsChart(statsObject[matchup].stages['FoD'], sc_FoD);
         populateStatsChart(statsObject[matchup].stages['DL'], sc_DL);

         console.log("BF chart: ", sc_BF);
         console.log("FD chart: ", sc_FD);
         console.log("DL chart: ", sc_DL);
         console.log("YS chart: ", sc_YS);
         console.log("FoD chart: ", sc_FoD);
         console.log("PS chart: ", sc_PS);

         var chart = new statsChart();
         chart = addStatsChart(sc_FD, sc_BF);
         chart = addStatsChart(chart, sc_YS);
         chart = addStatsChart(chart, sc_PS);
         chart = addStatsChart(chart, sc_FoD);
         chart = addStatsChart(chart, sc_DL);

         outHTML += getTableStringFromStatsChart(chart);
         outHTML += "</div>";
      }
   }

   statsDiv.insertAdjacentHTML('beforeend', outHTML);
   addStageTintDivToggles();
}

function main(){
   renderHTML(HistoryJSON);
   getStatsFromJSON(HistoryJSON);
   console.log("this is the statsObject:", statsObject);
   renderStats(statsObject);
}