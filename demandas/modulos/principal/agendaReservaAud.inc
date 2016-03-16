<?php

if ($_POST['tipoSemana'] && $_POST['data'] && $_POST['pesquisar'] == 1 && $_POST['mesp'] && $_POST['anop']) {
	header('content-type: text/html; charset=ISO-8859-1');
		
	
	if(strlen(trim($_POST['data'])) == 10){
		$anoD = substr(trim($_POST['data']), -4,4);
		$mesD = substr(trim($_POST['data']), -7,2);
		$diaD = substr(trim($_POST['data']), -10,2);
		
		$data =  date('d-m-Y',mktime(0,0,0,$mesD +1,$diaD,$anoD));

		$ano = substr(trim($data), -4,4);
		$mes = substr(trim($data), -7,2);
		$dia = substr(trim($data), -10,2);
		
	}
	
	$mes = $_POST['mesp'];
	if(strlen($mes) == 1) $mes = "0".$mes;
	
	$ano = $_POST['anop'];
	

	graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$dia,$mes,$ano);
	exit;
}





if ($_POST['tipoSemana'] && $_POST['data'] && $_POST['avancar'] == 1) {
	header('content-type: text/html; charset=ISO-8859-1');
		
	
	if(strlen(trim($_POST['data'])) == 10){
		$anoD = substr(trim($_POST['data']), -4,4);
		$mesD = substr(trim($_POST['data']), -7,2);
		$diaD = substr(trim($_POST['data']), -10,2);
		
		$data =  date('d-m-Y',mktime(0,0,0,$mesD +1,$diaD,$anoD));

		$ano = substr(trim($data), -4,4);
		$mes = substr(trim($data), -7,2);
		$dia = substr(trim($data), -10,2);
		
	}

	graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$dia,$mes,$ano);
	exit;
}

if ($_POST['tipoSemana'] && $_POST['data'] && $_POST['recuar'] == 1) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	if(strlen(trim($_POST['data'])) == 10){
		$anoD = substr(trim($_POST['data']), -4,4);
		$mesD = substr(trim($_POST['data']), -7,2);
		$diaD = substr(trim($_POST['data']), -10,2);
		
		$data =  date('d-m-Y',mktime(0,0,0,$mesD -1,$diaD,$anoD));

		$ano = substr(trim($data), -4,4);
		$mes = substr(trim($data), -7,2);
		$dia = substr(trim($data), -10,2);
	}
	
	graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$dia,$mes,$ano);
	exit;
}




if ($_POST['tipoSemana'] && $_POST['data'] && $_POST['mensal'] == 1) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	if(strlen(trim($_POST['data'])) == 10){
		$anoD = substr(trim($_POST['data']), -4,4);
		$mesD = substr(trim($_POST['data']), -7,2);
		$diaD = substr(trim($_POST['data']), -10,2);
		
		$_POST['tipoSemana'] = "mes";
		
		$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD,$anoD));

		$ano = substr(trim($data), -4,4);
		$mes = substr(trim($data), -7,2);
		$dia = substr(trim($data), -10,2);
		
	}
	
	graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$dia,$mes,$ano);
	exit;
}


