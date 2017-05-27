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


/*
matchupStatInit() {
   "wins" : {
         "total": 0,
         "unranked": {
            "---": 0
         },
         "bronze": {
            "I": 0,
            "II": 0,
            "III": 0
         },
         "silver": {
            "I": 0,
            "II": 0,
            "III": 0
         },
         "gold": {
            "I": 0,
            "II": 0,
            "III": 0
         },
         "platinum": {
            "I": 0,
            "II": 0,
            "III": 0
         },
         "master": {
            "I": 0,
            "II": 0,
            "III": 0
         },
         "grandsmasher": {
            "I": 0,
            "II": 0,
            "III": 0
         }
   },
   "losses" : {
      .
      .
      .
   },
   "stages": {
      "FD": {
         "wins" : {
            "total": 0,
            "unranked": {
               "---": 0
            },
            "bronze": {
               "I": 0,
               "II": 0,
               "III": 0
            },
            "silver": {
               "I": 0,
               "II": 0,
               "III": 0
            },
            "gold": {
               "I": 0,
               "II": 0,
               "III": 0
            },
            "platinum": {
               "I": 0,
               "II": 0,
               "III": 0
            },
            "master": {
               "I": 0,
               "II": 0,
               "III": 0
            },
            "grandsmasher": {
               "I": 0,
               "II": 0,
               "III": 0
            }
         },
         "losses" : {
            .
            .
            .
         },
      },
      "BF": {
         .
         .
         .
      },
      "DL": {
         .
         .
         .
      },
      "YS": {
         .
         .
         .
      },
      "FoD": {
         .
         .
         .
      },
      "PS": {
         .
         .
         .
      }
   }
}
*/