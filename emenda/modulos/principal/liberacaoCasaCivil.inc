<?php

//if( is_array( $_POST['edeid'] ) ){
if( $_POST['requisicao'] == 'salvar' ){
//	ver($_POST, d);

	$sql = "SELECT DISTINCT
				CASE WHEN ede.emdliberado = 'S' THEN
					CASE WHEN (
						SELECT
							count(exf2.semid)
						FROM
							emenda.execucaofinanceira exf2
						WHERE exf2.pedid in 
							( SELECT pede2.pedid FROM emenda.ptemendadetalheentidade pede2 WHERE pede2.edeid IN (
							SELECT ede2.edeid FROM emenda.emendadetalheentidade ede2 WHERE ede2.emdid IN 
								( SELECT ed2.emdid FROM emenda.emendadetalhe ed2 WHERE ed2.emeid = ee.emeid ) 
							))
						AND
							exf2.semid = 4 
						) = 0 THEN 
                		0
                	END	
                END as acao,
                ede.edeid,
				case when ee.emerelator = 'S' then ea.autnome||' - Relator Geral' else ea.autnome end as autor,
				ee.emecod as numero,
                --ef.acacod||' - '||ef.acadsc as uo,
				(CASE WHEN ee.emetipo = 'E' THEN 'Emenda'
					ELSE 'Complemento' END) as tipoemenda,
                CASE WHEN ee.resid is not null THEN er.resdsc ELSE 'Não Informado' END as responsavel,
				ef.fupfuncionalprogramatica as funcional,
                ef.fupdsc,
                ed.foncod,
				ed.gndcod, 
			    ed.mapcod,
				ent.enbcnpj||' ',
				ent.enbnome as entidade,
				ede.edevalor as valorentidade
			FROM emenda.emenda ee
			INNER JOIN emenda.autor ea ON ea.autid = ee.autid
			LEFT JOIN emenda.v_funcionalprogramatica ef ON ef.acaid = ee.acaid
			LEFT JOIN public.unidade pu ON pu.unicod = ef.unicod
			INNER JOIN emenda.emendadetalhe ed ON ed.emeid = ee.emeid 
			left JOIN emenda.emendadetalheentidade ede ON ede.emdid = ed.emdid
			left JOIN emenda.entidadebeneficiada ent ON ent.enbid = ede.enbid 
			LEFT JOIN emenda.responsavel er ON er.resid = ee.resid
			LEFT JOIN emenda.entbeneficiadacontrapartida ebc on ebc.enbid = ent.enbid
					AND ebc.ebcexercicio = '{$_SESSION["exercicio"]}'
			LEFT JOIN
				(select max(iedid) as iedid, emdid from emenda.iniciativaemendadetalhe GROUP BY emdid) ie ON ie.emdid = ed.emdid
			inner join public.gnd gn on gn.gndcod = ed.gndcod 
				and gn.gndstatus = 'A'
		    inner join public.modalidadeaplicacao map on map.mapcod = ed.mapcod
		    left join public.fonterecurso fon on fon.foncod = ed.foncod 
		    	and fon.fonstatus = 'A'
--		    LEFT JOIN
--		    	emenda.ptemendadetalheentidade pede ON pede.edeid = ede.edeid
--			LEFT JOIN
--				emenda.execucaofinanceira exf ON exf.pedid = pede.pedid
			WHERE
				ede.edestatus = 'A' 
				AND ee.emeano >= '{$_SESSION["exercicio"]}'
				{$_SESSION['emenda']['where']}				
			ORDER BY
				responsavel, autor, numero, entidade";
				
	$dados = $db->carregar( $sql );
	
	unset($_SESSION['emenda']['where']);
	
//	ver($sql, d);
	
	$arrEdeid = array();
	foreach ($dados as $dado ){
		if( $dado['acao'] == '0' ){
			array_push( $arrEdeid, $dado['edeid'] );
		}
	}

	if( is_array( $_POST['edeid'] ) ){
		$arrDif = array_diff( $arrEdeid, $_POST['edeid'] );
	} else {
		$arrDif = $arrEdeid;
	}

	if( is_array( $_POST['edeid'] ) ){
		foreach ($_POST['edeid'] as $key => $v) {
			$sql = "UPDATE emenda.emendadetalheentidade SET emdliberado = 'S' WHERE edeid = $v";
			$db->executar( $sql );		
		}
	}

	if( count($arrDif) ){
		foreach ($arrDif as $key => $v) {
			$sql = "UPDATE emenda.emendadetalheentidade SET emdliberado = 'N' WHERE edeid = $v";
			$db->executar( $sql );		
		}
	}
	$db->commit();
	$db->sucesso('principal/liberacaoCasaCivil');
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo('Limite Autorizado', '');
?>
<form action="" method="post" id="formliberacaoCasaCivil" name="formliberacaoCasaCivil">
<input type="hidden" value="" name="requisicao" id="requisicao"/>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
<tr>
	<td width="15%" class="subtitulodireita"><b>Emenda N°:</b></td>
	<td colspan="3" width="35%">
		<?
			$emecod = $_REQUEST['emecod'];
			echo campo_texto('emecod', 'N', 'S', '', 27, 27, '[#]', '', '', '', 0, '', '','', "this.value=mascaraglobal('[#]',this.value);" );
		?>
	</td>
</tr>
<tr>
	<td class="subtitulodireita"><b>GND:</b></td>
	<td>
		<?php 
			
			$gndcod = $_REQUEST["gndcod"];
		
			$sql = "SELECT DISTINCT 
						ed.gndcod as codigo,
						ed.gndcod as descricao
					FROM
						emenda.emendadetalhe ed
					INNER JOIN
						emenda.emenda ee ON ee.emeid = ed.emeid";
			
			$db->monta_combo("gndcod", $sql, "S", "Todos", '', '', '', '', 'N','gndcod');
			
		?>
	</td>
</tr>
<tr>
	<td class="subtitulodireita"><b>Autor:</b></td>
	<td colspan="3">
		<?
			$autid = $_REQUEST['autid'];
			
			$sql = "SELECT 
					  autid as codigo,
					  autnome as descricao
					FROM 
					  emenda.autor
					WHERE 
					  autstatus = 'A'
					ORDER BY autnome";
			$db->monta_combo( "autid", $sql, 'S', 'Selecione...', '', '', '', '265','','autid');
			
		?>
	</td>
</tr>
<tr>
	<td class="subtitulodireita"><b>Responsável:</b></td>
	<td colspan="3">
		<?
			$resid = $_REQUEST['resid'];
			echo comboResponsavel($_SESSION['usucpf'], $resid, '');
		?>
	</td>
</tr>
<tr>
	<td class="subtitulodireita"><b>CNPJ:</b></td>
	<td colspan="3">
		<?
			$enbcnpj = $_REQUEST['enbcnpj']; 
			echo campo_texto( 'enbcnpj', 'N', 'S', '', 27, 20, '', '', '', '', 0, '', "this.value=mascaraglobal('##.###.###/####-##',this.value);" );
		?>
	</td>
	<td class="subtitulodireita" width="15%"><b>Nome do Órgão ou Entidade:</b></td>
	<td width="35%">
		<?
			$enbnome = $_REQUEST['enbnome']; 
			echo campo_texto( 'enbnome', 'N', 'S', '', 35, 50, '', '', '', '', 0, '' );
		?>
	</td>
</tr>
<tr>
	<td class="subtitulodireita"><b>Limite Autorizado:</b></td>
	<td>
		<input type="radio" value="S" id="liberadocasacivil" name="liberadocasacivil" <? if($_REQUEST["liberadocasacivil"] == "S") { echo "checked"; } ?> /> Sim
		<input type="radio" value="N" id="liberadocasacivil" name="liberadocasacivil" <? if($_REQUEST["liberadocasacivil"] == "N") { echo "checked"; } ?> /> Não
		<input type="radio" value="" id="liberadocasacivil" name="liberadocasacivil" <? if($_REQUEST["liberadocasacivil"] == "") { echo "checked"; } ?> /> Todos
	</td>
</tr>
<tr bgcolor="#D0D0D0">
	<td align="left" width="20%">
		<label for='seleciona_todos'><input type='checkbox' id='seleciona_todos' onclick="selecionaTodos();"  /> <b>Selecionar Todos</b></lable>
	</td>
	<td align="left" width="80%" colspan="5"><input type="button" value="Pesquisar" id="btnPesquisar" name="btnPesquisar" onclick="pesquisarLimite();" style="cursor: pointer;"/>&nbsp;
				<input type="button" value="Limpar Pesquisa" id="btnLimpar" name="btnLimpar" onclick="limparPesquisa();" style="cursor: pointer;"/>&nbsp;
	</td>
</tr>
<tr bgcolor="#D0D0D0">
	<td colspan="6">
	<?
	$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );
