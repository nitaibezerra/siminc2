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

  </script>
 </head>
 
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
	<? demandas_lista_usuarios(); ?>
</body>

</html>

<?php

/**
 * Função que cria a lista com os usuários que podem ser cadastrados
 * a uma determinada demanda
 * 
 * @author Fernando A. Bagno da Silva
 */
function demandas_lista_usuarios(){
	
	global $db,$dmdid,$usucpf;

	$sql = "SELECT
				sidid
	 		FROM
	 			demandas.demanda
	 		WHERE 
	 			dmdid=".$dmdid;
	
	$sidid = $db->pegaUm($sql);
	
	if ($sidid){
		$sql = "SELECT
					u.usucpf
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
					u.usucpf
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
				";
	
		/*
		$sql = "SELECT DISTINCT
					u.usucpf || ' ' AS codigo,
					'<a href=\'javascript: vincula(' || u.usucpf || ','  || u.usunome ||  ');\'>' || u.usunome || '</a>' AS descricao
					--'<a href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdid || '</a>' AS codigo,
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
				ORDER BY 2
				";
		*/
	}
	$dados = $db->carregar($sql);
	$dados = $dados ? $dados : array();

	$arrRows[] = "";
	
	foreach($dados as $dados2){
			$arrRows[] = "".$dados2['usucpf']."";
   	}

	//echo count($arrRows);
	//die;
   	
	$sql = "SELECT 
				d.usucpfexecutor as cpf, 
				--'<a href=\'javascript: vincula(' || d.usucpfexecutor || ','  || REPLACE(u.usunome, ' ', '-') ||  ');\'>' || u.usunome || '</a>' AS nome,
				u.usunome AS nome,
				count(d.usucpfexecutor) as qtd
			FROM
				demandas.demanda as d
			INNER JOIN 
				seguranca.usuario as u ON u.usucpf = d.usucpfexecutor
			LEFT JOIN 
				workflow.documento doc ON doc.docid 	  = d.docid
			LEFT JOIN 
				workflow.estadodocumento ed ON ed.esdid = doc.esdid
			WHERE 
				d.usucpfexecutor in ('".implode("','",$arrRows)."')
				AND d.dmdstatus = 'A'
				AND ed.esdstatus = 'A' 
				AND doc.esdid in (91,92,93,94)
			GROUP BY 
				d.usucpfexecutor, u.usunome
			ORDER BY 
				3	
			";
	
	//$cabecalho = array("CPF", "Nome do Responsável", "Demandas em Aberto");
	
	//$db->monta_lista( $sql, $cabecalho, 200, 10, 'N', 'left', '');
	
			?>
			<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<tr >
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; "  >
						<b>CPF</b>		 					
					</td>
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
						<b>Nome do Responsável</b>
					</td>
					<td align='center' valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
						<b>Demandas em Aberto</b>
					</td>
				</tr>
			<?
			$listas = $db->carregar( $sql );
			$listas = $listas ? $listas : array();	
			$listausu = array();
			$i=0;
			foreach($listas as $lts){
				$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ;	

				$listausu[] = $lts['cpf'];
				$listausu[] = $lts['nome'];
			?>
			 	<tr bgcolor="<?=$cor?>" onmouseout="this.style.backgroundColor='<?= $cor ?>';" onmouseover="this.style.backgroundColor='#ffffcc';">
			 		<td >
			 			<?=$lts['cpf']?>
			 		</td>
			 		<td >
			 			<a href="javascript: vincula('<?=$lts['cpf']?>','<?=$lts['nome']?>');"><?=$lts['nome']?></a>
			 		</td>
			 		<td align='center' >
			 			<?=$lts['qtd']?>
			 		</td>
			 	</tr>
			<?
			$i++;
			}
			?>
			</table>
			<?
	
}

print_r($listas);
//script que pega o conteudo dos chamados por tecnicos
	/*
	$sql = "SELECT 
				d.usucpfexecutor as cpf, 
				--'<a href=\'javascript: vincula(' || d.usucpfexecutor || ','  || REPLACE(u.usunome, ' ', '-') ||  ');\'>' || u.usunome || '</a>' AS nome,
				u.usunome AS nome,
				count(d.usucpfexecutor) as qtd
			FROM
				demandas.demanda as d
			INNER JOIN 
				seguranca.usuario as u ON u.usucpf = d.usucpfexecutor
			LEFT JOIN 
				workflow.documento doc ON doc.docid 	  = d.docid
			LEFT JOIN 
				workflow.estadodocumento ed ON ed.esdid = doc.esdid
			WHERE 
				d.usucpfexecutor in ('".implode("','",$arrRows)."')
				AND d.dmdstatus = 'A'
				AND ed.esdstatus = 'A' 
				AND doc.esdid in (91,92,93,94)
			GROUP BY 
				d.usucpfexecutor, u.usunome
			ORDER BY 
				3	
			";
		*/
?>

<p>
<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">WEEK</div>
<div style="margin: 0; padding: 0; height: 50%; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="itensComposicaoSubAcao2008">
	<?=graphWeek();?>
</div>


<?php
/*
Script que lista os dias da semana atual
*/

function graphWeek()
{

	$mes = date("m");
	$ano = date("Y");
	$dia = date("d");
	$dia_semana = date("w");
	$hora = array("8","9","10","11","12","13","14","15","16","17","18");
	
	$cont=0;
	echo "<table border=1 align=center width=95%>";
	echo "<tr><td align=center>Técnico(s)</td>";
	While($cont<=7)
	{
		if($cont>0){
		   echo "<td align=center nowrap>";
			
		   $dia_calendario = date("d/m",mktime(0,0,0,$mes,$dia-$dia_semana,$ano));
		   $dia_s_calendario = dia(date("w",mktime(0,0,0,$mes,$dia-$dia_semana,$ano)));
		   
		   
		   $linha = $dia_s_calendario .'<br>'. $dia_calendario;
		   
		   If($dia_calendario==$dia."/".$mes)
		   {
		      echo "<font color=red>";
		      echo $linha;
		      echo "</font>";
		   }
		   Else
		   {
		      echo $linha;
		   }
	
		   echo "<br>";
		   
		   foreach($hora as $hora2){
		   	echo $hora2. ':00';
		   	echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
		   }
		   
		   
		   echo "</td>";
		}
	   $dia_semana--;
	   $cont++;
	}
	echo "</tr>";
	
	//foreach($listausu as $listausu2){
		echo "<tr>";
			echo "<td nowrap>";
				echo 'FELIPE CHIAVICATTI';
				//echo $listausu['nome'];
			echo "</td>";
			echo "<td colspan=7>-";
			echo "</td>";
			echo "</tr>";
	//}
	
	echo "</table>";
	
}



Function dia($dia)
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

?>





