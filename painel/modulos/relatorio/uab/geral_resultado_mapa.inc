<?php

$agrupadores = array("mantenedor" => array("codigo"=>"co.mantenedor","descricao"=>"Mantenedor"),
					 "polo" => array("codigo"=>"pl.poldsc","descricao"=>"Pólo"),
					 "nomecurso" => array("codigo"=>"co.nomecurso","descricao"=>"Nome do Curso"),
					 "tipocurso" => array("codigo"=>"co.tipocurso","descricao"=>"Tipo do Curso"),
					 "ies" => array("codigo"=>"ie.iesdsc","descricao"=>"IES"),
					 "municipio" => array("codigo"=>"mu.mundescricao","descricao"=>"Município"),
					 "uf" => array("codigo"=>"es.estuf","descricao"=>"UF"),
					 "regiao" => array("codigo"=>"re.regdescricao","descricao"=>"Região"),
					 "pais" => array("codigo"=>"pa.paidescricao","descricao"=>"País"));

if(!$_REQUEST['groupers']) $_REQUEST['groupers'] = "pais";
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
$html_dados .= "<option value=1>Agrupador</option><option value=2>Número de polos</option><option value=4>Número Matriculas Ativas</option><option value=6>Turmas Ativas</option>";
$html_dados .= "</select></td></tr>";
$html_dados .= "<tr><td colspan=2 align=center><input type=button value=Ok onclick=agrupar();> <input type=button value=Cancelar onclick=fecharFiltros();></td></tr></table>".$html_groupers_old.$html_filtro_old."</div>";

echo $html_dados;

$sql = "SELECT 
			COUNT( DISTINCT pl.polid) as tot_numpolos,
			SUM(co.nummatriculasativas::numeric) as tot_nummatriculasativas,
			SUM(co.turmasativas::numeric) 	     as tot_turmasativas
		FROM painel.consultaofertas co 
		LEFT JOIN painel.polo pl ON pl.polid = trim(co.idpolo)::integer 
		LEFT JOIN painel.ies ie ON ie.iesid = trim(co.idies)::integer 
		LEFT JOIN territorios.municipio mu ON mu.muncod = trim(co.muncod) 
		LEFT JOIN territorios.estado es ON es.estuf = mu.estuf 
		LEFT JOIN territorios.regiao re ON re.regcod = es.regcod 
		LEFT JOIN territorios.pais pa ON pa.paiid = re.paiid 
		".(($where)?"WHERE ".implode(" AND ", $where):"");

$totais = $db->pegaLinha($sql);

// monta o sql 
$sql = "SELECT 
			'".(($comselect)?"<img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"agrupacao(\'{$_REQUEST['groupers']}\',this);\" id=\"img_'||removeacento({$agrupadores[$_REQUEST['groupers']]['codigo']})||'\">":"")." '||".$agrupadores[$_REQUEST['groupers']]['codigo']." as {$_REQUEST['groupers']},
			COUNT( DISTINCT pl.polid) as numpolos,
			ROUND(((COUNT( DISTINCT pl.polid)::numeric/{$totais['tot_numpolos']})*100),1) as porcent_polo,
			SUM(co.nummatriculasativas::numeric) as nummatriculasativas,
			ROUND(((SUM(co.nummatriculasativas::numeric)/{$totais['tot_nummatriculasativas']})*100),1) as porcent_matriculasativas,
			SUM(co.turmasativas::numeric) 	     as turmasativas,
			ROUND(((SUM(co.turmasativas::numeric)/{$totais['tot_turmasativas']})*100),1) as porcent_turmasativas,
			'<center>'||ROUND(SUM(co.nummatriculasativas::numeric)/COUNT( DISTINCT pl.polid),2)||'</center>' as relacao_mat_polo, 
			'<center>'||ROUND(SUM(co.nummatriculasativas::numeric)/SUM(co.turmasativas::numeric),2)||'</center>' as relacao_mat_turma 
		FROM painel.consultaofertas co 
		LEFT JOIN painel.polo pl ON pl.polid = trim(co.idpolo)::integer 
		LEFT JOIN painel.ies ie ON ie.iesid = trim(co.idies)::integer 
		LEFT JOIN territorios.municipio mu ON mu.muncod = trim(co.muncod) 
		LEFT JOIN territorios.estado es ON es.estuf = mu.estuf 
		LEFT JOIN territorios.regiao re ON re.regcod = es.regcod 
		LEFT JOIN territorios.pais pa ON pa.paiid = re.paiid 
		".(($where)?"WHERE ".implode(" AND ", $where):"")." GROUP BY {$_REQUEST['groupers']} 
		ORDER BY {$_REQUEST['orders']}";

$cabecalho = array($agrupadores[$_REQUEST['groupers']]['descricao'],"Número de polos(A)","%","Número Matriculas Ativas(B)","%","Turmas Ativas(C)","%","B/A","B/C");
$db->monta_lista_simples($sql,$cabecalho,6000,5,'S','100%',$par2);

?>