//	if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) || in_array( ANALISTA_FNDE, $arPerfil ) ){
	if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil )){
		echo '<input type="button" value="Salvar Limite" id="btnSalvar" name="btnSalvar" onclick="salvarCasaCivil();" style="cursor: pointer;"/>';
	} else {
		echo '<input type="button" value="Salvar Limite" id="btnSalvar" name="btnSalvar" disabled style="cursor: pointer;"/>';
	}
	?>
	</td>
</tr>
</table>
</form>

<?

$where .= ( is_numeric($_POST['emecod']) ) ? " AND ee.emecod = '".$_POST['emecod']."'" : "";
$where .= ( is_numeric($_POST['gndcod']) ) ? " AND ed.gndcod = '".$_POST['gndcod']."'" : "";
$where .= ( is_numeric($_POST['autid']) ) ? " AND ea.autid = '".$_POST['autid']."'" : "";
$where .= ($_POST['resid']) ? " AND er.resid = ".$_POST['resid'] : "";
$where .= ($_POST['enbcnpj']) ? " AND ent.enbcnpj = '".str_replace( array(".","/","-"), "", $_POST['enbcnpj'] )."'" : "";
$where .= ($_POST['enbnome']) ? " AND UPPER( removeacento(ent.enbnome) ) like '%".removeAcentos( str_to_upper( $_POST['enbnome']) )."%'" : "";
$where .= ($_POST['liberadocasacivil']) ? " AND ede.emdliberado = '".$_POST['liberadocasacivil']."'" : "";