function exibeInformacao($evaid){
	
	global $db;
	$sql = "SELECT e.evaid, e.evaevento, 
					CASE e.evalocal 
						WHEN 'A' THEN 'Anexo'
						WHEN 'S' THEN 'Sede'
					END as local, 
					e.evanumpart, 
					u.usunome as solicitante, 
					'(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
					upper(ug.ungabrev)||' - '||ug.ungdsc as orgao,
       				CASE WHEN evaprojetor='S' THEN ' - Projetor &nbsp;&nbsp;&nbsp;' END || 
       				CASE WHEN evacomput='S' THEN ' - Computador para Projeção &nbsp;&nbsp;&nbsp;' END ||
       				CASE WHEN evatelao='S' THEN ' - Telão &nbsp;&nbsp;&nbsp;' END ||
       				CASE WHEN evamicrofone='S' THEN ' - Microfones &nbsp;&nbsp;&nbsp;' END ||
       				CASE WHEN evainternet='S' THEN ' - Acesso a Internet  &nbsp;&nbsp;&nbsp;' END ||
       				'' as equip,
       				evaobs
	        FROM
				demandas.eventoauditorio e
			INNER JOIN public.unidadegestora ug ON ug.ungcod = e.ungcod
			INNER JOIN seguranca.usuario u ON u.usucpf = e.usucpf
			WHERE evaid = $evaid";
	$dados = $db->carregar($sql);
	
	echo ("<div><center><b>EVENTO</b></center><br />
	<b>Código:</b> {$dados[0]['evaid']} <br />
	<b>Nome:</b> {$dados[0]['evaevento']} <br />
	<b>Local:</b> {$dados[0]['local']}<br />
	<b>Órgão/Secretaria:</b> {$dados[0]['orgao']}<br />
	<b>Nº de Participantes:</b> {$dados[0]['evanumpart']}<br />
	<b>Equipamentos Necessários:</b> {$dados[0]['equip']}<br />
	<b>Observações:</b> {$dados[0]['evaobs']}<br />
	<br />
	<b>Solicitante:</b> {$dados[0]['solicitante']}<br />
	<b>Telefone:</b> {$dados[0]['fone']}<br />
	</div>
	");
  
	exit;
	
	
}



if ($_REQUEST['evaidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	exibeInformacao($_REQUEST['evaidAjax']);
	exit;
}


if ($_POST['tipoSemana'] && $_POST['data'] && $_POST['horario']) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	if(strlen(trim($_POST['data'])) == 10){
		$anoD = substr(trim($_POST['data']), -4,4);
		$mesD = substr(trim($_POST['data']), -7,2);
		$diaD = substr(trim($_POST['data']), -10,2);
		
		$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD,$anoD));

		$ano = substr(trim($data), -4,4);
		$mes = substr(trim($data), -7,2);
		$dia = substr(trim($data), -10,2);
		
	}
	
	graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$dia,$mes,$ano);
	exit;
}


monta_titulo( 'Agenda Auditório', 'Escolha uma data para adicioná-lo à reserva do auditório.' );

