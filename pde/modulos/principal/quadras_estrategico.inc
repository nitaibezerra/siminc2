<?PHP
include_once APPRAIZ . 'pde/www/_funcoes_cockpit.php';

$_SESSION['obras2']['orgid'] = 3;

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
                    CASE WHEN e.prfid in (50) then 1 else 0 end as coluna1,
                    CASE WHEN e.prfid in (55) then 1 else 0 end as coluna2,
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
                AND e.prfid IN (50,55)
                AND o.tooid IN (1)
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
<meta http-equiv="X-UA-Compatible" content="IE=7" />
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
  	.fundo_td_roxo{background-color:#5333AD}
  	.fundo_td_roxo:hover{background-color:#6A5ACD}
  	.fundo_td_vermelho:hover{background-color:#DD0000}
  	.div_fotos{background-color:#7B68EE;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:350px;margin-bottom:2px}
  	body{background-image:url('../imagens/fundo_cockpit.jpg');background-repeat:repeat-x;background-color:#00466A;margin:0px;padding-top:0px;}
  	.fundo_titulo{background-image:url('../imagens/fundoquadras.jpg');background-repeat:repeat-xt;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
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
	
    function acessarTermos() {
		window.open('/par/par.php?modulo=principal/termoPac&acao=A&assinado=1&tipoobra=Q&pesquisar=1', '_blank');
	}
    
	function acessarQuadras(obrid) {
		window.open('/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid='+obrid, '_blank');
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
            url: "/pde/estrategico.php?modulo=principal/quadras_estrategico&acao=A&pesquisa_vistoria=1&esdid=" + jQuery('#esdid').val() + '&min=' + min + '&max=' +max + '&dias_vistoria=' + jQuery('.dias_vistoria:checked').val(),
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
        	<div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/quadras_estrategico&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
  <!-- Título-->
  <tr>
                <td class="fundo_titulo" style="text-align:center" colspan="6" ><br>Construção e cobertura de quadras esportivas escolares<br><br></td>
  </tr>
  <!-- Fim Título-->
  <tr>
                <!-- Tabela Indicadores-->
                <td class="fundo_td" width="45%" >
                	<?php $sql = "select mtinivel, mtidsc as descricao, count(*) as total from painel.indicador i
								inner join pde.monitoratipoindicador mti ON mti.mtiid = i.mtiid
								and acaid = (select atiacaid from pde.atividade where atiid = 129596)
								group by mtinivel, mtidsc
								order by mtinivel"; 
                			$dados = $db->carregar( $sql );?>
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/indicador.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" ><a href="#" onclick="abreIndicadores(129596, '')"  >Indicadores</a><br/>
                			<?php if(1==2): ?>
                				<span class="subtitulo_box" ><?=$dados[0]['descricao'] ?>: <?=$dados[0]['total'] ?> | <?=$dados[1]['descricao'] ?>: <?=$dados[1]['total'] ?> | <?=$dados[2]['descricao'] ?>: <?=$dados[2]['total'] ?></span>
                			<?php else: ?>
                				<span class="subtitulo_box" >Impacto | Produto | Processo</span>
                			<?php endif; ?>
                		</div>
                	</div>
                </td>
				<td class="fundo_td">
                <?
				$sql = "select count(pre.preid) as quantidade
						from obras.preobra pre
						inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
						inner join workflow.documento doc on doc.docid = pre.docid
						where doc.esdid in (228,360,365,366,367,683,754,755) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra IN ('C','Q') and pre.preanoselecao='2010'";
				$quantidadeaprovada_2011= $db->pegaUm($sql,0,86400);
                ?>
                	<div style="cursor:pointer" onclick="acessarPainelGerenciamento('2','5','2010');" >
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
						where doc.esdid in (228,360,365,366,367,683,754,755) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra IN ('C','Q') and pre.preanoselecao='2011'";
				$quantidadeaprovada_2012= $db->pegaUm($sql,0,86400);
                ?>
                	<div style="cursor:pointer" onclick="acessarPainelGerenciamento('2','5','2011');" >
                		<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" class="titulo_box" ><?=number_format($quantidadeaprovada_2012,0,"",".") ?><br/><span class="subtitulo_box" >Aprovadas<br/>em 2012</span></div>
                	</div>
                </td>


                <td class="fundo_td">
                <?
				$sql = "select count(pre.preid) as quantidade
						from obras.preobra pre
						inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
						inner join workflow.documento doc on doc.docid = pre.docid
						where doc.esdid in (228,360,365,366,367,683,754,755) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra IN ('C','Q')";
				$quantidadeaprovada = $db->pegaUm($sql,0,86400);
                ?>
                	<div style="cursor:pointer" onclick="acessarPainelGerenciamento('2','5','');" >
                		<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
                		<div style="float:left;" class="titulo_box" ><?=number_format($quantidadeaprovada,0,"",".") ?><br/><span class="subtitulo_box" >Aprovadas<br/>até <?php echo date("d/m/Y") ?></span></div>
                	</div>
                </td>
                
                <td class="fundo_td">
                	<div>
                		<img style="float:left;width: 50px;height: 50px" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Meta 2014<br>10.116<br/><span class="subtitulo_box" >Contrução e cobertura</span></div>
                	</div>
                </td>
                <!-- Fim Tabela Indicadores-->
                <!-- Tabela Lateral-->
                <td class="fundo_td_roxo" style="vertical-align:top;"  rowspan="4">
                <?
				$sql = "select count(*) 
						from obras2.obras o
						inner join obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
						inner join workflow.documento d ON d.docid = o.docid
						where o.obrstatus = 'A'
						and e.orgid=3
						and e.prfid in(50,55)
						and d.esdid in (690, 693) --Execução e Concluída
						and o.obridpai is null
						and o.obrid not in (7828,7829,7840,1000015,1000046,1000049)";
				$crechesfuncionamento = $db->pegaUm($sql);
                ?>
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
                		<div style="float:left;" class="titulo_box" ><?=number_format($crechesfuncionamento,0,"",".") ?> Quadras Concluídas <br/> ou Em Construção</div>
                	</div>
                	<div style="clear:both" id="div_fotos"  >
	                	<?php
	                	
	                	$sql = "select	o.obrnome as obrdesc,
								mun.mundescricao,
								mun.estuf,
								tpl.tpodsc,
								o.obrid,
								max(ft.arqid) as arqid,
								ft.supid
							from obras2.obras o
							inner join obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
							inner join workflow.documento d ON d.docid = o.docid
							inner join entidade.endereco ed on o.endid = ed.endid 
							inner join territorios.municipio mun on mun.muncod = ed.muncod 
							inner join obras2.tipologiaobra tpl on o.tpoid = tpl.tpoid
							inner join
								(select	f.obrid, f.arqid, f.supid
								from obras2.fotos f
								inner join obras2.obras obr on obr.obrid = f.obrid and obr.obrstatus='A'
								where f.supid in (SELECT s.supid FROM obras2.supervisao s WHERE s.supstatus = 'A' AND s.obrid=obr.obrid ORDER BY s.supdata DESC, s.supid DESC LIMIT 1)
								and f.arqid IS NOT NULL
								) as ft on ft.obrid = o.obrid
							where o.obrstatus='A'
							and ((o.obrpercentultvistoria >=50 and d.esdid not in (770)) or d.esdid in (693) )
							and e.orgid = 3
							and e.prfid in(50,55)
							group by o.obrnome, mun.mundescricao, mun.estuf, tpl.tpodsc, o.obrid, ft.supid
							order by random()
							limit 50";
               				$arrFotos = $db->carregar($sql);
	                		?>
		                	<?php for($x=8;$x>=1;$x--): ?>
		                		<div style="clear:both" class="div_fotos_interno"  >	
	                			<?php for($i=$x;$i<=(10+$x);$i++): ?>
		                			<div class="div_fotos" onclick="acessarQuadras('<?=$arrFotos[$i]['obrid'] ?>');" >
				                		<table border="0" >
				                			<tr>
				                				<td>
				                					<img width="80" height="60" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=obras2" />
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
                            <td class="fundo_td_wap link" align="center" width="20%">
                                <?php echo $db->monta_combo("esdid", $sql, 'S', "Selecione...", "", "", "", "150", "S", "esdid", "", 690); ?>
                            </td>
                            <td class="fundo_td_wap link"  width="20%">
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
                            <td width="5%" class="fundo_td_wap link numero" onclick="abreRelatorioVistoria('5', '50', '1', '0');"><span id="pesquisa_coluna1"></span></td>
                            <td width="5%" class="fundo_td_wap link numero" onclick="abreRelatorioVistoria('5', '55', '1', '0');"><span id="pesquisa_coluna2"></span></td>
                            <td width="5%" class="fundo_td_wap link numero" onclick="abreRelatorioVistoria('5', '50,55', '1', '0');"><span id="pesquisa_total"></span></td>

                        </tr>

                    </table>

                	<?=montaTabelaVistoriaObras(2, 'S');?>
                </td>
                <td class="fundo_td_azul" colspan="4" >
					<?php
					$anoReferencia = 2013; //DATE_PART('YEAR', CURRENT_TIMESTAMP)
					?>
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Pactuação<br/></div>
                		<div style="clear:both;width:98%" >
		                	<div style="float:left;width:90%;margin-left:5%;">	
			                	<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="100%" >
		                		<tr>
		                			<td></td>
		                			<td class="center bold" ><b>Construção</b></td>
		                			<td class="center bold" ><b>Cobertura</b></td>
		                			<td class="center bold" ><b>Total</b></td>
		                		</tr>
			                	<?php
									$sql = "select * from (
											select codigo, situacao, sum(quantidadecobe) as quantidadecobe, sum(quantidadequad) as quantidadequad from(
											select '1'::text as codigo, '1. Ofertadas'::text as Situacao, CASE WHEN ptoclassificacaoobra='C' THEN 1 ELSE 0 END as quantidadecobe, CASE WHEN ptoclassificacaoobra='Q' THEN 1 ELSE 0 END as quantidadequad
											from obras.preobra pre
											inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
											where  pre.prestatus = 'A'  and pto.ptoclassificacaoobra in('C','Q') --and preanoselecao = $anoReferencia
											) foo 
											group by codigo, situacao
											union
											select codigo, situacao, sum(quantidadecobe) as quantidadecobe, sum(quantidadequad) as quantidadequad from(
											select '2'::text as codigo, '2. Cadastradas'::text as Situacao, CASE WHEN ptoclassificacaoobra='C' THEN 1 ELSE 0 END as quantidadecobe, CASE WHEN ptoclassificacaoobra='Q' THEN 1 ELSE 0 END as quantidadequad
											from obras.preobra pre
											inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
											inner join workflow.documento doc on doc.docid = pre.docid
											where  pre.prestatus = 'A'  and pto.ptoclassificacaoobra in('C','Q') and doc.esdid not in (229,193) --and preanoselecao = $anoReferencia
											) foo 
											group by codigo, situacao
											union
											select codigo, situacao, sum(quantidadecobe) as quantidadecobe, sum(quantidadequad) as quantidadequad from(
											select '3'::text as codigo, '3. Em análise'::text as Situacao, CASE WHEN ptoclassificacaoobra='C' THEN 1 ELSE 0 END as quantidadecobe, CASE WHEN ptoclassificacaoobra='Q' THEN 1 ELSE 0 END as quantidadequad
											from obras.preobra pre
											inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
											inner join workflow.documento doc on doc.docid = pre.docid
											where  pre.prestatus = 'A'  and pto.ptoclassificacaoobra in('C','Q') and doc.esdid in (194,215,218,195,217,213,221,212,210,211) --and preanoselecao = $anoReferencia 
											) foo
											group by codigo, situacao
											union
											select codigo, situacao, sum(quantidadecobe) as quantidadecobe, sum(quantidadequad) as quantidadequad from(
											select '4'::text as codigo, '4. Deferidas'::text as Situacao, CASE WHEN ptoclassificacaoobra='C' THEN 1 ELSE 0 END as quantidadecobe, CASE WHEN ptoclassificacaoobra='Q' THEN 1 ELSE 0 END as quantidadequad
											from obras.preobra pre
											inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
											inner join workflow.documento doc on doc.docid = pre.docid
											where doc.esdid in (214) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra in('C','Q') --and preanoselecao = $anoReferencia
											) foo
											group by codigo, situacao
											union
											select codigo, situacao, sum(quantidadecobe) as quantidadecobe, sum(quantidadequad) as quantidadequad from(
											select '5'::text as codigo, '5. Aprovadas'::text as Situacao, CASE WHEN ptoclassificacaoobra='C' THEN 1 ELSE 0 END as quantidadecobe, CASE WHEN ptoclassificacaoobra='Q' THEN 1 ELSE 0 END as quantidadequad
											from obras.preobra pre
											inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
											inner join workflow.documento doc on doc.docid = pre.docid
											where doc.esdid in (228,360,365,366,367,683,754,755) and pre.prestatus = 'A'  and pto.ptoclassificacaoobra in('C','Q') --and preanoselecao = $anoReferencia 
											) foo
											group by codigo, situacao
											) as foo2
											
											order by 1";
									
									$arrDados = $db->carregar( $sql, null, 3200 );
			                	
									foreach( $arrDados as $dado ){
			                	?>
			                		<tr>
			                			<td><img border="0" style="cursor:pointer" onclick="abreMapasMeta('<?=$dado['codigo'] ?>')" src="/imagens/icone_br.png" title="Exibir Mapa"> <img style="cursor:pointer;background-color:#FFFFFF" onclick="acessarPainelGerenciamento('2','<?=$dado['codigo'] ?>','');" src="../imagens/consultar.gif"> <?=str_replace(array("1.","2.","3.","4.","5."),"",$dado['situacao'])?></td>
			                			<td class="numero" ><?=number_format($dado['quantidadequad'],0,"",".") ?></td>
			                			<td class="numero" ><?=number_format($dado['quantidadecobe'],0,"",".") ?></td>
			                			<td class="numero" ><?=number_format($dado['quantidadequad']+$dado['quantidadecobe'],0,"",".") ?></td>
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
                 <td class="fundo_td" rowspan="2" >
                	<?=montaTabelaSituacaoObras(2, 'S')?>
                </td>
                <td class="fundo_td" colspan="4" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/financeiro.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Orçamentário / Financeiro<br/></div>
                	</div>
                	<?
                	
					$sql = "select rofano, sum(rofdot_ini) as rofdot_ini, sum(rofautorizado) as rofautorizado,  sum(rofempenhado) as empenhado,
				 					sum(rofliquidado_favorecido) as rofliquidado_favorecido, sum(rofpago) as rofpago
				 			from financeiro.execucao 
				 			where acacod = '12KV' and unicod = '26298' and loccod = '0001' and rofano in('2011','2012')
				 			group by prgcod,acacod, unicod,loccod,rofano";
					
					$dadosfinanceiros = $db->carregar($sql,null,3200);
					
					if($dadosfinanceiros[0]) {
						foreach($dadosfinanceiros as $fin) {
							$_financeiro[$fin['rofano']] = array("rofdot_ini"=>$fin['rofdot_ini'],"empenhado"=>$fin['empenhado'],"rofpago"=>$fin['rofpago']);
						}
					}
                	
                	
                	?>
                	<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" >
		                		<tr>
		                			<td>&nbsp;</td>
		                			<td class="center" ><b>2011<b></td>
		                			<td class="center" ><b>2012<b></td>
		                		</tr>
		                		<tr>
		                			<td>Dotação Inicial</td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2011&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2011']['rofdot_ini'],2,",",".") ?></td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2012&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2012']['rofdot_ini'],2,",",".") ?></td>
		                		</tr>

		                		<tr>
		                			<td>Empenhado</td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2011&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2011']['empenhado'],2,",",".") ?></td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2012&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2012']['empenhado'],2,",",".") ?></td>
		                		</tr>
		                		<tr>
		                			<td>Pago</td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2011&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2011']['rofpago'],2,",",".") ?></td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2012&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2012']['rofpago'],2,",",".") ?></td>
		                		</tr>
		                		<tr>
		                			<td class="bold" >Total</td>
		                			<td class="numero bold" ><?=number_format($_financeiro['2011']['rofpago']+$_financeiro['2011']['empenhado']+$_financeiro['2011']['rofdot_ini'],2,",",".") ?></td>
		                			<td class="numero bold" ><?=number_format($_financeiro['2012']['rofpago']+$_financeiro['2012']['empenhado']+$_financeiro['2012']['rofdot_ini'],2,",",".") ?></td>
		                		</tr>
		                	</table>
                </td>
                <!-- Tabela Financeiro -->
  </tr>
  <!-- 3 QUADROS -->
  <tr>
                <td class="fundo_td" colspan="2">
                	<?
                	$sql = "select sum(prevalorobra) from obras.preobra  pre
							inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
							inner join par.termoobra tob on tob.preid = pre.preid
							inner join par.termocompromissopac t on t.terid = tob.terid and t.terstatus = 'A'
							where  pto.ptoclassificacaoobra IN('Q','C') and pre.prestatus = 'A'";
                	$valor_contratado = $db->pegaUm($sql);
                	
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
								AND p.estuf IS NULL AND protipo in('C','Q') AND ter.terassinado = TRUE UNION ALL (SELECT te.tobid
									FROM 
										par.processoobra p 
									LEFT JOIN par.resolucao r ON r.resid=p.resid
									INNER JOIN par.termocompromissopac ter ON ter.proid = p.proid AND ter.terstatus='A' 
									INNER JOIN par.termoobra te ON te.terid = ter.terid
									INNER JOIN territorios.estado e ON e.estuf=p.estuf  
									WHERE 1=1
									and p.prostatus = 'A' 
								AND p.estuf IS NOT NULL AND protipo in('C','Q') AND ter.terassinado = TRUE)
							) foo";
					
					$termosassinados = $db->pegaUm($sql, 0, 3200);
                	?>
                	<div>
                		<img style="float:left;width: 40px;height: 40px;" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" onclick="acessarTermos();" class="titulo_box" ><?=number_format($termosassinados,0,"",".") ?><br/><span class="subtitulo_box" >Obras com Termos Assinados</span><br /><br />R$ <?=number_format($valor_contratado,2,"",".") ?><br/><span class="subtitulo_box" >Valor Contratado</span></div>
                	</div>
                </td>
                <td class="fundo_td">
                	<?
                	$sql = "select count(ligid) as recebida from callcenter.ligacao where temid = 1 and tlgid = 1 group by tlgid";
                	$contatosefetuados = $db->pegaUm($sql);
                	?>
                	<div>
                		<img style="float:left;width: 40px;height: 40px;" src="../imagens/icones/icons/call.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" class="titulo_box" onclick="acessarCallCenter();" ><?=number_format($contatosefetuados,0,"",".") ?><br/><span class="subtitulo_box" >Contatos<br/>Efetuados</span></div>
                	</div>
                </td>
                <td class="fundo_td" nowrap="nowrap">
                	<div onclick="abreUsuarios()">
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