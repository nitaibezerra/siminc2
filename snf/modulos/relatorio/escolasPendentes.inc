<?php
	ini_set("memory_limit", "3000M");
	set_time_limit(30000);
	
	include  APPRAIZ."includes/cabecalho.inc";
	
	$sql = "
		Select 	Distinct pd.pdicodinep, 
				pd.pdenome, 
				pd.estuf, 
				pd.muncod
		From pdeinterativo.pdinterativo pd 
		Inner Join workflow.documento d ON pd.formacaodocid = d.docid
		Inner Join territorios.municipio m ON m.muncod = pd.muncod
		Where d.esdid = 416 ".( count($arWhere) ? ' AND '.implode(' AND ', $arWhere) : '')."
	";
	
	$rsPendencia = $db->carregar($sql);
	
	if($rsPendencia){
		echo "<center>";	
		echo "<div style=\"width:95%;text-align:left;padding:25px;font-size:14px;border:1px solid black;margin:15px;\">As escolas abaixo relacionadas não enviaram o Plano de Formação Continuada de seu corpo docente.<p>";
		
		echo "<ul>";
		foreach($rsPendencia as $dados){
			echo "<li>".$dados['pdenome']."</li>";
		}
		echo "</ul>";
		 
		echo "</p>Você não possui autorização para avaliar o planejamento.</div>";
		echo "</center>";
	}else{
		echo "Sem registros.";
	}
?>