?>
 <head>
 <style type="text/css">
 
 .titulo1{background-color:rgb(227, 227, 227);text-align:center;font-weight:bold;}
 .linha1{background-color:#f5f5f5;}
 .linha2{background-color:#fdfdfd;}
 </style>
 <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
 <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
 <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
 <script src="../includes/calendario.js"></script>
 <script src="./js/ajax.js" type="text/javascript"></script>
 <script src="./js/demandas.js" type="text/javascript"></script>
 <script type="text/javascript" src="../includes/prototype.js"></script>
 <script type="text/javascript" src="../includes/funcoes.js"></script>
 <script type="text/javascript" src="../includes/remedial.js"></script>
 <script type="text/javascript" src="../includes/superTitle.js"></script>
 <script language="JavaScript" src="../includes/wz_tooltip.js"></script>
 <script type="text/javascript">

function extraiScript(texto){  
	//desenvolvido por Skywalker.to, Micox e Pita.  
	//http://forum.imasters.uol.com.br/index.php?showtopic=165277  
	var ini, pos_src, fim, codigo;  
	var objScript = null;  
	ini = texto.indexOf('<script', 0)  
	while (ini!=-1){  
		var objScript = document.createElement("script");  
		//Busca se tem algum src a partir do inicio do script  
		pos_src = texto.indexOf(' src', ini)  
		ini = texto.indexOf('>', ini) + 1;

		//Verifica se este e um bloco de script ou include para um arquivo de scripts  
		if (pos_src < ini && pos_src >=0){//Se encontrou um "src" dentro da tag script, esta e um include de um arquivo script  
			//Marca como sendo o inicio do nome do arquivo para depois do src  
			ini = pos_src + 4;  
			//Procura pelo ponto do nome da extencao do arquivo e marca para depois dele  
			fim = texto.indexOf('.', ini)+4;  
			//Pega o nome do arquivo  
			codigo = texto.substring(ini,fim);  
			//Elimina do nome do arquivo os caracteres que possam ter sido pegos por engano  
			codigo = codigo.replace("=","").replace(" ","").replace("\"","").replace("\"","").replace("\'","").replace("\'","").replace(">","");  
			// Adiciona o arquivo de script ao objeto que sera adicionado ao documento  
			objScript.src = codigo;  
		}else{
		//Se nao encontrou um "src" dentro da tag script, esta e um bloco de codigo script  
			// Procura o final do script
			fim = texto.indexOf('</script', ini);  
			// Extrai apenas o script
			codigo = texto.substring(ini,fim);  
			// Adiciona o bloco de script ao objeto que sera adicionado ao documento  
			objScript.text = codigo;
		}
		
		//Adiciona o script ao documento  
		document.body.appendChild(objScript);
		
		// Procura a proxima tag de <script>  
		ini = texto.indexOf('<script', fim);
		
		//Limpa o objeto de script  
		objScript = null;  
	}  
}  

d = document;
  			
	/*
	 * Valida e envia para página pai
	*/

	function selecionaTR(cor,id){
		var cor2 = cor;
		var tr = id;
		document.getElementById('tec_' + tr).style.backgroundColor = cor2;
	}
	
	function chamaDia(dia){
		d.getElementById('data_pesq').value = dia;
		var data = d.getElementById('data_pesq').value
		tipoDiario();
	}
	
	
	function pesquisar(){
		var mesp = d.getElementById('mesp').value;
		var anop = d.getElementById('anop').value;
		if(mesp == ''){
			alert ('Selecione o Mês.');
			d.getElementById('mesp').focus();
			return false;
		}
		if(anop == ''){
			alert ('Selecione o Ano.');
			d.getElementById('anop').focus();
			return false;
		}
			
		var data = d.getElementById('data_pesq').value;
		var tipoSemana = d.getElementById('tipoSemana').value;
		var horario = d.getElementById('horario').value;
		if(tipoSemana && data){		
		var div = d.getElementById('exibeDemandas');
		
		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/agendaReservaAud&acao=A', {
								        method:     'post',
								        parameters: '&pesquisar=1&tipoSemana=' + tipoSemana +'&data=' + data +'&horario=' + horario +'&mesp=' + mesp +'&anop=' + anop,
								        onComplete: function (res)
								        {			
											div.innerHTML = res.responseText; 
											extraiScript(res.responseText);
								        }
								  });
		}
	}	
	
	
	function avancar(){
		var data = d.getElementById('data_pesq').value;
		var tipoSemana = d.getElementById('tipoSemana').value;
		var horario = d.getElementById('horario').value;
		if(tipoSemana && data){		
		var div = d.getElementById('exibeDemandas');
		
		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/agendaReservaAud&acao=A', {
								        method:     'post',
								        parameters: '&avancar=1&tipoSemana=' + tipoSemana +'&data=' + data +'&horario=' + horario,
								        onComplete: function (res)
								        {			
											div.innerHTML = res.responseText; 
											extraiScript(res.responseText);
								        }
								  });
		}
	}	
	
	function recuar(){
		var data = d.getElementById('data_pesq').value;
		var tipoSemana = d.getElementById('tipoSemana').value;
		var horario = d.getElementById('horario').value;
		if(tipoSemana && data){		
		var div = d.getElementById('exibeDemandas');
		
		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/agendaReservaAud&acao=A', {
								        method:     'post',
								        parameters: '&recuar=1&tipoSemana=' + tipoSemana +'&data=' + data +'&horario=' + horario,
								        onComplete: function (res)
								        {			
											div.innerHTML = res.responseText;
											extraiScript(res.responseText);
								        }
								  });
		}
	}
	
	
	
	
	function infoAud(evaid){
		var dmdid = id;
		var div = d.getElementById('alteraDemanda');
		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/agendaReservaAud&acao=A', {
								        method:     'post',
								        parameters: '&evaidAjax=' + evaid,
								        onComplete: function (res)
								        {
											div.style.display = "";
											div.innerHTML = res.responseText;
											extraiScript(res.responseText);
								        }
								  });
	}	
	
	
	function marcaItem(id, datax, turnox) {	
	
		if(turnox==0) turnox = "Manhã";
		if(turnox==1) turnox = "Almoço";
		if(turnox==2) turnox = "Tarde";
							
		var tabela = window.opener.document.getElementById("tabela_agenda");			
			
		if(document.getElementById(id).checked == true) {				
			var tamanho = tabela.rows.length;
						
			if(tamanho == 1) {			
				var linha = tabela.insertRow(tamanho);
				linha.style.backgroundColor = "#f4f4f4";
			} else {
				var linha = tabela.insertRow(Number(tamanho)-1);
				if(tabela.rows[tamanho-2].style.backgroundColor == "rgb(224, 224, 224)") {
					linha.style.backgroundColor = "#f4f4f4";					
				} else {
					linha.style.backgroundColor = "#e0e0e0";					
				}
			}
											
			linha.id = "linha_"+id;
								
			var colAcao = linha.insertCell(0);
			var colData = linha.insertCell(1);
			var colTurno = linha.insertCell(2);
			
			colAcao.style.textAlign = "center";
			colData.style.textAlign = "center";
			colTurno.style.textAlign = "center";
			
			colAcao.innerHTML = "<span onclick=\"excluiItem('"+id+"');\"><img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
			colData.innerHTML = datax + '<input type="hidden" id="datax['+id+']" name="datax[]" value="'+datax+'">';
			colTurno.innerHTML = turnox + '<input type="hidden" id="turnox['+id+']" name="turnox[]" value="'+turnox+'">';
		
		}else{
			var linha = window.opener.document.getElementById("linha_"+id).rowIndex;
			tabela.deleteRow(linha);
			/*	
			if(tabela.rows.length == 2) {
				tabela.deleteRow(1);
			}
			*/
		}					
	}
	
	
	
	function alteraDemanda(id){
		var dmdid = id;
		var div = d.getElementById('alteraDemanda');
		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/agendaReservaAud&acao=A', {
								        method:     'post',
								        parameters: '&dmdidAlteraAajx=' + dmdid,
								        onComplete: function (res)
								        {
											div.style.display = "";
											div.innerHTML = res.responseText;
											extraiScript(res.responseText);
								        }
								  });
	}
	
	function fechar(id){
		var dmdid = id;
		var div = d.getElementById(dmdid);
		div.style.display = "none";
	}
	
	
