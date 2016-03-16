<?PHP
    include_once APPRAIZ . 'pde/www/_funcoes_cockpit.php';

    permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if ($_REQUEST['pesquisa_vistoria']) {

	if($_REQUEST['dias_vistoria'] == 45){
		$where = " and DATE_PART('days', NOW() - o.obrdtultvistoria) < 45";
	} elseif($_REQUEST['dias_vistoria'] == 60){
		$where = " and DATE_PART('days', NOW() - o.obrdtultvistoria) > 60";
    } elseif($_REQUEST['dias_vistoria'] == 'entre45e60'){
		$where = " and DATE_PART('days', NOW() - o.obrdtultvistoria) between 45 and 60 ";
	}

    if(!($_REQUEST['min'] == 0 && $_REQUEST['max'] == 100)){
        $where .= " and o.obrpercentultvistoria between '{$_REQUEST['min']}' and '{$_REQUEST['max']}' ";
    }

    $sql = "SELECT
				nivelpreenchimento,
				sum(coluna1) AS coluna1,
				sum(coluna2) AS coluna2,
				sum(contador) AS total,
				esdid
			FROM (
				SELECT d.esdid,
					CASE WHEN o.tooid in (2,4) THEN 1 ELSE 0 END AS coluna1,
					CASE WHEN o.tooid in (1) THEN 1 ELSE 0 END AS coluna2,
					ed.esddsc AS nivelpreenchimento,
					1 AS CONTADOR
				FROM obras2.obras o
					INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
					INNER JOIN workflow.documento d ON d.docid = o.docid
					INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE o.obrstatus = 'A'
				AND e.orgid=3
				AND d.esdid <> 770 --Etapa Concluída
				AND o.obridpai IS NULL
				AND o.obrid NOT IN (7828,7829,7840,1000015,1000046,1000049) --Obras de teste
				AND e.prfid IN (41)
				AND o.tooid IN (1,2,4)
				AND d.esdid = '{$_REQUEST['esdid']}'
				$where
			) as FOO
			GROUP BY nivelpreenchimento, esdid
			ORDER BY 1";

//    ver($sql);

	$dados = $db->pegaLinha($sql);
    $dados = $dados ? $dados : array('coluna1'=>'', 'coluna2'=>'', 'total'=>'');
//ver($dados);
	echo simec_json_encode($dados);
	die;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=9" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<body onload="refreshAutomatico();">
<?php

if( $_REQUEST['useronline'] ){
	echo pegaUsuarioOnline('99,15,23');
	exit;
}


?>
<link rel="stylesheet" type="text/css" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css"/>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
<script language="javascript" type="text/javascript" src="js/estrategico.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
 <style>
  	.fundo_td{background-color:#0F6D39}
  	.titulo_pagina{font-weight:bold;font-size:20px;color:#FFFFFF}
  	.titulo_box{font-weight:bold;font-size:18px;color:#FFFFFF;margin-top:15px;text-shadow:#000000 0px 1px 2px}
  	.subtitulo_box{font-weight:normal;font-size:10px;color:#FFFFFF}
  	.fundo_td:hover {background-color:#0D8845}
  	.fundo_td{text-align:left;vertical-align:top;}
  	.tabela_painel{font-weight:bold;font-size:8px;color:#FFFFFF;font-family:fantasy}
  	.lista_metas{float:left}
  	#busca{background: none repeat scroll 0% 0% rgb(255, 255, 255); width:400px;border-width: 1px; border-style: solid; border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204); color: rgb(0, 0, 0); font: 18px arial,sans-serif bold; height: 35px;}
  	.tabela_box{color:#FFFFFF;}
  	.tabela_box td{background-color:#3CB371;text-shadow:#000000 0px 2px 2px}
  	.tabela_box_azul td{background-color:#63B8FF;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
  	.fundo_td_azul{background-color:#2B86EE}
  	.fundo_td_azul:hover{background-color:#01A2D8}
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

  	#slider0 .ui-slider-range { background: green; }

  	.checkbok_button{
		text-shadow: none !important;
		color: black;
  	}


    #span_todos{
        /*background-color: #E95646 !important;*/
        /*                                                            border: 0px solid  #f0f0f0;
                                                                    -moz-border-radius: 2px;
                                                                    -webkit-border-radius: 2px;
                                                                    border-radius: 4px;*/
    }
    /*ui-button ui-widget ui-state-default ui-button-text-only ui-state-active*/
    #label_45{
        background-color: #80BC44 !important;
        border: 0px solid  #f0f0f0;
        -moz-border-radius: 2px;
        -webkit-border-radius: 2px;
        border-radius: 4px;
    }

    #label_4560{
        background-color: #FFC211 !important;
        border: 0px solid  #f0f0f0;
        -moz-border-radius: 2px;
        -webkit-border-radius: 2px;
        border-radius: 4px;

        /*background: -webkit-gradient(linear, left top, left bottom, from(yellow), to(#FFC211)) repeat-X;*/
    }

    #label_60{
        background-color: #E95646 !important;
        border: 0px solid  #f0f0f0;
        -moz-border-radius: 2px;
        -webkit-border-radius: 2px;
        border-radius: 4px;
    }
    .ui-state-active #span_todos , .ui-state-active #label_45 , .ui-state-active #label_4560 , .ui-state-active #label_60 {
        font-weight: bold;
    }

    .ui-state-active {
        border: 0px solid  #f0f0f0;
        box-shadow: 0px 0px 10px 0px #666;

    }

    .ui-state-default, .ui-widget-content .ui-state-default, .ui-widget-header .ui-state-default {
        border: 1px solid #d3d3d3/*{borderColorDefault}*/;
        background: #e6e6e6/*{bgColorDefault}*/ url(images/ui-bg_glass_75_e6e6e6_1x400.png)/*{bgImgUrlDefault}*/ 50%/*{bgDefaultXPos}*/ 50%/*{bgDefaultYPos}*/ repeat-x/*{bgDefaultRepeat}*/;
        font-weight: normal/*{fwDefault}*/;
        color: #555555/*{fcDefault}*/;
    }
 </style>
<script>
    var min = 60;
    var max = 100;

	jQuery.noConflict();
    jQuery('.div_fotos_interno').cycle({
	    fx: 'scrollDown'
	});

	atualizaUsuario();

    function abreMapa(tipo, tooid){
    	window.open('../obras/obras.php?modulo=relatorio/mapa_resultado&acao=A&painel='+tipo+'&tooid='+tooid);
    }
    function acessarTermos() {
		window.open('/par/par.php?modulo=principal/termoPac&acao=A&assinado=1&tipoobra=P&pesquisar=1', '_blank');
	}

	function acessarProInfantil(pinid, obrid) {
		window.open('/proinfantil/proinfantil.php?modulo=principal/fotosObraMunicipio&acao=A&_sisarquivo=proinfantil&pinid='+pinid+'&obrid='+obrid, '_blank');
	}

	function acessarCallCenter() {
		window.open('/callcenter/callcenter.php?modulo=principal/temas/termopac&acao=A&temid=1&ligacao=1&pesquisar=1', '_blank');
	}
	function abreMapasMeta(parametros)
	{
	 	url = "/painel/painel.php?modulo=principal/mapas/mapaPadrao&acao=A&mapid=24&carregaMapaAutomativo=1&cmb_tema=" + parametros;
	 	window.open(url,'_blank');
	}

    function abreRelatorioVistoria(quadro, prfid, tooid, stoid){
        window.open('/pde/estrategico.php?modulo=principal/popuplistaobras&acao=A&quadro='+quadro+'&prfid='+prfid+'&tooid='+tooid+'&tipo='+stoid+'&esdid=' + jQuery('#esdid').val() + '&min=' + min + '&max=' +max + '&dias_vistoria=' + jQuery('.dias_vistoria:checked').val());
    }

	jQuery(function() {

        jQuery( "#radio" ).buttonset();
        
        var divRadio = jQuery('#radio').find('label .ui-button-text');
        jQuery(divRadio[0]).attr('id', 'span_todos');
        jQuery(divRadio[1]).attr('id', 'label_45');
        jQuery(divRadio[2]).attr('id', 'label_4560');
        jQuery(divRadio[3]).attr('id', 'label_60');

        jQuery(".slider-range").slider({
            range: true,
            min: 0,
            max: 100,
            step: 5,
            values: [min, max],
            slide: function( event, ui ) {
                jQuery( "#amount" ).html( ui.values[ 0 ] + "% - " + ui.values[ 1 ] );

                if(ui.values[1] <= 45){
                    jQuery(this).find('.ui-slider-range').css('background', '#ef2929');
                }else if(ui.values[1] <= 60){
                    jQuery(this).find('.ui-slider-range').css('background', '#ff0');
                }else{
                    jQuery(this).find('.ui-slider-range').css('background', 'green');
                }

            },
            change: function( event, ui ) {
                min = ui.values[0];
                max = ui.values[1];

                atualizarVistoria();
            },
          });

        jQuery( "#amount" ).html( jQuery( ".slider-range" ).slider( "values", 0 ) + "% - " + jQuery( ".slider-range" ).slider( "values", 1 ) );

        jQuery('#esdid').change(function(){
            atualizarVistoria();
        }).change();

        jQuery('.dias_vistoria').click(function(){
            atualizarVistoria();
        });
    });

    function atualizarVistoria()
    {
        jQuery.ajax({
            url: "/pde/estrategico.php?modulo=principal/cockpit_estrategico&acao=A&pesquisa_vistoria=1&esdid=" + jQuery('#esdid').val() + '&min=' + min + '&max=' +max + '&dias_vistoria=' + jQuery('.dias_vistoria:checked').val(),
            success: function(resultado){
                obj = JSON.parse(resultado);
                jQuery('#pesquisa_coluna1').html(obj.coluna1);
                jQuery('#pesquisa_coluna2').html(obj.coluna2);
                jQuery('#pesquisa_total').html(obj.total);
            }
        });
    }
</script>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
		<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
        	<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_estrategico&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
  <!-- Título-->
  <tr>
		<td class="fundo_titulo" style="text-align:center" colspan="7" ><br>CRECHES E PRÉ-ESCOLAS<br><br></td>
  </tr>
  <!-- Fim Título-->
  <tr>
                <!-- Tabela Indicadores-->
                <td class="fundo_td" width="50%">
					<div>
                		<img style="float:left" src="../imagens/icones/icons/indicador.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >
							<a href="#" onclick="abreIndicadores(129185,'')"  >Indicadores<br/>
                			<span class="subtitulo_box" >Impacto | Produto | Processo</span>
							</a>
                		</div>
                	</div>
                </td>
                <td class="fundo_td">
                <?
				$sql = "select count(pre.preid) as quantidade
						from obras.preobra pre
						inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
						inner join workflow.documento doc on doc.docid = pre.docid
						where doc.esdid in (228,360,365,366,367,683,754,755,889) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra = 'P' and pre.preanoselecao='2010'";
				$quantidadeaprovada_2011= $db->pegaUm($sql,0,3200);
                ?>
                	<div style="cursor:pointer" onclick="acessarPainelGerenciamento('1','5','2010');" >
                		<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" class="titulo_box" ><?=number_format($quantidadeaprovada_2011,0,"",".") ?><br/><span class="subtitulo_box" >Aprovadas<br/>em 2011</span></div>
                	</div>
                </td>

                <td class="fundo_td">
                <?
				$sql = "select count(pre.preid) as quantidade
						from obras.preobra pre
						inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
						inner join workflow.documento doc on doc.docid = pre.docid
						where doc.esdid in (228,360,365,366,367,683,754,755,889) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra = 'P' and pre.preanoselecao='2011'";
				$quantidadeaprovada_2012= $db->pegaUm($sql,0,3200);
                ?>
                	<div style="cursor:pointer" onclick="acessarPainelGerenciamento('1','5','2011');" >
                		<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" class="titulo_box" ><?=number_format($quantidadeaprovada_2012,0,"",".") ?><br/><span class="subtitulo_box" >Aprovadas<br/>em 2012</span></div>
                	</div>
                </td>
				
				<td class="fundo_td">
                <?
				//PREANOMETA E PREANOSELECAO ESTÃO DIFERENTES
				//$sql = "select count(pre.preid) as quantidade
				//		from obras.preobra pre
				//		inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
				//		inner join workflow.documento doc on doc.docid = pre.docid
				//		where doc.esdid in (228,360,365,366,367,683,754,755,889) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra = 'P' and pre.preanoselecao='2013'";
				//$quantidadeaprovada_2013= $db->pegaUm($sql,0,3200);
				$quantidadeaprovada_2013 = 2131;
                ?>
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" class="titulo_box" ><?=number_format($quantidadeaprovada_2013,0,"",".") ?><br/><span class="subtitulo_box" >Aprovadas<br/>em 2013</span></div>
                	</div>
                </td>

                <td class="fundo_td">
                <?
				$sql = "select count(pre.preid) as quantidade
						from obras.preobra pre
						inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
						inner join workflow.documento doc on doc.docid = pre.docid
						where doc.esdid in (228,360,365,366,367,683,754,755,889) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra = 'P'";
				$quantidadeaprovada = $db->pegaUm($sql,0,3200);
                ?>
                	<div style="cursor:pointer" onclick="acessarPainelGerenciamento('1','5','');" >
                		<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
                		<div style="float:left;" class="titulo_box" ><?=number_format($quantidadeaprovada,0,"",".") ?><br/><span class="subtitulo_box" >Aprovadas<br/>até <?php echo date("d/m/Y") ?></span></div>
                	</div>
                </td>
                <td class="fundo_td">
                	<div>
                		<img style="float:left;width: 50px;height: 50px" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >6.000<br/><span class="subtitulo_box" >Meta 2014</span></div>
                	</div>
                </td>
                <!-- Fim Tabela Indicadores-->
                <!-- Tabela Lateral-->
                <td class="fundo_td_azul_escuro" style="vertical-align:top;"  rowspan="5">
                <?
				$sql = "select count(DISTINCT o.obrid)
						from obras2.obras o
						inner join obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
						inner join workflow.documento d ON d.docid = o.docid
						inner join obras2.funcionamentounidade f on f.obrid = o.obrid and f.funstatus = 'A'
						inner join (select max(fu.funid) fi, obrid from obras2.funcionamentounidade fu where fu.funstatus = 'A' group by fu.obrid) fun on fun.fi = f.funid
						where o.obrstatus = 'A'
						and e.orgid=3
						and e.prfid=41
						and d.esdid <> 770 --Etapa Concluída
						and o.obridpai is null
						and o.obrid not in (7828,7829,7840,1000015,1000046,1000049) --Obras de teste
						AND o.tooid in (1,2,4) -- Obras do PAC
						and f.funundfuncionamento=true";
				$crechesfuncionamento = $db->pegaUm($sql,0,3200);
                ?>
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" onclick="window.open( '/obras/obras.php?modulo=relatorio/relatorio_geral&acao=A&prtid=1498&pesquisa=1&form=1', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );" class="titulo_box" >Creches<br/>em funcionamento</div>
                	</div>
                	<div style="clear:both" id="div_fotos"  >
	                	<?php
							$sql = "select
										arqid,
										(select arqdescricao from public.arquivo where arqid=foo.arqid) as arqdescricao,
										obrdesc,
										mundescricao,
										estuf,
										tpodsc,
										obrid,
										pinid
									from (
										select
											(select arq.arqid from public.arquivo arq inner join proinfantil.fotos fot on fot.arqid = arq.arqid where fot.fotstatus = 'A' and fot.pinid=pro.pinid group by arq.arqid, arq.arqdescricao, pro.pinid order by random() limit 1) as arqid,
											upper(obr.obrnome) as obrdesc,
											mun.mundescricao,
											mun.estuf,
											coalesce(tpl.tpodsc,'N/A') as tpodsc,
											obr.obrid,
											pro.pinid
										from proinfantil.proinfantil pro
										inner join obras2.obras obr on obr.obrid = pro.obrid
										inner join obras2.empreendimento e ON e.empid = obr.empid AND e.empstatus = 'A'
										inner join entidade.endereco ed on e.endid = ed.endid
										inner join territorios.municipio mun on mun.muncod = ed.muncod
										inner join obras2.tipologiaobra tpl on e.tpoid = tpl.tpoid
										inner join workflow.documento doc on doc.docid = pro.docid
										where pro.pinststus='A' and doc.esdid in(370,372,373)
										order by random() limit 50
									) foo" ;
							$arrFotos = $db->carregar($sql);
	                		?>
		                	<?php for($x=8;$x>=1;$x--): ?>
		                		<div style="clear:both" class="div_fotos_interno"  >
	                			<?php for($i=$x;$i<=(10+$x);$i++): ?>
		                			<div class="div_fotos" onclick="acessarProInfantil('<?=$arrFotos[$i]['pinid'] ?>','<?=$arrFotos[$i]['obrid'] ?>');" >
				                		<table border="0" >
				                			<tr>
				                				<td>
				                					<img width="80" height="60" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=proinfantil" />
				                				</td>
				                				<td style="color:#FFFFFF" >
				                					<?php echo substr($arrFotos[$i]['obrdesc'],0,20) ?>...<br/>
													<?php echo $arrFotos[$i]['mundescricao'] ?> / <?php echo $arrFotos[$i]['estuf'] ?><br/>
													<?php echo $arrFotos[$i]['tpodsc'] ?>
				                				</td>
				                			</tr>
				                		</table>
			                		</div>
		                		<?php endfor; ?>
	                			</div>
	                	<?php endfor; ?>
	                </div>
                </td>
                <!-- Fim Tabela Lateral-->
  </tr>
  <tr>
                <td class="fundo_td" >
                <?php
                	$sql = "SELECT distinct ed.esddsc as descricao, d.esdid as codigo
							FROM obras2.obras o
								INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
								INNER JOIN workflow.documento d ON d.docid = o.docid
								INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
							WHERE o.obrstatus = 'A'
							AND e.orgid=3
							AND o.obridpai IS NULL
							AND o.obrid NOT IN (7828,7829,7840,1000015,1000046,1000049) --Obras de teste
							AND e.prfid IN (41) AND o.tooid IN (1,2,4)
							order by ed.esddsc";

                	$dados = $db->carregar($sql);
                ?>

					<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
						<tr class="fundo_tr_wap">
							<td class="fundo_td_wap center bold" >Situação</td>
							<td class="fundo_td_wap center bold" width="50%" >Dias sem vistoria</td>
							<td class="fundo_td_wap center bold" >Pré-PAC</td>
							<td class="fundo_td_wap center bold" >PAC2</td>
							<td class="fundo_td_wap center bold" >Total</td>
						</tr>
						<tr class="fundo_tr_wap">
							<td class="fundo_td_wap link" align="center">
								<?php echo $db->monta_combo("esdid", $sql, 'S', "Selecione...", "", "", "", "200", "S", "esdid", "", 690); ?>
							</td>
							<td class="fundo_td_wap link" >
								<div id="radio" style="width: 95% !important; margin: 5px auto;">
							    	<input type="radio" class="dias_vistoria" value="todos"  id="todos" name="radio" checked="checked" /><label for="todos"><span class="checkbok_button">Todos</span></label>
							    	<input type="radio" class="dias_vistoria" value="45"  id="ate_45" name="radio"/><label for="ate_45"><span class="checkbok_button">&lt; 45 dias</span></label>
                                                                <input type="radio" class="dias_vistoria" value="entre45e60" id="entre45e60" name="radio"/><label for="entre45e60"><span class="checkbok_button">De 45 a 60 </span></label>
							    	<input type="radio" class="dias_vistoria" value="60" id="mais_60" name="radio" /><label for="mais_60" id="span_ate60"><span class="checkbok_button">&gt; 60 dias</span></label>
							  	</div>
								<div style="width: 95% !important; margin: 0 auto; text-align: center;">
                                    <hr/>
                                    <div style="margin-bottom: 5px; font-weight: bold;">
                                        Percentual de execução:
                                        <span id="amount" style="border: 0; color: #fff; font-weight: bold;"></span>%
                                    </div>
									<div class="slider-range" id="slider0"></div>
								</div>
							</td>
							<td class="fundo_td_wap link numero" onclick="abreRelatorioVistoria('5', '41', '2, 4', '0');"><span id="pesquisa_coluna1"></span></td>
							<td class="fundo_td_wap link numero" onclick="abreRelatorioVistoria('5', '41', '1', '0');"><span id="pesquisa_coluna2"></span></td>
							<td class="fundo_td_wap link numero" onclick="abreRelatorioVistoria('5', '41', '1, 2, 4', '0');"><span id="pesquisa_total"></span></td>
                                                        
						</tr>
                                            
			        </table><script>
                                                            setTimeout(function(){
//                                                                jQuery('#radio').find('label .ui-button-text').last().attr('id', 'span_ate60');
                                                                
                                                            }, 50);
                                                        </script>

                	<?php echo montaTabelaVistoriaObras(1, 'S');?>

                    <?php
                    $sql = "-- OBRAS EM VERMELHO
                            select count(*) as qtd, o.pendencia
                            from obras2.v_regras_obras_em_vermelho o
                            group by o.pendencia

                            union
/*
                            -- OBRAS PARALISADAS
                            select count(*), o.pendencia
                            from obras2.v_regras_obras_paralisadas o
                            group by o.pendencia

                            union
*/
                            -- OBRAS SEM EXECUÇÃO E RECEBEU A PRIMEIRA PARCELA HÁ MAIS DE 365 DIAS
                            select count(*), o.pendencia
                            from obras2.v_regras_obras_saldo_sem_execucao o
                            group by o.pendencia
/*
                            union

                            -- OBRAS COM MENOS DE 10% DE EVOLUÇÃO NOS ÚLTIMOS 3 MESES
                            select count(*), o.pendencia
                            from obras2.v_regras_obras_evolucao o
                            group by o.pendencia
*/
                    ";

                    $dadosPendencias = $db->carregar($sql);
//                    ver($dadosPendencias, d);

                    ?>

                    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" style="margin-top: 20px" >
                        <tr>
                            <td align="center"><b>Situação</b></td>
                            <td align="center"><b>Quantidade<b></td>
                        </tr>
                        <?php
                        $total = 0;
                        foreach($dadosPendencias as $dado):
                          $total += $dado['qtd'];
                          ?>
                          <tr class="link" >
                              <td><?php echo $dado['pendencia'] ?></td>
                              <td class="numero bold" ><?php echo number_format($dado['qtd'],0,",",".") ?></td>
                          </tr>
                          <?php endforeach; ?>
                          <tr>
                              <td class="bold" >Total</td>
                              <td class="numero bold" ><?php echo number_format($total,0,",",".") ?></td>
                          </tr>
                    </table>
                </td>
                <td class="fundo_td_azul" colspan="5" >
                	<div>
						<?php
						$anoReferencia = "2011,2012,2013"; //DATE_PART('YEAR', CURRENT_TIMESTAMP)
						?>
                		<img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Pactuação <br/><span class="subtitulo_box" >PAC2 (2011, 2012 e 2013)</span></div>
                		<div style="clear:both;width:98%" >
		                	<div style="float:left;width:90%;margin-left:5%;">
			                	<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="100%" >
			                	<?php

									$sql = "select * from (

											select 1 as codigo, '1 - Ofertadas' Situacao, count(pre.preid) as quantidade
											from obras.preobra pre
											inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
											where  pre.prestatus = 'A'  and pto.ptoclassificacaoobra = 'P' and preanometa IN ($anoReferencia)
											union
											select 2 as codigo, '2 - Cadastradas' Situacao, count(pre.preid) as quantidade
											from obras.preobra pre
											inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
											inner join workflow.documento doc on doc.docid = pre.docid
											where  pre.prestatus = 'A'  and pto.ptoclassificacaoobra = 'P' and doc.esdid not in (229,193) and preanometa IN ($anoReferencia)
											union
											select 3 as codigo, '3 - Em análise' Situacao, count(pre.preid) as quantidade
											from obras.preobra pre
											inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
											inner join workflow.documento doc on doc.docid = pre.docid
											where  pre.prestatus = 'A'  and pto.ptoclassificacaoobra = 'P' and doc.esdid in (194,215,218,195,217,213,221,212,210,211) and preanometa IN ($anoReferencia)
											union
											select 4 as codigo, '4 - Deferidas' Situacao, count(pre.preid) as quantidade
											from obras.preobra pre
											inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
											inner join workflow.documento doc on doc.docid = pre.docid
											where pre.prestatus = 'A'  and pto.ptoclassificacaoobra = 'P' and doc.esdid in (214) and preanometa IN ($anoReferencia)
											union
											select 5 as codigo, '5 - Aprovadas' Situacao, count(pre.preid) as quantidade
											from obras.preobra pre
											inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
											inner join workflow.documento doc on doc.docid = pre.docid
											where doc.esdid in (228,360,365,366,367,683,754,755,889) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra = 'P' and preanometa IN ($anoReferencia)

											) as foo

											order by 1";
									$arrDados = $db->carregar( $sql, null, 3200 );

									foreach( $arrDados as $dado ){
			                	?>
			                		<tr>
			                			<td><img border="0" style="cursor:pointer" onclick="abreMapasMeta('<?=$dado['codigo'] ?>')" src="/imagens/icone_br.png" title="Exibir Mapa"> <img style="cursor:pointer;background-color:#FFFFFF" onclick="acessarPainelGerenciamento('1','<?=$dado['codigo'] ?>','');" src="../imagens/consultar.gif"> <?=str_replace(array("1 - ","2 - ","3 - ","4 -", "5 - "),"",$dado['situacao'])?></td>
			                			<td class="numero" ><?=number_format($dado['quantidade'],0,"",".") ?></td>
			                		</tr>
			                	<?php } ?>
			                	</table>
			                </div>
		                </div>
                	</div>
                </td>
    			<!-- Fim Obras -->
  </tr>
  <tr>
                <!-- Tabela Financeiro -->
                <td class="fundo_td" rowspan="2">
                    <?=montaTabelaSituacaoObras(1, 'S')?>
                </td>
                <td class="fundo_td" colspan="5" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/financeiro.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Orçamentário / Financeiro<br/></div>
                	</div>
                	<?

                	// Parâmetros para a nova conexão com o banco do SIAFI
					$servidor_bd = $servidor_bd_siafi;
					$porta_bd    = $porta_bd_siafi;
					$nome_bd     = $nome_bd_siafi;
					$usuario_db  = $usuario_db_siafi;
					$senha_bd    = $senha_bd_siafi;

					// Cria o novo objeto de conexão
					$db2 = new cls_banco();

					// Parâmetros da nova conexão com o banco do SIAFI para o componente 'combo_popup'.
					$dados_conexao = array(
					     'servidor_bd' => $servidor_bd_siafi,
					     'porta_bd' => $porta_bd_siafi,
					     'nome_bd' => $nome_bd_siafi,
					     'usuario_db' => $usuario_db_siafi,
					     'senha_bd' => $senha_bd_siafi
					    );

					   $sql = "/* Consulta Financeiro do Creche Pré-Escola */
						SELECT
					    '2011' as exercicio, sum(valor1) AS dotacao,sum(valor2) AS empenhado,sum(valor3) + sum(valor4) AS pago
					    FROM
					    (SELECT
					       sld.acacod,
					       aca.acadsc,
					       CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130201','192140100','192140200','192190101','192190109','192190201','192190209','192190301','192190302') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('295210201','295210202') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor4
					          FROM
					       dw.saldo2011 sld
					       LEFT JOIN dw.acao aca ON aca.acacod = sld.acacod
					       WHERE sld.acacod in ('12KU') AND sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130201','192140100','192140200','192190101','192190109','192190201','192190209','192190301','192190302','292130100','292130201','292130202','292130203','292130301','292130301','292410403','295210201','295210202')) as foo
					    WHERE
					    valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0
					union all
					SELECT
					    '2012' as exercicio, sum(valor1) AS dotacao,sum(valor2) AS empenhado,sum(valor3) + sum(valor4) AS pago
					    FROM
					    (SELECT
					       sld.acacod,
					       aca.acadsc,
					       CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130201','192140100','192140200','192190101','192190109','192190201','192190209','192190301','192190302') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('295210201','295210202') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor4
					          FROM
					       dw.saldo2012 sld
					       LEFT JOIN dw.acao aca ON aca.acacod = sld.acacod
					       WHERE sld.acacod in ('12KU') AND sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130201','192140100','192140200','192190101','192190109','192190201','192190209','192190301','192190302','292130100','292130201','292130202','292130203','292130301','292130301','292410403','295210201','295210202')) as foo
					    WHERE
					    valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0
					union all
					SELECT
					    '2013' as exercicio, sum(valor1) AS dotacao,sum(valor2) AS empenhado,sum(valor3) + sum(valor4) AS pago
					    FROM
					    (SELECT
					       sld.acacod,
					       aca.acadsc,
					       CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130201','192140100','192140200','192190101','192190109','192190201','192190209','192190301','192190302') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('295210201','295210202') THEN
					    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
					    ELSE 0 END AS valor4
					          FROM
					       dw.saldo2013 sld
					       LEFT JOIN dw.acao aca ON aca.acacod = sld.acacod
					       WHERE sld.acacod in ('12KU') AND sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130201','192140100','192140200','192190101','192190109','192190201','192190209','192190301','192190302','292130100','292130201','292130202','292130203','292130301','292130301','292410403','295210201','295210202')) as foo
					    WHERE
					    valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0";
					$arrDados = $db2->carregar($sql,null,3200);

                	?>
                	<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
		                		<tr>
		                			<td><b>Exercício</b></td>
		                			<td class="center" ><b>Dotação Autorizada<b></td>
		                			<td class="center" ><b>Empenhado<b></td>
		                			<td class="center" ><b>Pago<b></td>
		                		</tr>
		                		<?php foreach($arrDados as $dado): ?>
			                		<tr class="link" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['exercicio'] ?>&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KU&alterar_ano=0')" >
			                			<td><?php echo $dado['exercicio'] ?></td>
			                			<td class="numero" ><?php $total_dotacao+=$dado['dotacao']; echo number_format($dado['dotacao'],2,",",".") ?></td>
			                			<td class="numero" ><?php $total_empenhado+=$dado['empenhado']; echo number_format($dado['empenhado'],2,",",".") ?></td>
			                			<td class="numero" ><?php $total_pago+=$dado['pago'];  echo number_format($dado['pago'],2,",",".") ?></td>
			                		</tr>
		                		<?php endforeach; ?>
		                		<tr>
		                			<td class="bold" >Total</td>
		                			<td class="numero bold" ><?php echo number_format($total_dotacao,2,",",".") ?></td>
		                			<td class="numero bold" ><?php echo number_format($total_empenhado,2,",",".") ?></td>
		                			<td class="numero bold" ><?php echo number_format($total_pago,2,",",".") ?></td>
		                		</tr>
		                	</table>
                </td>
                <!-- Tabela Financeiro -->
  </tr>
  <tr>
                <td class="fundo_td" colspan="5">
                	<?
					$sql = "select sum(prevalorobra) from obras.preobra  pre
							inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
							inner join par.termoobra tob on tob.preid = pre.preid
							inner join par.termocompromissopac t on t.terid = tob.terid and t.terstatus = 'A'
							where  pto.ptoclassificacaoobra IN('P') and pre.prestatus = 'A'";
                	$valor_contratado = $db->pegaUm($sql,0,3200);

					$sql = "SELECT count(*) FROM (
							SELECT te.tobid
									FROM
										par.processoobra p
									LEFT JOIN par.resolucao r ON r.resid=p.resid
									INNER JOIN territorios.municipio m ON m.muncod=p.muncod
									INNER JOIN par.termocompromissopac ter ON ter.proid = p.proid AND ter.terstatus='A'
									INNER JOIN par.termoobra te ON te.terid = ter.terid
									WHERE 1=1
									and p.prostatus = 'A' 
								AND p.estuf IS NULL AND protipo='P' AND ter.terassinado = TRUE UNION ALL (SELECT te.tobid
									FROM
										par.processoobra p
									LEFT JOIN par.resolucao r ON r.resid=p.resid
									INNER JOIN par.termocompromissopac ter ON ter.proid = p.proid AND ter.terstatus='A'
									INNER JOIN par.termoobra te ON te.terid = ter.terid
									INNER JOIN territorios.estado e ON e.estuf=p.estuf
									WHERE 1=1
									and p.prostatus = 'A' 
								AND p.estuf IS NOT NULL AND protipo='P' AND ter.terassinado = TRUE)
							) foo";

					$termosassinados = $db->pegaUm($sql, 0, 3200);
                	?>
                	<div>
                		<img style="float:left;width: 40px;height: 40px;" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                		<table>
                		<tr>
                		<td><div style="float:left;cursor:pointer;" onclick="acessarTermos();" class="titulo_box" ><?=number_format($termosassinados,0,"",".") ?><br><span class="subtitulo_box" >Obras com Termos Assinados</span></div></td>
                		</tr>
                		<tr>
                		<td><div class="titulo_box" >R$ <?=number_format($valor_contratado,0,"",".") ?><br/><span class="subtitulo_box" >Valor contratado</span></div></td>
                		</tr>
                		</table>
                	</div>

                	<br /><br /><br />

                	<?
                	$sql = "select count(ligid) as recebida from callcenter.ligacao where temid = 1 and tlgid = 1 group by tlgid";
                	$contatosefetuados = $db->pegaUm($sql,0,3200);
                	?>
                	<div>
                		<img style="float:left;width: 40px;height: 40px;" src="../imagens/icones/icons/call.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" class="titulo_box" onclick="acessarCallCenter();" ><?=number_format($contatosefetuados,0,"",".") ?><br/><span class="subtitulo_box" >Contatos<br/>Efetuados</span></div>
                	</div>

                	<div style="clear: both;"></div>
                	<br /><br /><br />

					<div onclick="abreUsuarios('')">
                		<img style="float:left;width: 40px;height: 40px;" src="../imagens/icones/icons/chat.png" style="vertical-align:middle;"  />
                		<div id="usuOnline" style="float:left;" class="titulo_box" ><?=0 . ' <span class="subtitulo_box" >On-line<br/>'.date("d/m/Y").'<br>'.date("g:i:s").'</span>' ?></div>
                	</div>

                </td>
  </tr>
	<!-- Fim 3 QUADROS -->
	<tr>
		<td class="fundo_td_laranja" colspan="6">
			<div style="text-align:center;"  >
				<img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
				<input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
				<img src="../imagens/icones/icons/Find.png"  style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
			</div>
		</td>
	</tr>
</table>
</body>
</html>