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

if (!empty($_REQUEST["emenda_relator"])){
	$_REQUEST["emenda_relator"]  == 'S' ? $filtro[] = " e.emerelator = 'S' " : $filtro[] = " e.emerelator = 'N' ";
}

!empty($_REQUEST["descentralizacao"]) ? $filtro[] = " e.emedescentraliza = '{$_REQUEST["descentralizacao"]}'" : "";

if( $_SESSION['exercicio'] > 2014 ){
	$filtroImp = ' and ei.edeid = ede.edeid';
	$joinImp = " left join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A' ";
} 

$sql = "select 
			vf.esfcod,
            vf.unicod,
            vf.funcod,
            vf.sfucod,
            vf.prgcod,
            vf.acacod,
            vf.loccod,
            (CASE WHEN ed.gndcod = '3' 
            	THEN '3.3.'||ed.mapcod||'.0000'
	                WHEN ed.gndcod = '4' THEN '4.4.'||ed.mapcod||'.0000'
	                  END) as naturezaDesp,
			ed.emdimpositiva,
            ed.foncod,
            e.emecod,
            ed.emdvalor,
            formata_cpf_cnpj(eb.enbcnpj) as enbcnpj,
            case when e.etoid = 1 then pa.objeto
            	when e.etoid = 2 then ted.objeto
                when e.etoid = 3 then con.objeto
                when e.etoid = 4 then unf.objeto
            else ' - ' end as objeto,
		    ede.edevalor,
            ede.edevalordisponivel,
		    ei.edidescricao,
		    (array_to_string(array(select
		                              im.emicodigosiop||' - '||im.emidescricao
		                          from 
		                              emenda.emendaimpositivo_tiposjust eit
		                              inner join emenda.emendaimpositivo im on eit.emiid = im.emiid
		                          where
		                              eit.ediid = ei.ediid), ', ')) as tipoimpedimento
		from
			emenda.emenda e
		    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
		    inner join emenda.autor a on a.autid = e.autid
            inner join emenda.partido p on p.parid = a.parid and p.parstatus = 'A'
		    inner join emenda.v_funcionalprogramatica vf on vf.acaid = e.acaid and vf.acastatus = 'A'
            inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
            inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid and eb.enbstatus = 'A'
		    left join emenda.emendadetalheimpositivo ei on ei.emdid = ed.emdid and ei.edistatus = 'A' and ei.edeid = ede.edeid
            left join(
            	select ve.edeid, ptr.ptrid, ptr.ptrobjetivofederal as objeto
                from emenda.v_emendadetalheentidade ve
                	inner join emenda.ptemendadetalheentidade pte on pte.edeid = ve.edeid
                    inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid and ptr.ptrstatus = 'A'
                where ve.edestatus = 'A'
                	and ve.etoid = 4
                    and ve.emeano = '".$_SESSION['exercicio']."' and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
            ) unf on unf.edeid = ede.edeid
            left join(
            	select ve.edeid, ptr.ptrid, array_to_string(array(select ini.ininome from emenda.ptiniciativa pti
                													inner join emenda.iniciativa ini on ini.iniid = pti.iniid
                                                                   where pti.ptrid = ptr.ptrid ), ', ') as objeto 
                from emenda.v_emendadetalheentidade ve
                	inner join emenda.ptemendadetalheentidade pte on pte.edeid = ve.edeid
                    inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid and ptr.ptrstatus = 'A'
                where ve.edestatus = 'A'
                	and ve.etoid = 3
                    and ve.emeano = '".$_SESSION['exercicio']."' and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
            ) con on con.edeid = ede.edeid
            left join(
            	SELECT ve.edeid, 
                    ( array_to_string(
                                   array(    SELECT s.sbadsc
                                                   FROM par.subacaoemendapta se
                                                   INNER JOIN par.subacaodetalhe sd ON sd.sbdid = se.sbdid
                                                   INNER JOIN par.subacao s ON s.sbaid = sd.sbaid  
                                                   WHERE se.emdid = ve.emdid AND se.ptrid = pt.ptrid
                                                   ), ',', '*'   ) 
                    ) AS objeto
                FROM emenda.v_emendadetalheentidade ve
                    INNER JOIN emenda.entidadebeneficiada eb ON eb.enbid = ve.entid and eb.enbstatus = 'A'
                    INNER JOIN emenda.ptemendadetalheentidade ped ON ped.edeid = ve.edeid  
                    INNER JOIN emenda.planotrabalho pt ON pt.ptrid = ped.ptrid and pt.ptrstatus = 'A'
                WHERE ve.edestatus = 'A'
                	AND ve.emeano = 2015
                    and pt.ptrexercicio = '".$_SESSION['exercicio']."'
                    and ve.etoid = 1
            ) pa on pa.edeid = ede.edeid
            left join(
            	select ve.edeid, array_to_string(array(select jus.objetivo from ted.justificativa jus where jus.tcpid = e.tcpid ), ', ') as objeto                                                                    
                from emenda.v_emendadetalheentidade ve
                    inner join ted.emendas e on cast(e.emecod as varchar) = ve.emecod and cast(e.emeano as integer) = ve.emeano
                where ve.edestatus = 'A'
                    and ve.etoid = 2
                    and ve.emeano = '".$_SESSION['exercicio']."'
            ) ted on ted.edeid = ede.edeid
		where
			vf.acastatus = 'A'
			and ed.emdimpositiva = 6
			" . ( !empty($filtro) ? " AND " . implode(" AND ", $filtro) : "" ) . "";

montaBotaoForm();
$cabecalhoBrasao .= "<table cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\" style=\"width: 100% !important\">";
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

$cabecalho = array("Esfera", "UO", "Função", "SubFunção", "Programa", "Ação", "Localizador", "Natureza", "RP atual", "Fonte", "Emenda", "Valor Emenda", "CNPJ", "Objeto", "Valor RCL Apurada", "Valor Disponível", "Justificativa", "Tipo Impedimento");

if($_REQUEST['tipo'] == "excel"){
	global $db;
	
	ob_clean();
	/*header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatImpositivo".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatImpositivo".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );*/
	//$db->monta_lista_tabulado($sql,$cabecalho,10000000,5,'N','100%',$par2, '', '', '');
	
	$formato = array();
	foreach ($cabecalho as $c) {
	
	
		if( strstr( strtoupper($c), strtoupper('valor') ) ) {
			$formato[] = "n";
		} else {
			$formato[] = "s";
		}
	}
	
	$db->sql_to_excel($sql, 'relEmendasImpositivo', $cabecalho, $formato);
	
	exit;
	$exporta = "false";
} else {
	$db->monta_lista_simples($sql, $cabecalho, 10000000, 1, 'N', '100%', 'N', false, false, false, true);
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