</script>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
	

		

		<!-- 
		<form id="formulario" name="formulario" action="" method="post">
		 -->
		 <form>
		 
		<table class="tabela" name="formR" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<?$_SESSION['evalocal'] = $_REQUEST['evalocal'] ? $_REQUEST['evalocal'] : $_SESSION['evalocal']; ?>
			<input type="hidden" name="evalocal" value="<?=$_SESSION['evalocal']?>">
			<tr>
				<td width="40%" align="right" class="SubTituloDireita" >LOCAL:</td>
				<td>
					<?
						
						if($_SESSION['evalocal'] == 'S'){
							echo "<b>Sede</b> (110 cadeiras fixas)";
						}
						elseif($_SESSION['evalocal'] == 'A'){
							echo "<b>Anexo</b> (109 cadeiras fixas)"; 
						}
					?>
					
				</td>
			</tr>
			<tr>
				<td width="40%" align="right" class="SubTituloDireita" >Mês / Ano:</td>
				<td valign="top" >
						<select  name='mesp' id='mesp'  class='CampoEstilo'  style='width: auto'><option value="">--</option>
						<?
							for($i=1; $i<13; $i++) echo "<option value='$i'>".mes($i)."</option>";
						?>
						</select>
						/
						<select  name='anop' id='anop'  class='CampoEstilo'  style='width: auto'><option value="">--</option>
						<?
							
							for($i=2009; $i<date("Y")+2; $i++) echo "<option value='$i'>".$i."</option>";
						?>
						</select>
						&nbsp;
						
						<input type="button" name="pesq" value="Pesquisar" onclick="pesquisar();">
				</td>
			</tr>
			<tr>
				<td width="40%" align="right" class="SubTituloDireita" colspan=2> &nbsp;</td>
				
			</tr>			
			<tr bgcolor="#C0C0C0">
				<td align="center" COLSPAN=2>
				<span style="cursor:pointer;border:#000000 solid 1px;background-color:#fcfcfc;width:120px;text-align:center;" onclick="recuar();">&nbsp;<< Mês Anterior&nbsp;</span>
				&nbsp;&nbsp;&nbsp;
				<span style="cursor:pointer;border:#000000 solid 1px;background-color:#fcfcfc;width:120px;text-align:center;" onclick="avancar();">&nbsp;Próximo Mês >>&nbsp;</span>
				</td>
			</tr>
		</table>
		</form>
		
		<center>
		<div style="display:none;margin: 0 auto;width:1px;"  id="alteraDemanda"></div>
		<div style="margin: 0 auto; padding: 0; height: 212px; width: 95%; background-color: #eee; border: none;" class="div_rolagem" id="exibeDemandas">
			<?php
			if(strlen(trim($_POST['data_pesq'])) == 10){
				$ano = substr(trim($_POST['data_pesq']), -4,4);
				$mes = substr(trim($_POST['data_pesq']), -7,2);
				$dia = substr(trim($_POST['data_pesq']), -10,2);
			}
			?>
			<?=graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$diaD = $dia,$mesD = $mes,$anoD = $ano, $Idx = $_REQUEST['evaid']);?>
		</div>
		</center>
		







