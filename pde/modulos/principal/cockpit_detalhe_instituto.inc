<?php

#PARA O USUÁRIO QUE ACESSAO MONITORAMENTO PELO SISTEMA REDE FEDERAL ESSE PROCEDIMENTO VERIFICA SE O USUÁRIO TEM UM INSTITUIÇÃO RELACIONADA A ELE. CASO NÃO TENHO NÃO PERMITE O ACESSO.
if( $_REQUEST['entid'] != '' && $_REQUEST['redefederal'] == 'SIM' ){
    $usucpf = $_SESSION['usucpf'];

    $sql = "
        SELECT entid FROM academico.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '{$usucpf}' AND pflcod IN( 526, 1409, 1410, 1411, 1413 )
    ";
    $entid = $db->pegaUm($sql);

    if( $entid == '' ){
        $db->sucesso('','', 'Não há uma Instituição relacionada ao seu usuário. Entre em contato com o Administrador do sistema!', 'N', 'N', 'N');
    }
}

set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

if(!$_REQUEST['tipo']){
    $_SESSION['exportar1'] = "";
    $_SESSION['exportar2'] = "";
    $_SESSION['exportar3'] = "";
    $_SESSION['exportar4'] = "";
    $_SESSION['exportar5'] = "";
    $_SESSION['exportar6'] = "";
    $_SESSION['exportar7'] = "";
    $_SESSION['exportar8'] = "";
    $_SESSION['exportar9'] = "";
    $_SESSION['exportar10'] = "";
    $_SESSION['exportar11'] = "";
    $_SESSION['exportar12'] = "";
    $_SESSION['exportar13'] = "";
    $_SESSION['exportar14'] = "";
    $_SESSION['exportar15'] = "";
    $_SESSION['exportar16'] = "";
    $_SESSION['exportar17'] = "";
    $_SESSION['exportar18'] = "";
    $_SESSION['exportar19'] = "";
    $_SESSION['exportar20'] = "";
    $_SESSION['exportar21'] = "";
    $_SESSION['exportar22'] = "";
    $_SESSION['exportar23'] = "";
}

