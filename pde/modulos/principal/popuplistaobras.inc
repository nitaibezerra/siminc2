<?php

include_once APPRAIZ . 'www//obras2/_funcoes_obras_par.php';

if($_REQUEST['exibirXls']==1) {
	
	ob_clean();
	
	$sql = "SELECT 
				 foo.obrid,
	             foo.preid,
	             foo.predescricao,
	             foo.estuf,
	             foo.mundescricao,
	             foo.muncod,
	             foo.mediatempo
			FROM (
			".$_SESSION['estrategico']['sqlexcel']."
			) foo";
	
	$nomeArquivo = 'estrategico_obras_'.date("Ymd");
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=".$nomeArquivo.".xls");
	header ( "Content-Disposition: attachment; filename=".$nomeArquivo.".xls");
	header ( "Content-Description: MID Gera excel" );
	
	$cabecalho = array('Pré-ID', 'Obra-ID', 'Pré-Obra', 'UF', 'Município', 'IBGE', 'Dias nesta situação');
	$db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'N','100%', 'S');
	
	exit;
	
	
}

if($_REQUEST['exibirXls']==2) {

    ob_clean();

    $sql = "select	distinct
                o.obrid_1,
                m.estuf,
                m.muncod,
                m.mundescricao,
                o.obrnome,
                o.obranoconvenio,
                case when crt.crtvalorexecucao is null then o.obrvalorprevisto else crt.crtvalorexecucao end as crtvalorexecucao,
                esd.esddsc,
                ((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(o.obrperccontratoanterior,0))::numeric(20,2) AS obrpercentultvistoria,
                tpo.tpodsc,
                ed.endcep AS cep, ed.endlog AS logradouro, ed.endbai AS bairro, ed.endnum AS numero, ed.medlatitude, ed.medlongitude
            from obras2.obras o
            inner join obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
            inner join workflow.documento d ON d.docid = o.docid
            inner join workflow.estadodocumento esd ON esd.esdid = d.esdid
            inner join entidade.endereco ed ON ed.endid = o.endid
            inner join territorios.municipio m ON m.muncod = ed.muncod
            inner join obras2.tipologiaobra tpo ON tpo.tpoid = o.tpoid
            LEFT join obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            left join obras2.contrato crt ON crt.crtid = oc.crtid  AND crt.crtstatus = 'A'
            where o.obrstatus = 'A'
            and e.orgid=3
            and d.esdid NOT IN (770) --Etapa Concluída
            and o.obridpai is null
            AND e.prfid IN (41)
            and o.preid IS NULL and o.tooid IN (2,4)
            ORDER BY o.obrid_1";

    $nomeArquivo = 'PrePac_'.date("Ymd");

    header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
    header ( "Pragma: no-cache" );
    header ( "Content-type: application/xls; name=".$nomeArquivo.".xls");
    header ( "Content-Disposition: attachment; filename=".$nomeArquivo.".xls");
    header ( "Content-Description: MID Gera excel" );

    $cabecalho = array('Obra-ID', 'UF', 'IBGE', 'Município', 'Nome da Obra', 'Ano Convênio', 'Valor Execução', 'Situação', 'Percentual', 'Tipo', 'CEP', 'Logradouro', 'Bairro', 'Núnero', 'Latitude', 'Longitude');
    $db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'N','100%', 'S');

    exit;


}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=9" />
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<?php
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

$tipo = $_REQUEST['tipo'];
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
<script language="javascript" type="text/javascript" src="js/estrategico.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

<!-- Bootstrap CSS -->
<link href="/library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">

<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css"/>

