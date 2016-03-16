<?php
$dmdid  = $_SESSION['dmdid'];
$usucpf = $_GET['usucpf'];

if (!$dmdid){
	die('<script>
			alert(\'Falta parametros! Tente novamente.\');
			window.close();
		 </script>');
}

monta_titulo( 'Pesquisa de Responsável', 'Escolha o Técnico desejado para adicioná-lo à demanda' );
?>
<html>
 <head>
 <style type="text/css">
 
 .titulo1{background-color:rgb(227, 227, 227);text-align:center;font-weight:bold;}
 .linha1{background-color:#f5f5f5;}
 .linha2{background-color:#fdfdfd;}
 
 </style>
 <script src="../includes/calendario.js"></script>
  <script type="text/javascript" src="/includes/prototype.js"></script>
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
  <script type="text/javascript">
  	d = document;
  			
	/*
	 * Valida e envia para página pai
	*/
	function enviar(){
		usucpf = d.getElementsByName('usucpf')[0];
		
		if (usucpf.value != ''){
			window.opener.d.getElementById('usucpfexecutor').value  	  = usucpf.value;
			window.opener.d.getElementsByName('usunomeexecutor')[0].value = usucpf.options[usucpf.selectedIndex].text;;
		}else{
			window.opener.d.getElementById('usucpfexecutor').value  	  = '';
			window.opener.d.getElementsByName('usunomeexecutor')[0].value = '';
			alert('Nenhum responsável foi vinculado!');			
		}
		window.close();
	}	
	
	function vincula(cpf,nome){
		window.opener.d.getElementById('usucpfexecutor').value  	  = cpf;
		window.opener.d.getElementsByName('usunomeexecutor')[0].value = nome;
		window.close();
	}
	function montaGrafico(hr_ini,hr_fim,data,usucpf,dmdid,qtd_dmd,cor_h){
		var ini = hr_ini;
		var fim = hr_fim;
		var cpf = usucpf;
		var qtd_d = qtd_dmd;
		var demanda = dmdid;
		var cor = cor_h;
		
		ini = ini.toString().replace("01","1");
		ini = ini.toString().replace("02","2");
		ini = ini.toString().replace("03","3");
		ini = ini.toString().replace("04","4");
		ini = ini.toString().replace("05","5");
		ini = ini.toString().replace("06","6");
		ini = ini.toString().replace("07","7");
		ini = ini.toString().replace("08","8");
		ini = ini.toString().replace("09","9");
		ini = ini * 1;
		fim = fim.toString().replace("01","1");
		fim = fim.toString().replace("02","2");
		fim = fim.toString().replace("03","3");
		fim = fim.toString().replace("04","4");
		fim = fim.toString().replace("05","5");
		fim = fim.toString().replace("06","6");
		fim = fim.toString().replace("07","7");
		fim = fim.toString().replace("08","8");
		fim = fim.toString().replace("09","9");
		fim = fim * 1;
		
		while(ini <= fim){
			var div = 'cpf_' + cpf + '_dem_' + qtd_d + '_hr_' + ini + '_data_' + data;
			if(document.getElementById(div)){
				document.getElementById(div).style.backgroundColor = cor;
				document.getElementById(div).style.cursor = 'pointer';
				document.getElementById(div).innerHTML = "<div style=\"text-align:center;color: " + cor + "\" onclick=\"abreDemanda('" + demanda  + "')\" > - </div>";
			}
			ini= ini + 1; 
		}
	}
	
	function selecionaTR(cor,id){
		var cor2 = cor;
		var tr = id;
		document.getElementById(tr).style.backgroundColor = cor2;
	}
	
	function abreDemanda(demanda){
		var dmdid = demanda;
		alert('Abrirá demanda dmdid-> ' + dmdid);
	}

  </script>
 </head>
 
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
	
	<? 
	$sql = "SELECT
				sidid
	 		FROM
	 			demandas.demanda
	 		WHERE 
	 			dmdid=".$dmdid;
	
	$sidid = $db->pegaUm($sql);
	
	if ($sidid){
		$sql = "SELECT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.sidid = " . $sidid . "
				ORDER BY u.usunome	
				";		
	}else{
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario  AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.sidid IS NULL AND
					ur.ordid = (
							 	SELECT 
							 		tp.ordid
								FROM 
									demandas.demanda AS d
								LEFT JOIN 
									demandas.tiposervico AS tp ON tp.tipid = d.tipid
								WHERE 
									dmdid = $dmdid)
				ORDER BY u.usunome
				";
	
	}
	
			?>
			<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<tr >
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
						<b>Nome do Responsável</b>
					</td>
					<td align='center' valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
						<b>Qtd. Demandas Pendentes</b>
					</td>
				</tr>
			<?
			$sqlx = $sql;
			$listas = $db->carregar( $sql );
			$listas = $listas ? $listas : array();	
			foreach($listas as $lts){
				$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ;	
			?>
			 	<tr bgcolor="<?=$cor?>" onmouseout="this.style.backgroundColor='<?= $cor ?>';" onmouseover="this.style.backgroundColor='#ffffcc';">
			 		<td >
			 			<a href="javascript: vincula('<?=$lts['usucpf']?>','<?=$lts['usunome']?>');"><?=$lts['usunome']?></a>
			 		</td>
			 		<td align='center' >
			 			<?//=$lts['qtd']?>
						<?
							$sql = "SELECT 
								count(*) as qtd
							FROM
								demandas.demanda as d
							LEFT JOIN 
								workflow.documento doc ON doc.docid 	  = d.docid
							LEFT JOIN 
								workflow.estadodocumento ed ON ed.esdid = doc.esdid
							WHERE 
								d.usucpfexecutor = '".$lts['usucpf']."'
								AND d.dmdstatus = 'A'
								AND ed.esdstatus = 'A' 
								AND doc.esdid in (91,92,93,94)
							";
							
							echo $db->PegaUm( $sql );
						?>			 			
			 		</td>
			 	</tr>
			<?
			}
			?>
			</table>

<form id="formulario" name="formulario" action="" method="post">
<table class="tabela" bgcolor="#f5f5f5"
	cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="15%"  align='right' class="SubTituloDireita">Tipo:</td>
		<td>
			<input type="radio" <? (!$_POST['tipoSemana'] || $_POST['tipoSemana'] == "1")? print "checked=\"checked\"" : print ""; ?> id="tipo_diario" value="1" name="tipoSemana"> Diário 
			<input type="radio" <? ($_POST['tipoSemana'] == "7")? print "checked=\"checked\"" : print ""; ?> id="tipo_semana" value="7" name="tipoSemana"> Semanal
		</td>		
	</tr>
	<tr>
		<td width="15%"  align='right' class="SubTituloDireita">Horário:</td>
		<td>
			<input type="radio" <? (!$_POST['horario'] || $_POST['horario'] == "simples")? print "checked=\"checked\"" : print ""; ?> id="hr_simples" value="simples" name="horario"> Simples 
			<input type="radio" <? ($_POST['horario'] == "completo")? print "checked=\"checked\"" : print ""; ?> id="hr_completo" value="completo" name="horario"> Completo
		</td>		
	</tr>
	<tr>
		<td width="15%"  align='right' class="SubTituloDireita">Data:</td>
		<td>
			<?= campo_data( 'data_pesq', 'N', 'S', '', 'S' ); ?>
		</td>		
	</tr>
	<tr bgcolor="#C0C0C0">
		<td></td>
		<td colspan="2">
		<input type="button" class="botao" name="pesq" onclick="submit();" value="Pesquisar" /> 
		</td>
	</tr>
</table>
</form>
<div class="SubtituloEsquerda" style="padding: 0; margin: 0 auto;text-align: center;width:95%">Relatório de Atividades Técnicas - Demandas</div>
<div style="margin: 0 auto; padding: 0; height: 70%; width: 95%; background-color: #eee; border: none;" class="div_rolagem" id="itensComposicaoSubAcao2008">

<?php
if(strlen(trim($_POST['data_pesq'])) == 10){
	$ano = substr(trim($_POST['data_pesq']), -4,4);
	$mes = substr(trim($_POST['data_pesq']), -7,2);
	$dia = substr(trim($_POST['data_pesq']), -10,2);
}	 			
?>


		<?=graphWeek($sqlx,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$diaD = $dia,$mesD = $mes,$anoD = $ano);?>
</div>
</body>

</html>

<?php
/*
Script que lista os dias da semana atual
*/

function graphWeek($sqlx = null,$horario = null, $tipoSemana = null,$diaD = null,$mesD = null,$anoD = null)
{
	global $db;
	
	//Paramentros específicos de datas:
	($diaD)? $dia = $diaD : $dia = date("d");
	($mesD)? $mes = $mesD : $mes = date("m");
	($anoD)? $ano = $anoD : $ano = date("Y");
	$dia_semana = date("w", strtotime ("$ano/$mes/$dia"));
	
	
	//Parametro Horário: Simples (Default) / Completo
	if($horario == 'completo'){
		$hora = array("0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23");
	}
	else{
		$hora = array("8","9","10","11","12","13","14","15","16","17","18");
	}
	
	//Parametro Tipo de Gráfico: Diário (Default) / Semanal
	if($tipoSemana == '7'){
		$tipo = 7; //Semanal
		//Soma os dias da semana
		switch($dia_semana){
			case 0:
				$mais = 7;
				$menos = 0;
				break;
			case 1:
				$mais = 6;
				$menos = 1;
				break;
			case 2:
				$mais = 5;
				$menos = 2;
				break;
			case 3:
				$mais = 4;
				$menos = 3;
				break;
			case 4:
				$mais = 3;
				$menos = 4;
				break;
			case 5:
				$mais = 2;
				$menos = 5;
				break;
			case 6:
				$mais = 1;
				$menos = 6;
				break;
			default:
				$mais = 1;
				$menos = 6;
		}
		$data_inicial =  date('Y-m-d',mktime(0,0,0,$mes,$dia-$menos,$ano));
		$data_final =  date('Y-m-d',mktime(0,0,0,$mes,$dia+$mais,$ano));

	}
	else{
		$tipo = 1; //Dário
	}
	
	//Exibe o início da tabela com a classe do SIMEC
	echo "<table align=center width=95% class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\" >";
	echo "<tr class=\"titulo1\" ><td rowspan=2 align=center>Técnico(s)</td>";

	//Contador que pecorrerá o tipo de Grafico (até 7 p/ Semanal : até 1 para Diário)
	$cont=0;
	while($cont < $tipo){
		//Exibe cada dia da Semana com colspan de acordo com o tipo de Horário selecionado
		echo "<td colspan=".count($hora)." align=center nowrap>";
		
		if($tipoSemana == '7'){
			//Pega dia / mes
			$dia_calendario = date("d/m",mktime(0,0,0,$mes,$dia-$dia_semana,$ano));
			//Pega dia da semana e aplica a função "dia" (Dom, Seg, Ter, Qua, Qui, Sex, Sab)
			$dia_s_calendario = dia(date("w",mktime(0,0,0,$mes,$dia-$dia_semana,$ano)));
			//Joga na variável linha o dia da semna / dia e mes
			$linha = $dia_s_calendario .'<br>'. $dia_calendario;
			//Verifica se o dia corrente é o dia correspondente no LOOP
		}
		else{
			$dia_calendario = $dia."/".$mes;
			$linha = dia($dia_semana) .'<br>'. $dia_calendario;
		}
		
		if($dia_calendario == date("d")."/".date("m")){
			//Aplica cor vermelha no texto
			echo "<font color=red>";
		    echo $linha;
		    echo "</font>";
		}
		//Se não for o dia corrente, simplesmente exibe o texto normal
		else{
			echo $linha;
		}
		echo "<br>";
		echo "</td>";
		$dia_semana--; //decrementa o dia da semana
		$cont++; //incrementa o contador
	}
	echo "</tr>";
	echo "<tr class=\"titulo1\" >";
	
	//Zera novamente o contador para utilizar em outro LOOP
	$cont=0;
	//Exibe as horas de acordo com o tipo de Horário escolhido (12 / 24)
	while($cont < $tipo){
		foreach($hora as $hora2){
					echo "<td >".((strlen($hora2) == 1)? "0$hora2" : $hora2). "</td>";
		}
		$cont++;
	}
	echo "</tr>";
	
	//Carrega os dados do SQL
	$listausu = $db->carregar( $sqlx );
	$listausu = $listausu ? $listausu : array();
		
	$cor = 1; //variável utilizada para exibir as cores das TDs
	//Verifica a existência de dados no array e faz o LOOP
	foreach($listausu as $listausu2){
		//SQL para contar o número de demandas de cada técnico
		$sql = "SELECT count(dmdid)
		 		FROM demandas.demanda
		 		WHERE usucpfexecutor = '{$listausu2['usucpf']}'
	 			AND dmdstatus = 'A' ";
	 	//Carrega a quatidade de demandas voltada do SQL
	 	$qtd_demandas = $db->pegaUm($sql);
		//verifica se exite demanda para exibir a TR do técnico
	 	if($qtd_demandas != 0){
		 	//Faz a alternancia das cores d cada TD
		 	($cor % 2)? $linhaX = "#fdfdfd" : "#f5f5f5";
			//Exibe a TD com o nome do Técnico 
		 	echo "<tr id=\"{$listausu2['usucpf']}\" style=\"background-color:$linhaX;\" onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='$linhaX'\">";
			echo "<td ".(($qtd_demandas != 0 && $qtd_demandas != 1)? "rowspan=$qtd_demandas" : " ")." nowrap>";
			echo $listausu2['usunome'];
			echo "</td>";
			
			//verifica o número de demandas do técnico, para criar uma nova linha p/ cada demanda
			if($qtd_demandas != 1){
				//faz o LOOP enquando a demanda for maior que zero
				while($qtd_demandas > 0){		
					unset($dia_calendario); 
					unset($dia_s_calendario);
					unset($linha);
					
					//Paramentros específicos de datas:
					($diaD)? $dia = $diaD : $dia = date("d");
					($mesD)? $mes = $mesD : $mes = date("m");
					($anoD)? $ano = $anoD : $ano = date("Y");
					$dia_semana = date("w", strtotime ("$ano/$mes/$dia"));
						
					$wk=0; //contador dos dias da semana
					//Faz o LOOP enquanto o contador for menor que o tipo ( Diário = 1 / Semanal = 7)
					while($wk < $tipo){
						if($tipoSemana == '7'){
							$dia_calendario = date("d_m_Y",mktime(0,0,0,$mes,$dia-$dia_semana,$ano));
							$linha = $dia_calendario;
						}
						else{
							$dia_calendario = date("d_m_Y",strtotime("$ano/$mes/$dia"));
							$linha = $dia_calendario;
						}
						$hrs=0; //contador para exibir as cores nas TDs de horas
						//Cria as TDs para as horas de acordo com o Horario (12 / 24)
						while($hrs < count($hora)){
							($hrs % 2)? $cor_td = '#fdfdfd' : $cor_td = '#f5f5f5';
							echo "<td onmouseover=\"selecionaTR('#ffffcc','{$listausu2['usucpf']}');\" onmouseout=\"selecionaTR('$linhaX','{$listausu2['usucpf']}');\" style=\"background-color:$cor_td;color:$cor_td\" id=\"cpf_{$listausu2['usucpf']}_dem_{$qtd_demandas}_hr_{$hora[$hrs]}_data_$linha\" ><center>-</center></td>";
							$hrs++;
						}
						$dia_semana--; //decrementa o dia da semana
						$wk++; //incrementa o dia da semana
					}
					$qtd_demandas--; //decrementa as linhas por demanda
					echo "</tr>";
				}
			}else{
				unset($dia_calendario);
				unset($dia_s_calendario);
				unset($linha);
				
				//Paramentros específicos de datas:
				($diaD)? $dia = $diaD : $dia = date("d");
				($mesD)? $mes = $mesD : $mes = date("m");
				($anoD)? $ano = $anoD : $ano = date("Y");
				$dia_semana = date("w", strtotime ("$ano/$mes/$dia"));
				
				$wk=0; // contador para dias da semana
				while($wk < $tipo){		
					if($tipoSemana == '7'){
							$dia_calendario = date("d_m_Y",mktime(0,0,0,$mes,$dia-$dia_semana,$ano));
							$linha = $dia_calendario;
						}
						else{
							$dia_calendario = date("d_m_Y",strtotime("$ano/$mes/$dia"));
							$linha = $dia_calendario;
					}
					//$dia_calendario = date("d_m_Y",mktime(0,0,0,$mes,$dia-$dia_semana,$ano));
					//$linha = $dia_calendario;	
					$hrs=0; //contador para as cores das TDs de horas
					while($hrs < count($hora)){
						($hrs % 2)? $cor_td = '#fdfdfd' : $cor_td = '#f5f5f5';
						echo "<td style=\"background-color:$cor_td;color:$cor_td\" id=\"cpf_{$listausu2['usucpf']}_dem_1_hr_{$hora[$hrs]}_data_$linha\" ><center>-</center></td>";
						$hrs++;
					}
					$dia_semana--;
					$wk++;
				}
		 	}
			$demandas = pegaDemandas($listausu2['usucpf']); //função que faz as marcações nas TDs criadas									
			echo "</tr>";
			$cor++;
	 	}
	}
	echo "</table>";
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

function pegaDemandas ($usucpf){
	global $db;
	 $sql = "SELECT dmdid,
	 				priid,
	 				dmddatainiprevatendimento,
	 				dmddatafimprevatendimento
	 		FROM demandas.demanda
	 		WHERE usucpfexecutor = '$usucpf'
	 			AND dmdstatus = 'A'";
	
	 $demandas = $db->carregar($sql);

	 (!$demandas)? $demandas = array(): '';
	 
	 $qtd_dmd = 1;
	 foreach($demandas as $data){
	 	$hr_ini = date('H', strtotime($data['dmddatainiprevatendimento']));
	 	$hr_fim = date('H', strtotime($data['dmddatafimprevatendimento']));
	 	$data_i = date('d_m_Y', strtotime($data['dmddatainiprevatendimento']));
	 	$data_f = date('d_m_Y', strtotime($data['dmddatafimprevatendimento']));
	 	$dmdid = $data['dmdid'];
	 	$priid = $data['priid'];
	 	
	 	switch($priid){
	 		case 1:
	 			$cor = "#32CD32";
	 			break;
	 		case 2:
	 			$cor = "#FFFF00";
	 			break;
	 		case 3:
	 			$cor = "#FF0000";
	 			break;
	 		default:
	 			$cor = "#6495ED";
	 			break;
	 	}
	 	
	 	if($data_i == $data_f){
	 		print "<script>montaGrafico('$hr_ini','$hr_fim','$data_i','$usucpf','$dmdid','$qtd_dmd','$cor');</script>";
	 	}
	 	else{
	 		list($diai,$mesi,$anoi) = explode("_", $data_i);
	 		list($diaf,$mesf,$anof) = explode("_", $data_f);
	 		
	 		$data1 = $anoi.$mesi.$diai;
	 		$data2 = $anof.$mesf.$diaf;
	 		
	 		print "<script>montaGrafico('$hr_ini','23','$data_i','$usucpf','$dmdid','$qtd_dmd','$cor');</script>";
	 		print "<script>montaGrafico('0','$hr_fim','$data_f','$usucpf','$dmdid','$qtd_dmd','$cor');</script>";
	 		
	 		while((int)$data1 < (int)$data2){
	 			$ano = substr($data1, -8,4);
	 			$mes = substr($data1, -4,2);
	 			$dia = substr($data1, -2,2);
	 			print "<script>montaGrafico('$hr_ini','23','".$dia."_".$mes."_".$ano."','$usucpf','$dmdid','$qtd_dmd','$cor');</script>";
	 			$hr_ini = 0;
	 			$data1++;
	 		}
	 	}
	 	$qtd_dmd++;
	 }
	 
	 	return $demandas;
}
?>