if($_REQUEST['campus_escolhido']){
    getDadosCampus($_REQUEST['campus_escolhido']);
    die();
}
if($_REQUEST['campus_escolhido_scrool']){
    getDadosScrool($_REQUEST['campus_escolhido_scrool']);
    die();
}
if(isset($_REQUEST['detalhe_redefederal1'])){
    gerarGraficoRedeFederal1($_REQUEST['detalhe_redefederal1']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal2'])){
    gerarGraficoRedeFederal2($_REQUEST['detalhe_redefederal2']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal3'])){
    gerarGraficoRedeFederal3($_REQUEST['detalhe_redefederal3']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal4'])){
    gerarGraficoRedeFederal4($_REQUEST['detalhe_redefederal4']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal5'])){
    gerarGraficoRedeFederal5($_REQUEST['detalhe_redefederal5']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal6'])){
    gerarGraficoRedeFederal6($_REQUEST['detalhe_redefederal6']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal7'])){
    gerarGraficoRedeFederal7($_REQUEST['detalhe_redefederal7']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal8'])){
    gerarGraficoRedeFederal8($_REQUEST['detalhe_redefederal8']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal9'])){
    gerarGraficoRedeFederal9($_REQUEST['detalhe_redefederal9']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal10'])){
    gerarGraficoRedeFederal10($_REQUEST['detalhe_redefederal10']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal11'])){
    gerarGraficoRedeFederal11($_REQUEST['detalhe_redefederal11']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal12'])){
    gerarGraficoRedeFederal12($_REQUEST['detalhe_redefederal12']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal13'])){
    gerarGraficoRedeFederal13($_REQUEST['detalhe_redefederal13']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal14'])){
    gerarGraficoRedeFederal14($_REQUEST['detalhe_redefederal14']);
    die;
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

	<link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
	<script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

	<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
	<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

	<script language="javascript" src="/estrutura/js/funcoes.js"></script>
	<script language="JavaScript" src="../includes/funcoes.js"></script>

	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
		.fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
		#popdiv{display:block;width:100%;height:357px;overflow-y: scroll;}
	</style>
	<script>
		jQuery.noConflict();

        jQuery(function(){
            jQuery('.selecionar-campus').click(function(){
                if(jQuery(this).attr('entid')){
                    jQuery('#dados_campus, #dados_scrool').html('');
                    jQuery('#dados_campus').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&campus_escolhido='+jQuery(this).attr('entid'));
					jQuery('#dados_scrool').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&campus_escolhido_scrool='+jQuery(this).attr('entid'));
                }
            });
        });
		atualizaUsuario();
		
		function abreObras1(obrid){
    		window.open('/obras/obras.php?modulo=principal/cadastro&acao=A&obrid='+obrid);
    	}

        function visualizarXLS(tipo){
            window.location='estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&tipo='+tipo;
        }

        jQuery(function(){
            jQuery('.filtrar_redefederal1').click(function(){
                jQuery('#div_redefederal1').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal1='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal2').click(function(){
                jQuery('#div_redefederal2').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal2='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal3').click(function(){
                jQuery('#div_redefederal3').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal3='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal4').click(function(){
                jQuery('#div_redefederal4').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal4='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal9').click(function(){
                jQuery('#div_redefederal9').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal9='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal11').click(function(){
                jQuery('#div_redefederal11').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal11='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal12').click(function(){
                jQuery('#div_redefederal12').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal12='+jQuery(this).val());
            });
            jQuery('body').on('click', '.filtrar_redefederal5', function(){
                jQuery('#div_redefederal5').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal5='+jQuery(this).val());
            });
            jQuery('body').on('click', '.filtrar_redefederal6', function(){
                jQuery('#div_redefederal6').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal6='+jQuery(this).val());
            });
            jQuery('body').on('click', '.filtrar_redefederal7', function(){
                jQuery('#div_redefederal7').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal7='+jQuery(this).val());
            });
            jQuery('body').on('click', '.filtrar_redefederal8', function(){
                jQuery('#div_redefederal8').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal8='+jQuery(this).val());
            });
            jQuery('body').on('click', '.filtrar_redefederal10', function(){
                jQuery('#div_redefederal10').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal10='+jQuery(this).val());
            });
            jQuery('body').on('click', '.filtrar_redefederal13', function(){
                jQuery('#div_redefederal13').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal13='+jQuery(this).val());
            });
            jQuery('body').on('click', '.filtrar_redefederal14', function(){
                jQuery('#div_redefederal14').load('estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&detalhe_redefederal14='+jQuery(this).val());
            });
        });
	</script>
</head>
<body>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
        <?php if($_REQUEST['redefederal']){ ?>
            <div>
        <?php }else{ ?>
            <div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
        <?php } ?>
            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
        	<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=<?=$_REQUEST['entid'] ?>';">
		<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%"  cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_padrao" colspan="6" >
			<?php
			$sql = "SELECT
						UPPER(ent.entsig) AS sigla, 
						ent.entnome AS universidade, 
						m.estuf AS uf, 
						m.mundescricao AS municipio, 
						entreitor.entnome AS reitor, 
						entinterlocutor.entnome AS interlocutor
					FROM entidade.entidade ent
					INNER JOIN entidade.endereco ed ON ed.entid = ent.entid
					INNER JOIN territorios.municipio m ON m.muncod = ed.muncod
					LEFT JOIN (
						SELECT entreitor.entnome, ent.entid
						FROM entidade.entidade ent
						INNER JOIN entidade.funentassoc fea ON fea.entid = ent.entid
						INNER JOIN entidade.funcaoentidade fue ON fue.fueid = fea.fueid AND fue.fuestatus = 'A' AND fue.funid = 21
						INNER JOIN entidade.entidade entreitor ON entreitor.entid = fue.entid
						INNER JOIN academico.membroconselho mcs ON mcs.feaid = fea.feaid AND mcs.mcssituacao IS TRUE
						) AS entreitor ON entreitor.entid = ent.entid
					LEFT JOIN (
						SELECT entinterlocutor.entnome, ent.entid
						FROM entidade.entidade ent
						INNER JOIN entidade.funentassoc fea ON fea.entid = ent.entid
						INNER JOIN entidade.funcaoentidade fue ON fue.fueid = fea.fueid AND fue.fuestatus = 'A' AND fue.funid = 40
						INNER JOIN entidade.entidade entinterlocutor ON entinterlocutor.entid = fue.entid
						INNER JOIN academico.membroconselho mcs ON mcs.feaid = fea.feaid AND mcs.mcssituacao IS TRUE
						) AS entinterlocutor ON entinterlocutor.entid = ent.entid
					WHERE ent.entid = " . $_REQUEST['entid'];
			$arrDados = $db->pegaLinha($sql);
			$nomeUniversidade = $arrDados['sigla'] . ' - ' . $arrDados['universidade'];
			?>
			<table width="100%" border="0" >
				<tr>
					<td width="10%" valign="center">
						<!--<br><img width="110px" src="cockpit/images/universidades/<?=strtolower($arrDados['sigla'])?>.jpg">-->
					</td>
					<td width="90%">
						<h1><?=$arrDados['sigla'] ?> - <?= $arrDados['universidade'] ?></h1>
						<h2><?=$arrDados['uf'] ?> / <?=$arrDados['municipio'] ?><br>
						Reitor: <?=$arrDados['reitor'] ?><br>
						Interlocutor Institucional: <?=$arrDados['interlocutor'] ?></h2>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
        <td class="fundo_padrao" width="20%">
            <div>
                <?php exibirTitulo('doc', 'Total de matrículas','(<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>)'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpe.dpeanoref AS ano,
                        CASE
                            WHEN dsh.tidid1 IN (6995, 6994, 6996, 6997, 6998, 6999, 7867, 7869) OR sh.indid IN (2903) THEN 'Técnico'
                            WHEN dsh.tidid1 IN (6984, 6985, 6986, 6987) THEN 'Pós-Graduação'
                            WHEN dsh.tidid1 IN (7000, 7866) THEN 'FIC >= 160h'
                            WHEN dsh.tidid1 IN (9264) THEN 'FIC < 160h'
                            WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
                            WHEN dsh.tidid1 IN (6992, 6988, 6989, 6990, 6991, 6993) THEN 'Superior'
                        END AS etapa,
                        SUM(dsh.dshqtde) as total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN entidade.funcaoentidade fue ON fue.entid = dsh.entid AND fue.funid = 17
                    INNER JOIN entidade.funentassoc fea ON fea.fueid = fue.fueid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid IN (2575, 2905, 2903)
                    AND dpe.dpeanoref >= '2010'
                    AND fea.entid = {$_REQUEST['entid']}
                    GROUP BY etapa, ano
                    ORDER BY etapa, ano";
            $arrDados = $db->carregar($sql);
            $total2010 = 0;
            $total2011 = 0;
            $total2012 = 0;
            $total2013 = 0;
            $total2014 = 0;
            $total2015 = 0;
            $count = -1;
            $arrSituacao = array();
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                <tr>
                    <th class="" >Tipo</th>
                    <th class="" >2010</th>
                    <th class="" >2011</th>
                    <th class="" >2012</th>
                    <th class="" >2013</th>
                    <th class="" >2014</th>
                    <th class="" >2015</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado){
                        $arrSituacao[$dado['etapa']][$dado['ano']]['total'][] = $dado['total'];
                    }
                    foreach($arrSituacao as $chave => $sit):
                        $count++;
                        $soma2010 = (is_array($sit['2010']['total'])?array_sum($sit['2010']['total']):0);
                        $soma2011 = (is_array($sit['2011']['total'])?array_sum($sit['2011']['total']):0);
                        $soma2012 = (is_array($sit['2012']['total'])?array_sum($sit['2012']['total']):0);
                        $soma2013 = (is_array($sit['2013']['total'])?array_sum($sit['2013']['total']):0);
                        $soma2014 = (is_array($sit['2014']['total'])?array_sum($sit['2014']['total']):0);
                        $soma2015 = (is_array($sit['2015']['total'])?array_sum($sit['2015']['total']):0);
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $chave ?></td>
                            <td class="numero" ><?php echo number_format($soma2010,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2011,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2012,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2013,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2014,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2015,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2010+$soma2011+$soma2012+$soma2013+$soma2014+$soma2015,0,",",".") ?></td>
                        </tr>
                        <?php
                        $total2010+=$soma2010;
                        $total2011+=$soma2011;
                        $total2012+=$soma2012;
                        $total2013+=$soma2013;
                        $total2014+=$soma2014;
                        $total2015+=$soma2015;
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($total2010,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2011,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2012,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2013,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2014,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2015,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2010+$total2011+$total2012+$total2013+$total2014+$total2015,0,",",".") ?></th>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2575);" width="20%" >
            <div>
                <?php exibirTitulo('doc', 'Total de matrículas<br>regulares'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpe.dpeanoref AS ano,
                        CASE
                            WHEN dsh.tidid1 IN (6995, 6994, 6996, 6997, 6998, 6999, 7867, 7869) OR sh.indid IN (2903) THEN 'Técnico'
                            WHEN dsh.tidid1 IN (6984, 6985, 6986, 6987) THEN 'Pós-Graduação'
                            WHEN dsh.tidid1 IN (7000, 7866) THEN 'FIC >= 160h Regular'
                            WHEN dsh.tidid1 IN (9264) THEN 'FIC < 160h Regular'
                            WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
                            WHEN dsh.tidid1 IN (6992, 6988, 6989, 6990, 6991, 6993) THEN 'Superior'
                        END AS etapa,
                        SUM(dsh.dshqtde) as total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN entidade.funcaoentidade fue ON fue.entid = dsh.entid AND fue.funid = 17
                    INNER JOIN entidade.funentassoc fea ON fea.fueid = fue.fueid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid IN (2575)
                    AND fea.entid = {$_REQUEST['entid']}
                    GROUP BY etapa, ano
                    ORDER BY etapa, ano";
            $arrDados = $db->carregar($sql);
            $total2011 = 0;
            $total2012 = 0;
            $total2013 = 0;
            $total2014 = 0;
            $total2015 = 0;
            $count = -1;
            $arrSituacao = array();
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                <tr>
                    <th class="" >Tipo</th>
                    <th class="" >2011</th>
                    <th class="" >2012</th>
                    <th class="" >2013</th>
                    <th class="" >2014</th>
                    <th class="" >2015</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado){
                        $arrSituacao[$dado['etapa']][$dado['ano']]['total'][] = $dado['total'];
                    }
                    foreach($arrSituacao as $chave => $sit):
                        $count++;
                        $soma2011 = (is_array($sit['2011']['total'])?array_sum($sit['2011']['total']):0);
                        $soma2012 = (is_array($sit['2012']['total'])?array_sum($sit['2012']['total']):0);
                        $soma2013 = (is_array($sit['2013']['total'])?array_sum($sit['2013']['total']):0);
                        $soma2014 = (is_array($sit['2014']['total'])?array_sum($sit['2014']['total']):0);
                        $soma2015 = (is_array($sit['2015']['total'])?array_sum($sit['2015']['total']):0);
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $chave ?></td>
                            <td class="numero" ><?php echo number_format($soma2011,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2012,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2013,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2014,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2015,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2011+$soma2012+$soma2013+$soma2014+$soma2015,0,",",".") ?></td>
                        </tr>
                        <?php
                        $total2011+=$soma2011;
                        $total2012+=$soma2012;
                        $total2013+=$soma2013;
                        $total2014+=$soma2014;
                        $total2015+=$soma2015;
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($total2011,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2012,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2013,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2014,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2015,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2011+$total2012+$total2013+$total2014+$total2015,0,",",".") ?></th>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2905);" width="20%" >
            <div>
                <?php exibirTitulo('doc', 'Matrículas Bolsa<br>Formação'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpe.dpeanoref AS ano,
                        CASE
                            WHEN dsh.tidid1 IN (7866) THEN 'FIC'
                            WHEN dsh.tidid1 IN (7867,7869) THEN 'Técnico'
                        ELSE tid1.tiddsc
                        END AS etapa,
                        SUM(dshqtde) AS total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN entidade.entidade e ON e.entunicod= dsh.unicod AND e.entstatus = 'A'
                    INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid = 2905
                    AND e.entid = {$_REQUEST['entid']}
                    AND dpe.dpeanoref BETWEEN '2011' AND '2015'
                    GROUP BY ano, etapa";
            $arrDados = $db->carregar($sql);
            $total2011 = 0;
            $total2012 = 0;
            $total2013 = 0;
            $total2014 = 0;
            $total2015 = 0;
            $totalMatriculas = 0;
            $count = -1;
            $arrSituacao = array();
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                <tr>
                    <th class="" >Etapa</th>
                    <th class="" >2011</th>
                    <th class="" >2012</th>
                    <th class="" >2013</th>
                    <th class="" >2014</th>
                    <th class="" >2015</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado){
                        $arrSituacao[$dado['etapa']][$dado['ano']]['total'][] = $dado['total'];
                    }
                    foreach($arrSituacao as $chave => $sit):
                        $count++;
                        $soma2011 = (is_array($sit['2011']['total'])?array_sum($sit['2011']['total']):0);
                        $soma2012 = (is_array($sit['2012']['total'])?array_sum($sit['2012']['total']):0);
                        $soma2013 = (is_array($sit['2013']['total'])?array_sum($sit['2013']['total']):0);
                        $soma2014 = (is_array($sit['2014']['total'])?array_sum($sit['2014']['total']):0);
                        $soma2015 = (is_array($sit['2015']['total'])?array_sum($sit['2015']['total']):0);
                        $somaMatriculas = $soma2011 + $soma2012 + $soma2013 + $soma2014 + $soma2015;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $chave ?></td>
                            <td class="numero" ><?php echo number_format($soma2011,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2012,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2013,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2014,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2015,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($somaMatriculas,0,",",".") ?></td>
                        </tr>
                    <?php
                    $total2011+=$soma2011;
                    $total2012+=$soma2012;
                    $total2013+=$soma2013;
                    $total2014+=$soma2014;
                    $total2015+=$soma2015;
                    $totalMatriculas = $total2011 + $total2012 + $total2013 + $total2014 + $total2015;
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($total2011,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2012,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2013,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2014,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2015,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($totalMatriculas,0,",",".") ?></th>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2903);" width="20%">
            <div>
                <?php exibirTitulo('doc', 'Matrículas E-Tec'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS ano, SUM(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN entidade.funcaoentidade fue ON fue.entid = dsh.entid AND fue.funid = 17
                    INNER JOIN entidade.funentassoc fea ON fea.fueid = fue.fueid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid = 2903
                    AND fea.entid = {$_REQUEST['entid']}
                    AND dpe.dpeanoref >= '2011'
                    GROUP BY ano
                    ORDER BY ano";
            $arrDados = $db->carregar($sql);
            $totalMatriculas = 0;
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                <tr>
                    <th class="" >Ano</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado):
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['ano'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['valor'],0,",",".") ?></td>
                        </tr>
                        <?php
                        $totalMatriculas+= $dado['valor'];
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($totalMatriculas,0,",",".") ?></th>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" colspan="2" width="20%">
            <div>
                <?php exibirTitulo('doc', 'Quadro de Pessoal'); ?>
            </div>
            <?php
            $sql = "SELECT tid1.tiddsc AS etapa, SUM(dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN entidade.entidade e ON e.entunicod= dsh.unicod AND e.entstatus = 'A'
                    INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid = 2902
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND e.entid = {$_REQUEST['entid']}
                    GROUP BY etapa
                    ORDER BY etapa";
            $arrDados = $db->carregar($sql);
            $totalTecnicos = 0;
            $count = -1;
            ?>
            <table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" onclick="abreIndicadorPopUp(2902);">
                <tr>
                    <th class="" >Técnicos</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado):
                        $totalTecnicos+=$dado['valor'];
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['etapa'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['valor'],0,",",".") ?></td>
                        </tr>
                    <?php
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($totalTecnicos,0,",",".") ?></th>
                </tr>
            </table>
            <br><br>
            <?php
            $sql = "SELECT tid1.tiddsc AS etapa, SUM(dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN entidade.entidade e ON e.entunicod= dsh.unicod AND e.entstatus = 'A'
                    INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid = 2901
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND e.entid = {$_REQUEST['entid']}
                    GROUP BY etapa
                    ORDER BY etapa";
            $arrDados = $db->carregar($sql);
            $totalProfessores = 0;
            $count = -1;
            ?>
            <table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" onclick="abreIndicadorPopUp(2901);">
                <tr>
                    <th class="" >Professores</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado):
                        $totalProfessores+=$dado['valor'];
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['etapa'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['valor'],0,",",".") ?></td>
                        </tr>
                    <?php
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($totalProfessores,0,",",".") ?></th>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao">
            <?php
            $sql="SELECT dpe.dpeordem || '/' || dpe.dpeanoref AS edicao
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND sh.indid = 2671";
            $edicao = $db->pegaUm($sql);
            ?>
            <div>
                <?php exibirTitulo('doc', 'SISU - Edição '.$edicao); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 2671
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" <?= ($valor ? 'onclick="abreIndicadorPopUp(2671);"' : '') ?>>
                    <td class="bold subtitulo_box">Vagas disponibilizadas</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 1435
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(1435);">
                    <td class="bold subtitulo_box">Inscrições</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao"  >
            <div>
                <?php exibirTitulo('doc', 'Bolsa Permanência'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.indid = 2019
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(2019);">
                    <td class="bold subtitulo_box">Quantidade de bolsas</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao"  >
            <div>
                <?php exibirTitulo('doc', 'PROEXT'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.indid = 334
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(334);">
                    <td class="bold subtitulo_box">Projetos apoiados</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
		<td class="fundo_padrao"  >
			<div>
				<?php exibirTitulo('doc', 'PARFOR'); ?>
			</div>
			<table border="0" cellpadding="5" cellspacing="5" width="100%" >
				<?php
				$sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 1367
						AND ies.entid = {$_REQUEST['entid']}";
				$valor = $db->pegaUm($sql);
				?>
				<tr class="link" onclick="abreIndicadorPopUp(1367);">
					<td class="bold subtitulo_box">Turmas ativas</td>
					<td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
				</tr>
				<?php
				$sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.indid = 1513
						AND ies.entid = {$_REQUEST['entid']}";
				$valor = $db->pegaUm($sql);
				?>
				<tr class="link" onclick="abreIndicadorPopUp(1513);">
					<td class="bold subtitulo_box">Matrículas ativas</td>
					<td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
				</tr>
			</table>
		</td>
        <td class="fundo_padrao"  >
            <div>
                <?php exibirTitulo('doc', 'PIBID'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.indid = 108
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(108);">
                    <td class="bold subtitulo_box">Bolsas aprovadas</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
		<td class="fundo_padrao"  >
			<div>
				<?php exibirTitulo('doc', 'PET'); ?>
			</div>
			<table border="0" cellpadding="5" cellspacing="5" width="100%" >
				<?php
				$sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.indid = 580
						AND ies.entid = {$_REQUEST['entid']}";
				$valor = $db->pegaUm($sql);
				?>
				<tr class="link" onclick="abreIndicadorPopUp(580);">
					<td class="bold subtitulo_box">Grupos participantes</td>
					<td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
				</tr>
			</table>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%"  cellspacing="4" cellpadding="1" class="tabela_painel">
    <tr>
        <td class="fundo_padrao" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Evolução de matrículas atendidas <br>('.$nomeUniversidade.')', '<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_redefederal1" name="redefederal1" value="1" checked="checked" onclick="" />Todos
                <input type="radio" class="filtrar_redefederal1" name="redefederal1" value="2" onclick="" />Rede Federal
                <input type="radio" class="filtrar_redefederal1" name="redefederal1" value="3" onclick="" />Bolsa-Formação
                <input type="radio" class="filtrar_redefederal1" name="redefederal1" value="4" onclick="" />e-Tec
            </div>
            <br>
            <div id="div_redefederal1">
                <?php gerarGraficoRedeFederal1(1); ?>
            </div>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(1);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Evolução de ingressantes <br>('.$nomeUniversidade.')', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_redefederal2" name="redefederal2" value="1" checked="checked" onclick="" />Todos
                <input type="radio" class="filtrar_redefederal2" name="redefederal2" value="2" onclick="" />Rede Federal
                <input type="radio" class="filtrar_redefederal2" name="redefederal2" value="3" onclick="" />Bolsa-Formação
                <input type="radio" class="filtrar_redefederal2" name="redefederal2" value="4" onclick="" />e-Tec
            </div>
            <br>
            <div id="div_redefederal2">
                <?php gerarGraficoRedeFederal2(1); ?>
            </div>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(2);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Evolução de matrículas atendidas - FIC <br>('.$nomeUniversidade.')', '<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_redefederal11" name="redefederal11" value="1" checked="checked" onclick="" />Todos
                <input type="radio" class="filtrar_redefederal11" name="redefederal11" value="2" onclick="" />Rede Federal
                <input type="radio" class="filtrar_redefederal11" name="redefederal11" value="3" onclick="" />Bolsa-Formação
            </div>
            <br>
            <div id="div_redefederal11">
                <?php gerarGraficoRedeFederal11(1); ?>
            </div>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(20);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Evolução de ingressantes - FIC <br>('.$nomeUniversidade.')', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_redefederal12" name="redefederal12" value="1" checked="checked" onclick="" />Todos
                <input type="radio" class="filtrar_redefederal12" name="redefederal12" value="2" onclick="" />Rede Federal
                <input type="radio" class="filtrar_redefederal12" name="redefederal12" value="3" onclick="" />Bolsa-Formação
            </div>
            <br>
            <div id="div_redefederal12">
                <?php gerarGraficoRedeFederal12(1); ?>
            </div>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(21);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Evolução de matrículas atendidas - Técnico <br>('.$nomeUniversidade.')', '<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_redefederal3" name="redefederal3" value="1" checked="checked" onclick="" />Todos
                <input type="radio" class="filtrar_redefederal3" name="redefederal3" value="2" onclick="" />Rede Federal
                <input type="radio" class="filtrar_redefederal3" name="redefederal3" value="3" onclick="" />Bolsa-Formação
                <input type="radio" class="filtrar_redefederal3" name="redefederal3" value="4" onclick="" />e-Tec
            </div>
            <br>
            <div id="div_redefederal3">
                <?php gerarGraficoRedeFederal3(1); ?>
            </div>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(3);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Evolução de ingressantes - Técnico <br>('.$nomeUniversidade.')', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_redefederal4" name="redefederal4" value="1" checked="checked" onclick="" />Todos
                <input type="radio" class="filtrar_redefederal4" name="redefederal4" value="2" onclick="" />Rede Federal
                <input type="radio" class="filtrar_redefederal4" name="redefederal4" value="3" onclick="" />Bolsa-Formação
                <input type="radio" class="filtrar_redefederal4" name="redefederal4" value="4" onclick="" />e-Tec
            </div>
            <br>
            <div id="div_redefederal4">
                <?php gerarGraficoRedeFederal4(1); ?>
            </div>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(4);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" width="50%">
            <div class="link" onclick="abreIndicadorPopUp(2575);">
                <?php exibirTitulo('indicador', 'Evolução de matrículas de graduação<br>('.$nomeUniversidade.')'); ?>
            </div>
            <?php
            $arrEtapa = array();
            $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (6990, 6993) THEN 'Tecnólogo'
                    WHEN dsh.tidid1 IN (6992, 6989) THEN 'Licenciatura'
                    WHEN dsh.tidid1 IN (6988, 6991) THEN 'Bacharelado'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            INNER JOIN entidade.entidade e ON e.entunicod = dsh.unicod AND e.entstatus = 'A'
            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
            AND sh.indid IN (2575)
            AND dsh.tidid1 IN (6990, 6993, 6992, 6989, 6988, 6991)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND e.entid = {$_REQUEST['entid']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $_SESSION['exportar11'] = $arrDados;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
                }

                $arrX = array();
                $arrA = array();
                $arrB = array();
                $arrC = array();
                $arrTotal = array();

                foreach($arrEtapa as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $vA = Round((is_array($sit['Tecnólogo']['total'])?array_sum($sit['Tecnólogo']['total']):0));
                    $vB = Round((is_array($sit['Licenciatura']['total'])?array_sum($sit['Licenciatura']['total']):0));
                    $vC = Round((is_array($sit['Bacharelado']['total'])?array_sum($sit['Bacharelado']['total']):0));
                    $vTotal = 0;

                    $arrA[] = $vA;
                    $vTotal += $vA;
                    $arrB[] = $vB;
                    $vTotal += $vB;
                    $arrC[] = $vC;
                    $vTotal += $vC;
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Tecnologo', 'data'=>$arrA),
                    array('name' => 'Licenciatura', 'data'=>$arrB),
                    array('name' => 'Bacharelado', 'data'=>$arrC),
                    array('name' => 'Total por Ano', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasInstitutosGraduacao", "Etapas/Modalidades",300,$formatoValores);
            }
            ?>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(11);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" width="50%">
            <div class="link" onclick="abreIndicadorPopUp(2347);">
                <?php exibirTitulo('indicador', 'Evolução de ingressantes de graduação<br>('.$nomeUniversidade.')'); ?>
            </div>
            <?php
            $arrEtapa = array();
            $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid2 IN (5141, 5133) THEN 'Tecnólogo'
                    WHEN dsh.tidid2 IN (5140, 5134) THEN 'Licenciatura'
                    WHEN dsh.tidid2 IN (5135, 5139) THEN 'Bacharelado'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            INNER JOIN entidade.entidade e ON e.entunicod = dsh.unicod AND e.entstatus = 'A'
            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
            AND sh.indid IN (2347)
            AND dsh.tidid2 IN (5141, 5133, 5140, 5134, 5135, 5139)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND e.entid = {$_REQUEST['entid']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $_SESSION['exportar12'] = $arrDados;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
                }

                $arrX = array();
                $arrA = array();
                $arrB = array();
                $arrC = array();
                $arrTotal = array();

                foreach($arrEtapa as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $vA = Round((is_array($sit['Tecnólogo']['total'])?array_sum($sit['Tecnólogo']['total']):0));
                    $vB = Round((is_array($sit['Licenciatura']['total'])?array_sum($sit['Licenciatura']['total']):0));
                    $vC = Round((is_array($sit['Bacharelado']['total'])?array_sum($sit['Bacharelado']['total']):0));
                    $vTotal = 0;

                    $arrA[] = $vA;
                    $vTotal += $vA;
                    $arrB[] = $vB;
                    $vTotal += $vB;
                    $arrC[] = $vC;
                    $vTotal += $vC;
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Tecnologo', 'data'=>$arrA),
                    array('name' => 'Licenciatura', 'data'=>$arrB),
                    array('name' => 'Bacharelado', 'data'=>$arrC),
                    array('name' => 'Total por Ano', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesInstitutosGraduacao", "Etapas/Modalidades",300,$formatoValores);
            }
            ?>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(12);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" width="50%">
            <div class="link" onclick="abreIndicadorPopUp(2575);">
                <?php exibirTitulo('indicador', 'Evolução de matrículas de pós-graduação<br>('.$nomeUniversidade.')'); ?>
            </div>
            <?php
            $arrEtapa = array();
            $sql = "SELECT
                        dpe.dpeanoref AS periodo,
                        CASE
                            WHEN dsh.tidid1 IN (6985) THEN 'Especialização'
                            WHEN dsh.tidid1 IN (6987) THEN 'Mestrado profissional'
                            WHEN dsh.tidid1 IN (6986) THEN 'Mestrado acadêmico'
                            WHEN dsh.tidid1 IN (6984) THEN 'Doutorado'
                        END AS etapa,
                        sum(dsh.dshqtde) as total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN entidade.entidade e ON e.entunicod = dsh.unicod AND e.entstatus = 'A'
                    INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
                    AND sh.indid IN (2575)
                    AND dsh.tidid1 IN (6985, 6987, 6986, 6984)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref >= '2010'
                    AND e.entid = {$_REQUEST['entid']}
                    GROUP BY periodo, etapa
                    ORDER BY periodo, etapa";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $_SESSION['exportar13'] = $arrDados;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
                }

                $arrX = array();
                $arrA = array();
                $arrB = array();
                $arrC = array();
                $arrD = array();
                $arrTotal = array();

                foreach($arrEtapa as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $vA = Round((is_array($sit['Especialização']['total'])?array_sum($sit['Especialização']['total']):0));
                    $vB = Round((is_array($sit['Mestrado profissional']['total'])?array_sum($sit['Mestrado profissional']['total']):0));
                    $vC = Round((is_array($sit['Mestrado acadêmico']['total'])?array_sum($sit['Mestrado acadêmico']['total']):0));
                    $vD = Round((is_array($sit['Doutorado']['total'])?array_sum($sit['Doutorado']['total']):0));
                    $vTotal = 0;

                    $arrA[] = $vA;
                    $vTotal += $vA;
                    $arrB[] = $vB;
                    $vTotal += $vB;
                    $arrC[] = $vC;
                    $vTotal += $vC;
                    $arrD[] = $vD;
                    $vTotal += $vD;
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Especializacao', 'data'=>$arrA),
                    array('name' => 'Mestrado profissional', 'data'=>$arrB),
                    array('name' => 'Mestrado academico', 'data'=>$arrC),
                    array('name' => 'Doutorado', 'data'=>$arrD),
                    array('name' => 'Total por Ano', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasInstitutosPosGraduacao", "Etapas/Modalidades",300,$formatoValores);
            }
            ?>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(13);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" width="50%">
            <div class="link" onclick="abreIndicadorPopUp(2347);">
                <?php exibirTitulo('indicador', 'Evolução de ingressantes de pós-graduação<br>('.$nomeUniversidade.')'); ?>
            </div>
            <?php
            $arrEtapa = array();
            $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid2 IN (5136) THEN 'Especialização'
                    WHEN dsh.tidid2 IN (5191) THEN 'Mestrado profissional'
                    WHEN dsh.tidid2 IN (5137) THEN 'Mestrado acadêmico'
                    WHEN dsh.tidid2 IN (5138) THEN 'Doutorado'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            INNER JOIN entidade.entidade e ON e.entunicod = dsh.unicod AND e.entstatus = 'A'
            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
            AND sh.indid IN (2347)
            AND dsh.tidid2 IN (5136, 5191, 5137, 5138)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND e.entid = {$_REQUEST['entid']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $_SESSION['exportar14'] = $arrDados;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
                }

                $arrX = array();
                $arrA = array();
                $arrB = array();
                $arrC = array();
                $arrD = array();
                $arrTotal = array();

                foreach($arrEtapa as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $vA = Round((is_array($sit['Especialização']['total'])?array_sum($sit['Especialização']['total']):0));
                    $vB = Round((is_array($sit['Mestrado profissional']['total'])?array_sum($sit['Mestrado profissional']['total']):0));
                    $vC = Round((is_array($sit['Mestrado acadêmico']['total'])?array_sum($sit['Mestrado acadêmico']['total']):0));
                    $vD = Round((is_array($sit['Doutorado']['total'])?array_sum($sit['Doutorado']['total']):0));
                    $vTotal = 0;

                    $arrA[] = $vA;
                    $vTotal += $vA;
                    $arrB[] = $vB;
                    $vTotal += $vB;
                    $arrC[] = $vC;
                    $vTotal += $vC;
                    $arrD[] = $vD;
                    $vTotal += $vD;
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Especializacao', 'data'=>$arrA),
                    array('name' => 'Mestrado profissional', 'data'=>$arrB),
                    array('name' => 'Mestrado academico', 'data'=>$arrC),
                    array('name' => 'Doutorado', 'data'=>$arrD),
                    array('name' => 'Total por Ano', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesInstitutosPosGraduacao", "Etapas/Modalidades",300,$formatoValores);
            }
            ?>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(14);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Ingressantes Etapa / Modalidade', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">E-TEC</span>'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_redefederal9" name="redefederal9" value="1" checked="checked" onclick="" />Todos
                <input type="radio" class="filtrar_redefederal9" name="redefederal9" value="2" onclick="" />Rede Federal
                <input type="radio" class="filtrar_redefederal9" name="redefederal9" value="3" onclick="" />Bolsa-Formação
                <input type="radio" class="filtrar_redefederal9" name="redefederal9" value="4" onclick="" />e-Tec
            </div>
            <br>
            <div id="div_redefederal9">
                <?php gerarGraficoRedeFederal9(1); ?>
            </div>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(9);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div class="link" onclick="abreIndicadorPopUp(2897);">
                <?php exibirTitulo('indicador', 'Investimentos <br>('.$nomeUniversidade.')'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, tid1.tidid AS modalidade, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN entidade.entidade e ON e.entunicod::integer = dsh.unicod::integer
                    WHERE sh.indid in (2897)
                    AND sh.sehstatus <> 'I'
                    AND e.entid = {$_REQUEST['entid']}
                    GROUP BY periodo, modalidade
                    ORDER BY periodo, modalidade";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $_SESSION['exportar15'] = $arrDados;
            $arrX = array();
            $arrA = array();
            $arrB = array();
            $arrC = array();
            $arrSituacao = array();
            $arrTotal = array();
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['periodo']][$dado['modalidade']]['total'][] = $dado['valor'];
                }
                foreach($arrSituacao as $periodo => $sit){
                    $vA = Round((is_array($sit['7838']['total'])?array_sum($sit['7838']['total']):0));
                    $vB = Round((is_array($sit['7839']['total'])?array_sum($sit['7839']['total']):0));
                    $vC = Round((is_array($sit['7840']['total'])?array_sum($sit['7840']['total']):0));
                    $vTotal = 0;
                    $arrX[] = ($periodo=='2014' ? '*2014' : $periodo);
                    $arrA[] = $vA;
                    $vTotal += $vA;
                    $arrB[] = $vB;
                    $vTotal += $vB;
                    $arrC[] = $vC;
                    $vTotal += $vC;
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Pessoal Ativo e Encargos Sociais', 'data'=>$arrA),
                    array('name' => 'Outras Despesas Correntes (Inclui Juros e Encargos da Divida)', 'data'=>$arrB),
                    array('name' => 'Investimentos (Inclui Inversoes Financeiras e Amortizacao da Divida)', 'data'=>$arrC),
                    array('name' => 'Total por Ano', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return 'R$ ' + number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoDespesaUniversidades", "Investimentos",300, $formatoValores);
            }
            ?>
            <br><br>
            <table><tr><td class="bold">*2014: Orçamento Previsto</td></tr></table>
            <table>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(15);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%"  cellspacing="4" cellpadding="1" class="tabela_painel">
	<tr>
        <td width="30%" class="fundo_padrao">
            <?php
            $sql = "SELECT entcampus.entnome AS campus, entcampus.entid
                    FROM entidade.entidade entcampus
                        INNER JOIN entidade.funcaoentidade fn ON entcampus.entid = fn.entid AND fn.funid = 17 AND entcampus.entstatus = 'A' AND fn.fuestatus = 'A'
                        INNER JOIN entidade.funentassoc fue ON fue.fueid = fn.fueid
                    where fue.entid = {$_REQUEST['entid']}
                    ORDER BY campus";
            $arrDados = $db->carregar($sql);
            $count=0;
            $contacolunas=0;
            if($arrDados && is_array($arrDados)){
                $entidPrimeiro = current($arrDados);
                $entidPrimeiro = $entidPrimeiro['entid'];
                $mudaCor = "";
                echo '<table>';
                foreach($arrDados as $dado){
					$sql = "SELECT speid, dinsitlicitacao FROM academico.demandainfra WHERE entidcampus = {$dado['entid']}";
					$arrDemanda = $db->carregar($sql);
					$mudaCor = "";
					if($arrDemanda){
						foreach($arrDemanda as $dem){
							if($dem['speid']!= 1 || $dem['dinsitlicitacao']!= 'L' || $dem['dinsitlicitacao'] == '' ){
								$mudaCor = "<font color='red'>";
                            }
						}
					}
                    $count++;
                    $contacolunas++;
                    echo "<tr>";
                    echo "<td width='25%' class='fundo_padrao' align='left'>";
                    echo "<p class='toggle_consolidado".$count." titulo_box link selecionar-campus' entid='{$dado['entid']}'>".$dado['campus']."&nbsp;";
                    //echo ($mudaCor ? "<img src='/imagens/0_inativo.png' title='Este campus possui problemas nas demandas de infraestrutura'>" : "");
                    echo "</p>";
                    echo "</td>
                </tr>";
                }
                echo '</table>';
            }else{
                $entidPrimeiro = '';
            }
            ?>
        </td>
        <td width="70%">
            <div id="dados_campus">
                <?php if($entidPrimeiro){
                    $entidPrimeiro = $_SESSION['entid_campus'] ? $_SESSION['entid_campus'] : $entidPrimeiro;
                    getDadosCampus($entidPrimeiro);
                } ?>
            </div>
        </td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_td_laranja">
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

<?php
function getDadosCampus($entidCampus)
{
    global $db;
    $_SESSION['entid_campus'] = $entidCampus;

    $sql = "SELECT entnome AS campus FROM entidade.entidade WHERE entid = '$entidCampus'";
    $nomeCampus = $db->pegaUm($sql);
	
	$sql = "select
					entcampus.entnome,
					UPPER(exi.exidsc) as status,
					case cmp.cmpsituacao
						when 'F' then 'FUNCIONANDO'
						when 'N' then 'NÃO FUNCIONANDO'
						else 'Nao Informado'
					end as situacao,
					case cmp.cmpinstalacao
						when 'S' then 'SEM INSTALAÇÕES'
						when 'D' then 'INSTALAÇÕES DEFINITIVAS'
						when 'P' then 'INSTALAÇOES PROVISÓRIAS'
					    else 'Não Informado'
					end as instalacao,
					UPPER(coalesce(nvc.nvcprevisto,'-')) as previsao
			from entidade.entidade entcampus
			inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 17 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
			inner join academico.campus cmp on cmp.entid = entcampus.entid
			inner join academico.existencia exi on exi.exiid = cmp.exiid
			left join academico.novoscampus nvc on nvc.entidcampus = cmp.entid
			where entcampus.entid = '$entidCampus'
			order by status, situacao, instalacao";
	$arrDadosCampus = $db->pegaLinha($sql);
    ?>
    <table class="" cellspacing="4" cellpadding="5" width="100%" >
        <tr>
            <td class="fundo_padrao" colspan="3">
				<h1><?=$nomeCampus ?></h1>
				<h2>
				Status: <?=$arrDadosCampus['status']?><br>
				Situação: <?=$arrDadosCampus['situacao']?><br>
				Instalações: <?=$arrDadosCampus['instalacao']?><br>
				Previsão de Funcionamento: <?=$arrDadosCampus['previsao']?>
				</h2>
			</td>
			<td class="fundo_padrao">
				<div>
					<?php exibirTitulo('obras', 'Obras'); ?>
				</div>
				<div id="dados_scrool">
					<?php if($entidCampus){
						getDadosScrool($entidCampus);
					} ?>			
				</div>
			</td>
        </tr>
        <tr>
            <td class="fundo_padrao" width="25%">
                <div>
                    <?php exibirTitulo('doc', 'Total de matrículas','(<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>)'); ?>
                </div>
                <?php
                $sql = "SELECT
                        dpe.dpeanoref AS ano,
                        CASE
                            WHEN dsh.tidid1 IN (6995, 6994, 6996, 6997, 6998, 6999, 7867, 7869) OR sh.indid IN (2903) THEN 'Técnico'
                            WHEN dsh.tidid1 IN (6984, 6985, 6986, 6987) THEN 'Pós-Graduação'
                            WHEN dsh.tidid1 IN (7000, 7866) THEN 'FIC >= 160h'
                            WHEN dsh.tidid1 IN (9264) THEN 'FIC < 160h'
                            WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
                            WHEN dsh.tidid1 IN (6992, 6988, 6989, 6990, 6991, 6993) THEN 'Superior'
                        END AS etapa,
                        SUM(dsh.dshqtde) as total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid IN (2575, 2905, 2903)
                    AND dpe.dpeanoref >= '2010'
                    AND dsh.entid = '$entidCampus'
                    GROUP BY etapa, ano
                    ORDER BY etapa, ano";
                $arrDados = $db->carregar($sql);
                $total2010 = 0;
                $total2011 = 0;
                $total2012 = 0;
                $total2013 = 0;
                $total2014 = 0;
                $total2015 = 0;
                $count = -1;
                $arrSituacao = array();
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                    <tr>
                        <th class="" >Tipo</th>
                        <th class="" >2010</th>
                        <th class="" >2011</th>
                        <th class="" >2012</th>
                        <th class="" >2013</th>
                        <th class="" >2014</th>
                        <th class="" >2015</th>
                        <th class="" >Total</th>
                    </tr>
                    <?php
                    if($arrDados){
                        foreach($arrDados as $dado){
                            $arrSituacao[$dado['etapa']][$dado['ano']]['total'][] = $dado['total'];
                        }
                        foreach($arrSituacao as $chave => $sit):
                            $count++;
                            $soma2010 = (is_array($sit['2010']['total'])?array_sum($sit['2010']['total']):0);
                            $soma2011 = (is_array($sit['2011']['total'])?array_sum($sit['2011']['total']):0);
                            $soma2012 = (is_array($sit['2012']['total'])?array_sum($sit['2012']['total']):0);
                            $soma2013 = (is_array($sit['2013']['total'])?array_sum($sit['2013']['total']):0);
                            $soma2014 = (is_array($sit['2014']['total'])?array_sum($sit['2014']['total']):0);
                            $soma2015 = (is_array($sit['2015']['total'])?array_sum($sit['2015']['total']):0);
                            ?>
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                <td><?php echo $chave ?></td>
                                <td class="numero" ><?php echo number_format($soma2010,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2011,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2012,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2013,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2014,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2015,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2010+$soma2011+$soma2012+$soma2013+$soma2014+$soma2015,0,",",".") ?></td>
                            </tr>
                            <?php
                            $total2010+=$soma2010;
                            $total2011+=$soma2011;
                            $total2012+=$soma2012;
                            $total2013+=$soma2013;
                            $total2014+=$soma2014;
                            $total2015+=$soma2015;
                        endforeach;
                    }?>
                    <tr>
                        <th class="bold" >Total</th>
                        <th class="numero" ><?php echo number_format($total2010,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2011,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2012,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2013,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2014,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2015,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2010+$total2011+$total2012+$total2013+$total2014+$total2015,0,",",".") ?></th>
                    </tr>
                </table>
            </td>
            <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2575);" width="25%">
                <div>
                    <?php exibirTitulo('doc', 'Total de<br>matrículas<br>regulares'); ?>
                </div>
                <?php
                $sql = "SELECT
                        dpe.dpeanoref AS ano,
                        CASE
                            WHEN dsh.tidid1 IN (6995, 6994, 6996, 6997, 6998, 6999, 7867, 7869) OR sh.indid IN (2903) THEN 'Técnico'
                            WHEN dsh.tidid1 IN (6984, 6985, 6986, 6987) THEN 'Pós-Graduação'
                            WHEN dsh.tidid1 IN (7000, 7866) THEN 'FIC >= 160h Regular'
                            WHEN dsh.tidid1 IN (9264) THEN 'FIC < 160h Regular'
                            WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
                            WHEN dsh.tidid1 IN (6992, 6988, 6989, 6990, 6991, 6993) THEN 'Superior'
                        END AS etapa,
                        SUM(dsh.dshqtde) as total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid IN (2575)
                    AND dsh.entid = '$entidCampus'
                    GROUP BY etapa, ano
                    ORDER BY etapa, ano";
                $arrDados = $db->carregar($sql);
                $total2011 = 0;
                $total2012 = 0;
                $total2013 = 0;
                $total2014 = 0;
                $total2015 = 0;
                $count = -1;
                $arrSituacao = array();
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                    <tr>
                        <th class="" >Tipo</th>
                        <th class="" >2011</th>
                        <th class="" >2012</th>
                        <th class="" >2013</th>
                        <th class="" >2014</th>
                        <th class="" >2015</th>
                        <th class="" >Total</th>
                    </tr>
                    <?php
                    if($arrDados){
                        foreach($arrDados as $dado){
                            $arrSituacao[$dado['etapa']][$dado['ano']]['total'][] = $dado['total'];
                        }
                        foreach($arrSituacao as $chave => $sit):
                            $count++;
                            $soma2011 = (is_array($sit['2011']['total'])?array_sum($sit['2011']['total']):0);
                            $soma2012 = (is_array($sit['2012']['total'])?array_sum($sit['2012']['total']):0);
                            $soma2013 = (is_array($sit['2013']['total'])?array_sum($sit['2013']['total']):0);
                            $soma2014 = (is_array($sit['2014']['total'])?array_sum($sit['2014']['total']):0);
                            $soma2015 = (is_array($sit['2015']['total'])?array_sum($sit['2015']['total']):0);
                            ?>
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                <td><?php echo $chave ?></td>
                                <td class="numero" ><?php echo number_format($soma2011,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2012,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2013,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2014,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2015,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2011+$soma2012+$soma2013+$soma2014+$soma2015,0,",",".") ?></td>
                            </tr>
                            <?php
                            $total2011+=$soma2011;
                            $total2012+=$soma2012;
                            $total2013+=$soma2013;
                            $total2014+=$soma2014;
                            $total2015+=$soma2015;
                        endforeach;
                    }?>
                    <tr>
                        <th class="bold" >Total</th>
                        <th class="numero" ><?php echo number_format($total2011,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2012,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2013,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2014,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2015,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2011+$soma2012+$total2013+$total2014+$total2015,0,",",".") ?></th>
                    </tr>
                </table>
            </td>
            <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2905);" width="25%">
                <div>
                    <?php exibirTitulo('doc', 'Matrículas Bolsa<br>Formação'); ?>
                </div>
                <?php
                $sql = "SELECT
                        dpe.dpeanoref AS ano,
                        CASE
                            WHEN dsh.tidid1 IN (7866) THEN 'FIC'
                            WHEN dsh.tidid1 IN (7867,7869) THEN 'Técnico'
                        ELSE tid1.tiddsc
                        END AS etapa,
                        SUM(dshqtde) AS total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid = 2905
                    AND dsh.entid = '$entidCampus'
                    AND dpe.dpeanoref BETWEEN '2011' AND '2015'
                    GROUP BY ano, etapa";
                $arrDados = $db->carregar($sql);
                $total2011 = 0;
                $total2012 = 0;
                $total2013 = 0;
                $total2014 = 0;
                $total2015 = 0;
                $totalMatriculas = 0;
                $count = -1;
                $arrSituacao = array();
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                    <tr>
                        <th class="" >Tipo</th>
                        <th class="" >2011</th>
                        <th class="" >2012</th>
                        <th class="" >2013</th>
                        <th class="" >2014</th>
                        <th class="" >2015</th>
                        <th class="" >Total</th>
                    </tr>

                    <?php
                    if($arrDados){
                        foreach($arrDados as $dado){
                            $arrSituacao[$dado['etapa']][$dado['ano']]['total'][] = $dado['total'];
                        }
                        foreach($arrSituacao as $chave => $sit):
                            $count++;
                            $soma2011 = (is_array($sit['2011']['total'])?array_sum($sit['2011']['total']):0);
                            $soma2012 = (is_array($sit['2012']['total'])?array_sum($sit['2012']['total']):0);
                            $soma2013 = (is_array($sit['2013']['total'])?array_sum($sit['2013']['total']):0);
                            $soma2014 = (is_array($sit['2014']['total'])?array_sum($sit['2014']['total']):0);
                            $soma2015 = (is_array($sit['2015']['total'])?array_sum($sit['2015']['total']):0);
                            $somaMatriculas = $soma2011 + $soma2012 + $soma2013 + $soma2014 + $soma2015;
                            ?>
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                <td><?php echo $chave ?></td>
                                <td class="numero" ><?php echo number_format($soma2011,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2012,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2013,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2014,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($soma2015,0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($somaMatriculas,0,",",".") ?></td>
                            </tr>
                            <?php
                            $total2011+=$soma2011;
                            $total2012+=$soma2012;
                            $total2013+=$soma2013;
                            $total2014+=$soma2014;
                            $total2015+=$soma2015;
                            $totalMatriculas = $total2011 + $total2012 + $total2013 + $total2014 + $total2015;
                        endforeach;
                    }?>
                    <tr>
                        <th class="bold" >Total</th>
                        <th class="numero" ><?php echo number_format($total2011,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2012,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2013,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2014,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($total2015,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($totalMatriculas,0,",",".") ?></th>
                    </tr>
                </table>
            </td>
            <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2903);" width="25%">
                <div>
                    <?php exibirTitulo('doc', 'Matrículas E-Tec'); ?>
                </div>
                <?php
                $sql = "SELECT dpe.dpeanoref AS ano, SUM(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid = 2903
                    AND dsh.entid = '$entidCampus'
                    AND dpe.dpeanoref >= '2011'
                    GROUP BY ano
                    ORDER BY ano";
                $arrDados = $db->carregar($sql);
                $totalMatriculas = 0;
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                    <tr>
                        <th class="" >Ano</th>
                        <th class="" >Total</th>
                    </tr>
                    <?php
                    if($arrDados){
                        foreach($arrDados as $dado):
                            $count++;
                            ?>
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                <td><?php echo $dado['ano'] ?></td>
                                <td class="numero" ><?php echo number_format($dado['valor'],0,",",".") ?></td>
                            </tr>
                            <?php
                            $totalMatriculas+= $dado['valor'];
                        endforeach;
                    }?>
                    <tr>
                        <th class="bold" >Total</th>
                        <th class="numero" ><?php echo number_format($totalMatriculas,0,",",".") ?></th>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
    <table class="" cellspacing="4" cellpadding="5" width="100%" id="tabela-1">
        <tr>
            <td class="fundo_padrao" width="50%">
                <div>
                    <?php exibirTitulo('indicador', 'Evolução de matrículas atendidas', '<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
                </div>
                <div style="width:100%;text-align:center">
                    <input type="radio" class="filtrar_redefederal5" name="redefederal5" value="1" checked="checked" onclick="" />Todos
                    <input type="radio" class="filtrar_redefederal5" name="redefederal5" value="2" onclick="" />Rede Federal
                    <input type="radio" class="filtrar_redefederal5" name="redefederal5" value="3" onclick="" />Bolsa-Formação
                    <input type="radio" class="filtrar_redefederal5" name="redefederal5" value="4" onclick="" />e-Tec
                </div>
                <br>
                <div id="div_redefederal5">
                    <?php gerarGraficoRedeFederal5(1); ?>
                </div>
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(5);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
            </td>
            <td class="fundo_padrao" width="50%">
                <div>
                    <?php exibirTitulo('indicador', 'Evolução de ingressantes', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
                </div>
                <div style="width:100%;text-align:center" >
                    <input type="radio" class="filtrar_redefederal6" name="redefederal6" value="1" checked="checked" onclick="" />Todos
                    <input type="radio" class="filtrar_redefederal6" name="redefederal6" value="2" onclick="" />Rede Federal
                    <input type="radio" class="filtrar_redefederal6" name="redefederal6" value="3" onclick="" />Bolsa-Formação
                    <input type="radio" class="filtrar_redefederal6" name="redefederal6" value="4" onclick="" />e-Tec
                </div>
                <br>
                <div id="div_redefederal6">
                    <?php gerarGraficoRedeFederal6(1); ?>
                </div>
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(6);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class="fundo_padrao" width="50%">
                <div>
                    <?php exibirTitulo('indicador', 'Evolução de matrículas atendidas - FIC', '<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>'); ?>
                </div>
                <div style="width:100%;text-align:center" >
                    <input type="radio" class="filtrar_redefederal13" name="redefederal13" value="1" checked="checked" onclick="" />Todos
                    <input type="radio" class="filtrar_redefederal13" name="redefederal13" value="2" onclick="" />Rede Federal
                    <input type="radio" class="filtrar_redefederal13" name="redefederal13" value="3" onclick="" />Bolsa-Formação
                </div>
                <br>
                <div id="div_redefederal13">
                    <?php gerarGraficoRedeFederal13(1); ?>
                </div>
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(22);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
            </td>
            <td class="fundo_padrao" width="50%">
                <div>
                    <?php exibirTitulo('indicador', 'Evolução de ingressantes - FIC', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>'); ?>
                </div>
                <div style="width:100%;text-align:center" >
                    <input type="radio" class="filtrar_redefederal14" name="redefederal14" value="1" checked="checked" onclick="" />Todos
                    <input type="radio" class="filtrar_redefederal14" name="redefederal14" value="2" onclick="" />Rede Federal
                    <input type="radio" class="filtrar_redefederal14" name="redefederal14" value="3" onclick="" />Bolsa-Formação
                </div>
                <br>
                <div id="div_redefederal14">
                    <?php gerarGraficoRedeFederal14(1); ?>
                </div>
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(23);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class="fundo_padrao" width="50%">
                <div>
                    <?php exibirTitulo('indicador', 'Evolução de matrículas atendidas - Técnico', '<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
                </div>
                <div style="width:100%;text-align:center" >
                    <input type="radio" class="filtrar_redefederal7" name="redefederal7" value="1" checked="checked" onclick="" />Todos
                    <input type="radio" class="filtrar_redefederal7" name="redefederal7" value="2" onclick="" />Rede Federal
                    <input type="radio" class="filtrar_redefederal7" name="redefederal7" value="3" onclick="" />Bolsa-Formação
                    <input type="radio" class="filtrar_redefederal7" name="redefederal7" value="4" onclick="" />e-Tec
                </div>
                <br>
                <div id="div_redefederal7">
                    <?php gerarGraficoRedeFederal7(1); ?>
                </div>
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(7);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
            </td>
            <td class="fundo_padrao" width="50%">
                <div>
                    <?php exibirTitulo('indicador', 'Evolução de ingressantes - Técnico', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
                </div>
                <div style="width:100%;text-align:center" >
                    <input type="radio" class="filtrar_redefederal8" name="redefederal8" value="1" checked="checked" onclick="" />Todos
                    <input type="radio" class="filtrar_redefederal8" name="redefederal8" value="2" onclick="" />Rede Federal
                    <input type="radio" class="filtrar_redefederal8" name="redefederal8" value="3" onclick="" />Bolsa-Formação
                    <input type="radio" class="filtrar_redefederal8" name="redefederal8" value="4" onclick="" />e-Tec
                </div>
                <br>
                <div id="div_redefederal8">
                    <?php gerarGraficoRedeFederal8(1); ?>
                </div>
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(8);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class="fundo_padrao" width="50%">
                <div class="link" onclick="abreIndicadorPopUp(2575);">
                    <?php exibirTitulo('indicador', 'Evolução de matrículas de graduação'); ?>
                </div>
                <?php
                $arrEtapa = array();
                $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (6990, 6993) THEN 'Tecnólogo'
                    WHEN dsh.tidid1 IN (6992, 6989) THEN 'Licenciatura'
                    WHEN dsh.tidid1 IN (6988, 6991) THEN 'Bacharelado'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            AND sh.indid IN (2575)
            AND dsh.tidid1 IN (6990, 6993, 6992, 6989, 6988, 6991)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND dsh.entid = {$_SESSION['entid_campus']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
                $arrDados = $db->carregar( $sql, null, 3200 );
                $_SESSION['exportar16'] = $arrDados;
                if($arrDados){
                    foreach($arrDados as $dado){
                        $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
                    }

                    $arrX = array();
                    $arrA = array();
                    $arrB = array();
                    $arrC = array();
                    $arrTotal = array();

                    foreach($arrEtapa as $periodo => $sit)
                    {
                        $arrX[] = $periodo;
                        $vA = Round((is_array($sit['Tecnólogo']['total'])?array_sum($sit['Tecnólogo']['total']):0));
                        $vB = Round((is_array($sit['Licenciatura']['total'])?array_sum($sit['Licenciatura']['total']):0));
                        $vC = Round((is_array($sit['Bacharelado']['total'])?array_sum($sit['Bacharelado']['total']):0));
                        $vTotal = 0;

                        $arrA[] = $vA;
                        $vTotal += $vA;
                        $arrB[] = $vB;
                        $vTotal += $vB;
                        $arrC[] = $vC;
                        $vTotal += $vC;
                        $arrTotal[] = $vTotal;
                    }

                    $aDados = array(
                        array('name' => 'Tecnologo', 'data'=>$arrA),
                        array('name' => 'Licenciatura', 'data'=>$arrB),
                        array('name' => 'Bacharelado', 'data'=>$arrC),
                        array('name' => 'Total por Ano', 'data'=>$arrTotal),
                    );

                    $formatoValores = array(
                        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                        'alinhamento' => "formatter: function() { return '1'; }"
                    );

                    echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasCampusGraduacao", "Etapas/Modalidades",300,$formatoValores);
                }
                ?>
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(16);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
            </td>
            <td class="fundo_padrao" width="50%">
                <div class="link" onclick="abreIndicadorPopUp(2347);">
                    <?php exibirTitulo('indicador', 'Evolução de ingressantes de graduação'); ?>
                </div>
                <?php
                $arrEtapa = array();
                $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid2 IN (5141, 5133) THEN 'Tecnólogo'
                    WHEN dsh.tidid2 IN (5140, 5134) THEN 'Licenciatura'
                    WHEN dsh.tidid2 IN (5135, 5139) THEN 'Bacharelado'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            AND sh.indid IN (2347)
            AND dsh.tidid2 IN (5141, 5133, 5140, 5134, 5135, 5139)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND dsh.entid = {$_SESSION['entid_campus']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
                $arrDados = $db->carregar( $sql, null, 3200 );
                $_SESSION['exportar17'] = $arrDados;
                if($arrDados){
                    foreach($arrDados as $dado){
                        $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
                    }

                    $arrX = array();
                    $arrA = array();
                    $arrB = array();
                    $arrC = array();
                    $arrTotal = array();

                    foreach($arrEtapa as $periodo => $sit)
                    {
                        $arrX[] = $periodo;
                        $vA = Round((is_array($sit['Tecnólogo']['total'])?array_sum($sit['Tecnólogo']['total']):0));
                        $vB = Round((is_array($sit['Licenciatura']['total'])?array_sum($sit['Licenciatura']['total']):0));
                        $vC = Round((is_array($sit['Bacharelado']['total'])?array_sum($sit['Bacharelado']['total']):0));
                        $vTotal = 0;

                        $arrA[] = $vA;
                        $vTotal += $vA;
                        $arrB[] = $vB;
                        $vTotal += $vB;
                        $arrC[] = $vC;
                        $vTotal += $vC;
                        $arrTotal[] = $vTotal;
                    }

                    $aDados = array(
                        array('name' => 'Tecnologo', 'data'=>$arrA),
                        array('name' => 'Licenciatura', 'data'=>$arrB),
                        array('name' => 'Bacharelado', 'data'=>$arrC),
                        array('name' => 'Total por Ano', 'data'=>$arrTotal),
                    );

                    $formatoValores = array(
                        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                        'alinhamento' => "formatter: function() { return '1'; }"
                    );

                    echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesCampusGraduacao", "Etapas/Modalidades",300,$formatoValores);
                }
                ?>
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(17);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class="fundo_padrao" width="50%">
                <div class="link" onclick="abreIndicadorPopUp(2575);">
                    <?php exibirTitulo('indicador', 'Evolução de matrículas de pós-graduação'); ?>
                </div>
                <?php
                $arrEtapa = array();
                $sql = "SELECT
                        dpe.dpeanoref AS periodo,
                        CASE
                            WHEN dsh.tidid1 IN (6985) THEN 'Especialização'
                            WHEN dsh.tidid1 IN (6987) THEN 'Mestrado profissional'
                            WHEN dsh.tidid1 IN (6986) THEN 'Mestrado acadêmico'
                            WHEN dsh.tidid1 IN (6984) THEN 'Doutorado'
                        END AS etapa,
                        sum(dsh.dshqtde) as total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    AND sh.indid IN (2575)
                    AND dsh.tidid1 IN (6985, 6987, 6986, 6984)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref >= '2010'
                    AND dsh.entid = {$_SESSION['entid_campus']}
                    GROUP BY periodo, etapa
                    ORDER BY periodo, etapa";
                $arrDados = $db->carregar( $sql, null, 3200 );
                $_SESSION['exportar18'] = $arrDados;
                if($arrDados){
                    foreach($arrDados as $dado){
                        $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
                    }

                    $arrX = array();
                    $arrA = array();
                    $arrB = array();
                    $arrC = array();
                    $arrD = array();
                    $arrTotal = array();

                    foreach($arrEtapa as $periodo => $sit)
                    {
                        $arrX[] = $periodo;
                        $vA = Round((is_array($sit['Especialização']['total'])?array_sum($sit['Especialização']['total']):0));
                        $vB = Round((is_array($sit['Mestrado profissional']['total'])?array_sum($sit['Mestrado profissional']['total']):0));
                        $vC = Round((is_array($sit['Mestrado acadêmico']['total'])?array_sum($sit['Mestrado acadêmico']['total']):0));
                        $vD = Round((is_array($sit['Doutorado']['total'])?array_sum($sit['Doutorado']['total']):0));
                        $vTotal = 0;

                        $arrA[] = $vA;
                        $vTotal += $vA;
                        $arrB[] = $vB;
                        $vTotal += $vB;
                        $arrC[] = $vC;
                        $vTotal += $vC;
                        $arrD[] = $vD;
                        $vTotal += $vD;
                        $arrTotal[] = $vTotal;
                    }

                    $aDados = array(
                        array('name' => 'Especializacao', 'data'=>$arrA),
                        array('name' => 'Mestrado profissional', 'data'=>$arrB),
                        array('name' => 'Mestrado academico', 'data'=>$arrC),
                        array('name' => 'Doutorado', 'data'=>$arrD),
                        array('name' => 'Total por Ano', 'data'=>$arrTotal),
                    );

                    $formatoValores = array(
                        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                        'alinhamento' => "formatter: function() { return '1'; }"
                    );

                    echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasCampusPosGraduacao", "Etapas/Modalidades",300,$formatoValores);
                }
                ?>
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(18);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
            </td>
            <td class="fundo_padrao" width="50%">
                <div class="link" onclick="abreIndicadorPopUp(2347);">
                    <?php exibirTitulo('indicador', 'Evolução de ingressantes de pós-graduação'); ?>
                </div>
                <?php
                $arrEtapa = array();
                $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid2 IN (5136) THEN 'Especialização'
                    WHEN dsh.tidid2 IN (5191) THEN 'Mestrado profissional'
                    WHEN dsh.tidid2 IN (5137) THEN 'Mestrado acadêmico'
                    WHEN dsh.tidid2 IN (5138) THEN 'Doutorado'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            AND sh.indid IN (2347)
            AND dsh.tidid2 IN (5136, 5191, 5137, 5138)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND dsh.entid = {$_SESSION['entid_campus']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
                $arrDados = $db->carregar( $sql, null, 3200 );
                $_SESSION['exportar19'] = $arrDados;
                if($arrDados){
                    foreach($arrDados as $dado){
                        $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
                    }

                    $arrX = array();
                    $arrA = array();
                    $arrB = array();
                    $arrC = array();
                    $arrD = array();
                    $arrTotal = array();

                    foreach($arrEtapa as $periodo => $sit)
                    {
                        $arrX[] = $periodo;
                        $vA = Round((is_array($sit['Especialização']['total'])?array_sum($sit['Especialização']['total']):0));
                        $vB = Round((is_array($sit['Mestrado profissional']['total'])?array_sum($sit['Mestrado profissional']['total']):0));
                        $vC = Round((is_array($sit['Mestrado acadêmico']['total'])?array_sum($sit['Mestrado acadêmico']['total']):0));
                        $vD = Round((is_array($sit['Doutorado']['total'])?array_sum($sit['Doutorado']['total']):0));
                        $vTotal = 0;

                        $arrA[] = $vA;
                        $vTotal += $vA;
                        $arrB[] = $vB;
                        $vTotal += $vB;
                        $arrC[] = $vC;
                        $vTotal += $vC;
                        $arrD[] = $vD;
                        $vTotal += $vD;
                        $arrTotal[] = $vTotal;
                    }

                    $aDados = array(
                        array('name' => 'Especializacao', 'data'=>$arrA),
                        array('name' => 'Mestrado profissional', 'data'=>$arrB),
                        array('name' => 'Mestrado academico', 'data'=>$arrC),
                        array('name' => 'Doutorado', 'data'=>$arrD),
                        array('name' => 'Total por Ano', 'data'=>$arrTotal),
                    );

                    $formatoValores = array(
                        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                        'alinhamento' => "formatter: function() { return '1'; }"
                    );

                    echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesCampusPosGraduacao", "Etapas/Modalidades",300,$formatoValores);
                }
                ?>
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(19);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class="fundo_padrao" colspan="2">
                <div>
                    <?php exibirTitulo('indicador', 'Ingressantes Etapa / Modalidade', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">E-TEC</span>'); ?>
                </div>
                <div style="width:100%;text-align:center" >
                    <input type="radio" class="filtrar_redefederal10" name="redefederal10" value="1" checked="checked" onclick="" />Todos
                    <input type="radio" class="filtrar_redefederal10" name="redefederal10" value="2" onclick="" />Rede Federal
                    <input type="radio" class="filtrar_redefederal10" name="redefederal10" value="3" onclick="" />Bolsa-Formação
                    <input type="radio" class="filtrar_redefederal10" name="redefederal10" value="4" onclick="" />e-Tec
                </div>
                <br>
                <div id="div_redefederal10">
                    <?php gerarGraficoRedeFederal10(1); ?>
                </div>
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(10);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
<?php
}

function getDadosScrool($entidCampus)
{
	global $db;
	$arrFotos = array();
	
	$sql = "SELECT
				obr.obrid, obr.obrdesc,
				mun.mundescricao AS municipio,
				mun.estuf AS uf,
				sto.stodesc,
				obr.obrpercexec,
				arqid,
                datafim,
                obr.stoid
			FROM obras.obrainfraestrutura obr
			INNER JOIN entidade.entidade ent ON ent.entid = obr.entidunidade
			LEFT JOIN obras.tipoobra tob ON tob.tobaid = obr.tobraid
			INNER JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
			INNER JOIN entidade.endereco edc ON edc.endid = obr.endid
			INNER JOIN territorios.municipio mun ON mun.muncod = edc.muncod
			INNER JOIN
				(select MAX(arqid) AS arqid, obrid FROM obras.fotos GROUP BY obrid) AS fot ON fot.obrid = obr.obrid
		    LEFT JOIN
                (select to_char(MAX(supvdt), 'DD/MM/YYYY') AS datafim, obrid from obras.supervisao WHERE stoid = 3 AND supstatus = 'A' GROUP BY OBRID) d ON d.obrid = obr.obrid
			WHERE obr.obsstatus = 'A'
			AND obr.entidcampus = '$entidCampus'
			ORDER BY random()
			LIMIT 50";
	$arrFotos = $db->carregar($sql);
	if($arrFotos){
	?>
		<div style="clear:both" id="div_fotos_padrao"  >
			<?php for($x=5;$x>=1;$x--): ?>
				<div style="clear:both" class="div_fotos_interno"  >
					<?php for($i=$x;$i<=(5+$x);$i++): ?>
						<?php if($arrFotos[$i]['obrid']){ ?>
							<div class="div_fotos_padrao" >
								<table border="0" >
									<tr onclick="abreObras1(<?=$arrFotos[$i]['obrid']?>);">
										<td>
											<img width="80" height="60" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=obras" />
										</td>
										<td style="color:#FFFFFF" >
											<?php echo substr($arrFotos[$i]['obrdesc'],0,60) ?>...<br/>
											<?php echo $arrFotos[$i]['municipio'] ?> / <?php echo $arrFotos[$i]['uf'] ?><br/>
                                            <?php echo $arrFotos[$i]['stodesc'] . ( $arrFotos[$i]['stoid']==3 ? ' em '. $arrFotos[$i]['datafim'] : '')  ?> <br/>
											<?php echo str_replace(array(".00","."),array("",","),$arrFotos[$i]['obrpercexec']) ?>% de execução
										</td>
									</tr>
								</table>
							</div>
						<?php } ?>
					<?php endfor; ?>
				</div>
			<?php endfor; ?>
		</div>
		<script>
			jQuery('.div_fotos_interno').cycle({ 
				fx: 'scrollDown'
			});
		</script>
	<?php
	}
}

function gerarGraficoRedeFederal1($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2575)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905)";
            break;
        case 4:
            $where = "WHERE sh.indid in (2903)";
            break;
        default:
            $where = "WHERE sh.indid in (2575, 2903, 2905)";
            break;
    }

    $arrEtapa = array();
    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (6994,6995,6996,6997,6998,6999, 5303, 5304, 7867,7869) OR sh.indid in (2903) THEN 'Técnico'
                    WHEN dsh.tidid1 IN (6987,6986,6985,6984) THEN 'Pós-Graduação'
                    WHEN dsh.tidid1 IN (7000, 9264, 5302, 7866) THEN 'FIC'
                    WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
                    WHEN dsh.tidid1 IN (6988,6989,6990,6991,6992,6993) THEN 'Superior'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            INNER JOIN entidade.entidade e ON e.entunicod = dsh.unicod AND e.entstatus = 'A'
            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND e.entid = {$_REQUEST['entid']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar1'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrC = array();
        $arrD = array();
        $arrE = array();
        $arrTotal = array();

        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['Técnico']['total'])?array_sum($sit['Técnico']['total']):0));
            $vB = Round((is_array($sit['Pós-Graduação']['total'])?array_sum($sit['Pós-Graduação']['total']):0));
            $vC = Round((is_array($sit['FIC']['total'])?array_sum($sit['FIC']['total']):0));
            $vD = Round((is_array($sit['Ensino Médio']['total'])?array_sum($sit['Ensino Médio']['total']):0));
            $vE = Round((is_array($sit['Superior']['total'])?array_sum($sit['Superior']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrC[] = $vC;
            $vTotal += $vC;
            $arrD[] = $vD;
            $vTotal += $vD;
            $arrE[] = $vE;
            $vTotal += $vE;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Tecnico', 'data'=>$arrA),
            array('name' => 'Pos-Graduacao', 'data'=>$arrB),
            array('name' => 'FIC', 'data'=>$arrC),
            array('name' => 'Ensino Medio', 'data'=>$arrD),
            array('name' => 'Superior', 'data'=>$arrE),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasInstitutos", "Etapas/Modalidades",300, $formatoValores);
    }
}
function gerarGraficoRedeFederal2($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2347)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905)";
            break;
        case 4:
            $where = "WHERE sh.indid in (2903)";
            break;
        default:
            $where = "WHERE sh.indid in (2347, 2903, 2905)";
            break;
    }

    $sql = "SELECT dpe.dpeanoref as periodo,
            CASE
                    WHEN dsh.tidid2 IN (5130, 5131, 5132, 5188, 5190, 5189) OR dsh.tidid1 IN (5303, 5304, 7867,7869) OR sh.indid in (2903) THEN 'Técnico'
                    WHEN dsh.tidid2 IN (5136, 5137, 5138, 5191) THEN 'Pós-Graduação'
                    WHEN dsh.tidid2 IN (5129, 9263) OR dsh.tidid1 IN (5302, 7866) THEN 'FIC'
                    WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
                    WHEN dsh.tidid2 IN (5135, 5140, 5141, 5133, 5134, 5139) THEN 'Superior'
            END AS etapa,
            SUM(dsh.dshqtde)::integer AS total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            INNER JOIN entidade.entidade e ON e.entunicod = dsh.unicod AND e.entstatus = 'A'
            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND e.entid = {$_REQUEST['entid']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar2'] = $arrDados;
    $arrEtapa = array();
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrC = array();
        $arrD = array();
        $arrE = array();
        $arrTotal = array();


        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['Técnico']['total'])?array_sum($sit['Técnico']['total']):0));
            $vB = Round((is_array($sit['Pós-Graduação']['total'])?array_sum($sit['Pós-Graduação']['total']):0));
            $vC = Round((is_array($sit['FIC']['total'])?array_sum($sit['FIC']['total']):0));
            $vD = Round((is_array($sit['Ensino Médio']['total'])?array_sum($sit['Ensino Médio']['total']):0));
            $vE = Round((is_array($sit['Superior']['total'])?array_sum($sit['Superior']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrC[] = $vC;
            $vTotal += $vC;
            $arrD[] = $vD;
            $vTotal += $vD;
            $arrE[] = $vE;
            $vTotal += $vE;
            $arrTotal[] = $vTotal;

            //$vTotalA += $vA;
            //$vTotalB += $vB;
            //$vTotalC += $vC;
            //$vTotalD += $vD;
            //$vTotalE += $vE;
            //$vTotal += $vA + $vB + $vC + $vD + $vE;
            //$arrTotal[] = $vTotal;
            //$arrA[] = $vTotalA;
            //$arrB[] = $vTotalB;
            //$arrC[] = $vTotalC;
            //$arrD[] = $vTotalD;
            //$arrE[] = $vTotalE;
        }

        $aDados = array(
            array('name' => 'Tecnico', 'data'=>$arrA),
            array('name' => 'Pos-Graduacao', 'data'=>$arrB),
            array('name' => 'FIC', 'data'=>$arrC),
            array('name' => 'Ensino Medio', 'data'=>$arrD),
            array('name' => 'Superior', 'data'=>$arrE),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantes", "Ingressantes",300,$formatoValores);
    }

}
function gerarGraficoRedeFederal3($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE (sh.indid in (2575) AND dsh.tidid1 IN (6994, 6995, 6996, 6997, 6999, 6998))";
            break;
        case 3:
            $where = "WHERE (sh.indid in (2905) AND dsh.tidid1 IN (7867, 7869))";
            break;
        case 4:
            $where = "WHERE (sh.indid in (2903))";
            break;
        default:
            $where = "WHERE (
                        (sh.indid in (2575) AND dsh.tidid1 IN (6994, 6995, 6996, 6997, 6999, 6998))
                        OR
                        (sh.indid in (2905) AND dsh.tidid1 IN (7867, 7869))
                        OR
                        (sh.indid in (2903))
                      )";
            break;
    }

    $arrEtapa = array();
    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (6994, 7867) THEN 'Técnico concomitante'
                    WHEN dsh.tidid1 IN (6995) THEN 'Técnico integrado'
                    WHEN dsh.tidid1 IN (6996) THEN 'Técnico PROEJA concomitante'
                    WHEN dsh.tidid1 IN (6997) THEN 'Técnico PROEJA integrado'
                    WHEN dsh.tidid1 IN (6999, 7869) THEN 'Técnico subsequente presencial'
                    WHEN dsh.tidid1 IN (6998) OR sh.indid in (2903) THEN 'Técnico subsequente a distância'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            INNER JOIN entidade.entidade e ON e.entunicod = dsh.unicod AND e.entstatus = 'A'
            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND e.entid = {$_REQUEST['entid']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar3'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrC = array();
        $arrD = array();
        $arrE = array();
        $arrF = array();
        $arrTotal = array();

        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['Técnico concomitante']['total'])?array_sum($sit['Técnico concomitante']['total']):0));
            $vB = Round((is_array($sit['Técnico integrado']['total'])?array_sum($sit['Técnico integrado']['total']):0));
            $vC = Round((is_array($sit['Técnico PROEJA concomitante']['total'])?array_sum($sit['Técnico PROEJA concomitante']['total']):0));
            $vD = Round((is_array($sit['Técnico PROEJA integrado']['total'])?array_sum($sit['Técnico PROEJA integrado']['total']):0));
            $vE = Round((is_array($sit['Técnico subsequente presencial']['total'])?array_sum($sit['Técnico subsequente presencial']['total']):0));
            $vF = Round((is_array($sit['Técnico subsequente a distância']['total'])?array_sum($sit['Técnico subsequente a distância']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrC[] = $vC;
            $vTotal += $vC;
            $arrD[] = $vD;
            $vTotal += $vD;
            $arrE[] = $vE;
            $vTotal += $vE;
            $arrF[] = $vF;
            $vTotal += $vF;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Tecnico concomitante', 'data'=>$arrA),
            array('name' => 'Tecnico integrado', 'data'=>$arrB),
            array('name' => 'Tecnico PROEJA concomitante', 'data'=>$arrC),
            array('name' => 'Tecnico PROEJA integrado', 'data'=>$arrD),
            array('name' => 'Tecnico subsequente presencial', 'data'=>$arrE),
            array('name' => 'Tecnico subsequente a distancia', 'data'=>$arrF),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasInstitutosTecnicos", "Etapas/Modalidades",300,$formatoValores);
    }

}
function gerarGraficoRedeFederal4($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE (sh.indid in (2347) AND dsh.tidid2 IN (5131, 5130, 5189, 5188, 5190, 5132))";
            break;
        case 3:
            $where = "WHERE (sh.indid in (2905) AND dsh.tidid1 IN (7867, 7869))";
            break;
        case 4:
            $where = "WHERE (sh.indid in (2903))";
            break;
        default:
            $where = "WHERE (
                        (sh.indid in (2347) AND dsh.tidid2 IN (5131, 5130, 5189, 5188, 5190, 5132))
                        OR
                        (sh.indid in (2905) AND dsh.tidid1 IN (7867, 7869))
                        OR
                        (sh.indid in (2903))
                      )";
            break;
    }

    $arrEtapa = array();
    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid2 IN (5131) OR dsh.tidid1 IN (7867) THEN 'Técnico concomitante'
                    WHEN dsh.tidid2 IN (5130) THEN 'Técnico integrado'
                    WHEN dsh.tidid2 IN (5189) THEN 'Técnico PROEJA concomitante'
                    WHEN dsh.tidid2 IN (5188) THEN 'Técnico PROEJA integrado'
                    WHEN dsh.tidid2 IN (5190) OR dsh.tidid1 IN (7869) THEN 'Técnico subsequente presencial'
                    WHEN dsh.tidid2 IN (5132) OR sh.indid in (2903) THEN 'Técnico subsequente a distância'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            INNER JOIN entidade.entidade e ON e.entunicod = dsh.unicod AND e.entstatus = 'A'
            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND e.entid = {$_REQUEST['entid']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar4'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrC = array();
        $arrD = array();
        $arrE = array();
        $arrF = array();
        $arrTotal = array();

        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['Técnico concomitante']['total'])?array_sum($sit['Técnico concomitante']['total']):0));
            $vB = Round((is_array($sit['Técnico integrado']['total'])?array_sum($sit['Técnico integrado']['total']):0));
            $vC = Round((is_array($sit['Técnico PROEJA concomitante']['total'])?array_sum($sit['Técnico PROEJA concomitante']['total']):0));
            $vD = Round((is_array($sit['Técnico PROEJA integrado']['total'])?array_sum($sit['Técnico PROEJA integrado']['total']):0));
            $vE = Round((is_array($sit['Técnico subsequente presencial']['total'])?array_sum($sit['Técnico subsequente presencial']['total']):0));
            $vF = Round((is_array($sit['Técnico subsequente a distância']['total'])?array_sum($sit['Técnico subsequente a distância']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrC[] = $vC;
            $vTotal += $vC;
            $arrD[] = $vD;
            $vTotal += $vD;
            $arrE[] = $vE;
            $vTotal += $vE;
            $arrF[] = $vF;
            $vTotal += $vF;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Tecnico concomitante', 'data'=>$arrA),
            array('name' => 'Tecnico integrado', 'data'=>$arrB),
            array('name' => 'Tecnico PROEJA concomitante', 'data'=>$arrC),
            array('name' => 'Tecnico PROEJA integrado', 'data'=>$arrD),
            array('name' => 'Tecnico subsequente presencial', 'data'=>$arrE),
            array('name' => 'Tecnico subsequente a distancia', 'data'=>$arrF),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesTecnicos", "Etapas/Modalidades",300,$formatoValores);
    }
}

function gerarGraficoRedeFederal5($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2575)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905)";
            break;
        case 4:
            $where = "WHERE sh.indid in (2903)";
            break;
        default:
            $where = "WHERE sh.indid in (2575, 2903, 2905)";
            break;
    }

    $arrEtapa = array();
    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (6994,6995,6996,6997,6998,6999, 5303, 5304, 7867,7869) OR sh.indid in (2903) THEN 'Técnico'
                    WHEN dsh.tidid1 IN (6987,6986,6985,6984) THEN 'Pós-Graduação'
                    WHEN dsh.tidid1 IN (7000, 9264, 5302, 7866) THEN 'FIC'
                    WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
                    WHEN dsh.tidid1 IN (6988,6989,6990,6991,6992,6993) THEN 'Superior'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND dsh.entid = {$_SESSION['entid_campus']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql );
    $_SESSION['exportar5'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrC = array();
        $arrD = array();
        $arrE = array();
        $arrTotal = array();

        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['Técnico']['total'])?array_sum($sit['Técnico']['total']):0));
            $vB = Round((is_array($sit['Pós-Graduação']['total'])?array_sum($sit['Pós-Graduação']['total']):0));
            $vC = Round((is_array($sit['FIC']['total'])?array_sum($sit['FIC']['total']):0));
            $vD = Round((is_array($sit['Ensino Médio']['total'])?array_sum($sit['Ensino Médio']['total']):0));
            $vE = Round((is_array($sit['Superior']['total'])?array_sum($sit['Superior']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrC[] = $vC;
            $vTotal += $vC;
            $arrD[] = $vD;
            $vTotal += $vD;
            $arrE[] = $vE;
            $vTotal += $vE;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Tecnico', 'data'=>$arrA),
            array('name' => 'Pos-Graduacao', 'data'=>$arrB),
            array('name' => 'FIC', 'data'=>$arrC),
            array('name' => 'Ensino Medio', 'data'=>$arrD),
            array('name' => 'Superior', 'data'=>$arrE),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasCampus", "Etapas/Modalidades",300,$formatoValores);
    }
}
function gerarGraficoRedeFederal6($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2347)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905)";
            break;
        case 4:
            $where = "WHERE sh.indid in (2903)";
            break;
        default:
            $where = "WHERE sh.indid in (2347, 2903, 2905)";
            break;
    }

    $sql = "SELECT
                dpe.dpeanoref as periodo,
                CASE
                        WHEN dsh.tidid2 IN (5130, 5131, 5132, 5188, 5190, 5189) OR dsh.tidid1 IN (5303, 5304, 7867,7869) OR sh.indid in (2903) THEN 'Técnico'
                        WHEN dsh.tidid2 IN (5136, 5137, 5138, 5191) THEN 'Pós-Graduação'
                        WHEN dsh.tidid2 IN (5129, 9263) OR dsh.tidid1 IN (5302, 7866) THEN 'FIC'
                        WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
                        WHEN dsh.tidid2 IN (5135, 5140, 5141, 5133, 5134, 5139) THEN 'Superior'
                END AS etapa,
                SUM(dsh.dshqtde)::integer AS total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND dsh.entid = {$_SESSION['entid_campus']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql );
    $_SESSION['exportar6'] = $arrDados;
    $arrEtapa = array();
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrC = array();
        $arrD = array();
        $arrE = array();
        $arrTotal = array();


        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['Técnico']['total'])?array_sum($sit['Técnico']['total']):0));
            $vB = Round((is_array($sit['Pós-Graduação']['total'])?array_sum($sit['Pós-Graduação']['total']):0));
            $vC = Round((is_array($sit['FIC']['total'])?array_sum($sit['FIC']['total']):0));
            $vD = Round((is_array($sit['Ensino Médio']['total'])?array_sum($sit['Ensino Médio']['total']):0));
            $vE = Round((is_array($sit['Superior']['total'])?array_sum($sit['Superior']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrC[] = $vC;
            $vTotal += $vC;
            $arrD[] = $vD;
            $vTotal += $vD;
            $arrE[] = $vE;
            $vTotal += $vE;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Tecnico', 'data'=>$arrA),
            array('name' => 'Pos-Graduacao', 'data'=>$arrB),
            array('name' => 'FIC', 'data'=>$arrC),
            array('name' => 'Ensino Medio', 'data'=>$arrD),
            array('name' => 'Superior', 'data'=>$arrE),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesCampus", "Ingressantes",300,$formatoValores);
    }
}
function gerarGraficoRedeFederal7($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE (sh.indid in (2575) AND dsh.tidid1 IN (6994, 6995, 6996, 6997, 6999, 6998))";
            break;
        case 3:
            $where = "WHERE (sh.indid in (2905) AND dsh.tidid1 IN (7867, 7869))";
            break;
        case 4:
            $where = "WHERE (sh.indid in (2903))";
            break;
        default:
            $where = "WHERE (
                        (sh.indid in (2575) AND dsh.tidid1 IN (6994, 6995, 6996, 6997, 6999, 6998))
                        OR
                        (sh.indid in (2905) AND dsh.tidid1 IN (7867, 7869))
                        OR
                        (sh.indid in (2903))
                      )";
            break;
    }

    $arrEtapa = array();
    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (6994, 7867) THEN 'Técnico concomitante'
                    WHEN dsh.tidid1 IN (6995) THEN 'Técnico integrado'
                    WHEN dsh.tidid1 IN (6996) THEN 'Técnico PROEJA concomitante'
                    WHEN dsh.tidid1 IN (6997) THEN 'Técnico PROEJA integrado'
                    WHEN dsh.tidid1 IN (6999, 7869) THEN 'Técnico subsequente presencial'
                    WHEN dsh.tidid1 IN (6998) OR sh.indid in (2903) THEN 'Técnico subsequente a distância'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND dsh.entid = {$_SESSION['entid_campus']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql );
    $_SESSION['exportar7'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrC = array();
        $arrD = array();
        $arrE = array();
        $arrF = array();
        $arrTotal = array();

        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['Técnico concomitante']['total'])?array_sum($sit['Técnico concomitante']['total']):0));
            $vB = Round((is_array($sit['Técnico integrado']['total'])?array_sum($sit['Técnico integrado']['total']):0));
            $vC = Round((is_array($sit['Técnico PROEJA concomitante']['total'])?array_sum($sit['Técnico PROEJA concomitante']['total']):0));
            $vD = Round((is_array($sit['Técnico PROEJA integrado']['total'])?array_sum($sit['Técnico PROEJA integrado']['total']):0));
            $vE = Round((is_array($sit['Técnico subsequente presencial']['total'])?array_sum($sit['Técnico subsequente presencial']['total']):0));
            $vF = Round((is_array($sit['Técnico subsequente a distância']['total'])?array_sum($sit['Técnico subsequente a distância']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrC[] = $vC;
            $vTotal += $vC;
            $arrD[] = $vD;
            $vTotal += $vD;
            $arrE[] = $vE;
            $vTotal += $vE;
            $arrF[] = $vF;
            $vTotal += $vF;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Tecnico concomitante', 'data'=>$arrA),
            array('name' => 'Tecnico integrado', 'data'=>$arrB),
            array('name' => 'Tecnico PROEJA concomitante', 'data'=>$arrC),
            array('name' => 'Tecnico PROEJA integrado', 'data'=>$arrD),
            array('name' => 'Tecnico subsequente presencial', 'data'=>$arrE),
            array('name' => 'Tecnico subsequente a distancia', 'data'=>$arrF),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasInstitutosTecnicosCampus", "Etapas/Modalidades",300,$formatoValores);
    }
}
function gerarGraficoRedeFederal8($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE (sh.indid in (2347) AND dsh.tidid2 IN (5131, 5130, 5189, 5188, 5190, 5132))";
            break;
        case 3:
            $where = "WHERE (sh.indid in (2905) AND dsh.tidid1 IN (7867, 7869))";
            break;
        case 4:
            $where = "WHERE (sh.indid in (2903))";
            break;
        default:
            $where = "WHERE (
                        (sh.indid in (2347) AND dsh.tidid2 IN (5131, 5130, 5189, 5188, 5190, 5132))
                        OR
                        (sh.indid in (2905) AND dsh.tidid1 IN (7867, 7869))
                        OR
                        (sh.indid in (2903))
                      )";
            break;
    }

    $arrEtapa = array();
    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid2 IN (5131) OR dsh.tidid1 IN (7867) THEN 'Técnico concomitante'
                    WHEN dsh.tidid2 IN (5130) THEN 'Técnico integrado'
                    WHEN dsh.tidid2 IN (5189) THEN 'Técnico PROEJA concomitante'
                    WHEN dsh.tidid2 IN (5188) THEN 'Técnico PROEJA integrado'
                    WHEN dsh.tidid2 IN (5190) OR dsh.tidid1 IN (7869) THEN 'Técnico subsequente presencial'
                    WHEN dsh.tidid2 IN (5132) OR sh.indid in (2903) THEN 'Técnico subsequente a distância'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND dsh.entid = {$_SESSION['entid_campus']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql );
    $_SESSION['exportar8'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrC = array();
        $arrD = array();
        $arrE = array();
        $arrF = array();
        $arrTotal = array();

        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['Técnico concomitante']['total'])?array_sum($sit['Técnico concomitante']['total']):0));
            $vB = Round((is_array($sit['Técnico integrado']['total'])?array_sum($sit['Técnico integrado']['total']):0));
            $vC = Round((is_array($sit['Técnico PROEJA concomitante']['total'])?array_sum($sit['Técnico PROEJA concomitante']['total']):0));
            $vD = Round((is_array($sit['Técnico PROEJA integrado']['total'])?array_sum($sit['Técnico PROEJA integrado']['total']):0));
            $vE = Round((is_array($sit['Técnico subsequente presencial']['total'])?array_sum($sit['Técnico subsequente presencial']['total']):0));
            $vF = Round((is_array($sit['Técnico subsequente a distância']['total'])?array_sum($sit['Técnico subsequente a distância']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrC[] = $vC;
            $vTotal += $vC;
            $arrD[] = $vD;
            $vTotal += $vD;
            $arrE[] = $vE;
            $vTotal += $vE;
            $arrF[] = $vF;
            $vTotal += $vF;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Tecnico concomitante', 'data'=>$arrA),
            array('name' => 'Tecnico integrado', 'data'=>$arrB),
            array('name' => 'Tecnico PROEJA concomitante', 'data'=>$arrC),
            array('name' => 'Tecnico PROEJA integrado', 'data'=>$arrD),
            array('name' => 'Tecnico subsequente presencial', 'data'=>$arrE),
            array('name' => 'Tecnico subsequente a distancia', 'data'=>$arrF),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesTecnicosCampus", "Etapas/Modalidades",300,$formatoValores);
    }
}

function gerarGraficoRedeFederal9($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2347)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905)";
            break;
        case 4:
            $where = "WHERE sh.indid in (2903)";
            break;
        default:
            $where = "WHERE sh.indid in (2347, 2905, 2903)";
            break;
    }

    $sql = "SELECT
                CASE
                    WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) OR dsh.tidid1 IN (7867, 7869) OR sh.indid in (2903) THEN 'Técnico'
                    WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
                    WHEN dsh.tidid2 IN (5129, 9263) OR dsh.tidid1 IN (7866) THEN 'FIC'
                    WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
                    WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
                END AS descricao,
                sum(dsh.dshqtde) as valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            INNER JOIN entidade.entidade e ON e.entunicod = dsh.unicod AND e.entstatus = 'A'
            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
            $where
            AND dpe.dpeanoref >= '2010'
            AND sh.sehstatus <> 'I'
            AND e.entid = {$_REQUEST['entid']}
            GROUP BY descricao
            ORDER BY descricao";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar9'] = $arrDados;
    if($arrDados){
        geraGrafico($arrDados, "graficoPizzaIngressantesInstituicao", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
    }
}
function gerarGraficoRedeFederal10($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2347)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905)";
            break;
        case 4:
            $where = "WHERE sh.indid in (2903)";
            break;
        default:
            $where = "WHERE sh.indid in (2347, 2905, 2903)";
            break;
    }

    $sql = "SELECT
                CASE
                    WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) OR dsh.tidid1 IN (7867, 7869) OR sh.indid in (2903) THEN 'Técnico'
                    WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
                    WHEN dsh.tidid2 IN (5129, 9263) OR dsh.tidid1 IN (7866) THEN 'FIC'
                    WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
                    WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
                END AS descricao,
                sum(dsh.dshqtde) as valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            $where
            AND dpe.dpeanoref >= '2010'
            AND sh.sehstatus <> 'I'
            AND dsh.entid = {$_SESSION['entid_campus']}
            GROUP BY descricao
            ORDER BY descricao";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar10'] = $arrDados;
    if($arrDados){
        geraGrafico($arrDados, "graficoPizzaIngressantesCampus", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
    }
}

function gerarGraficoRedeFederal11($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2575)  AND dsh.tidid1 IN (7000, 9264)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905) AND dsh.tidid1 IN (7866)";
            break;
        default:
            $where = "WHERE (
                        (sh.indid in (2575) AND dsh.tidid1 IN (7000, 9264))
                      OR
                        (sh.indid in (2905) AND dsh.tidid1 IN (7866))
                      )";
            break;
    }

    $arrEtapa = array();
    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (7000, 7866) THEN 'FIC >= 160h'
                    WHEN dsh.tidid1 IN (9264) THEN 'FIC < 160h'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            INNER JOIN entidade.entidade e ON e.entunicod = dsh.unicod AND e.entstatus = 'A'
            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND e.entid = {$_REQUEST['entid']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar20'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrTotal = array();

        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['FIC >= 160h']['total'])?array_sum($sit['FIC >= 160h']['total']):0));
            $vB = Round((is_array($sit['FIC < 160h']['total'])?array_sum($sit['FIC < 160h']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'FIC >= 160h', 'data'=>$arrA),
            array('name' => 'FIC < 160h', 'data'=>$arrB),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasInstitutosFIC", "Etapas/Modalidades",300, $formatoValores);
    }
}
function gerarGraficoRedeFederal12($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2347) AND dsh.tidid2 IN (5129, 9263)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905) AND dsh.tidid1 IN (7866)";
            break;
        default:
            $where = "WHERE (
                            (sh.indid in (2347) AND dsh.tidid2 IN (5129, 9263))
                      OR
                            (sh.indid in (2905) AND dsh.tidid1 IN (7866))
                      )";
            break;
    }

    $sql = "SELECT dpe.dpeanoref as periodo,
            CASE
                    WHEN dsh.tidid2 IN (5129) OR dsh.tidid1 IN (7866) THEN 'FIC >= 160h'
                    WHEN dsh.tidid2 IN (9263) THEN 'FIC < 160h'
                END AS etapa,
            SUM(dsh.dshqtde)::integer AS total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            INNER JOIN entidade.entidade e ON e.entunicod = dsh.unicod AND e.entstatus = 'A'
            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid AND fue.funid = 11
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND e.entid = {$_REQUEST['entid']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar21'] = $arrDados;
    $arrEtapa = array();
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrTotal = array();


        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['FIC >= 160h']['total'])?array_sum($sit['FIC >= 160h']['total']):0));
            $vB = Round((is_array($sit['FIC < 160h']['total'])?array_sum($sit['FIC < 160h']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'FIC >= 160h', 'data'=>$arrA),
            array('name' => 'FIC < 160h', 'data'=>$arrB),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesFIC", "Ingressantes",300,$formatoValores);
    }
}

