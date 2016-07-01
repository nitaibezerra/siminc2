<?php
pde_verificaSessao();
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pdeid = ($_POST['pdeid']) ? $_POST['pdeid'] : $_SESSION['pdeid'];

if(!$pdeid) {
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

if($_POST["pdcid"] && $_POST["pdcid"][0] != "") {
	$pdcid = $_POST["pdcid"];
} else {
	$sqlPdcid = "SELECT
							pdcid
						  FROM
			 				pdeescola.periododisciplinacritica
						  WHERE
			 				pdeid = ".$pdeid;
	$pdcid = $db->pegaUm( $sqlPdcid );
}
function salva() {
	global $db;

	if($_POST["submetido"]) {
		$db->executar("UPDATE pdeescola.disciplinacritica SET dicaltataxa = 'f' WHERE pdcid = ".$_POST["pdcid"]);

		for($i=0; $i<count($_POST["disciplina"]); $i++) {
			if($_POST["disciplina"] != ''){
				$db->executar("UPDATE pdeescola.disciplinacritica SET dicaltataxa = 't' WHERE dicid = ".$_POST["disciplina"][$i]);
		}
	}
	 
		$pdeid = $_SESSION["pdeid"];
		$existe_preenchimento = $db->pegaUm("SELECT 
											pprid
										 FROM 
										 	pdeescola.pdepreenchimento
										 WHERE
										 	pdeid = '$pdeid'
											 AND
											 	ppritem = 'fr3'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento ,ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'fr1', 'fr3','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p );
 
		}

		$db->commit();  
	}
}

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include APPRAIZ.'includes/cabecalho.inc';
echo '<br />';
$titulo_modulo = 'Instrumento 1 - Ficha Resumo 1';
monta_titulo($titulo_modulo, '');

if($pdcid) {
	$dados = $db->carregar("SELECT
								td.tdidescricao as disciplina,
								sc.scedescricao as serieciclo,
								dc.dicturma as turma,
								tp.titdescricao as turno,
								dc.dictaxareprovacao as taxa,
								dc.dicid as id,
								dc.dicaltataxa as altataxa
							FROM
								pdeescola.disciplinacritica dc
							INNER JOIN 
								pdeescola.tipodisciplina td ON td.tdiid = dc.tdiid
							INNER JOIN 
								pdeescola.seriecicloescolar sc ON sc.sceid = dc.sceid
							INNER JOIN 
								pdeescola.tipoturno tp ON tp.titid = dc.titid
							WHERE
					 			dc.pdcid = ".$pdcid."
							ORDER BY
								dc.dicid");

}

?>
<?=cabecalhoPDE();?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?=subCabecalho('3) Disciplinas com altas taxas de reprovação no Ensino Fundamental'); ?>
<form method="post" name="formFichaResumo" id="formFichaResumo">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<input type="hidden" name="pdcid" value="<?=$pdcid?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<thead>
	<tr>
		<th width="30%">DISCIPLINA</th>
		<th width="10%">SÉRIE/ANO/CICLO</th>
		<th width="10%">TURMA</th>
		<th width="20%">TURNO</th>
		<th width="10%">TAXA DE REPROVAÇÃO/RETENÇÃO</th>
		<th width="10%">ALTA TAXA</th>
	</tr>
</thead>
<tbody>
<?php

if($dados[0]["disciplina"]) {
	$count = 0;
	for($i=0; $i<count($dados); $i++) {
		$count++;
		$cor = ($count % 2) ? $cor = "#f4f4f4" : "#e0e0e0";
		
		$checked = ($dados[$i]["altataxa"] == 't') ? "checked" : "";
	
		echo "<tr align=\"center\">
			  	<td bgcolor=\"$cor\">
			  		".$dados[$i]["disciplina"]."
			  	</td>
			  	<td bgcolor=\"$cor\">
			  		".$dados[$i]["serieciclo"]."
			  	</td>
			  	<td bgcolor=\"$cor\">
			  		".$dados[$i]["turma"]."
			  	</td>
			  	<td bgcolor=\"$cor\">
			  		".$dados[$i]["turno"]."
			  	</td>
			  	<td bgcolor=\"$cor\">
			  		".round($dados[$i]["taxa"])."%
			  	</td>
			  	<td bgcolor=\"$cor\">
			  		<input type=\"checkbox\" name=\"disciplina[]\" value=\"".$dados[$i]["id"]."\" ".$checked." />
			  	</td>
			  </tr>";
	}
}
else {
	echo "<tr align=\"center\">
		  	<td bgcolor=\"#f4f4f4\" colspan=\"6\">
		  		<br />
		  		<div style=\"color:red;\">Não existem registros cadastrados.</div>
		  		<br />
		  	</td>
		  </tr>";
}
?>
</tbody>
<tfoot>
<?php 
if($pdcid)
	echo $cForm->montarbuttons("submeteFormFichaResumo()");
else
	echo $cForm->montarbuttons();
?>
</tfoot>
</table>
</form>

<script language="javascript" type="text/javascript">

function submeteFormFichaResumo() {
	document.getElementById('formFichaResumo').submit();
	return true;
}

</script>