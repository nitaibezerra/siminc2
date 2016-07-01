<?

 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo:seleciona_unid_perfilresp.php
  
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];

//echo "<pre>";
//print_r($_REQUEST);

if ($_REQUEST["arr"]){
	$disciplinaTipoExecucaoresp = $_REQUEST["arr"];
	atribuiDisciplinaTipoExecucao($usucpf, $pflcod, $disciplinaTipoExecucaoresp);
	exit;
}

function listaDisciplinaTipoExecucao() {
	global $tipoexecucao;
	
	$db = new cls_banco();
	
	$usuarioresponsabilidade = $db->carregar("SELECT * FROM fabrica.usuarioresponsabilidade WHERE usucpf='".$_REQUEST['usucpf']."' AND pflcod='".$_REQUEST['pflcod']."' AND rpustatus='A'");
	if($usuarioresponsabilidade[0]) {
		foreach($usuarioresponsabilidade as $ur) {
			$urs[$ur['dspid']][$ur['tpeid']] = true;
		}
	}
	
	// SQL para buscar estados existentes
	$disciplinas = $db->carregar("SELECT dspid, dspdsc
								   FROM fabrica.disciplina
							       WHERE dspstatus = 'A' 
								   ORDER BY dspdsc");
	$count = count($disciplinas);
	
	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++) {
		
		$codigo    = $disciplinas[$i]["dspid"];
		$descricao = $disciplinas[$i]["dspdsc"];
		
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "<tr bgcolor=\"".$cor."\">
			  <td align=\"right\" nowrap>".$descricao."</td>";
		
		if($tipoexecucao[0]) {
			foreach($tipoexecucao as $tpe) {
				echo "<td align=center><input type=radio name=arr[".$codigo."] value=".$tpe['tpeid']." ".(($urs[$codigo][$tpe['tpeid']])?"checked":"")."></td>";
			}
		}
	}
			
}

function atribuiDisciplinaTipoExecucao($usucpf, $pflcod, $arr) {
	
	$db = new cls_banco();
	
	$data = date("Y-m-d H:i:s");
	$db->executar("UPDATE fabrica.usuarioresponsabilidade SET rpustatus = 'I' 
				   WHERE usucpf = '". $usucpf ."' AND 
				   		 pflcod = '". $pflcod ."' AND 
				   		 dspid IS NOT NULL AND 
				   		 tpeid IS NOT NULL");
	
	if ($arr) {
		
		foreach($arr as $dspid => $tpeid) {
			
			$dadosur = $db->carregar("SELECT * FROM fabrica.usuarioresponsabilidade 
									  WHERE usucpf = '". $usucpf ."' AND 
									  		pflcod = '". $pflcod ."' AND 
									  		dspid = '". $dspid ."' AND 
									  		tpeid = '". $tpeid ."'");
			
			if($dadosur[0]) {
				// Se existir registro atualizar para ativo
				$db->carregar("UPDATE fabrica.usuarioresponsabilidade
   							   SET rpustatus = 'A', rpudata_inc= NOW()
 							   WHERE usucpf = '". $usucpf ."' AND 
 							   		 pflcod = '". $pflcod ."' AND 
 							   		 dspid = '". $dspid ."' AND 
 							   		 tpeid = '". $tpeid ."'");
				
			} else {
				
				// Se não existir, inserir novo
				$db->executar("INSERT INTO fabrica.usuarioresponsabilidade (
            				   pflcod, usucpf, dspid, tpeid, rpustatus, rpudata_inc)
    						   VALUES ('". $pflcod ."', '". $usucpf ."', '". $dspid ."', '". $tpeid ."', 'A', NOW());");
				
			}
			
		}
	}
	$db->commit();
	
	echo '<script>
			alert(\'Operação realizada com sucesso!\');
			window.parent.opener.location.reload();
			self.close();
		  </script>';
	
}

?>

<?flush();?>
<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Disciplinas</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<?
		$tipoexecucao = $db->carregar("SELECT tpeid, tpedsc
									   FROM fabrica.tipoexecucao
								       WHERE tpestatus = 'A' 
									   ORDER BY tpedsc");
		
		?>
		<form name="formassocia" action="cadastro_responsabilidade_disciplinatipoexecucao.php" method="post">
		<input type="hidden" name="usucpf" value="<?=$usucpf?>">
		<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		<div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
					<thead>
						<tr>
						<td class="SubTituloCentro">Disciplinas</td>
						<?
						if($tipoexecucao[0]) {
							foreach($tipoexecucao as $tpe) {
								echo "<td class=SubTituloCentro>".$tpe['tpedsc']."</td>";
							}
						}
						?>
						</tr>
					</thead>
					<?php listaDisciplinaTipoExecucao(); ?>
				</table>
		</div>
		</form>
		<!-- Submit do Formulário -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="Button" name="ok" value="OK" onclick="document.formassocia.submit();" id="ok">
					<input type="Button" name="ok" value="Limpar" onclick="limpar();">
				</td>
			</tr>
		</table>
	</body>
</html>

<script language="JavaScript">

function limpar() {
	var form = document.formassocia;
	
	for(i=0;i<form.elements.length;i++) {
		if(form.elements[i].type == "radio") {
			form.elements[i].checked = false;
		}
	}

}


</script>