<style>
	.fundo_td{background-color:#0F6D39}
	.titulo_pagina{font-weight:bold;font-size:20px;color:#FFFFFF}
	.titulo_box{font-weight:bold;font-size:18px;color:#FFFFFF;margin-top:15px;text-shadow:#000000 0px 1px 2px}
	.subtitulo_box{font-weight:normal;font-size:10px;color:#FFFFFF}
	.fundo_td{text-align:left;vertical-align:top;}
	.tabela_painel{font-weight:bold;font-size:8px;color:#FFFFFF;font-family:fantasy}
	.lista_metas{float:left}
	#busca{background: none repeat scroll 0% 0% rgb(255, 255, 255); width:400px;border-width: 1px; border-style: solid; border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204); color: rgb(0, 0, 0); font: 18px arial,sans-serif bold; height: 35px;}
	.tabela_box{color:#FFFFFF;}
	.tabela_box td{background-color:#3CB371;text-shadow:#000000 0px 2px 2px}
	.tabela_box_azul td{background-color:#63B8FF;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
	.fundo_td_azul{background-color:#2B86EE}
	.fundo_td_azul:hover{background-color:#01A2D8}
	.fundo_td_azul{text-align:left;vertical-align:top;}
	.fundo_td_laranja{background-color:#EE9200}
	.fundo_td_laranja:hover{background-color:#EBB513}
	.fundo_td_vermelho{background-color:#BB0000}
	.fundo_td_vermelho:hover{background-color:#DD0000}
	.fundo_td_roxo{background-color:#5333AD}
	.fundo_td_roxo:hover{background-color:#6A5ACD}
	.fundo_td_azul_escuro{background-color:#152D56}
	.fundo_td_azul_escuro:hover{background-color:#1F3864}
	.div_fotos{background-color:#204481;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:350px;margin-bottom:2px}
	body{background-image:url('../imagens/fundo_cockpit.jpg');background-repeat:repeat-x;background-color:#00466A;margin:0px;padding-top:0px;}
	.fundo_titulo{background-image:url('../imagens/fundocreche.jpg');background-repeat:repeat-xt;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
	.numero{text-align:right}
	.center{text-align:center}
	.titulo_box a{color:#FFFFFF;text-decoration:none;}
	.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
	.div_fotos_interno{margin-bottom:2px;width:98%}
	.link{cursor:pointer}
	.bold{font-weight:bold}
	.tabela_listagem {
		background-color: #FFFFFF;
		color: #000000;
	}

    .filtro_listagem{
       width: 70%;
    }
</style>
<script>
	jQuery.noConflict();
	jQuery('.div_fotos_interno').cycle({
		fx: 'scrollDown'
	});

	atualizaUsuario();

	function abreObrasPac(preid,muncod){
		window.open('/par/par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid='+preid+'&muncod='+muncod);
	}
	function abreObras(obrid){
		window.open('/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid='+obrid);
	}

    function submeterFormularioListagem(){
        jQuery('#formulario_filtro_listagem').attr('action', window.location.href).submit();
    }

    jQuery(function(){

        jQuery('.ordenador_listagem').click(function(){
            jQuery('#ordenador_listagem').val(jQuery(this).attr('ordenar'));
            submeterFormularioListagem();
        });

        jQuery('.img_filtro_listagem').click(function(){
            submeterFormularioListagem();
        });

        jQuery('#btn_excel').click(function(){
            window.location='estrategico.php?modulo=principal/popuplistaobras&acao=A&exibirXls=1';
        });

        jQuery('#btn_excel2').click(function(){
            window.location='estrategico.php?modulo=principal/popuplistaobras&acao=A&exibirXls=2';
        });

        jQuery('.filtro_listagem').keypress(function(e) {
            if(e.which == 13) {
                submeterFormularioListagem();
            }
        });
    });
</script>
</head>
<body>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<div style="float:right;cursor:pointer;" onclick="window.close();">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>

<form action="" method="post" name="formulario_filtro_listagem" id="formulario_filtro_listagem">
    <input type="hidden" name="ordenador_listagem" id="ordenador_listagem" value="<?php echo $_REQUEST['ordenador_listagem']; ?>"/>

    <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">

    	<!-- Título-->
    	<tr>
    		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Lista de Obras</div></td>
    	</tr>
    	<?php
    	$tempo = "";

        // Recupera processo e saldo pelo preid, caso tenha
        $joinProcesso = "   LEFT JOIN  par.processoobraspaccomposicao ppc
                                    LEFT JOIN  par.processoobra ppac
                                            LEFT JOIN  obras2.pagamentopac pago on pago.ppaprocesso = ppac.pronumeroprocesso
                                    on ppac.proid = ppc.proid
                            on ppc.preid = pre.preid
                            LEFT JOIN  par.processoobrasparcomposicao ppcpar
                                    LEFT JOIN  par.processoobraspar ppar 
                                            LEFT JOIN  obras2.pagamentopac pagopar on pagopar.ppaprocesso = ppar.pronumeroprocesso
                                    on ppar.proid = ppcpar.proid and ppar.prostatus = 'A'
                            on ppcpar.preid = pre.preid	";

        $camposProcesso = " case
                                when coalesce(ppac.pronumeroprocesso, '0') != '0' then ppac.pronumeroprocesso
                                else ppar.pronumeroprocesso
                            end as numeroprocesso";

        $sql = "SELECT
    				obrid,
    				preid,
    				predescricao,
    				estuf,
    				mundescricao,
    				muncod,
    				CASE
    					WHEN mediatempo < '24:00:00' THEN '1'
    					ELSE SUBSTRING(mediatempo::varchar,0,STRPOS(mediatempo::varchar,'d'))
    				END as mediatempo,
    				numeroprocesso,
    				-- Recuperando valor do saldo do Mes anterior (dados da tabela painel.dadosfinanceirosconvenios)
    				(select (dfi.dfisaldoconta + dfi.dfisaldofundo + dfi.dfisaldopoupanca + dfi.dfisaldordbcdb) AS saldo from painel.dadosfinanceirosconvenios dfi where dfi.dfiprocesso = numeroprocesso and to_char(dfi.dfidatasaldo, 'YYYYMM') = to_char((now() - INTERVAL '1 MONTH'), 'YYYYMM') limit 1) as saldo
    			FROM(
    				SELECT DISTINCT
    						o.obrid,
    						pre.preid,
							case
								when pre.predescricao <> '' then pre.predescricao
							else o.obrnome
							end as predescricao,
    						m.estuf,
    						m.mundescricao,
    						m.muncod,
    						$camposProcesso,
    						";

    	if ($_REQUEST['quadro'] == 1 ){
			switch( $_REQUEST['tipo'] ){
				case '0':
    				$titulo = "Aguardando empenho";
    				$sql = $sql . "
    						avg(now() - htddata) AS mediatempo
    						from obras.preobra pre
    						inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
    						inner join workflow.documento doc on doc.docid = pre.docid
    						LEFT JOIN workflow.historicodocumento hst ON hst.hstid = doc.hstid
    						INNER JOIN territorios.municipio m on m.muncod = pre.muncod
    						LEFT JOIN obras2.obras o ON o.obrid = pre.obrid
    						$joinProcesso
    						where doc.esdid in (228) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra = 'P'
    						AND pre.obrid is null";
    			break;
    			default:
					
					if($_REQUEST['tipo'] == 'T'){
						$titulo = "Total";
					}else{
						$titulo = $db->pegaUm("SELECT esddsc FROM workflow.estadodocumento WHERE esdid = ". $_REQUEST['tipo']);
					}
					$sql = $sql . "
							avg(now() - htddata) AS mediatempo
								from obras2.obras o
								inner join obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
								inner join workflow.documento d ON d.docid = o.docid
								LEFT JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid
								LEFT JOIN entidade.endereco ed ON ed.endid = o.endid
								LEFT JOIN territorios.municipio m on m.muncod = ed.muncod
								LEFT JOIN obras.preobra pre ON pre.preid = o.preid
								$joinProcesso
								where o.obrstatus = 'A'
								and e.orgid=3
								and e.prfid IN (".$_REQUEST['prfid'].")
								and d.esdid NOT IN (770) --Etapa Concluída
								" . ( $_REQUEST['tipo'] <> 'T' ? 'and d.esdid = '."'". $_REQUEST['tipo']."'" : '' ) ."
								and o.obridpai is null
								" . ( $_REQUEST['tooid'] == '1' ? 'AND (o.preid IS NOT NULL)' : '') . "
								" . ( $_REQUEST['tooid'] == '2,4' ? 'AND (o.preid IS NULL AND o.tooid IN (2,4))' : '') . "
								" . ( $_REQUEST['tooid'] == '4' ? 'AND o.tooid IN (4) AND o.obranoconvenio IS NULL' : '') . "
								" . ( $_REQUEST['tooid'] == '1,2,4' ? 'AND ((o.preid IS NULL AND o.tooid IN (2,4)) OR o.preid IS NOT NULL)' : '');
				break;
    		}
    	}elseif($_REQUEST['quadro'] == 2){
    		switch( $_REQUEST['tipo'] ){
    			case 1:
					//ANTES
					//doc.esdid in (228,360,365,366,367,683,754,755)
					//AGORA, APENAS OBRAS APROVADAS
    				$titulo = "Aguardando empenho";
    				$sql = $sql . "
    						avg(now() - htddata) AS mediatempo
    						from obras.preobra pre
    						inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
    						inner join workflow.documento doc on doc.docid = pre.docid
    						LEFT JOIN workflow.historicodocumento hst ON hst.hstid = doc.hstid
    						INNER JOIN territorios.municipio m on m.muncod = pre.muncod
    						LEFT JOIN obras2.obras o ON o.obrid = pre.obrid
    						$joinProcesso
    						where doc.esdid in (228) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra = 'P'
    						AND pre.obrid is null
    						AND pre.ptoid NOT IN (42,43,44,45)";
    			break;
    			default:
						if($_REQUEST['tipo']==762){
							$titulo = "Aguardando 1º repasse";
						}elseif($_REQUEST['tipo']==689){
							$titulo = "Em Planejamento pelo Proponente";
							$tempo = 90;
						}elseif($_REQUEST['tipo']==763){
							$titulo = "Em Licitação";
							$tempo = 120;
						}elseif($_REQUEST['tipo']==771){
							$titulo = "Aguardando registro de preços";
						}elseif($_REQUEST['tipo']==764){
							$titulo = "Aguardando contratação";
							$tempo = 30;
						}elseif($_REQUEST['tipo']==692){
							$titulo = "Celebração de aditivo";
						}elseif($_REQUEST['tipo']==770){
							$titulo = "Etapa Concluída";
						}elseif($_REQUEST['tipo']==693){
							$titulo = "Concluídas";
						}elseif($_REQUEST['tipo']==768){
							$titulo = "Em Reformulação";
						}elseif($_REQUEST['tipo']==691){
							$titulo = "Paralisadas";
						}elseif($_REQUEST['tipo']==690 && $_REQUEST['ordem']==7){
							$titulo = "Execução até 25%";
							$tempo = 120;
							$where = " AND COALESCE(o.obrpercentultvistoria,0) < 25";
						}elseif($_REQUEST['tipo']==690 && $_REQUEST['ordem']==8){
							$titulo = "Execução de 25% a 50%";
							$tempo = 120;
							$where = " AND COALESCE(o.obrpercentultvistoria,0) BETWEEN 25 AND 50";
						}elseif($_REQUEST['tipo']==690 && $_REQUEST['ordem']==9){
							$titulo = "Execução de 50% a 75%";
							$tempo = 120;
							$where = " AND COALESCE(o.obrpercentultvistoria,0) BETWEEN 50 AND 75";
						}elseif($_REQUEST['tipo']==690 && $_REQUEST['ordem']==10){
							$titulo = "Execução acima de 75%";
							$tempo = 120;
							$where = " AND COALESCE(o.obrpercentultvistoria,0) > 75";
						}else{
							$titulo = $db->pegaUm("SELECT esddsc FROM workflow.estadodocumento WHERE esdid = ". $_REQUEST['tipo']);
						}
    				$sql = $sql . "
    						avg(now() - htddata) AS mediatempo
    							from obras2.obras o
    							inner join obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
    							inner join workflow.documento d ON d.docid = o.docid
    							LEFT JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid
								INNER JOIN entidade.endereco ed ON ed.endid = o.endid
    							INNER JOIN territorios.municipio m on m.muncod = ed.muncod
								LEFT JOIN obras.preobra pre ON pre.preid = o.preid
								$joinProcesso
    							where o.obrstatus = 'A'
    							and e.orgid=3
    							and e.prfid=41
    							and d.esdid = ".$_REQUEST['tipo']."
    							and o.obridpai is null
    							AND e.prfid IN (41) AND ((o.preid IS NULL AND o.tooid IN (2,4)) OR o.preid IS NOT NULL)
    							AND o.tpoid NOT IN (104,105) ".$where;
    			break;
    		}
    	}elseif($_REQUEST['quadro'] == 3){
			//GERAÇÃO DE TERMO
			$sqlTermo = "select
						distinct po.preid
					from obras.preobra po
					inner join workflow.documento d on d.docid = po.docid AND d.esdid = 228
					where po.ptoid in ( 42,43,44,45 )
					AND po.preid not in (select preid from par.termoobraspaccomposicao)
					--union all
					--- Obras que foram aprovadas após a geração do termo
					--select
					--	distinct po.preid
					--from 
					--	obras.preobra po
					--inner join workflow.documento d on d.docid = po.docid AND d.esdid = 228
					--inner join par.empenhoobra eo on eo.preid = po.preid
					--inner join par.empenho emp on emp.empid = eo.empid AND emp.empsituacao <> 'CANCELADO'
					--inner join par.termoobraspaccomposicao tpc on tpc.preid = po.preid
					--inner join par.termocompromissopac ter on ter.terid = tpc.terid and terstatus = 'A'
					--inner join territorios.municipio mun on mun.muncod = po.muncod
					--where po.ptoid in ( 42,43,44,45 )
					--AND ter.terdatainclusao < to_char((select htddata from workflow.historicodocumento  where docid = d.docid and aedid in ( select aedid from workflow.acaoestadodoc where esdiddestino = 228 ) order by htddata desc limit 1),'YYYY-MM-DD')::DATE
					";

    		switch( $_REQUEST['tipo'] ){
    			case 'MI':
    				$titulo = $_REQUEST['parametro'];
    				$sql = $sql . "
								avg(now() - htddata) AS mediatempo
								FROM obras2.obras o
								INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
								INNER JOIN workflow.documento d ON d.docid = o.docid
								INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
								LEFT JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid
								INNER JOIN obras.preobra pre ON pre.preid = o.preid
								INNER JOIN territorios.municipio m on m.muncod = pre.muncod
								$joinProcesso
								INNER JOIN
									(SELECT MAX(aosdtalteracao), aoscnpjfornecedor AS cnpj, preid FROM par.adesaoobraspacsituacao GROUP BY cnpj, preid) AS f ON f.preid = o.preid
								WHERE o.obrstatus = 'A'
								AND e.orgid=3
								AND e.prfid=41
								AND o.obridpai is null
								AND o.tooid in (1,2,4)
								AND o.tpoid IN (104,105)
								AND esddsc = '".$_REQUEST['parametro']."'
								" . ( count($_REQUEST['cnpj']) ? 'AND cnpj = '."'". $_REQUEST['cnpj']."'" : '' ) ." ";
    			break;
    			case 'MICON':
    				$titulo = $_REQUEST['parametro'];
    				if($titulo=='Em Contratação'){
    					$where = "d.esdid IN (771,870,861,862,863,871,875)";
    				}elseif($titulo=='Contratadas'){
    					$where = "d.esdid IN (872,864,873,874)";
    				}elseif($titulo=='Obras Iniciadas'){
    					$where = "d.esdid IN (690,693,691,679)";
    				}
    				$sql = $sql . "
								avg(now() - htddata) AS mediatempo
								FROM obras2.obras o
								INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
								INNER JOIN workflow.documento d ON d.docid = o.docid
								LEFT JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid
								INNER JOIN obras.preobra pre ON pre.preid = o.preid
								INNER JOIN territorios.municipio m on m.muncod = pre.muncod
								$joinProcesso
								WHERE o.obrstatus = 'A'
								AND e.orgid=3
								AND e.prfid=41
								AND o.obridpai is null
								AND o.tooid in (1,2,4)
								AND o.tpoid IN (104,105)
								AND ".$where;
    			break;
    			case 'EMPRESA':
    				$titulo = $_REQUEST['parametro'];
    				$sql = $sql . "
								avg(now() - htddata) AS mediatempo
								FROM obras2.obras o
								INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
								INNER JOIN workflow.documento d ON d.docid = o.docid
								INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
								LEFT JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid
								INNER JOIN obras.preobra pre ON pre.preid = o.preid
								INNER JOIN territorios.municipio m on m.muncod = pre.muncod
								$joinProcesso
								INNER JOIN
									(SELECT MAX(aosdtalteracao), aoscnpjfornecedor AS cnpj, preid, aosnomefornecedor FROM par.adesaoobraspacsituacao GROUP BY cnpj, preid, aosnomefornecedor) AS f ON f.preid = o.preid
								WHERE o.obrstatus = 'A'
								AND e.orgid=3
								AND e.prfid=41
								AND o.obridpai is null
								AND o.tooid in (1,2,4)
								AND o.tpoid IN (104,105)
    							AND aosnomefornecedor = '".$_REQUEST['parametro']."'";
    			break;
				case 1: //Aguardando solicitação do município
    				$titulo = "Aguardando empenho";
    				$tempo = 10;
    				$sql = $sql . "
    							avg(now() - htddata) AS mediatempo
    							FROM obras.preobra pre
    							INNER JOIN workflow.documento d ON d.docid = pre.docid
    							INNER JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid
    							INNER JOIN territorios.municipio m on m.muncod = pre.muncod
    							LEFT JOIN obras2.obras o ON o.obrid = pre.obrid
    							$joinProcesso
    							WHERE pre.prestatus = 'A'
    							AND pre.preid NOT IN ( SELECT aop.preid FROM par.adesaoobraspac aop WHERE aop.aopstatus = 'A' )
    							AND pre.ptoid IN (42,43,44,45)
    							AND d.esdid IN (228)
								AND pre.obrid is null";
    			break;
				case 2: //Aguardando geração de termo
    				$titulo = "Aguardando geração de termo";
    				$tempo = 10;

                    $sql .= "
                                    avg(now() - htddata) AS mediatempo
                                    from obras.preobra pre
                                        LEFT JOIN obras2.obras o ON o.obrid = pre.obrid
                                        inner join territorios.municipio m on m.muncod = pre.muncod
                                        inner join workflow.documento d on d.docid = pre.docid AND d.esdid = 228
                                        INNER JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid
                                        $joinProcesso
                                    where pre.ptoid in ( 42,43,44,45 )
                                    AND ( (pre.preid not in (select preid from par.termoobraspaccomposicao)
                                                ) OR
                                        pre.preid IN ( 	SELECT
                                                            po.preid
                                                        FROM
                                                            obras.preobra po
                                                        inner join workflow.documento d on d.docid = po.docid AND d.esdid = 228
                                                        inner join par.empenhoobra eo on eo.preid = po.preid 
                                                        inner join par.empenho emp on emp.empid = eo.empid AND emp.empsituacao <> 'CANCELADO' and empstatus = 'A'
                                                        inner join par.termoobraspaccomposicao tpc on tpc.preid = po.preid
                                                        inner join par.termocompromissopac ter on ter.terid = tpc.terid  AND ter.terstatus = 'A'
                                                        where
                                                                po.ptoid in ( 42,43,44,45 ) AND
                                                                po.prestatus = 'A' AND
                                                                (ter.terdatainclusao+1) < (select htddata from workflow.historicodocumento  where docid = d.docid and aedid in ( select aedid from workflow.acaoestadodoc where esdiddestino = 228 and esdidorigem <> 755 ) order by htddata desc limit 1)  )
                                    )";


    			break;
				default:
						if($_REQUEST['tipo']==884){
							$titulo = "Aguardando solicitação do município";
							$tempo = 10;
							$where = " AND o.preid NOT IN ($sqlTermo)";
						}elseif($_REQUEST['tipo']==771){
							$titulo = "Aguardando registro de preços";
							$tempo = 10;
                        }elseif($_REQUEST['tipo']==870){
                            $titulo = "Análise da solicitação de utilização da ata";
                            $tempo = 10;
						}elseif($_REQUEST['tipo']==875){
							$titulo = "Solicitação cancelada";
						}elseif($_REQUEST['tipo']==861){
							$titulo = "Aguardando ciência do fornecedor";
							$tempo = 15;
						}elseif($_REQUEST['tipo']==862){
							$titulo = "Aguardando autorização do FNDE";
							$tempo = 5;
						}elseif($_REQUEST['tipo']==863){
							$titulo = "Aguardando geração do contrato pelo município";
							$tempo = 30;
						}elseif($_REQUEST['tipo']==871){
							$titulo = "Contrato em tramitação";
							$tempo = 30;
						}elseif($_REQUEST['tipo']==872){
							$titulo = "Aguardando emissão de OS (com pendência de regularização)";
							$tempo = 30;
						}elseif($_REQUEST['tipo']==864){
							$titulo = "Aguardando emissão de OS";
							$tempo = 10;
						}elseif($_REQUEST['tipo']==873){
							$titulo = "Aguardando aceite da OS pelo fornecedor";
							$tempo = 5;
						}elseif($_REQUEST['tipo']==874){
							$titulo = "OS Recusada";
						}elseif($_REQUEST['tipo']==690 && $_REQUEST['ordem']==13){
							$titulo = "Execução até 25%";
							$tempo = 60;
							$where = " AND COALESCE(o.obrpercentultvistoria,0) < 25";
						}elseif($_REQUEST['tipo']==690 && $_REQUEST['ordem']==14){
							$titulo = "Execução de 25% a 50%";
							$tempo = 40;
							$where = " AND COALESCE(o.obrpercentultvistoria,0) BETWEEN 25 AND 50";
						}elseif($_REQUEST['tipo']==690 && $_REQUEST['ordem']==15){
							$titulo = "Execução de 50% a 75%";
							$tempo = 40;
							$where = " AND COALESCE(o.obrpercentultvistoria,0) BETWEEN 50 AND 75";
						}elseif($_REQUEST['tipo']==690 && $_REQUEST['ordem']==16){
							$titulo = "Execução acima de 75%";
							$tempo = 70;
							$where = " AND COALESCE(o.obrpercentultvistoria,0) > 75";
						}elseif($_REQUEST['tipo']==874){
							$titulo = "OS Recusada";
						}elseif($_REQUEST['tipo']==693){
							$titulo = "Concluídas";
						}elseif($_REQUEST['tipo']==691){
							$titulo = "Paralisadas";
						}elseif($_REQUEST['tipo']==679){
							$titulo = "Canceladas";
						}else{
							$titulo = $db->pegaUm("SELECT esddsc FROM workflow.estadodocumento WHERE esdid = ". $_REQUEST['tipo']);
						}

                        if($_REQUEST['tipo']==884){
                            $sql .= "
                                    avg(now() - htddata) AS mediatempo
                                    from obras.preobra pre
                                            LEFT JOIN obras2.obras o ON o.obrid = pre.obrid
                                            inner join territorios.municipio m on m.muncod = pre.muncod
                                            inner join workflow.documento d on d.docid = pre.docid AND d.esdid = 228
                                            LEFT JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid
                                            $joinProcesso
                                    WHERE
                                    pre.ptoid in ( 42, 43 )
                                    AND pre.preid not in ( select preid from par.adesaoobraspac WHERE aopstatus = 'A' )
                                    AND pre.preid NOT IN (
                                            select  distinct po.preid
                                            from obras.preobra po
                                                inner join workflow.documento d on d.docid = po.docid AND d.esdid = 228
                                            where po.ptoid in ( 42,43,44,45 )
                                            AND po.preid not in (select preid from par.termoobraspaccomposicao)
                                    ) AND pre.preid NOT IN ( SELECT 
																po.preid
															FROM 
																obras.preobra po
															inner join workflow.documento d on d.docid = po.docid AND d.esdid = 228
															inner join par.empenhoobra eo on eo.preid = po.preid
															inner join par.empenho emp on emp.empid = eo.empid AND emp.empsituacao <> 'CANCELADO' and empstatus = 'A'
															inner join par.termoobraspaccomposicao tpc on tpc.preid = po.preid
															inner join par.termocompromissopac ter on ter.terid = tpc.terid  AND ter.terstatus = 'A'
															where
																	po.ptoid in ( 42,43,44,45 ) AND
																	po.prestatus = 'A' AND
																	(ter.terdatainclusao+1) < (select htddata from workflow.historicodocumento  where docid = d.docid and aedid in ( select aedid from workflow.acaoestadodoc where esdiddestino = 228 ) order by htddata desc limit 1)  )";
                        } else {
                            $sql = $sql . "
                                            avg(now() - htddata) AS mediatempo
                                            FROM obras2.obras o
                                                INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
                                                INNER JOIN workflow.documento d ON d.docid = o.docid
                                                LEFT JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid
                                                LEFT JOIN obras.preobra pre ON pre.preid = o.preid
                                                INNER JOIN territorios.municipio m on m.muncod = pre.muncod
                                                $joinProcesso
                                            WHERE o.obrstatus = 'A'
                                            and e.orgid=3
                                            and e.prfid=41
                                            and d.esdid = ".$_REQUEST['tipo']."
                                            and o.obridpai is null
                                            AND o.tooid in (1,2,4)
                                            AND o.tpoid IN (104,105) ".$where;
                        }

                        break;
            }
        } elseif($_REQUEST['quadro'] == 4) {
// 			$tipo = utf8_decode($_REQUEST['tipo']);
            $tipo = $_REQUEST['tipo'];
            $prfid = $_REQUEST['prfid'] ? $_REQUEST['prfid'] : 41;
            $titulo = $tipo;

                $nivelpreenchimento = "CASE
									WHEN d.esdid IN (690, 691) THEN
										(CASE
											WHEN DATE_PART('days', NOW() - o.obrdtultvistoria) < 45 THEN '1 - Obras atualizadas há menos de 45 dias atrás'
											WHEN DATE_PART('days', NOW() - o.obrdtultvistoria) BETWEEN 45  AND 60 THEN '2 - Obras atualizadas entre 45 e 60 dias'
                                            WHEN DATE_PART('days', NOW() - o.obrdtultvistoria) > 60 THEN '3 - Obras atualizadas há mais de 60 dias'
                                            ELSE '6 - Obras sem vistoria'
										END)
									WHEN d.esdid = 693 THEN '4 - Obras concluídas'
									ELSE '5 - Não se aplica'
								END AS nivelpreenchimento";

            $sql = "select *
					from(
							SELECT
								o.obrid,
		    						pre.preid,
									case
										when pre.predescricao <> '' then pre.predescricao
										else o.obrnome
									end as predescricao,
		    						m.estuf,
		    						m.mundescricao,
		    						DATE_PART('days', NOW() - o.obrdtultvistoria) as mediatempo,
		    						m.muncod,
							$nivelpreenchimento
							, $camposProcesso
						FROM obras2.obras o
						left join obras.preobra pre ON o.obrid = pre.obrid
						INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
						INNER JOIN workflow.documento d ON d.docid = o.docid
						LEFT JOIN  entidade.endereco ed on ed.endid = o.endid
						LEFT JOIN  territorios.municipio m on m.muncod = ed.muncod
						$joinProcesso
						WHERE o.obrstatus = 'A'
						AND e.orgid=3
						AND d.esdid <> 770 --Etapa Concluída
						AND o.obridpai IS NULL
						AND e.prfid IN ({$prfid})
						AND o.tooid IN ({$_REQUEST['tooid']})
					) as foo
					where nivelpreenchimento = '$tipo'";

//            ver($_REQUEST, $sql);
        } elseif($_REQUEST['quadro'] == 5) {

            $prfid = $_REQUEST['prfid'] ? $_REQUEST['prfid'] : 41;

            $vistoria = 'Todos';
            if($_REQUEST['dias_vistoria'] == 45){
                $where = " and DATE_PART('days', NOW() - o.obrdtultvistoria) < 45";
                $vistoria = '<= 45 dias';
            } elseif($_REQUEST['dias_vistoria'] == 60){
                $where = " and DATE_PART('days', NOW() - o.obrdtultvistoria) > 60";
                $vistoria = '>= 60 dias';
            } elseif($_REQUEST['dias_vistoria'] == 'entre45e60'){
                $where = " and DATE_PART('days', NOW() - o.obrdtultvistoria) between 45 and 60 ";
                $vistoria = 'entre 45 e 60 dias';
            }

            if(!($_REQUEST['min'] == 0 && $_REQUEST['max'] == 100)){
                $where .= " and o.obrpercentultvistoria between '{$_REQUEST['min']}' and '{$_REQUEST['max']}' ";
            }

            $sql = "select esddsc from workflow.estadodocumento esd where esdid = '{$_REQUEST['esdid']}'";
            $esddsc = $db->pegaUm($sql);

            $preenchimento = "Preenchimento: de {$_REQUEST['min']} a {$_REQUEST['max']} %";
            $vistoria = "Última vistoria: $vistoria";

            $titulo = " <div>
                            <span style='margin-right: 50px'>$esddsc</span>
                            <span style='margin-right: 50px'>$preenchimento</span>
                            <span style='margin-right: 50px'>$vistoria</span>
                        </div>
                        ";

            $sql = "
                        SELECT  o.obrid,
                                pre.preid,
                                    case
                                        when pre.predescricao <> '' then pre.predescricao
                                        else o.obrnome
                                    end as predescricao,
                                m.estuf,
                                m.mundescricao,
                                m.muncod,
                                o.tooid,
                                DATE_PART('days', NOW() - o.obrdtultvistoria) as mediatempo,
                                d.esdid
                        FROM obras2.obras o
                            left join obras.preobra pre ON o.obrid = pre.obrid
                            INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
                            INNER JOIN workflow.documento d ON d.docid = o.docid
                            LEFT JOIN  entidade.endereco ed on ed.endid = o.endid
                            LEFT JOIN  territorios.municipio m on m.muncod = ed.muncod
                            $joinProcesso
                        WHERE o.obrstatus = 'A'
                        AND e.orgid=3
                        AND d.esdid <> 770 --Etapa Concluída
                        AND o.obridpai IS NULL
                        AND e.prfid IN ($prfid)
                        AND o.tooid IN ({$_REQUEST['tooid']})
                        AND d.esdid = '{$_REQUEST['esdid']}'
                        $where
                        ";
        } elseif($_REQUEST['quadro'] == 6) {

            $aTitulo = array('1'=>'OS - Execução', '2'=>'OS - Sondagem', '3'=>'OS - Projeto de Implantação');

            $tosid = $_REQUEST['prfid'];
            $esddsc = $_REQUEST['tipo'];
            $titulo = $aTitulo[$tosid] . ' - ' . $_REQUEST['tipo'];

            $sql = $sql . "
                            avg(now() - htddata) AS mediatempo
                         FROM obras2.ordemservicomi os
                             JOIN obras2.obras o ON (os.obrid = o.obrid)
                             LEFT JOIN obras.preobra pre ON pre.preid = o.preid
                             JOIN entidade.entidade e ON (e.entid = o.entid)
                             JOIN obras2.tipoosmi tos ON (os.tomid = tos.tomid)
                             JOIN seguranca.usuario u ON (os.usucpf = u.usucpf)
                             JOIN workflow.documento d ON (os.docid = d.docid)
                             JOIN workflow.estadodocumento ed ON (ed.esdid = d.esdid)
                             LEFT JOIN entidade.endereco edo ON (edo.endid = o.endid)
                             -- LEFT JOIN public.uf uf ON (uf.regcod = edo.estuf)
                             -- LEFT JOIN public.municipio mun ON (mun.muncod = edo.muncod)
                             LEFT JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid
                             LEFT JOIN territorios.municipio m ON (m.muncod = edo.muncod)
                             LEFT JOIN obras2.supervisao_grupo_empresa sge ON (e.entid = sge.entid)
                             LEFT JOIN obras2.supervisaomi smi ON (smi.entidvistoriador = e.entid)
                             LEFT JOIN obras2.obrascontrato oc ON (oc.obrid = o.obrid)
                             $joinProcesso
                             LEFT JOIN(SELECT
                                 euf.emiid,
                                 euf.estuf,
                                 em.emidscresumo AS descricao
                                 --em.emidsc AS descricao
                             FROM obras2.empresami em
                             JOIN obras2.empresami_uf euf ON euf.emiid = em.emiid AND euf.eufstatus = 'A'
                         WHERE emistatus = 'A') foo ON (edo.estuf = foo.estuf)
                 WHERE ed.esdstatus = 'A' AND os.osmstatus = 'A'
                 and tos.tomid = '$tosid'
                 and public.removeacento(ed.esddsc) =  public.removeacento('$tipo')
";
        }

        $order = ' ORDER BY estuf, mundescricao, predescricao';
        if ($_REQUEST['ordenador_listagem']) {
            $order = " order by " . str_replace('__', ' ', $_REQUEST['ordenador_listagem']);
        }

        if ($_REQUEST['quadro'] != 4 && $_REQUEST['quadro'] != 5){
            $sql = $sql . "
    			GROUP BY o.obrid,
    					pre.preid,
    					predescricao,
						o.obrnome,
    					m.estuf,
    					m.mundescricao,
    					m.muncod,
    					numeroprocesso
    			) AS FOO ";
        }

        $sql = $sql . " {$order} ";

        $_SESSION['estrategico']['sqlexcel'] = $sql;

//        ver($sql);
    	$dados = $db->carregar($sql);
    	if($dados){
            foreach($dados as $dado):
                if($tempo){
                    if($tempo < $dado['mediatempo']){
                        if (verificarFiltroListaObras($dado, true)) {
                            $arrAtrasado[] = $dado;
                        }
                    }else{
                        if (verificarFiltroListaObras($dado)) {
                            $arrNormal[] = $dado;
                        }
                    }
                }else{
                    if (verificarFiltroListaObras($dado)) {
                        $arrNormal[] = $dado;
                    }
                }
            endforeach;
    	}

    	?>
    	<tr>
    		<td class="fundo_td">
    			<div>
    				<img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
    				<div style="float:left" class="titulo_box" ><?=$titulo?><br/> </div>
    				<img style="float:right; cursor:pointer;" src="../imagens/excel2013.png" id="btn_excel" title="Relatório Estratégico" width="62" height="62" style="vertical-align:middle;"  />
                    <?php
                    if($_REQUEST['quadro']==1 && $_REQUEST['prfid']==41 && $_REQUEST['tipo']=='T'){
                    ?>
                        <img style="float:right; cursor:pointer;" src="../imagens/obras/nota.png" id="btn_excel2" title="Relatório Pré-PAC" width="62" height="62" style="vertical-align:middle;"  />
                    <?php
                    }
                    ?>

    			</div>
    			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <td class="center bold" >
                            Pré-ID
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="preid" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="preid__desc"/>
                        </td>
                        <td class="center bold" >
                            Obra-ID
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrid" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrid__desc"/>
                        </td>
                        <td class="center bold" >
                            Pré-Obra
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="predescricao" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="predescricao__desc"/>
                        </td>
                        <td class="center bold" >
                            Processo
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="processo" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="processo__desc"/>
                        </td>
                        <td class="center bold" >
                            Saldo
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="saldo" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="saldo__desc"/>
                        </td>
                        <td class="center bold" >
                            UF
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="estuf" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="estuf__desc"/>
                        </td>
                        <td class="center bold" >
                            Município
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mundescricao" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mundescricao__desc"/>
                        </td>
                        <td class="center bold" >
                            Dias nesta situação
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mediatempo" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mediatempo__desc"/>
                        </td>
                    </tr>
                    <tr>
                        <td class="center" >
                            <img width="20px" class="img_filtro_listagem" src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_preid" value="<?php echo $_REQUEST['filtro_preid']; ?>" />
                        </td>
                        <td class="center" >
                            <img width="20px" class="img_filtro_listagem" src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_obrid" value="<?php echo $_REQUEST['filtro_obrid']; ?>" />
                        </td>
                        <td class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_predescricao" value="<?php echo $_REQUEST['filtro_predescricao']; ?>" />
                        </td>
                        <td class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_processo" value="<?php echo $_REQUEST['filtro_processo']; ?>" />
                        </td>
                        <td class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_saldo" value="<?php echo $_REQUEST['filtro_saldo']; ?>" />
                        </td>
                        <td class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_estuf" value="<?php echo $_REQUEST['filtro_estuf']; ?>" />
                        </td>
                        <td class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_mundescricao" value="<?php echo $_REQUEST['filtro_mundescricao']; ?>" />
                        </td>
                        <td class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_mediatempo" value="<?php echo $_REQUEST['filtro_mediatempo']; ?>" />
                        </td>
                    </tr>
    				<?php
    				$totalObras=0;
    				if($arrNormal){
    					foreach($arrNormal as $dado):
    					?>
    					<tr>
    						<td class="">
								<?php if($dado['preid']){?>
        							<img class="link" src="cockpit/images/icone_p.png" style="vertical-align:middle;" width="20" height="20" title="PAR" onclick="abreObrasPac(<?=$dado['preid']?>, <?=$dado['muncod']?>);" />
    							<?php }
                                echo $dado['preid']; ?>
    						</td>
    						<td class="">
    							<?php if($dado['obrid']){ ?>
    								<img class="link" src="cockpit/images/icone_o.png" style="vertical-align:middle;" width="20" height="20" title="Obras" onclick="abreObras(<?=$dado['obrid']?>);" />
    							<?php } ?>
    							<?php echo $dado['obrid'] ?>
    						</td>
    						<td class=""><?php echo $dado['predescricao'] ?></td>
    						<td align="center"><span class="processo_detalhe"><?php echo $dado['numeroprocesso']; ?></span></td>
    						<td align="right"><?php echo $dado['saldo'] ? number_format($dado['saldo'], 2, ',', '.') : 'Não Informado'; ?></td>
    						<td class=""><?php echo $dado['estuf'] ?></td>
    						<td class=""><?php echo $dado['mundescricao'] ?></td>
    						<td class="numero"><?php echo $dado['mediatempo'] ?></td>
    					</tr>
    					<?php
    					$totalObras+=1;
    					endforeach;
    						?>
    						<tr>
    							<td class="bold" colspan="8">&nbsp;</td>
    						</tr>
    						<tr>
    							<td class="bold" colspan="7">Total de Obras</td>
    							<td class="bold numero" ><?php echo $totalObras ?></td>
    						</tr>
    					<?php
    					}else{
    					?>
    						<tr>
    							<td  class="center" style="color:red" colspan="8">Não existem obras nessa situação.</td>
    						</tr>
    					<?php
    					}
    					?>
    			</table>
    		</td>
    		<?php
    		if($tempo){
    		?>
    			<td class="fundo_td">
    				<div>
    					<img style="float:left" src="../imagens/icones/icons/obras.png" style="vertical-align:middle;"  />
    					<div style="float:left" class="titulo_box" ><FONT COLOR="RED">EM ATRASO</FONT></div>
    				</div>
    				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" >
                                Pré-ID
                                <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="preid" />
                                <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="preid__desc"/>
                            </td>
                            <td class="center bold" >
                                Obra-ID
                                <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrid" />
                                <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrid__desc"/>
                            </td>
                            <td class="center bold" >
                                Pré-Obra
                                <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="predescricao" />
                                <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="predescricao__desc"/>
                            </td>
                            <td class="center bold" >
                                Processo
                                <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="processo" />
                                <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="processo__desc"/>
                            </td>
                            <td class="center bold" >
                                Saldo
                                <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="saldo" />
                                <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="saldo__desc"/>
                            </td>
                            <td class="center bold" >
                                UF
                                <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="estuf" />
                                <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="estuf__desc"/>
                            </td>
                            <td class="center bold" >
                                Município
                                <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mundescricao" />
                                <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mundescricao__desc"/>
                            </td>
                            <td class="center bold" >
                                Dias nesta situação
                                <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mediatempo" />
                                <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mediatempo__desc"/>
                            </td>
                        </tr>
                        <tr>
                            <td class="center" >
                                <img width="20px" class="img_filtro_listagem" src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                                <input type="text" class="filtro_listagem" name="filtro_preid_atraso" value="<?php echo $_REQUEST['filtro_preid_atraso']; ?>" />
                            </td>
                            <td class="center" >
                                <img width="20px" class="img_filtro_listagem" src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                                <input type="text" class="filtro_listagem" name="filtro_obrid_atraso" value="<?php echo $_REQUEST['filtro_obrid_atraso']; ?>" />
                            </td>
                            <td class="center" >
                                <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                                <input type="text" class="filtro_listagem" name="filtro_predescricao_atraso" value="<?php echo $_REQUEST['filtro_predescricao_atraso']; ?>" />
                            </td>
                            <td class="center" >
                                <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                                <input type="text" class="filtro_listagem" name="filtro_processo_atraso" value="<?php echo $_REQUEST['filtro_processo_atraso']; ?>" />
                            </td>
                            <td class="center" >
                                <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                                <input type="text" class="filtro_listagem" name="filtro_saldo_atraso" value="<?php echo $_REQUEST['filtro_saldo_atraso']; ?>" />
                            </td>
                            <td class="center" >
                                <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                                <input type="text" class="filtro_listagem" name="filtro_estuf_atraso" value="<?php echo $_REQUEST['filtro_estuf_atraso']; ?>" />
                            </td>
                            <td class="center" >
                                <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                                <input type="text" class="filtro_listagem" name="filtro_mundescricao_atraso" value="<?php echo $_REQUEST['filtro_mundescricao_atraso']; ?>" />
                            </td>
                            <td class="center" >
                                <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                                <input type="text" class="filtro_listagem" name="filtro_mediatempo_atraso" value="<?php echo $_REQUEST['filtro_mediatempo_atraso']; ?>" />
                            </td>
                        </tr>
    					<?php
    					$totalObras=0;
    					if($arrAtrasado){
    						foreach($arrAtrasado as $dado):
    						?>
    						<tr>
                                <td class="">
                                    <?php if($dado['preid']){?>
                                        <img class="link" src="cockpit/images/icone_p.png" style="vertical-align:middle;" width="20" height="20" title="PAR" onclick="abreObrasPac(<?=$dado['preid']?>, <?=$dado['muncod']?>);" />
                                    <?php }
                                    echo $dado['preid']; ?>
                                </td>
                                <td class="">
                                    <?php if($dado['obrid']){ ?>
                                        <img class="link" src="cockpit/images/icone_o.png" style="vertical-align:middle;" width="20" height="20" title="Obras" onclick="abreObras(<?=$dado['obrid']?>);" />
                                    <?php } ?>
                                    <?php echo $dado['obrid'] ?>
                                </td>
    							<td class=""><?php echo $dado['predescricao'] ?></td>
                                <td align="center"><span class="processo_detalhe" ><?php echo $dado['numeroprocesso']; ?></span></td>
    							<td align="right" ><?php echo $dado['saldo'] ? number_format($dado['saldo'], 2, ',', '.') : 'Nao Informado'; ?></td>
    							<td class=""><?php echo $dado['estuf'] ?></td>
    							<td class=""><?php echo $dado['mundescricao'] ?></td>
    							<td class="numero"><?php echo $dado['mediatempo'] ?></td>
    						</tr>
    						<?php
    						$totalObras+=1;
    						endforeach;
    						?>
    						<tr>
    							<td class="bold" colspan="8">&nbsp;</td>
    						</tr>
    						<tr>
    							<td class="bold" colspan="7">Total de Obras</td>
    							<td class="bold numero" ><?php echo $totalObras ?></td>
    						</tr>
    					<?php
    					}else{
    					?>
    						<tr>
    							<td  class="center" style="color:red" colspan="8">Não existem obras atrasadas nessa situação.</td>
    						</tr>
    					<?php
    					}
    					?>
    				</table>
    			</td>
    		<?php
    		}
    		?>
    	</tr>
    </table>
</form>
</body>
</html>
<?php
prepararDetalheProcesso();
?>
