<?php
include APPRAIZ . 'includes/cabecalho.inc';


//$evento = new TipoEvento();
//$dados = $evento->listaCombo();
//echo $db->monta_combo("agdid", $dados, 'S','Selecione...','', '', '',240,'S','agdid');
?>
<link rel='stylesheet' type='text/css' href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.7.2/themes/start/jquery-ui.css' />
<link rel='stylesheet' type='text/css' href='../includes/JQuery/week/jquery.weekcalendar.css' />
<link rel='stylesheet' type='text/css' href='../includes/JQuery/week/full_demo/demo.css' />
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type='text/javascript' src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.7.2/jquery-ui.min.js'></script>
<script type='text/javascript' src='../includes/JQuery/week/jquery.weekcalendar.js'></script>

<style>


/*CABEÇALHO das datas*/
.wc-header td {
        background-color: #CAE1FF; 
}

/*BARRA LATERAL DAS HORAS*/
.wc-business-hours {
        background-color: #CAE1FF;
        border-bottom: 1px solid #ccc;
        color: #fff;
        font-weight: bold;
        font-size: 1.4em;
}

/*LINHAS DE FUNDO*/
.wc-scrollable-grid {
        overflow: auto;
        overflow-x: hidden !important;
        overflow-y: auto !important; 
        position: relative;
        background-color: #fff;
        width: 100%;
}

/*CABEÇALHO DO EVENTO*/
.wc-cal-event .wc-time {
        background-color: #2b72d0;
        border: 1px solid #1b62c0;
        color: #fff;
        padding: 0;
        font-weight: bold;
        text-align: center;
}

/*CORPO DO EVENTO*/
.wc-cal-event {
        background-color: #68a1e5;      
        filter:alpha(opacity=80);
        -moz-opacity:0.8;
        -khtml-opacity: 0.8;
        opacity: 0.8;
        position: absolute;     
        text-align: left;
        overflow: hidden;
        cursor: pointer;
        color: #000;
        width: 100%;
        display: none;
}

.wc-container {
        font-size: 14px;        
        font-family: arial, helvetica;
}

.wc-nav {
        padding: 1em;
        text-align: right;      
}

.wc-nav button {
        margin: 0 0.5em;        
}
 

.wc-container table {
        border-collapse: collapse;
    border-spacing: 0;
}
.wc-container table td {
        margin: 0;
        padding: 0;
}

.wc-header {
        background: #eee;
        border-top: 1px solid #aaa;
        border-bottom: 1px solid #aaa;
        width: 100%;
        color: #000;
}

.wc-header .wc-time-column-header {
        width: 6%;      
}

.wc-header .wc-scrollbar-shim {
        width: 16px;    
}

.wc-header .wc-day-column-header {
        text-align: center;
        padding: 0.4em;
}


.wc-grid-timeslot-header {
        width: 6%;
        background: #eee;       
}

table.wc-time-slots {
        width: 100%;
        table-layout: fixed;
        cursor: default;
        
}

.wc-day-column {
        width: 13.5%;
        border-left: 1px solid #ddd;    
        overflow: visible;
        vertical-align: top;
}

.wc-day-column-inner {
        width: 100%;
        position:relative;  
}

.wc-time-slot-wrapper {
        position:relative;      
        height: 1px;
        top: 1px;
}

.wc-time-slots {
        position: absolute;     
        width: 100%;
}



.wc-time-header-cell {
        padding: 5px;   
        height: 80px; /* reference height */
        
}


.wc-time-slot {
        border-bottom: 1px dotted #ddd; 
        /*height: 19px!important;*/
        
}

.wc-hour-header {
        text-align: right;
}

.wc-hour-end, .wc-hour-header {
        border-bottom: 1px solid #ccc;  
        color: #555;
}


.wc-business-hours .wc-am-pm {
        font-size: 0.6em;       
}

.wc-day-header-cell {
        text-align: center;
        vertical-align: middle;
        padding: 5px;
}



.wc-time-slot-header .wc-header-cell {
        text-align: right;
        padding-right: 10px;    
}

.wc-header .wc-today  {
        font-weight: bold;
}

.wc-time-slots .wc-today {
        background-color: #ffffcc;      
}



.wc-cal-event div {
        padding: 0 5px;
        
}

.wc-container .ui-draggable .wc-time {
        cursor: move;
}

.wc-cal-event .wc-title {
        position: relative;     
}

.wc-container .ui-resizable-s {
        height: 10px;
        bottom: -8px;   
}


.wc-container .ui-draggable-dragging {
   z-index: 1000;
}
</style>