</body>

</html>
<?php


/*
Script que lista os dias da semana atual
*/

function graphWeek($sqlx = null,$horario = null, $tipoSemana = null,$diaD = null,$mesD = null,$anoD = null, $Idx = null)
{
	global $db;
	
	
	$evalocal = $_SESSION['evalocal'];
	
	//Paramentros específicos de datas:
	($diaD)? $dia = $diaD : $dia = date("d");
	($mesD)? $mes = $mesD : $mes = date("m");
	($anoD)? $ano = $anoD : $ano = date("Y");
	$dia_semana = date("w", strtotime ("$ano/$mes/$dia"));
	
	
	//Parametro Tipo de Gráfico: Diário (Default) / Semanal / Mensal
    //Função que pega a quatidade de dias do mês
	$tipo = cal_days_in_month(CAL_GREGORIAN, $mes, $ano);
	$data_inicial =  date('Y-m-d',mktime(0,0,0,$mes - $menos,$dia,$ano));
	$data_final =  date('Y-m-d',mktime(0,0,0,$mes + $mais,$dia,$ano));
	
	
	//Exibe o início da tabela com a classe do SIMEC
	echo "<input type=\"hidden\" name=\"tipoSemana\" id=\"tipoSemana\" value=\"mes\" />";
	echo "<input type=\"hidden\" name=\"horario\" id=\"horario\" value=\"completo\" />";
	echo "<input type=\"hidden\" name=\"data_pesq\" id=\"data_pesq\" value=\"$dia/$mes/$ano\" />";

	echo "<table align=center width=95% class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\" >";
	
	echo "<tr class=\"titulo1\" ><td colspan=".($tipo +1)." align=center><font size=3>".mes($mes)."/".$ano."</font></td></tr>";
	
	echo "<tr class=\"titulo1\" ><td rowspan=2 align=center>Turnos</td>";
	
		//Contador que pecorrerá o tipo de Grafico (até 7 p/ Semanal : até 1 para Diário)
		$cont=1;
		while($cont <= $tipo){
			//pegar dia da semana
			$dia_s =  dia(date("w",mktime(0,0,0,$mes,(strlen($cont) == 1)? "0".$cont : $cont,$ano)));
			
			($dia_s == "Domingo" || $dia_s == "Sábado")? $cor = "#CDC9C9" : $cor = "";
			
			//Exibe cada dia do mes de acordo com o calendário
			echo "<td rowspan=2 style=\"background-color:$cor\"  align=center nowrap width=20 >";
			
			if( ((strlen($cont) == 1)? "0".$cont : $cont)."/".$mes."/".$ano == date("d")."/".date("m")."/".date("Y")){
				//Aplica cor vermelha no texto
				echo "<font color=blue><b>";
			    echo "$cont<br />".substr($dia_s, 0,3);
			    echo "</b></font>";
			}
			else{
				echo "$cont<br />".substr($dia_s, 0,3);
			}
			echo "</td>";
			$cont++; //incrementa o contador
		}
		echo "<tr></tr>";
	
	echo "</tr>";
	
	
	//Carrega os dados do SQL
	//$turnos = $db->carregar( $sqlx );
	//$turnos = $turnos ? $turnos : array();
	$turnos = array(0 => "Manhã", 1 => "Almoço", 2 => "Tarde");
	
		$contador = 1;
		for($y=0; $y<count($turnos); $y++){
			($contador % 2)? $corTR = "#fdfdfd" : $corTR = "#f5f5f5";
			$contador++;
		 	echo "<tr style=\"background-color:$corTR;\" onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='$corTR'\"   ><td nowrap id=\"tec_{$y}\" >{$turnos[$y]}</td>";
		 	$td = 1;
		 	while($td <= $tipo){
			 	//pegar dia da semana
				$dia_s =  dia(date("w",mktime(0,0,0,$mes,(strlen($td) == 1)? "0".$td : $td,$ano)));
				
				
				
				$diax = ((strlen($td) == 1)? "0".$td : $td);
				$datax = $diax."/".$mes."/".$ano;
				$link = "<input title='Disponível para Reserva' id='".$y.$ano.$mes.$diax."' name='".$y.$ano.$mes.$diax."' type='checkbox' value='".$datax."' onclick=\"marcaItem('".$y.$ano.$mes.$diax."', '".$datax."', '".$y."');\">";
				
				if($ano.$mes.$diax < date("Ymd")) $link = "";
				
				$evaid = 0;
				if($y==0){
					$query = "select evaid
							  from demandas.eventoauditorio e
							  inner join demandas.agendaauditorio a on a.aga_evaid_manha = e. evaid
							  where e.evalocal = '$evalocal' and a.agastatus = 'A' and a.agadata = '".$ano.$mes.$diax."'";
					$evaid = $db->PegaUm($query);
				}
				elseif($y==1){
					$query = "select evaid
							  from demandas.eventoauditorio e
							  inner join demandas.agendaauditorio a on a.aga_evaid_almoco = e. evaid
							  where e.evalocal = '$evalocal' and a.agastatus = 'A' and a.agadata = '".$ano.$mes.$diax."'";
					$evaid = $db->PegaUm($query);
				}
				elseif($y==2){
					$query = "select evaid
							  from demandas.eventoauditorio e
							  inner join demandas.agendaauditorio a on a.aga_evaid_tarde = e. evaid
							  where e.evalocal = '$evalocal' and a.agastatus = 'A' and a.agadata = '".$ano.$mes.$diax."'";
					$evaid = $db->PegaUm($query);
				}
				
				if($evaid != 0){
					
					if($Idx == $evaid){
						$link = "<input title='Disponível para Alterar Reserva' id='".$y.$ano.$mes.$diax."' name='".$y.$ano.$mes.$diax."' type='checkbox' value='".$datax."' checked onclick=\"marcaItem('".$y.$ano.$mes.$diax."', '".$datax."', '".$y."');\">";
					}
					else{
						$link = "<div onmouseout=\"SuperTitleOff(this);\" onmousemove=\"SuperTitleAjax('demandas.php?modulo=principal/agendaReservaAud&acao=A&evaidAjax=" . $evaid . "',this)\" style=\"width:100%;text-align:center;color:red\"><b>R</b><div>";
					}
				}
				
				if($dia_s == "Domingo" || $dia_s == "Sábado"){
					$cor = "#EEE9E9";
					$link = '';
				}
				else{
					$cor = $corTR; 
				}
		 		echo "<td id=\"{$y}_data_".((strlen($td) == 1)? "0".$td : $td)."_{$mes}_{$ano}\"  style=\"text-align:center;background-color:".(($cor == "#EEE9E9")? $cor : $corTR).";color:".(($cor == "#EEE9E9")? $cor : $corTR)."\" >$link</td>";
		 		$td++;
		 	}
		 	echo "</tr>";

		 	
		}
	
	echo "</table>";
	echo "<br><center><input type='button' name='fechar' value='Fechar' onclick='self.close();'></center><div align=left><font size=1 color=blue><input type=checkbox name=labelbox> = Disponível</font><br><font size=1 color=red>&nbsp;&nbsp;<b>R</b> &nbsp; = Reservado</font></div>";
}


function dia($dia)
{
   switch($dia)
   {
      case "0" : return "Domingo"; break;
      case "1" : return "Segunda-Feira"; break;
      case "2" : return "Terça-Feira"; break;
      case "3" : return "Quarta-Feira"; break;
      case "4" : return "Quinta-Feira"; break;
      case "5" : return "Sexta-Feira"; break;
      case "6" : return "Sábado"; break;
   }
}

function mes($mes)
{
   switch($mes)
   {
      case "01" : return "Janeiro"; break;
      case "02" : return "Fevereiro"; break;
      case "03" : return "Março"; break;
      case "04" : return "Abril"; break;
      case "05" : return "Maio"; break;
      case "06" : return "Junho"; break;
      case "07" : return "Julho"; break;
      case "08" : return "Agosto"; break;
      case "09" : return "Setembro"; break;
      case "10" : return "Outubro"; break;
      case "11" : return "Novembro"; break;
      case "12" : return "Dezembro"; break;
   }
}


?>