function gerarGraficoRedeFederal13($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2575) AND dsh.tidid1 IN (7000, 9264)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905) AND dsh.tidid1 IN (7866)";
            break;
        default:
            $where = "WHERE (
                            (sh.indid in (2575) AND dsh.tidid1 IN (7000, 9264))
                      OR
                            (sh.indid in (2905) AND dsh.tidid1 IN (7866))
                      )";
            break;
    }

    $arrEtapa = array();
    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (7000, 7866) THEN 'FIC >= 160h'
                    WHEN dsh.tidid1 IN (9264) THEN 'FIC < 160h'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND dsh.entid = {$_SESSION['entid_campus']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar22'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrTotal = array();

        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['FIC >= 160h']['total'])?array_sum($sit['FIC >= 160h']['total']):0));
            $vB = Round((is_array($sit['FIC < 160h']['total'])?array_sum($sit['FIC < 160h']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'FIC >= 160h', 'data'=>$arrA),
            array('name' => 'FIC < 160h', 'data'=>$arrB),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasCampusFIC", "Etapas/Modalidades",300, $formatoValores);
    }
}
function gerarGraficoRedeFederal14($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2347) AND dsh.tidid2 IN (5129, 9263)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905) AND dsh.tidid1 IN (7866)";
            break;
        default:
            $where = "WHERE (
                                (sh.indid in (2347) AND dsh.tidid2 IN (5129, 9263))
                            OR
                                (sh.indid in (2905) AND dsh.tidid1 IN (7866))
                            )";
            break;
    }

    $sql = "SELECT dpe.dpeanoref as periodo,
            CASE
                    WHEN dsh.tidid2 IN (5129) OR dsh.tidid1 IN (7866) THEN 'FIC >= 160h'
                    WHEN dsh.tidid2 IN (9263) THEN 'FIC < 160h'
                END AS etapa,
            SUM(dsh.dshqtde)::integer AS total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            AND dsh.entid = {$_SESSION['entid_campus']}
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar23'] = $arrDados;
    $arrEtapa = array();
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrTotal = array();


        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['FIC >= 160h']['total'])?array_sum($sit['FIC >= 160h']['total']):0));
            $vB = Round((is_array($sit['FIC < 160h']['total'])?array_sum($sit['FIC < 160h']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'FIC >= 160h', 'data'=>$arrA),
            array('name' => 'FIC < 160h', 'data'=>$arrB),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesFICCampus", "Ingressantes",300,$formatoValores);
    }
}

