// constructor
function divisionList(){
   this.I = 0;
   this.II = 0;
   this.III = 0;
}

// constructor
function recordList(){
   this.total = 0;
   this.unranked = {
      "---": 0
   };
   this.bronze = new divisionList();
   this.silver = new divisionList();
   this.gold = new divisionList();
   this.platinum = new divisionList();
   this.master = new divisionList();
   this.grandsmasher = new divisionList();
}

// constructor
function stageRecord(){
   this.wins = new recordList();
   this.losses = new recordList();
}

// constructor
function stageRecordList(){
   this.FD = new stageRecord();
   this.BF = new stageRecord();
   this.DL = new stageRecord();
   this.FoD = new stageRecord();
   this.PS = new stageRecord();
   this.YS = new stageRecord();
}

// constructor
function matchupStatInit(){
   this.wins = new recordList();
   this.losses = new recordList();
   this.stages = new stageRecordList();
}