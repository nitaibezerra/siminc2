<?php

if($_REQUEST['requisicao']=='montaComboMunicipioPorUf'){

	header('content-type: text/html; charset=ISO-8859-1');

	if (!$_REQUEST['estuf']) {
		die($db->monta_combo("muncodvisita", array(), 'S', 'Selecione o Estado', '', '', '', '', 'S', 'muncod_', false, null, 'Município'));
	}

	$sql = "select muncod as codigo, mundescricao as descricao
			from territorios.municipio
			where estuf = '" . $_REQUEST['estuf'] . "'
			order by mundescricao asc";

	$db->monta_combo("muncodvisita", $sql, 'S', 'Selecione...', '', '', '', '', 'N', 'muncod');
	die();
}

if($_REQUEST['requisicao'] == 'carregaUniversidades'){

	$sql = "select
				uniid as codigo,
				uninome as descricao
			from maismedicos.universidade
			".($_GET["q"] ? "WHERE uninome ILIKE '%{$_GET["q"]}%'" : "")."";

	$arDados = $db->carregar($sql);

	$q = strtolower($_GET["q"]);

	if($arDados){
		foreach ($arDados as $key=>$value){
			echo "{$value['descricao']}|{$value['codigo']}\n";
		}
	}else{
		echo "Sem registros|";
	}
	die;
}

include_once APPRAIZ . 'maismedicos/classes/Cockpit_Mais_Medicos.class.inc';
$obCockpitPMMB = new Cockpit_Mais_Medicos();

if($_REQUEST['requisicao']=='montaGraficoPeriodoSemVisita'){
	$obCockpitPMMB->montaGraficoPeriodoSemVisitaMedico($_REQUEST);
	die;
}

function contaIndicadorPorRegiao($indid=null)
{
	global $db;
	
	if($indid){
		$sql = "SELECT 
						regdescricao as descricao,
						sum(dsh.dshqtde) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN territorios.estado est ON dsh.dshuf = est.estuf
					INNER JOIN territorios.regiao reg ON reg.regcod = est.regcod
					WHERE sh.indid IN ({$indid})
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND sh.sehstatus <> 'I'
					GROUP BY regdescricao
					ORDER BY regdescricao";
		
		return $db->carregar($sql);
	}
	return false;
}

function contaMunicipiosPorRegiao()
{
	global $db;
	
	$sql = "select
					regdescricao as descricao,
					count(distinct mun.muncod) as valor
				from territorios.municipio mun
				join territorios.estado est on est.estuf = mun.estuf
				join territorios.regiao reg on reg.regcod = est.regcod
				group by regdescricao
				order by regdescricao";

	return $db->carregar($sql);
}

set_time_limit(0);
include_once APPRAIZ . 'pde/www/_funcoes_cockpit.php';
include_once APPRAIZ . 'includes/library/simec/funcoes.inc';

if($_REQUEST['requisicao']=='mudaTipoGegiao'){
	
	$config = array('rotation'=>'-45', 'dataLabels'=>'true');
		
	$formatoValores = array(
			'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
			'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
	);
	geraGraficoColuna($_REQUEST['metodo']($_REQUEST['id']), 'graficoPorRegiao', null, null, $formatoValores, null, "", 1000, 600, null, 300, $config);
	die;
}

permissaoPerfilConsultaCockpit('estrategico.php?modulo=principal/cockpit_mais_medicos&acao=A');

if($_REQUEST['requisicao']=='mostraDetalheCiclo'){
    montaTabelaCiclo($_REQUEST['indid']);
    die;
}

if($_REQUEST['requisicaoAjax']){
    $_REQUEST['requisicaoAjax']();
    die;
}

if(strstr($_SERVER['REQUEST_URI'],"maismedicos.php?modulo=relatorio/maismedicos&acao=C")){
    if($_REQUEST['requisicaoAjax'] != "popUpMaisMedicos"){
        echo "<script>alert('Acesso negado.');history.back(-1);</script>";
        die;
    }
}
?>
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=9" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
    <script language="javascript" type="text/javascript" src="js/estrategico.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

    <script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
    <script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
    <script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

    <script language="javascript" src="/estrutura/js/funcoes.js"></script>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    