<script type="text/javascript">
$(document).ready(function() {

   var $calendar = $('#calendar');
   var id = 10;

   $calendar.weekCalendar({
      timeslotsPerHour 		: 7,
      allowCalEventOverlap 	: true,
      overlapEventsSeparate	: true,
      firstDayOfWeek 		: 1,
      businessHours 		: {start: 9, end: 21, limitDisplay: true },
      daysToShow 			: 5,
      dateFormat 			: "d/m/Y",
      shortMonths 			: ['Jan', 'Fev', 'Mar', 'Abr', 'Maio', 'Jun', 'Jul', 'Ago', 'Set', 'Out', 'Nov', 'Dez'],
      longMonths 			: ['Janeiro', 'Fevereiro', 'Março', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
      shortDays 			: ['Dom', 'Seg', 'Ter', 'Qua', 'Qui', 'Sex', 'Sáb'],
      longDays 				: ['Domingo', 'Segunda', 'Terça', 'Quarta', 'Quinta', 'Sexta', 'Sábdo'],
      timeSeparator			: ' até ',
	  buttonText : {
            today 	 : "Semana atual",
            lastWeek : "&nbsp;&lt;&nbsp;",
            nextWeek : "&nbsp;&gt;&nbsp;"
      },
      height 				: function($calendar) {
         return $(window).height() - $("h1").outerHeight()-100 ;
      },
      eventRender 			: function(calEvent, $event) {
      
         if (calEvent.end.getTime() < new Date().getTime()) {
            $event.css("backgroundColor", "#aaa");
            $event.find(".wc-time").css({
               "backgroundColor" : "#999",
               "border" : "1px solid #888"
            });
         }else{
          	$event.css("backgroundColor", "#CAE1FF");
            $event.find(".wc-time").css({
               "backgroundColor" : "#607B8B",
               "border" : "1px solid #888"
            });
         }
         //prioridade alta
         if (calEvent.priority == '1' && !(calEvent.end.getTime() < new Date().getTime())) {
            $event.find(".wc-time").css({
               "backgroundColor" : "#CD4F39"
            });
         }
         else {
         	//prioridade média
         	if (calEvent.priority == '2' && !(calEvent.end.getTime() < new Date().getTime())) {
            	$event.find(".wc-time").css({
               		"backgroundColor" : "#EEAD0E"
            	});
            }else{
            	//prioridade baixa
	         	if(calEvent.priority == '3' && !(calEvent.end.getTime() < new Date().getTime())) {
	            	$event.find(".wc-time").css({
	               		"backgroundColor" : "#6E8B3D"
	            	});
	            }
         	}
         }
      
      },
      draggable 			: function(calEvent, $event) {
		return calEvent.readOnly != true;
      },
      resizable 			: function(calEvent, $event) {
		return calEvent.readOnly != true;
      },
      eventNew 				: function(calEvent, $event) {
		$calendar.weekCalendar("removeEvent", calEvent.id);
      },
      eventDrop 			: function(calEvent, $event) {
      },
      eventResize 			: function(calEvent, $event) {
      },
      eventClick 			: function(calEvent, $event) {
      },
      eventMouseover 		: function(calEvent, $event) {
      },
      eventMouseout 		: function(calEvent, $event) {
      },
      noEvents 				: function() {

      },
      data 					: function(start, end, callback) {
         callback(getEventData());
      }
   });

   function getEventData() {
      var year  = new Date().getFullYear();
      var month = new Date().getMonth();
      var day   = new Date().getDate();
      
      return {
         events :
         <?php 
         $evento 	 = new Evento();
         $dados  	 = $evento->listaByAgenda( $_REQUEST['agdid'] );
		 $listEvento = array();		
		 
		 $content = "";
		 
         foreach ( $dados as $dados ){
         	
         	$content =  '<strong> '. $dados['tipoevento'] . ' - Código: <a href="agenda.php?modulo=principal/cadastroEvento&acao=E&eveid='.$dados['eveid'].'&agdid='.$_REQUEST['agdid'].'">'. $dados['eveid'] . '/' . $dados['anoinicio'] . '</a></strong><br />'. 
         			 	'Assunto: <a href="agenda.php?modulo=principal/cadastroEvento&acao=E&eveid='.$dados['eveid'].'&agdid='.$_REQUEST['agdid'].'">' . $dados['eveassunto']. '</a><br />'.
         				'Solicitante: '. $dados['solicitante']; 	
			$itmEvento  .= "{";
         	$itmEvento .= "\"id\" 		: {$dados['eveid']}, ";	
         	$itmEvento .= "\"priority\" : {$dados['priority']}, ";
         	$itmEvento .= "\"start\" 	: new Date({$dados['anoinicio']}, {$dados['mesinicio']}, {$dados['diainicio']}, {$dados['horainicio']}, {$dados['mininicio']}), ";	
         	$itmEvento .= "\"end\" 		: new Date({$dados['anofim']}, {$dados['mesfim']}, {$dados['diafim']}, {$dados['horafim']}, {$dados['minfim']}), ";	
         	$itmEvento .= "\"title\" 	: '" . (str_replace(array("\n", "\t", "\r"), '<br>', $content )) . "'";	
			$itmEvento .= "}";
			
			$listEvento[] = $itmEvento;
			$itmEvento  = '';
         }
         
         $listEvento = "[" . implode(", ", $listEvento) . "]";
         
         echo $listEvento;
         ?>
      };
   }
});
</script>
<?php
$agenda = new Agenda( $_REQUEST['agdid']);
$dados  = $agenda->getDados();

?>
<div style="font-size: 18pt; margin-left: 50px; margin-top: 20px;">
Agenda - <?php echo $dados['agddsc'] ?>
</div>
<center>

<div id='calendar' style="width:95%;"></div>
</center>
