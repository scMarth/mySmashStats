// constructor
function statsColumn(){
   this.total = 0;
   this.unranked = 0;
   this.bronze = 0;
   this.silver = 0;
   this.gold = 0;
   this.platinum = 0;
   this.master = 0;
   this.grandsmasher = 0;
}

// constructor
function statsChart(){
   this.wins = new statsColumn();
   this.losses = new statsColumn();
   this.total_games = new statsColumn();
   this.winrates = new statsColumn();
}

var medals = ['unranked', 'bronze', 'silver', 'gold', 'platinum', 'master', 'grandsmasher'];

// Populates statsChart based on the data in stageObject
// Example of a stageObject: statsObject[matchup].stages[stage]
function populateStatsChart(stageObject, statsChart){
   // console.log("populateStatsChart: stageObject:", stageObject);
   // console.log("populateStatsChart: statsChart:", statsChart);

   for (i=0; i<medals.length; i++){

      if (medals[i] == 'unranked'){
         statsChart.wins.unranked += stageObject.wins.unranked['---'];
         statsChart.losses.unranked += stageObject.losses.unranked['---'];
      }else{
         statsChart.wins[medals[i]] += stageObject.wins[medals[i]]['I'];
         statsChart.wins[medals[i]] += stageObject.wins[medals[i]]['II'];
         statsChart.wins[medals[i]] += stageObject.wins[medals[i]]['III'];

         statsChart.losses[medals[i]] += stageObject.losses[medals[i]]['I'];
         statsChart.losses[medals[i]] += stageObject.losses[medals[i]]['II'];
         statsChart.losses[medals[i]] += stageObject.losses[medals[i]]['III'];          
      }

      statsChart.wins.total += statsChart.wins[medals[i]];
      statsChart.losses.total += statsChart.losses[medals[i]];

      statsChart.total_games[medals[i]] = statsChart.wins[medals[i]] + statsChart.losses[medals[i]];
      statsChart.total_games.total += statsChart.total_games[medals[i]];

      if (statsChart.total_games[medals[i]] == 0){
         statsChart.winrates[medals[i]] = 0;
      }else{
         statsChart.winrates[medals[i]] = (statsChart.wins[medals[i]]/statsChart.total_games[medals[i]])*100;         
      }
   }
}



// Subtracts scB's values from scA and returns the result
// i.e. returns scA - scB
function subtractStatsChart(scA, scB){
   var result = new statsChart();

   result.wins['total'] = scA.wins['total'] - scB.wins['total']; // need parseInt?
   result.losses['total'] = scA.losses['total'] - scB.losses['total'];
   result.total_games.total += result.wins['total'] + result.losses['total'];
   if (result.total_games.total == 0) result.winrates.total = 0;
   else result.winrates.total = (result.wins['total']/result.total_games.total)*100;

   for (i=0; i<medals.length; i++){
      result.wins[medals[i]] = scA.wins[medals[i]] - scB.wins[medals[i]];
      result.losses[medals[i]] = scA.losses[medals[i]] - scB.losses[medals[i]];
      result.total_games[medals[i]] = result.losses[medals[i]] + result.wins[medals[i]];
      if (result.total_games[medals[i]] == 0)
         result.winrates[medals[i]] = 0;
      else
         result.winrates[medals[i]] = (result.wins[medals[i]]/result.total_games[medals[i]])*100;
   }

   return result;
}

// Adds scA's and scB's values and returns the result
// i.e. returns scA + scB
function addStatsChart(scA, scB){
   var result = new statsChart();

   result.wins['total'] = scA.wins['total'] + scB.wins['total']; // need parseInt?
   result.losses['total'] = scA.losses['total'] + scB.losses['total'];
   result.total_games.total += result.wins['total'] + result.losses['total'];
   if (result.total_games.total == 0) result.winrates.total = 0;
   else result.winrates.total = (result.wins['total']/result.total_games.total)*100;

   for (i=0; i<medals.length; i++){
      result.wins[medals[i]] = scA.wins[medals[i]] + scB.wins[medals[i]];
      result.losses[medals[i]] = scA.losses[medals[i]] + scB.losses[medals[i]];
      result.total_games[medals[i]] = result.losses[medals[i]] + result.wins[medals[i]];
      if (result.total_games[medals[i]] == 0)
         result.winrates[medals[i]] = 0;
      else
         result.winrates[medals[i]] = (result.wins[medals[i]]/result.total_games[medals[i]])*100;
      
   }

   return result;
}