$_SESSION['emenda']['where'] = $where;

$sql = "SELECT DISTINCT
				CASE WHEN ede.emdliberado = 'N' THEN
					'<center><input type=\"checkbox\" id=\"edeid[' || ede.edeid || ']\" name=\"edeid[]\" value=\"' || ede.edeid || '\" /><input type=\"hidden\" id=\"requisicao\" name=\"requisicao\" value=\"salvar\"></center>'
                ELSE
					CASE WHEN (
						SELECT
							count(exf2.semid)
						FROM
							emenda.execucaofinanceira exf2
						WHERE exf2.pedid in 
							( SELECT pede2.pedid FROM emenda.ptemendadetalheentidade pede2 WHERE pede2.edeid IN (
							SELECT ede2.edeid FROM emenda.emendadetalheentidade ede2 WHERE ede2.emdid IN 
								( SELECT ed2.emdid FROM emenda.emendadetalhe ed2 WHERE ed2.emeid = ee.emeid ) 
							))
						AND
							exf2.semid = 4 
						) > 0 THEN 
               -- 	CASE WHEN exf.semid = 4 THEN 
                		'<center><input type=\"checkbox\" id=\"edeid[' || ede.edeid || ']\" name=\"edeid[]\" value=\"' || ede.edeid || '\" disabled=\"disabled\" checked /><input type=\"hidden\" id=\"requisicao\" name=\"requisicao\" value=\"salvar\"></center>'
                	ELSE
                		'<center><input type=\"checkbox\" id=\"edeid[' || ede.edeid || ']\" name=\"edeid[]\" value=\"' || ede.edeid || '\" checked /><input type=\"hidden\" id=\"requisicao\" name=\"requisicao\" value=\"salvar\"></center>'
                	END	
                END as acao,
				case when ee.emerelator = 'S' then ea.autnome||' - Relator Geral' else ea.autnome end as autor,
				ee.emecod as numero,
                --ef.acacod||' - '||ef.acadsc as uo,
				(CASE WHEN ee.emetipo = 'E' THEN 'Emenda'
					ELSE 'Complemento' END) as tipoemenda,
                CASE WHEN ee.resid is not null THEN er.resdsc ELSE 'Não Informado' END as responsavel,
				ef.fupfuncionalprogramatica as funcional,
                ef.fupdsc,
                ed.foncod,
				ed.gndcod, 
			    ed.mapcod,
				ent.enbcnpj||' ',
				ent.enbnome as entidade,
				ede.edevalor as valorentidade
			FROM emenda.emenda ee
			INNER JOIN emenda.autor ea ON ea.autid = ee.autid
			LEFT JOIN emenda.v_funcionalprogramatica ef ON ef.acaid = ee.acaid
			LEFT JOIN public.unidade pu ON pu.unicod = ef.unicod
			INNER JOIN emenda.emendadetalhe ed ON ed.emeid = ee.emeid 
			left JOIN emenda.emendadetalheentidade ede ON ede.emdid = ed.emdid
			left JOIN emenda.entidadebeneficiada ent ON ent.enbid = ede.enbid 
			LEFT JOIN emenda.responsavel er ON er.resid = ee.resid
			LEFT JOIN emenda.entbeneficiadacontrapartida ebc on ebc.enbid = ent.enbid
					AND ebc.ebcexercicio = '{$_SESSION["exercicio"]}'
			LEFT JOIN
				(select max(iedid) as iedid, emdid from emenda.iniciativaemendadetalhe GROUP BY emdid) ie ON ie.emdid = ed.emdid
			inner join public.gnd gn on gn.gndcod = ed.gndcod 
				and gn.gndstatus = 'A'
		    inner join public.modalidadeaplicacao map on map.mapcod = ed.mapcod
		    left join public.fonterecurso fon on fon.foncod = ed.foncod 
		    	and fon.fonstatus = 'A'
