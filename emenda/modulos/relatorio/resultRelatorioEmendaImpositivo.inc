<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<?php
set_time_limit(0);

!empty( $_REQUEST["unicod"] )   ? $filtro[] = " vf.unicod = '{$_REQUEST["unicod"]}'" : "";
!empty( $_REQUEST["acacod"] ) 	? $filtro[] = " vf.acacod = '{$_REQUEST["acacod"]}'" : "";
!empty( $_REQUEST["autid"] )    ? $filtro[] = " e.autid  = '{$_REQUEST["autid"]}'"  : "";
!empty( $_REQUEST["etoid"] ) 	? $filtro[] = " e.etoid = {$_REQUEST["etoid"]}" : "";
!empty( $_REQUEST["emeano"] ) 	? $filtro[] = " e.emeano = {$_REQUEST["emeano"]}" : "";

($_REQUEST["ediimpositivo"] == 'PA' || $_REQUEST["ediimpositivo"] == 'NH' || $_REQUEST["ediimpositivo"] == 'TO' ) ? $filtro[] = " ei.ediimpositivo = '{$_REQUEST["ediimpositivo"]}'" : "";

if( !empty($_REQUEST['pta']) ){
	if( $_REQUEST['pta'] == 'S' ){
		$filtro[] = " ed.emdid in (select emdid from emenda.emendadetalheentidade ede
										inner join emenda.ptemendadetalheentidade pt on pt.edeid = ede.edeid 
                                    	inner join emenda.planotrabalho pr on pr.ptrid = pt.ptrid
                                    where ede.edestatus = 'A' and pr.ptrstatus = 'A' and pr.ptrexercicio = '{$_SESSION['exercicio']}') ";
	}elseif( $_REQUEST['pta'] == 'N' ){
		$filtro[] = " ed.emdid not in (select emdid from emenda.emendadetalheentidade ede
										inner join emenda.ptemendadetalheentidade pt on pt.edeid = ede.edeid 
                                    	inner join emenda.planotrabalho pr on pr.ptrid = pt.ptrid
                                    where ede.edestatus = 'A' and pr.ptrstatus = 'A' and pr.ptrexercicio = '{$_SESSION['exercicio']}') ";
	}
}

if( !empty($_REQUEST['analise_merito']) ){
	if( $_REQUEST['analise_merito'] == 'S' ){
		$filtro[] = " ed.emdid in (select emdid from emenda.emendadetalheentidade ede
										inner join emenda.ptemendadetalheentidade pte on pte.edeid = ede.edeid
									    inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid and ptr.ptrstatus = 'A'
									    inner join emenda.analise a on a.ptrid = ptr.ptrid and a.anastatus = 'A'
									where
										ede.edestatus = 'A'
									    and a.anatipo = 'M'
									    and ptr.ptrexercicio = '{$_SESSION['exercicio']}') ";		
	}elseif( $_REQUEST['analise_merito'] == 'N' ){
		$filtro[] = " ed.emdid not in (select emdid from emenda.emendadetalheentidade ede
										inner join emenda.ptemendadetalheentidade pte on pte.edeid = ede.edeid
									    inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid and ptr.ptrstatus = 'A'
									    inner join emenda.analise a on a.ptrid = ptr.ptrid and a.anastatus = 'A'
									where
										ede.edestatus = 'A'
									    and a.anatipo = 'M'
									    and ptr.ptrexercicio = '{$_SESSION['exercicio']}') ";	
	}
}

if (!empty($_REQUEST["emenda_relator"])){
	$_REQUEST["emenda_relator"]  == 'S' ? $filtro[] = " e.emerelator = 'S' " : $filtro[] = " e.emerelator = 'N' ";
}

if (!empty($_REQUEST["emdimpositiva"])){
	if($_REQUEST["emdimpositiva"] == 'S'){
		$filtro[] = " e.emecod in (select distinct
			                            e.emecod
			                        from
			                            emenda.emenda e
			                            inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
			                        where
			                            e.emeano = '{$_SESSION['exercicio']}'
			                            and ed.emdimpositiva = 6)";
	} elseif($_REQUEST["emdimpositiva"] == 'N'){
		$filtro[] = " e.emecod not in (select distinct
				                            e.emecod
				                        from
				                            emenda.emenda e
				                            inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				                        where
				                            e.emeano = '{$_SESSION['exercicio']}'
				                            and ed.emdimpositiva = 6)";
	}
}

!empty($_REQUEST["descentralizacao"]) ? $filtro[] = " e.emedescentraliza = '{$_REQUEST["descentralizacao"]}'" : "";

if( $_SESSION['exercicio'] > 2014 ){
	$filtroImp = ' and ei.edeid = ede.edeid';
	$joinImp = " left join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A' ";
} 