<!--     <script type="text/javascript" src="/includes/jquery-autocomplete/jquery.autocomplete.js"></script> -->
<!-- 	<link rel="stylesheet" type="text/css" href="/includes/jquery-autocomplete/jquery.autocomplete.css" /> -->

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
    <script>
        atualizaUsuario();

        function exibeRelatorioMaisMedico(tipo)
        {
            var url = window.location + "&requisicaoAjax="+tipo;
            var div = jQuery("#div_tabela_relatorio_mais_medico");
            div.html('<center><p>&nbsp;</p><p><img src="../imagens/carregando.gif" align="absmiddle" />&nbsp;Carregando...</p></center>')
            jQuery.ajax({
                type: "POST",
                url: url,
                async: false,
                success: function(response){
                    div.html(response);
                }
            });
        }

        function exibeInformacaoPagamento(periodo)
        {
            //var url = window.location + "&requisicaoAjax=listaRemessaCreditoMaisMedicos&rmcid="+rmcid+"&periodo="+periodo;
            var url = window.location + "&requisicaoAjax=listaRemessaCreditoMaisMedicos&periodo="+periodo;
            janela(url,800,600,"Pagamento");
        }

        jQuery.noConflict();

        jQuery(function(){

            jQuery('.scrollbar').perfectScrollbar({wheelPropagation:true});

            jQuery('.lista-noticias').totemticker({
                row_height  :   '100px',
                next        :   '#ticker-next',
                previous    :   '#ticker-previous',
                stop        :   '#stop',
                start       :   '#start',
                mousestop   :   true,
            });
            jQuery('.btnDetalharPagamento').click(function(){
                arParams = this.id.split('_');
                if(jQuery(this).attr('src')=='../imagens/mais.gif'){
                    jQuery('.pagamentos_'+arParams[1]).show();
                    jQuery(this).attr('src', '../imagens/menos.gif')
                }else{
                    jQuery('.pagamentos_'+arParams[1]).hide();
                    jQuery(this).attr('src', '../imagens/mais.gif')
                }
            });

            jQuery('.btnMostraDetalheCiclo').click(function(){
                indid = this.id;
                if(jQuery(this).attr('src')=='../imagens/mais.gif'){
                    jQuery.ajax({
                        url		: '/pde/estrategico.php?modulo=principal/cockpit_mais_medicos&acao=A',
                        type	: 'post',
                        data	: 'requisicao=mostraDetalheCiclo&indid='+indid,
                        success	: function(e){
                            jQuery('#detalhe_'+indid).html(e).show();
                        }
                    });
                    jQuery(this).attr('src', '../imagens/menos.gif');
                }else{

                    jQuery('#detalhe_'+indid).hide();
                    jQuery(this).attr('src', '../imagens/mais.gif');
                }
            });
        });
    </script>

    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('cockpit/images/banner/bannerMaisMedicos.jpg');}
        #div_tabela_relatorio_mais_medico{display:block;width:100%;height:357px;overflow-y: scroll;}
    </style>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="titulo_pagina" >
            <div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
                <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;" />
                <div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
            </div>
            <img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_mais_medicos&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >Programa Mais Médicos para o Brasil</div></td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2792);" width="25%">
            <div>
                <?php exibirTitulo('alvo', 'Médicos'); ?>
            </div>
            <?php
            $sql = "SELECT sum(dsh.dshqtde) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2792)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND sh.sehstatus <> 'I'";
            $totalMeta = $db->pegaUm( $sql );
            $valorMeta = 15500;
            $porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="60%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <?=number_format($totalMeta,0,",",".")?> médicos ativos
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                            <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                <?= $porcentagem ?>%
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <font size=-1><?=number_format($valorMeta,0,",",".")?> médicos previstos (até 2016)</font>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2293);" width="25%">
            <div>
                <?php exibirTitulo('alvo', 'Supervisores'); ?>
            </div>
            <?php
            $sql = "SELECT sum(dsh.dshqtde) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2293)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND sh.sehstatus <> 'I'";
            $totalMeta = $db->pegaUm( $sql );
            $valorMeta = 2100;
            $porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <?=number_format($totalMeta,0,",",".")?> supervisores incorporados
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                            <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                <?= $porcentagem ?>%
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <font size=-1><?=number_format($valorMeta,0,",",".")?> supervisores previstos (até 2016)</font>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2292);" width="25%">
            <div>
                <?php exibirTitulo('alvo', 'Tutores'); ?>
            </div>
            <?php
            $sql = "SELECT sum(dsh.dshqtde) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2292)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND sh.sehstatus <> 'I'";
            $totalMeta = $db->pegaUm( $sql );
            $valorMeta = 210;
            $porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <?=number_format($totalMeta,0,",",".")?> tutores incorporados
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                            <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                <?= $porcentagem ?>%
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <font size=-1><?=number_format($valorMeta,0,",",".")?> tutores previstos (até 2016)</font>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" width="25%">
            <div>
                <?php exibirTitulo('casas', 'Instituições Supervisoras'); ?>
            </div>
            <?php
            $sql = "SELECT sum(dsh.dshqtde) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2291)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND sh.sehstatus <> 'I'";
            $totalIS = $db->pegaUm( $sql );

            $sql = "SELECT count(distinct dshcodmunicipio) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2792)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND sh.sehstatus <> 'I'";
            $totalMun = $db->pegaUm( $sql );
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <span class="link" onclick="abreIndicadorPopUp(2291);"><?=number_format($totalIS,0,",",".")?> instituições supervisoras</span> atuando em <span class="link" onclick="abreIndicadorPopUp(2792);"><?=number_format($totalMun,0,",",".")?> municípios</span>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('mapas', 'Mapa do Projeto Mais Médicos para o Brasil'); ?>
            </div>
            <!-- Tabela Adesão - Instituições Participantes -->
            <?php
            // POR UF
            $sql = "select count(distinct ies) as ies, count(distinct muncod) as muncod
                                from (
                                    select  uni.uniid as ies,
                                        mun.muncod,
                                        count(distinct (tut.tutid)) + count(distinct (sup.tutid)) as total
                                    from maismedicos.universidade uni
                                        inner join territorios.municipio mun ON mun.muncod = uni.muncod
                                        left join maismedicos.tutor tut ON uni.uniid = tut.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true
                                        left join maismedicos.tutor sup ON uni.uniid = sup.uniid and sup.tuttipo = 'S' and sup.tutstatus = 'A' and sup.tutvalidade is true
                                    group by mun.muncod, ies
                                    order by mun.muncod, ies
                                ) as tbl
                                where total > 0";

            $dados = $db->pegaLinha($sql);
            ?>
            <table style="width:100%;">
                <tr>
                    <td style="width:65%;">
                        <a target="_blank" href="/painel/painel.php?modulo=principal/mapas/mapaPadrao&acao=A&mapid=32&cockpit=1&id_132=checked">
                            <img class="link" src="../imagens/cockpit/mapa_cockpit.png" border="0"/></a>
                    </td>
                    <td>
                        <span class="subtitulo_box" ><b><font size="3"><?php echo $dados['ies']; ?></font></b>&nbsp;IES</span><br />
                        <span class="subtitulo_box" ><b><font size="3"><?php echo $dados['muncod']; ?></font></b>&nbsp;MUNICÍPIOS</span>
                    </td>
                </tr>
            </table>
            <!-- FIM Tabela Adesão - Instituições Participantes -->
        </td>
        <!-- Tabela de Relatório -->
        <td class="fundo_padrao" width="30%" colspan="2" valign="top" align="left">
            <?php exibirTitulo('executiverel', 'Relatório'); ?>
            <div style="width:100%;text-align:center" >
                <input type="radio" name="tipo_exibicao_mais_medicos" value="regiao" onclick="exibeRelatorioMaisMedico('exibeRelatorioMaisMedicoRegiao')" />Região
                <input type="radio" name="tipo_exibicao_mais_medicos" value="uf" checked="checked" onclick="exibeRelatorioMaisMedico('exibeRelatorioMaisMedicos')" />UF
                <input type="radio" name="tipo_exibicao_mais_medicos" value="saude" onclick="exibeRelatorioMaisMedico('exibeRelatorioMaisMedicosRegioesSaude')" />Regiões da Saúde
                <input type="radio" name="tipo_exibicao_mais_medicos" value="saude" onclick="exibeRelatorioMaisMedico('exibeRelatorioInstituicaoSupervisora')" />Instituição Supervisora
            </div>
            <div id="div_tabela_relatorio_mais_medico" >
                <?php exibeRelatorioMaisMedicos() ?>
            </div>
            Obs.: Algumas instituições atuam em mais de uma UF, tendo seus dados referentes à contagem de instituições, tutores e supervisores computados em todas as UFs em que atuam, no modo detalhado.
    </td>
        <!-- FIM Tabela de Relatório -->
    </tr>
    <tr>
        <td class="fundo_padrao link" colspan="2">
        		<div style="" id="td_projeto_regiao" onclick="abreIndicadorPopUp(2792);">
                	<?php exibirTitulo('executiverel', 'Relatório do Projeto, por Regiões'); ?>
        		</div>	
        			<p><center>
					<input type="radio" name="relatorioRegiao" id="" value="contaMunicipiosPorRegiao" /> Municípios  
					<input type="radio" name="relatorioRegiao" id="2292" value="contaIndicadorPorRegiao" /> Tutores   
					<input type="radio" name="relatorioRegiao" id="2293" value="contaIndicadorPorRegiao" /> Supervisores   
					<input type="radio" name="relatorioRegiao" id="2792" value="contaIndicadorPorRegiao" checked/> Médicos do Programa
					</center></p><br/>
					
					<script>
						jQuery(function(){
							jQuery('[name=relatorioRegiao]').click(function(){
								metodo = this.value;
								id = this.id;
								jQuery('#td_projeto_regiao').attr('onclick', 'abreIndicadorPopUp('+id+');');
								jQuery('#montaGraficoRegiao').html('<center><p><img src="../imagens/carregando.gif" border="0" />&nbsp;Carregando...<p></center>');
								jQuery.ajax({
									url		: document.location,
									type	: 'post',
									data	: 'requisicao=mudaTipoGegiao&metodo='+metodo+'&id='+id,
									success	: function(e){
										jQuery('#montaGraficoRegiao').html(e);
									}
								});
							});
						});
					</script>
				
				<div id="montaGraficoRegiao">
					<?php //$obCockpit->montaGraficoBarrasPorRegiao(); ?>
					<?php 
					$config = array('rotation'=>'-45', 'dataLabels'=>'true');
					
					$formatoValores = array(
							'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
							'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
					);
					geraGraficoColuna(contaIndicadorPorRegiao('2792'), 'graficoPorRegiao', null, null, $formatoValores, null, "", 1000, 600, null, 300, $config);
					?>
				</div>
				
        </td>
        <td class="fundo_padrao link" colspan="2">
             <?php exibirTitulo('executive', 'Visitas realizadas'); ?>
             
             
             <div id="divFiltrosVisitaPeriodos" style="width:100%">
             	<table width="100%" celpadding="3" cellspacing="1" align="center">
             		<tr>
	             		<td><?php
							$sql = "select	
										e.regcod as codigo,
										e.regdescricao as descricao
									from territorios.regiao e
									order by e.regdescricao asc";
							
							$db->monta_combo( "regcodvisita", $sql, 'S', 'Região', '', '', '', '', 'N', 'regcodvisita',false,'','Estado');
							?></td>
	             		<td><?php
							$sql = "select	
										e.estuf as codigo,
										e.estdescricao as descricao
									from territorios.estado e
									order by e.estdescricao asc";
							
							$db->monta_combo( "estufvisita", $sql, 'S', 'UF', '', '', '', '', 'N', 'estufvisita',false,'','Estado');
							?></td>
	             		<td id="municipio"><?php $db->monta_combo( "muncodvisita", array(), 'S', 'Município', '', '', '', '', 'N', 'muncodvisita',false,null,'Município'); ?></td>
	             		<td rowspan="2"><input style="height:100%;" type="button" value="Filtrar" id="btnFiltraPeriodoVisita" /></td>
	             	</tr>
             		<tr>
             			<td>
			             	<?php 
			             	$arCiclos = array(
			             		array('codigo'=>'1','descricao'=>'Primeiro ciclo'),
			             		array('codigo'=>'2','descricao'=>'Segundo ciclo'),
			             		array('codigo'=>'3','descricao'=>'Terceiro ciclo'),
			             		array('codigo'=>'4','descricao'=>'Quarto ciclo'),
			             		array('codigo'=>'5','descricao'=>'Quinto ciclo'),
			             		array('codigo'=>'sem','descricao'=>'Sem ciclo')
			             	);
			             	$db->monta_combo( "ciclovisita", $arCiclos, 'S', 'Ciclo', '', '', '', '', 'N', 'muncodvisita',false,null,'Ciclo'); 
			             	?>
						</td>     
	             		<td>
	             		<?php
							$sql = "select	
										e.uniid as codigo,
										e.estuf || ' - ' || e.unisigla as descricao
									from maismedicos.universidade e
									where unistatus = 'A'
									order by e.estuf, e.uninome asc";
							
							$db->monta_combo( "uniid", $sql, 'S', 'Instituição', '', '', '', '', 'N', 'uniid',false,'','Instituição');
							?>
		             		<? //=//campo_texto('uniid_auto','N','S','',1,255,'','', 'left', '', '', 'id="uniid_auto"', '', '');?>