--		    LEFT JOIN
--		    	emenda.ptemendadetalheentidade pede ON pede.edeid = ede.edeid
--			LEFT JOIN
--				emenda.execucaofinanceira exf ON exf.pedid = pede.pedid
			WHERE
				ede.edestatus = 'A' 
				AND ee.emeano >= '{$_SESSION["exercicio"]}'
				$where				
			ORDER BY
				responsavel, autor, numero, entidade";

/*$html = '<form action="" method="post" id="formulario" name="formulario">
		<input type="hidden" value="" name="requisicao" id="requisicao"/>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr bgcolor="#D0D0D0">
				<td align="left" width="20%">
					<label for=\'seleciona_todos\'><input type=\'checkbox\' id=\'seleciona_todos\' onclick="selecionaTodos();"  /> <b>Selecionar Todos</b></lable>
				</td>
				<td align="left" width="80%"><input type="button" value="Pesquisar" id="btnPesquisar" name="btnPesquisar" onclick="pesquisarLimite();" style="cursor: pointer;"/>&nbsp;';
				if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) || in_array( ANALISTA_FNDE, $arPerfil ) ){
					$html .= '<input type="button" value="Salvar" id="btnSalvar" name="btnSalvar" onclick="salvarCasaCivil();" style="cursor: pointer;"/>';
				} else {
					$html .= '<input type="button" value="Salvar" id="btnSalvar" name="btnSalvar" disabled style="cursor: pointer;"/>';
				}
$html .= '		</td>
			</tr>
		</table></form>';

echo $html;*/
$cabecalho = array( "", "Autor", "Código", "Tipo Recurso", "Resp", "Funcional Prg", "Título", "Fon", "GND", "Mod", "CNPJ", "Entidade", "Valor(R$)");
//$db->monta_lista_simples($sql, $cabecalho, 100000, 30, 'N', '95%', '', '', '', '', true);	
//echo '</form>';
//$arWidth = array('', '7%', '4%', '4%', '3%', '10%', '30%', '3%', '3%', '3%', '6%', '20%', '5%');
//$arTotal = $db->carregar( $sql );
//echo '<div id="div_rolagem" style="overflow-x: auto; overflow-y: auto; width:97%; height:450px;">';
$db->monta_lista($sql, $cabecalho, 5000000, 10, 'N', 'center', '', 'formlibera', '','');
//echo '</div>';
/*echo '<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
		<tbody><tr bgcolor="#ffffff"><td><b>Total de Registros: '.sizeof($arTotal).'</b></td><td></td></tr></tbody></table>';*/

?>
<script type="text/javascript">

/*window.onload = function(){
	document.getElementById('btnSalvar').onclick = salvarCasaCivil;
	document.getElementById('seleciona_todos').onclick = selecionaTodos;	
};*/

function selecionaTodos(){
	var selecionaTodos 	= document.getElementById("seleciona_todos");
	var edeid 			= document.getElementsByName("edeid[]");
	
	for(var i=0; i<edeid.length; i++) {
		if( edeid[i].disabled == false ){
			if(selecionaTodos.checked) {
				if(!edeid[i].checked && !edeid[i].disabled) edeid[i].checked = true;
			} else {
				if(edeid[i].checked) edeid[i].checked = false;
			}
		}
	}
}

function pesquisarLimite(){
	var form = document.getElementById('formliberacaoCasaCivil');
	document.getElementById('requisicao').value = 'pesquisa';
	form.submit();
}
function limparPesquisa(){
	var form = document.getElementById('formliberacaoCasaCivil');
	var resid			=	document.getElementsByName("resid")[0];
	var enbcnpj			=	document.getElementsByName("enbcnpj")[0];
	var enbnome			=	document.getElementsByName("enbnome")[0];
	var emecod			=	document.getElementsByName("emecod")[0];
	var autid			=	document.getElementsByName("autid")[0];
	var gndcod			=	document.getElementsByName("gndcod")[0];
	var liberadocasacivil =	document.getElementsByName("liberadocasacivil")[0];
	
	resid.value			=	"";
	emecod.value		=	"";
	autid.value			=	"";
	enbcnpj.value	=	"";
	enbnome.value		=	"";
	gndcod.value		=	"";
	liberadocasacivil.value	=	"";
	document.getElementById('requisicao').value = '';
	form.submit();
}

function salvarCasaCivil(){
	//var form = document.getElementById('formulario');
	var form = document.getElementById('formlibera');
	//document.getElementById('requisicao').value = 'salvar';
	form.submit();
}
</script>