<?php

$agrupadores = array("paidescricao" => array("codigo"=>"pa.paidescricao","descricao"=>"País"),
					 "regdescricao" => array("codigo"=>"re.regdescricao","descricao"=>"Região"),
					 "estdescricao" => array("codigo"=>"es.estdescricao","descricao"=>"Estado"),
					 "mundescricao" => array("codigo"=>"mu.mundescricao","descricao"=>"Município"),
					 "dependencia_administrativa" => array("codigo"=>"d.dependencia_administrativa","descricao"=>"Dependência Administrativa"),
					 "localizacao" => array("codigo"=>"d.localizacao","descricao"=>"Localização")
					 );

if(!$_REQUEST['groupers']) $_REQUEST['groupers'] = "paidescricao";
if(!$_REQUEST['orders']) $_REQUEST['orders'] = "1";


if($_REQUEST['filtro'])$_REQUEST['filtros_old'][]=$_REQUEST['filtro'];

$_REQUEST['groupers_old'][] = $_REQUEST['groupers'];


if($_REQUEST['filtros_old']) {
	foreach($_REQUEST['filtros_old'] as $key => $f_old) {
		$where[] = "removeacento(".$agrupadores[$_REQUEST['groupers_old'][$key]]['codigo'].")=removeacento('".$f_old."')";
		$html_filtro_old .= "<input type=hidden name=filtros_old[] value=\"".tirar_acentos($f_old)."\">";
	}
}

if($_REQUEST['groupers_old']) {
	foreach($_REQUEST['groupers_old'] as $key => $g_old) {
		$html_groupers_old .= "<input type=hidden name=groupers_old[] value=\"".$g_old."\">";
	}
}


//montando
$html_dados .= "<div id=\"id_".$_REQUEST['groupers']."\" style=display:none>";
$html_dados .= "<table><tr><td align=right><font size=1>Agrupador:</font></td><td>";
$html_dados .= "<select id=select_grouper>";
$comselect=false;
foreach($agrupadores as $indice => $arrA) {
	if(!in_array($indice,$_REQUEST['groupers_old'])) {
		$html_dados .= "<option value=".$indice.">".$arrA['descricao']."</option>";
		$comselect=true;		
	}
}

$html_dados .= "</select></td></tr>";
$html_dados .= "<tr><td align=right><font size=1>Ordem:</font></td><td><select id=select_order>";
$html_dados .= "<option value=1>Agrupador</option>
				<option value=2>Fundamental Incompleto</option>
				<option value=3>Fundamental Completo</option>
				<option value=4>Ensino Médio - Normal/Magistério</option>
				<option value=5>Ensino Médio - Normal/Magistério Específico Indígena</option>
				<option value=6>Ensino Médio</option>
				<option value=7>Superior sem Licenciatura</option>
				<option value=7>Superior com Licenciatura</option>
				<option value=8>Total</option>";
$html_dados .= "</select></td></tr>";
$html_dados .= "<tr><td colspan=2 align=center><input type=button value=Ok onclick=agrupar();> <input type=button value=Cancelar onclick=fecharFiltros();></td></tr></table>".$html_groupers_old.$html_filtro_old."</div>";

echo $html_dados;

$where[] = "id_dependencia_adm IN('1','2','3')";

$sql = "SELECT 
		'".(($comselect)?"<img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"agrupacao(\'{$_REQUEST['groupers']}\',this);\" id=\"img_'||removeacento({$agrupadores[$_REQUEST['groupers']]['codigo']})||'\">":"")." '||".$agrupadores[$_REQUEST['groupers']]['codigo']." as {$_REQUEST['groupers']},
		SUM(docfundi::integer) as funinc,
		SUM(docfundc::integer) as funcom,
		SUM(docemnm::integer) as ensnor,
		SUM(docemnmei::integer) as ensesi,
		SUM(docem::integer) as ensmed,
		SUM(docfundi::integer)+SUM(docfundc::integer)+SUM(docemnm::integer)+SUM(docemnmei::integer)+SUM(docem::integer) as semnivel,
		SUM(docsupslic::integer) as supsel,
		SUM(docsupclic::integer) as suplic,
		SUM(doc::integer) as total 
		FROM painel.docente_sem_mat_na_graduacao d 
		LEFT JOIN territorios.municipio mu ON mu.muncod = trim(d.cod_municipio) 
		LEFT JOIN territorios.estado es ON es.estuf = mu.estuf 
		LEFT JOIN territorios.regiao re ON re.regcod = es.regcod 
		LEFT JOIN territorios.pais pa ON pa.paiid = re.paiid 
		".(($where)?"WHERE ".implode(" AND ", $where):"")." GROUP BY {$_REQUEST['groupers']} 
		ORDER BY {$_REQUEST['orders']}";


$cabecalho = array($agrupadores[$_REQUEST['groupers']]['descricao'],"Fundamental Incompleto","Fundamental Completo",
					   "Ensino Médio - Normal/Magistério","Ensino Médio - Normal/Magistério Específico Indígena",
					   "Ensino Médio","Sem Nível Superior","Superior sem Licenciatura","Superior com Licenciatura","Total");

$db->monta_lista_simples($sql,$cabecalho,6000,5,'S','100%',$par2);

?>