//GERAR DADOS EM EXCEL
$gerarExcel = $_REQUEST['tipo'];
switch($gerarExcel){
    case 1:
        //Evolução de matrículas atendidas
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar1'], "Evolucao_matriculas");
        break;
    case 2:
        //Evolução de ingressantes
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar2'], "Evolucao_ingressantes");
        break;
    case 3:
        //Evolução de matrículas atendidas - Técnico
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar3'], "Evolucao_matriculas_tecnico");
        break;
    case 4:
        //Evolução de ingressantes - Técnico
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar4'], "Evolucao_ingressantes_tecnico");
        break;
    case 5:
        //Evolução de matrículas atendidas
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar5'], "Evolucao_matriculas_campus");
        break;
    case 6:
        //Evolução de ingressantes
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar6'], "Evolucao_ingressantes_campus");
        break;
    case 7:
        //Evolução de matrículas atendidas - Técnico
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar7'], "Evolucao_matriculas_tecnico_campus");
        break;
    case 8:
        //Evolução de ingressantes - Técnico
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar8'], "Evolucao_ingressantes_tecnico_campus");
        break;
    case 9:
        //Ingressantes Etapa / Modalidade
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar9'], "Ingressantes_Etapa_Modalidade");
        break;
    case 10:
        //Ingressantes Etapa / Modalidade
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar10'], "Ingressantes_Etapa_Modalidade_Campus");
        break;
    case 11:
        //Evolução de matrículas de graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar11'], "Evolucao_matriculas_graduacao");
        break;
    case 12:
        //Evolução de ingressantes de graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar12'], "Evolucao_ingressantes_graduacao");
        break;
    case 13:
        //Evolução de matrículas de pós-graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar13'], "Evolucao_matriculas_pos_graduacao");
        break;
    case 14:
        //Evolução de ingressantes de pós-graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar14'], "Evolucao_ingressantes_pos_graduacao");
        break;
    case 15:
        //Investimentos
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar15'], "Investimentos");
        break;
    case 16:
        //Evolução de matrículas de graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar16'], "Evolucao_matriculas_graduacao_campus");
        break;
    case 17:
        //Evolução de ingressantes de graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar17'], "Evolucao_ingressantes_graduacao_campus");
        break;
    case 18:
        //Evolução de matrículas de pós-graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar18'], "Evolucao_matriculas_pos_graduacao_campus");
        break;
    case 19:
        //Evolução de ingressantes de pós-graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar19'], "Evolucao_ingressantes_pos_graduacao_campus");
        break;
    case 20:
        //Evolução de matrículas atendidas - FIC
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar20'], "Evolucao_matriculas_fic");
        break;
    case 21:
        //Evolução de ingressantes - FIC
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar21'], "Evolucao_ingressantes_tecnico");
        break;
    case 22:
        //Evolução de matrículas atendidas - FIC
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar20'], "Evolucao_matriculas_fic_campus");
        break;
    case 23:
        //Evolução de ingressantes - FIC
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar21'], "Evolucao_ingressantes_tecnico_campus");
        break;
}
?>