$sql = "select 
			e.emecod||'  ' as emenda,
		    case when e.emerelator = 'S' then a.autnome||' - Relator Geral' else a.autnome end as autnome,
		    un.unicod||' - '||un.unidsc as unidade,
		    vf.acacod||' - '||vf.acadsc as acao,
		    vf.fupfuncionalprogramatica||'  ' as funcional,
		    gn.gndcod||' - '||gn.gnddsc as gnd,
		    fr.foncod||' - '||fr.fondsc as fonte,
		    ed.emdvalor as valoremenda,
		    (case when ed.emdliberacaosri is null then 0.00 else ed.emdliberacaosri end) as valoremendasri,
		    case when ei.ediimpositivo = 'NH' then 'Não Há'
		    	when ei.ediimpositivo = 'TO' then 'Total'
		        when ei.ediimpositivo = 'PA' then 'Parcial'
		    else '-' end as impedimento,
		    ei.edivalor,
		    (array_to_string(array(select
		                              im.emidescricao
		                          from 
		                              emenda.emendaimpositivo_tiposjust eit
		                              inner join emenda.emendaimpositivo im on eit.emiid = im.emiid
		                          where
		                              eit.ediid = ei.ediid), ', ')) as tipoimpedimento
		from
			emenda.emenda e
		    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
		    $joinImp
		    inner join emenda.autor a on a.autid = e.autid
		    inner join emenda.v_funcionalprogramatica vf on vf.acaid = e.acaid and vf.acastatus = 'A'
		    inner join public.unidade un on un.unicod = vf.unicod
		    inner join public.gnd gn on gn.gndcod = ed.gndcod
		    inner join public.fonterecurso fr on fr.foncod = ed.foncod
		    left join emenda.emendadetalheimpositivo ei on ei.emdid = ed.emdid and ei.edistatus = 'A' $filtroImp
		where
			vf.acastatus = 'A'
			and ed.emdimpositiva = 6
			" . ( !empty($filtro) ? " AND " . implode(" AND ", $filtro) : "" ) . "";

montaBotaoForm();
$cabecalhoBrasao .= "<table cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\" style=\"width: 100%\">";
$cabecalhoBrasao .= "<tr>" .
				"<td colspan=\"100\">" .			
					monta_cabecalho_relatorio('100') .
				"</td>" .
			  "</tr>
			  </table>";

echo $cabecalhoBrasao;

/*echo '<table class="tabela" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none; width: 100%">
	<tbody>
		<tr>
			<td width="100%" align="center">
				<label class="TituloTela" style="color:#000000;">Relatórios Emenda Impositivo</label>
			</td>
		</tr>
	</tbody>
	</table>';*/

$cabecalho = array("Emenda", "Autor", "Unidade Orçamentária", "Ação Orçamentária", "Funcional", "GND", "Fonte", "Valor Emenda", "Valor Autorizado(SRI)", "Impedimento", "Valor Impedimento", "Tipo Impedimento");

if($_REQUEST['tipo'] == "excel"){
	global $db;
	//ob_clean();
	//header('content-type: text/html; charset=ISO-8859-1');	
	//$db->sql_to_excel($sql, 'relEmendasPTA', $cabecalho, $formato);
	//echo '<script>alert("teste"); window.close();</script>';
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatImpositivo".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatImpositivo".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,10000000,5,'S','100%',$par2, '', '', '');
	
	exit;
	$exporta = "false";
} else {
	$db->monta_lista_simples($sql, $cabecalho, 10000000, 1, 'S', '100%', 'N', false, false, false, true);
}

montaBotaoForm();

echo '<script>
		$(\'loader-container\').hide();
	  </script>';

function montaBotaoForm(){
	echo '	<html>
			<head>
				<style>
				
				#loader-container,
				#LOADER-CONTAINER{
				    background: transparent;
				    position: absolute;
				    width: 100%;
				    text-align: center;
				    z-index: 8000;
				    height: 100%;
				}
				
				
				#loader {
				    background-color: #fff;
				    color: #000033;
				    width: 300px;
				    border: 2px solid #cccccc;
				    font-size: 12px;
				    padding: 25px;
				    font-weight: bold;
				    margin: 150px auto;
				}
				</style>
				</head>
			<script type="text/javascript" src="/includes/prototype.js"></script>
			<body>
			<style type="">
				@media print {.notprint { display: none } .div_rolagem{display: none} }	
				@media screen {.notscreen { display: none; }
				
				.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
				
			</style>
			<div id="loader-container" style="display: none">
		   		<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
			</div>
			<form action="" method="post" name="formulario" id="formulario">
			<table  align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td style="height: 20px;"></td>
				</tr>
				<tr>
					<td style="text-align: center;" class="div_rolagem">
						<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
					</td>
				</tr>
			</table>
			</form>
			</body>
			<script type="text/javascript">				
				function exportarExcel(){
					document.getElementById(\'formulario\').submit();
				}
				$(\'loader-container\').show();
			</script>
			</html>';
}
?>