<!-- 							<input type="hidden" name="uniid" id="uniid" value="" /> -->
	             		</td>
	             		<td>
	             		<?php 
			             	$arCiclos = array(
			             		array('codigo'=>'S','descricao'=>'Sim'),
			             		array('codigo'=>'N','descricao'=>'Não')
			             	);
			             	$db->monta_combo( "dseivisita", $arCiclos, 'S', 'DSEI', '', '', '', '', 'N', 'dseivisita',false,null,'DSEI'); 
			             	?>
	             		</td>
	             		
	             	</tr>
	             	
	             </table>
             </div>
             
             <div style="width:100%;max-height:380px;overflow-y:visible;overflow-x:hidden;">
             	<div id="montaGraficoPeriodoSemVisita">
				<?php
				$obCockpitPMMB->montaGraficoPeriodoSemVisitaMedico();
				?>
				</div>
			</div>
			
			<script>
             jQuery(function(){

                 jQuery("[name=dseivisita], [name=uniid], [name=regcodvisita], [name=estufvisita], [name=muncodvisita], [name=ciclovisita]").css({'width':'100%'});

//                  jQuery("[name=uniid_auto]").autocomplete(document.location.href+'&requisicao=carregaUniversidades', {			
//          			matchContains: true,
//          			minChars: 0, 			
//          			cacheLength:1000,
//          			width: 440,
//          			autoFill: false,
//          			max: 1000    		
//          		}).result(function(event, data, formatted) {
//          			var uniid = data[1];
//          			jQuery('[name=uniid]').val(uniid);
//          		});
                 
                 jQuery('#btnFiltraPeriodoVisita').click(function(){

                	 	regcod = jQuery('[name=regcodvisita]').val();
                	 	estuf = jQuery('[name=estufvisita]').val();
                	 	muncod = jQuery('[name=muncodvisita]').val();
                	 	ciclo = jQuery('[name=ciclovisita]').val();
                	 	uniid = jQuery('[name=uniid]').val();
                	 	dsei = jQuery('[name=dseivisita]').val();

                	 	params = 'requisicao=montaGraficoPeriodoSemVisita&dsei='+dsei+'&estuf='+estuf+'&muncod='+muncod;
                	 	params += '&regcod='+regcod+'&ciclo='+ciclo+'&uniid='+uniid

                	 	div = jQuery('#montaGraficoPeriodoSemVisita');

                	 	div.html('<center><p>&nbsp;</p><p><img src="../imagens/carregando.gif" align="absmiddle" />&nbsp;Carregando...</p></center>')                	 	
                	 	
						jQuery.ajax({
							url		: document.location.href,
							type	: 'post',
							data	: params,
							success	: function(e){
									div.html(e);
								}
							});
                     });
                });
             
             	jQuery('[name=estufvisita]').change(function(){

             		estuf = this.value;
             		
	         		var data = new Array();
	         			data.push({name : 'requisicao', value : 'montaComboMunicipioPorUf'},
	         					  {name : 'estuf', value : estuf}
	         					 );
	
	         		jQuery.ajax({
	         			   type		: "POST",
	         			   url		: document.location.href,
	         			   data		: data,
	         			   async    : false,
	         			   success	: function(res){
	         							jQuery('#municipio').html(res);
	         							jQuery('#municipio').css('visibility','visible');
	         						  }
	         			 });
             	});
             </script>
             
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2792);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Quantitativo de médicos'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpedsc AS descricao,
                        SUM(qtde)::INTEGER AS valor
                    FROM(
                        SELECT
                            dp.dpedsc,
                            dp.dpedatainicio,
                            CASE WHEN (
                                SELECT
                                    d1.dpeid
                                FROM painel.detalheperiodicidade d1
                                INNER JOIN painel.seriehistorica sh ON sh.dpeid=d1.dpeid
                                WHERE d1.dpedatainicio>=dp.dpedatainicio
                                AND d1.dpedatafim<=dp.dpedatafim
                                AND sh.indid=d.indid
                                AND sehstatus <> 'I'
                                ORDER BY d1.dpedatainicio DESC
                                LIMIT 1
                                ) = d.dpeid THEN sum(d.qtde)
                            ELSE 0
                            END AS qtde
                        FROM painel.v_detalheindicadorsh d
                        INNER JOIN painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
                        WHERE dp.perid = 9
                        AND d.indid = 2792
                        AND sehstatus <> 'I'
                        GROUP BY dp.dpedsc, dp.dpedatainicio, dp.dpedatafim, d.indid, d.dpeid
                    ) foo
                    GROUP BY dpedatainicio, dpedsc
                    ORDER BY dpedatainicio";
            $dados = $db->carregar( $sql, null, 3200 );

            $config = array('rotation'=>'-45', 'dataLabels'=>'true');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($dados, 'graficoColunaMedicos', null, null, $formatoValores, null, "", 1000, 600, null, 300, $config);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2293);" width="34%">
            <div>
                <?php exibirTitulo('indicador', 'Quantitativo de Supervisores'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpedsc AS descricao,
                        SUM(qtde)::INTEGER AS valor
                    FROM(
                        SELECT
                            dp.dpedsc,
                            dp.dpedatainicio,
                            CASE WHEN (
                                SELECT
                                    d1.dpeid
                                FROM painel.detalheperiodicidade d1
                                INNER JOIN painel.seriehistorica sh ON sh.dpeid=d1.dpeid
                                WHERE d1.dpedatainicio>=dp.dpedatainicio
                                AND d1.dpedatafim<=dp.dpedatafim
                                AND sh.indid=d.indid
                                AND sehstatus <> 'I'
                                ORDER BY d1.dpedatainicio DESC
                                LIMIT 1
                                ) = d.dpeid THEN sum(d.qtde)
                            ELSE 0
                            END AS qtde
                        FROM painel.v_detalheindicadorsh d
                        INNER JOIN painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
                        WHERE dp.perid = 9
                        AND d.indid = 2293
                        AND sehstatus <> 'I'
                        GROUP BY dp.dpedsc, dp.dpedatainicio, dp.dpedatafim, d.indid, d.dpeid
                    ) foo
                    GROUP BY dpedatainicio, dpedsc
                    ORDER BY dpedatainicio";
            $dados = $db->carregar( $sql, null, 3200 );

            $config = array('rotation'=>'-45', 'dataLabels'=>'true');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($dados, 'graficoColunSupervisores', null, null, $formatoValores, null, "", 1000, 600, null, 300, $config);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2292);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Quantitativo de Tutores'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpedsc AS descricao,
                        SUM(qtde)::INTEGER AS valor
                    FROM(
                        SELECT
                            dp.dpedsc,
                            dp.dpedatainicio,
                            CASE WHEN (
                                SELECT
                                    d1.dpeid
                                FROM painel.detalheperiodicidade d1
                                INNER JOIN painel.seriehistorica sh ON sh.dpeid=d1.dpeid
                                WHERE d1.dpedatainicio>=dp.dpedatainicio
                                AND d1.dpedatafim<=dp.dpedatafim
                                AND sh.indid=d.indid
                                AND sehstatus <> 'I'
                                ORDER BY d1.dpedatainicio DESC
                                LIMIT 1
                                ) = d.dpeid THEN sum(d.qtde)
                            ELSE 0
                            END AS qtde
                        FROM painel.v_detalheindicadorsh d
                        INNER JOIN painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
                        WHERE dp.perid = 9
                        AND d.indid = 2292
                        AND sehstatus <> 'I'
                        GROUP BY dp.dpedsc, dp.dpedatainicio, dp.dpedatafim, d.indid, d.dpeid
                    ) foo
                    GROUP BY dpedatainicio, dpedsc
                    ORDER BY dpedatainicio";
            $dados = $db->carregar( $sql, null, 3200 );

            $config = array('rotation'=>'-45', 'dataLabels'=>'true');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($dados, 'graficoColunTutores', null, null, $formatoValores, null, "", 1000, 600, null, 300, $config);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao">
            <!-- Informativo de pagamento-->
            <div>
                <?php exibirTitulo('financeiro', 'Informativo de pagamento de bolsas<br />Tutores/Supervisores'); ?>
            </div>
            <div>
                <?php
                // Informativo de Pagamento de bolsas de Tutores / Supervisores
                $sql = "select * from (
                            select
                                substr(dt_ini_periodo, 5, 2) || '/' || substr(dt_ini_periodo, 1, 4) as periodo,
                                substr(dt_ini_periodo, 5, 2) as mes,
                                substr(dt_ini_periodo, 1, 4) as ano,
                                count(tut.tutid) as tutor,
                                count(sup.tutid) as supervisor,
                                sum(vl_credito) as total,
                                1 as ordem
                            from
                                maismedicos.autorizacaopagamento aut
                            inner join
                                maismedicos.remessadetalhe det ON det.rmdid = aut.rmdid
                            left join
                                maismedicos.tutor tut ON tut.tutid = det.tutid and tut.tuttipo = 'T'
                            left join
                                maismedicos.tutor sup ON sup .tutid = det.tutid and sup .tuttipo = 'S'
                            where
                                aut.apgstatus = 'A'
                            and
                                det.cs_ocorrencia = '0000'
                            group by
                                substr(dt_ini_periodo, 1, 4), substr(dt_ini_periodo, 5, 2)
                            order by
                                substr(dt_ini_periodo, 1, 4), substr(dt_ini_periodo, 5, 2)
                        ) as tbls order by ano desc, mes desc";
                $arrDados = $db->carregar($sql, null, 3200);
                if($arrDados){
                    foreach($arrDados as $dados){
                        $arPagamentos[$dados['ano']][$dados['mes']]['tutores'] = $dados['tutor'];
                        $arPagamentos[$dados['ano']][$dados['mes']]['supervisores'] = $dados['supervisor'];
                        $arPagamentos[$dados['ano']][$dados['mes']]['valor'] = $dados['total'];
                        $arPagamentosTotais[$dados['ano']]['tutores'] += $dados['tutor'] ? $dados['tutor'] : 0;
                        $arPagamentosTotais[$dados['ano']]['supervisores'] += $dados['supervisor'] ? $dados['supervisor'] : 0;
                        $arPagamentosTotais[$dados['ano']]['valor'] += $dados['total'] ? $dados['total'] : 0;
                        $arTotais['tutor'] += $dados['tutor']; 
                        $arTotais['supervisor'] += $dados['supervisor']; 
                        $arTotais['valor'] += $dados['total']; 
                    }
                }
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                    <thead>
                    <tr>
                        <th>Período de Referência</th>
                        <th>Tutores</th>
                        <th>Supervisores</th>
                        <th>Total</th>
                    </tr>
                    </thead>
                    <tbody>
                    <?php if($arPagamentos): ?>
                        <?php foreach($arPagamentos as $k => $v): ?>
                            <tr>
                                <td>
                                    <img src="../imagens/mais.gif" id="pagamentos_<?php echo $k; ?>" class="btnDetalharPagamento" style="cursor:pointer;" />
                                    <?php echo $k; ?>
                                </td>
                                <td><?php echo $arPagamentosTotais[$k]['tutores'] ?></td>
                                <td><?php echo $arPagamentosTotais[$k]['supervisores'] ?></td>
                                <td><?php echo number_format($arPagamentosTotais[$k]['valor'],2,",","."); ?></td>
                            </tr>
                            <?php foreach($arPagamentos[$k] as $i => $d): ?>
                                <tr title="Abrir informativo de pagamento de bolsas" alt="Abrir informativo de pagamento de bolsas" onclick="exibeInformacaoPagamento('<?php echo $i.'/'.$k; ?>')" class="link pagamentos_<?php echo $k; ?>" style="display:none;cursor:pointer;">
                                    <td>&nbsp;&nbsp;<img src="../imagens/seta_filho.gif" /><?php echo mes_extenso($i); ?></td>
                                    <td><?php echo $d['tutores'] ?></td>
                                    <td><?php echo $d['supervisores'] ?></td>
                                    <td><?php echo number_format($d['valor'],2,",","."); ?></td>
                                </tr>
                            <?php endforeach; ?>
                        <?php endforeach; ?>
                    <?php else: ?>
                        <tr>
                            <td colspan="2" algin="center"><b>Sem registros.</b></td>
                        </tr>
                    <?php endif; ?>
                    </tbody>
                    <tfoot>
                    	<tr>
                    		<td>Totais</td>
                    		<td><?php echo number_format($arTotais['tutor'], 0, ',', '.'); ?></td>
                    		<td><?php echo number_format($arTotais['supervisor'], 0, ',', '.'); ?></td>
                    		<td><?php echo number_format($arTotais['valor'], 2, ',', '.'); ?></td>
                    	</tr>
                    </tfoot>
                </table>
                <?php unset($arPagamentos); ?>
            </div>
            <!-- FIM Informativo de pagamento-->
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('control', 'Implantação do Programa Mais Médicos'); ?>
            </div>
            <?php
            $arCiclos = array(
                array("ciclo"=>"4º Ciclo","indid"=>"150573" ),
                array("ciclo"=>"3º Ciclo","indid"=>"148382" ),
                array("ciclo"=>"2º Ciclo - Segunda etapa","indid"=>"148074" ),
                array("ciclo"=>"2º Ciclo - Primeira etapa","indid"=>"147355" ),
                array("ciclo"=>"1º Ciclo","indid"=>"146835" )
            );
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                <tbody>
                <?php foreach($arCiclos as $ciclo): ?>
                    <tr>
                        <td>
                            <img id="<?php echo $ciclo['indid']; ?>" src="../imagens/mais.gif" class="btnMostraDetalheCiclo" style="cursor:pointer;" />
                            <?php echo $ciclo['ciclo']; ?>
                        </td>
                    </tr>
                    <tr>
                        <td id="detalhe_<?php echo $ciclo['indid']; ?>" style="background:#152D56;display:none;"></td>
                    </tr>
                <?php endforeach; ?>
                </tbody>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2885);">
            <div>
                <?php exibirTitulo('indicador', 'Situação dos Médicos Intercambistas<br>nos Módulos de Acolhimento e Avaliação'); ?>
            </div>
            <?
            $sql = "SELECT
                        tid2.tiddsc AS descricao,
                        sum(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid in (2885)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao
                    ORDER BY descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaSituacao", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, null, null, "#fff", true);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2079);" colspan="3">
            <div>
                <?php exibirTitulo('chat', 'Notícias Mais Médicos'); ?>
            </div>
            <div>
                <ul class="lista-noticias">
                    <?php
                    $xml = utf8_encode(file_get_contents('http://www.google.com/alerts/feeds/04180951689783258528/13040220840775365612'));
                    $rss = new SimpleXMLElement($xml);

                    foreach ($rss as $item) {
                        if(isset($item->title)) { ?>
                            <li>
                                <a target="_blank" href="<?php echo (string)$item->link['href']; ?>">
                                    <p><?php echo utf8_decode(utf8_decode($item->title)); ?></p><br>
                                    <p><?php echo utf8_decode(utf8_decode($item->content)); ?></p>
                                </a>
                            </li>
                        <?php }
                    } ?>
                </ul>
                <div class="clear"></div>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_td_laranja" colspan="3">
            <div style="text-align:center;" >
                <img src="../imagens/icones/icons/executiverel.png" style="vertical-align:middle;" />
                <input type="text" onclick="this.style.color='#000000';this.value='';" name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;' title='' class=' normal' />
                <img src="../imagens/icones/icons/Find.png" style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);" />
            </div>
        </td>
    </tr>
</table>
</body>
</html>