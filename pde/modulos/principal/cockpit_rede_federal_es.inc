<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
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
		#popdiv2{display:block;width:100%;height:357px;overflow-y: scroll;}
	</style>
	<script>
		jQuery.noConflict();
		jQuery('.div_fotos_interno').cycle({ 
			fx: 'scrollDown' 
		});

		atualizaUsuario();
		
		function listaUniversidades(estado)
		{
			document.getElementById('popdiv').style.display='block';
			
			jQuery.ajax({
				url		: 'estrategico.php?modulo=principal/cockpit_rede_federal_es&acao=A',
				type	: 'post',
				data	: 'requisicaoAjax=listaUniversidadesEstadoRedeFederal&estado='+estado,
				success	: function(e){
				jQuery('#popdiv').html(e);
				}
			});

		}
		
		function abreUniversidade(id)
		{
			window.location.href = 'estrategico.php?modulo=principal/cockpit_detalhe_universidade&acao=A&entid='+id;
		}

        jQuery(function(){
            jQuery('.toggle_consolidado1').click(function(){
                jQuery('#graficos_consolidados1').toggle('fade');
            });

            setTimeout(function() {jQuery('#graficos_consolidados1').hide()} , 500);

        });
	</script>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
		<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
        	<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_rede_federal_es&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<!-- Título-->
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Expansão das Universidades Federais</div></td>
	</tr>
	<!-- Fim Título-->
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(382);" width="40%">
			<?php
			$sql = "select count(distinct fn.entid) as campus, count(distinct a.entid) as universidades
					from entidade.funcaoentidade fn
					inner join entidade.funentassoc a on a.fueid = fn.fueid
					where fn.fuestatus = 'A'
					and fn.entid in (
						select
							distinct dsh.entid
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join entidade.entidade entcampus ON entcampus.entid = dsh.entid
						where sh.indid in (382)
						and sh.sehstatus <> 'I'
						and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
						and dsh.tidid1 = 365
						and dsh.tidid2 NOT IN (2589) --INCORPORADO
					)";
			$arrDados = $db->pegaLinha($sql);
			$valorMetaCampus = 321;
			$porcentagemCampus = number_format(($arrDados['campus']/$valorMetaCampus)*100,0,",",".");
			$horizontalCampus = $porcentagemCampus * 2;
			$valorMetaUniversidades = 63;
			$porcentagemUniversidades = number_format(($arrDados['universidades']/$valorMetaUniversidades)*100,0,",",".");
			$horizontalUniversidades = $porcentagemUniversidades * 2;
			?>
			<table cellpadding="0" cellspacing="0" border="0" valign="top" width="100%" >
				<tr>
					<td>
						<?php exibirTitulo('alvo', 'Metas e Execução 2014'); ?>
					</td>
				</tr>
				<tr>
					<td class="titulo_box bold" align="center"><br>
					<?php echo number_format($valorMetaUniversidades,0,",",".") ?> Universidades
					<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
						<div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalUniversidades ?>px;'><?= $porcentagemUniversidades ?>%&nbsp;</div>
					</div>
					<?php
					if($arrDados['universidades'] < $valorMetaUniversidades){
						echo number_format($arrDados['universidades'],0,",",".") . " Universidades";
					}	
					?>
					</td>
				</tr>
				<tr>
					<td class="titulo_box bold" align="center"><br>
					<?php echo number_format($valorMetaCampus,0,",",".") ?> Câmpus
					<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
						<div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalCampus ?>px;'><?= $porcentagemCampus ?>%&nbsp;</div>
					</div><?php echo number_format($arrDados['campus'],0,",",".") ?> Funcionando</td>
				</tr>
			</table>
            <br>
            <br>
            <?php
            $sql = "SELECT
                            SUM(expansao) AS totalexpansao,
                            SUM(funcionando) AS totalfuncionando,
                            SUM(provisoria) AS totalprovisoria,
                            SUM(definitiva) AS totaldefinitiva
                        FROM (
                            SELECT
                                1 AS expansao,
                                CASE
                                    WHEN cmp.cmpsituacao = 'F' THEN 1
                                    ELSE 0
                                END AS funcionando,
                                CASE
                                    WHEN cmp.cmpinstalacao = 'P' THEN 1
                                    ELSE 0
                                END AS provisoria,
                                CASE
                                    WHEN cmp.cmpinstalacao = 'D' THEN 1
                                    ELSE 0
                                END AS definitiva
                            FROM entidade.entidade entcampus
                            INNER JOIN entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                            INNER JOIN entidade.funentassoc a on a.fueid = fn.fueid
                            INNER JOIN entidade.entidade entinst on entinst.entid = a.entid
                            INNER JOIN entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
                            INNER JOIN academico.campus cmp on cmp.entid = entcampus.entid
                            INNER JOIN academico.existencia exi on exi.exiid = cmp.exiid
                            WHERE cmp.exiid IN (3, 5)
                        ) AS foo";
            $arrDados = $db->pegaLinha($sql);
            $valorMetaCampus = $arrDados['totalexpansao'];
            $porcentagemCampus = number_format(($arrDados['totalfuncionando']/$valorMetaCampus)*100,0,",",".");
            $horizontalCampus = $porcentagemCampus * 2;
            ?>
            <table cellpadding="0" cellspacing="0" border="0" valign="top" width="100%" >
                <tr>
                    <td>
                        <?php exibirTitulo('alvo', 'Expansão 2011/2014'); ?>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        <?php echo number_format($valorMetaCampus,0,",",".") ?> Câmpus
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalCampus ?>px;'><?= $porcentagemCampus ?>%&nbsp;</div>
                        </div><?php echo number_format($arrDados['totalfuncionando'],0,",",".") ?> Funcionando</td>
                </tr>
            </table>
		</td>
<?php /* ?>
        <td class="fundo_padrao"  width="25%">
                <?php
                $sql = "SELECT
                            SUM(expansao) AS totalexpansao,
                            SUM(funcionando) AS totalfuncionando,
                            SUM(provisoria) AS totalprovisoria,
                            SUM(definitiva) AS totaldefinitiva
                        FROM (
                            SELECT
                                1 AS expansao,
                                CASE
                                    WHEN cmp.cmpsituacao = 'F' THEN 1
                                    ELSE 0
                                END AS funcionando,
                                CASE
                                    WHEN cmp.cmpinstalacao = 'P' THEN 1
                                    ELSE 0
                                END AS provisoria,
                                CASE
                                    WHEN cmp.cmpinstalacao = 'D' THEN 1
                                    ELSE 0
                                END AS definitiva
                            FROM entidade.entidade entcampus
                            INNER JOIN entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                            INNER JOIN entidade.funentassoc a on a.fueid = fn.fueid
                            INNER JOIN entidade.entidade entinst on entinst.entid = a.entid
                            INNER JOIN entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
                            INNER JOIN academico.campus cmp on cmp.entid = entcampus.entid
                            INNER JOIN academico.existencia exi on exi.exiid = cmp.exiid
                            WHERE cmp.exiid IN (3, 5)
                        ) AS foo";
                $arrDados = $db->pegaLinha($sql);

                $qtdExpansao = $arrDados['totalexpansao'];
                $qtdeProvisoria = $arrDados['totalprovisoria'];
                $qtdeDefinitiva = $arrDados['totaldefinitiva'];
                $totalFuncionando = $arrDados['totalfuncionando'];

                $porcentagem = number_format(($totalFuncionando/$qtdExpansao)*100,0,",",".");
                $horizontal = $porcentagem * 2;

                $porcentagemProvisoria = number_format(($qtdeProvisoria/$totalFuncionando)*100,0,",",".");
                $porcentagemDefinitiva = number_format(($qtdeDefinitiva/$totalFuncionando)*100,0,",",".");

                $horizontalProvisoria = ($porcentagemProvisoria/100)*$horizontal;
                $horizontalDefinitiva = ($porcentagemDefinitiva/100)*$horizontal;

                $qtdeProvisoria = number_format($qtdeProvisoria,0,",",".");
                $qtdeDefinitiva = number_format($qtdeDefinitiva,0,",",".");

                $txtTitleGeral  = "<table>";
                $txtTitleGeral .= "<tr><td>&nbsp;</td><td><center><b>&nbsp;&nbsp;Qtde.&nbsp;&nbsp;</b></center></td><td><center><b>&nbsp;&nbsp;Concuído&nbsp;%&nbsp;&nbsp;</b></center></td></tr>";
                $txtTitleGeral .= "<tr><td><b>Provisória:</b></td><td><center>{$qtdeProvisoria}</center></td><td><center>".round((($porcentagemProvisoria/100)*$porcentagem))."%</center></td></tr>";
                $txtTitleGeral .= "<tr><td><b>Definitiva:</b></td><td><center>{$qtdeDefinitiva}</center></td><td><center>".round((($porcentagemDefinitiva/100)*$porcentagem))."%</center></td></tr>";
                $txtTitleGeral .= "<tr><td><b>Total:</b></td><td><center>{$totalFuncionando}</center></td><td><center>{$porcentagem}%</center></td></tr>";
                $txtTitleGeral .= "</table>";
                ?>
                <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
                <script type="text/javascript" src="../includes/remedial.js"></script>
                <script type="text/javascript" src="../includes/superTitle.js"></script>
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        <?php echo number_format($qtdExpansao,0,",",".") ?> Câmpus
                        <div onmousemove="SuperTitleOn(this, '<?php echo $txtTitleGeral; ?>' );" onmouseout="SuperTitleOff( this );" style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontal ?>px;'>
                                <div id="div_publica" style="float:left; border: 0px; background-color: #537A2C; text-align: right; color: white; height: 25px; width: <?= $horizontalProvisoria ?>px;">
                                    <?= round( (($porcentagemProvisoria/100)*$porcentagem) ).'%&nbsp;' ?>
                                </div>
                                <div id="div_privada" style="float:left; border: 0px; background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalDefinitiva ?>px;">
                                    <?= round( (($porcentagemDefinitiva/100)*$porcentagem) ).'%&nbsp;' ?>
                                </div>
                            </div>
                        </div>
                        <?php echo number_format($totalFuncionando,0,",",".") ?> Funcionando
                    </td>
                </tr>
                <tr>
                    <td class="bold"><br>
                        <div id="div_publica" style="float:left; border: 0px; background-color: #537A2C; text-align: right; color: white; height: 25px; width: 30px;"></div>
                        &nbsp;Instalações provisórias
                    </td>
                </tr>
                <tr>
                    <td class="bold"><br>
                        <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: right; color: white; height: 25px; width: 30px;"></div>
                        &nbsp;Instalações definitivas
                    </td>
                </tr>
            </table>
        </td>
<?php */ ?>
<?php /* ?>
        <td class="fundo_padrao link toggle_consolidado1" align="center">
            <div>
                <?php exibirTitulo('alvo', 'Previsão do início das atividades'); ?>
            </div>
            <?php
            $sql = "SELECT 'Nao funcionando' AS situacao, substr(cmp.cmpdataimplantacao,5,6) || '/' || substr(cmp.cmpdataimplantacao,1,4) AS acao, COUNT(0) AS valor
                    FROM entidade.entidade entcampus
                    INNER JOIN entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                    INNER JOIN entidade.funentassoc a on a.fueid = fn.fueid
                    INNER JOIN entidade.entidade entinst on entinst.entid = a.entid
                    INNER JOIN entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
                    INNER JOIN academico.campus cmp on cmp.entid = entcampus.entid
                    INNER JOIN academico.existencia exi on exi.exiid = cmp.exiid
                    WHERE exi.exiid IN (3,5)
                    AND cmp.cmpsituacao = 'N'
                    GROUP BY acao, cmp.cmpdataimplantacao
                    ORDER BY cmp.cmpdataimplantacao";
            $dados = $db->carregar( $sql );
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaPrevisao', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            }
            ?>
        </td>
 <?php */ ?>
        <td class="fundo_padrao" align="center" colspan="2">
            <div>
                <?php exibirTitulo('casas', 'Situação dos Câmpus'); ?>
            </div>
            <div id="popdiv2">
                <?php
                $sql = "SELECT
                        entinst.entsig AS universidade,
                        entcampus.entnome AS campus,
                        CASE
                            WHEN cmp.cmpinstalacao = 'P' THEN 'Provisória'
                            WHEN cmp.cmpinstalacao = 'D' THEN 'Definitiva'
                            ELSE 'Não Informado'
                        END AS instalacao,
                        cmp.cmpsituacao AS situacao
                    FROM entidade.entidade entcampus
                    INNER JOIN entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                    INNER JOIN entidade.funentassoc a on a.fueid = fn.fueid
                    INNER JOIN entidade.entidade entinst on entinst.entid = a.entid
                    INNER JOIN entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
                    INNER JOIN academico.campus cmp on cmp.entid = entcampus.entid
                    INNER JOIN academico.existencia exi on exi.exiid = cmp.exiid
                    WHERE cmp.exiid IN (3, 5)
                    AND cmp.cmpsituacao = 'F'
                    ORDER BY situacao, universidade, campus, instalacao";
                $arrDadosFuncionando = $db->carregar( $sql );

                $sql = "SELECT
                        entinst.entsig AS universidade,
                        entcampus.entnome AS campus,
                        CASE
                            WHEN cmp.cmpinstalacao = 'P' THEN 'Provisória'
                            WHEN cmp.cmpinstalacao = 'D' THEN 'Definitiva'
                            WHEN cmp.cmpinstalacao = 'S' THEN 'Sem instalações'
                            ELSE 'Não Informado'
                        END AS instalacao,
                        substr(cmp.cmpdataimplantacao,5,6) || '/' || substr(cmp.cmpdataimplantacao,1,4) AS previsao
                    FROM entidade.entidade entcampus
                    INNER JOIN entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                    INNER JOIN entidade.funentassoc a on a.fueid = fn.fueid
                    INNER JOIN entidade.entidade entinst on entinst.entid = a.entid
                    INNER JOIN entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
                    INNER JOIN academico.campus cmp on cmp.entid = entcampus.entid
                    INNER JOIN academico.existencia exi on exi.exiid = cmp.exiid
                    WHERE cmp.exiid IN (3, 5)
                    AND cmp.cmpsituacao = 'N'
                    ORDER BY previsao, universidade, campus, instalacao";
                $arrDadosNaoFuncionando = $db->carregar( $sql );
                ?>
                <table>
                    <tr>
                        <td>
                            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                                <tr>
                                    <th colspan="3">Funcionando</th>
                                </tr>
                                <tr>
                                    <th>Universidade</th>
                                    <th>Câmpus</th>
                                    <th>Instalações</th>
                                </tr>
                                <?php
                                if($arrDadosFuncionando){
                                    $count=-1;
                                    foreach($arrDadosFuncionando as $dado):
                                        $count++;
                                        ?>
                                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                            <td class="" ><?php echo $dado['universidade'] ?></td>
                                            <td class="" ><?php echo $dado['campus'] ?></td>
                                            <td class="" ><?php echo $dado['instalacao'] ?></td>
                                        </tr>
                                    <?php
                                    endforeach;
                                    echo "<tr>";
                                    echo "<th colspan='2'>Total</th>";
                                    echo "<th class='numero'>".count($arrDadosFuncionando)."</th>";
                                    echo "</tr>";
                                }
                                ?>

                            </table>
                        </td>
                        <td>
                            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                                <tr>
                                    <th colspan="3">Não Funcionando</th>
                                </tr>
                                <tr>
                                    <th>Universidade</th>
                                    <th>Câmpus</th>
                                    <th>Previsão</th>
                                </tr>
                                <?php
                                if($arrDadosNaoFuncionando){
                                    $count=-1;
                                    foreach($arrDadosNaoFuncionando as $dado):
                                        $count++;
                                        ?>
                                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                            <td class="" ><?php echo $dado['universidade'] ?></td>
                                            <td class="" ><?php echo $dado['campus'] ?></td>
                                            <td class="" ><?php echo $dado['previsao'] ?></td>
                                        </tr>
                                    <?php
                                    endforeach;
                                    echo "<tr>";
                                    echo "<th colspan='2'>Total</th>";
                                    echo "<th class='numero'>".count($arrDadosNaoFuncionando)."</th>";
                                    echo "</tr>";
                                }
                                ?>
                            </table>
                        </td>
                    </tr>
                </table>
            </div>
        </td>
	</tr>
<?php /* ?>
    <tr id="graficos_consolidados1">
        <td class="fundo_padrao" align="center">
            <div>
                <?php exibirTitulo('casas', 'Previsão por Câmpus'); ?>
            </div>
            <div id="popdiv">
                <?php
                $sql = "SELECT
                            entinst.entsig AS universidade,
                            entcampus.entnome AS campus,
                            substr(cmp.cmpdataimplantacao,5,6) || '/' || substr(cmp.cmpdataimplantacao,1,4) AS previsao
                        FROM entidade.entidade entcampus
                        INNER JOIN entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                        INNER JOIN entidade.funentassoc a on a.fueid = fn.fueid
                        INNER JOIN entidade.entidade entinst on entinst.entid = a.entid
                        INNER JOIN entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
                        INNER JOIN academico.campus cmp on cmp.entid = entcampus.entid
                        INNER JOIN academico.existencia exi on exi.exiid = cmp.exiid
                        WHERE exi.exiid IN (3,5)
                        AND cmp.cmpsituacao = 'N'
                        ORDER BY cmp.cmpdataimplantacao, universidade, campus";
                $arrDados = $db->carregar( $sql );
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <th colspan="3">Não Funcionando</th>
                    </tr>
                    <tr>
                        <th>Universidade</th>
                        <th>Câmpus</th>
                        <th>Previsão</th>
                    </tr>
                    <?php
                    if($arrDados){
                        $count=-1;
                        foreach($arrDados as $dado):
                            $count++;
                            ?>
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                <td class="" ><?php echo $dado['universidade'] ?></td>
                                <td class="" ><?php echo $dado['campus'] ?></td>
                                <td class="" ><?php echo $dado['previsao'] ?></td>
                            </tr>
                        <?php
                        endforeach;
                    }
                    ?>
                    <tr>
                        <th colspan="2">Total</th>
                        <th class="numero"><?=count($arrDados)?></th>
                    </tr>
                </table>
            </div>
        </td>
    </tr>
 <?php */ ?>
	<tr>
		<td class="fundo_padrao link" onclick="abreRedeFederal('S');" align="center" colspan="2">
			<div>
				<?php exibirTitulo('mapas', 'Mapas das Universidades', 'Clique no mapa para visualizar os detalhes.'); ?>
			</div>
			<?php
			/*$sql = "select
					count(mun.muncod) as qtde
					from entidade.entidade entcampus
					inner join entidade.endereco edc ON edc.entid = entcampus.entid
					LEFT JOIN territorios.municipio mun ON edc.muncod = mun.muncod
					LEFT JOIN territorios.estado est ON est.estuf = mun.estuf
					LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod
					inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
					inner join entidade.funentassoc a on a.fueid = fn.fueid
					inner join entidade.entidade entinst on entinst.entid = a.entid
					inner join entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
					inner join academico.campus cmp on cmp.entid = entcampus.entid
					inner join academico.existencia exi on exi.exiid = cmp.exiid
					left join academico.entidadedetalhe edt on edt.entid = entcampus.entid
					WHERE exi.exidsc IN ('Preexistentes','Criados em 2003/2010','Novos 2011/2012','Novos 2013/2014');";
			$totalMun = $db->pegaUm($sql);
			*/
			?>
			<img class="link" onclick="abreRedeFederal('S')" src="../imagens/cockpit/mapa_cockpit.png"  />
		</td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(382);" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Situação dos Câmpus / Unidades'); ?>
            </div>
            <?php
            $sql = "SELECT ordem, status, valor
                    FROM (
                        SELECT
                            CASE
                            WHEN dsh.tidid2 IN (1476) THEN 1
                            WHEN dsh.tidid2 IN (2258) THEN 2
                            WHEN dsh.tidid2 IN (2587, 2588) THEN 3
                            END AS ordem,
                            CASE
                            WHEN dsh.tidid2 IN (1476) THEN 'Preexistente até 2002'
                            WHEN dsh.tidid2 IN (2258) THEN 'Criados de 2003 a 2010'
                            WHEN dsh.tidid2 IN (2587, 2588) THEN 'Expansão 2011/2014'
                            ELSE tid2.tiddsc
                            END AS status,
                            sum(dsh.dshqtde)::INTEGER AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                        WHERE sh.indid in (382)
                        AND sh.sehstatus <> 'I'
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s WHERE s.indid = sh.indid)
                        AND dsh.tidid2 IN (2258, 2587, 2588, 1476)
                        GROUP BY status, ordem
                    UNION ALL
                        SELECT
                            2 AS ordem,
                            CASE
                            WHEN dsh.tidid2 IN (1476) THEN 'Preexistente até 2002'
                            ELSE tid2.tiddsc
                            END AS status,
                            sum(dsh.dshqtde)::INTEGER AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                        WHERE sh.indid in (382)
                        AND sh.sehstatus <> 'I'
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s WHERE s.indid = sh.indid)
                        AND dsh.tidid2 IN (1476)
                        GROUP BY status, ordem
                    UNION ALL
                        SELECT
                            3 AS ordem,
                            CASE
                            WHEN dsh.tidid2 IN (1476) THEN 'Preexistente até 2002'
                            WHEN dsh.tidid2 IN (2258) THEN 'Criados de 2003 a 2010'
                            END AS status,
                            sum(dsh.dshqtde)::INTEGER AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                        WHERE sh.indid in (382)
                        AND sh.sehstatus <> 'I'
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s WHERE s.indid = sh.indid)
                        AND dsh.tidid2 IN (2258, 1476)
                        GROUP BY status, ordem
                    ) AS FOO
                    ORDER BY ordem, status";
            $dados = $db->carregar( $sql );

            $dadosAgrupados = agruparDadosGrafico($dados, 'ordem', 'status', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.series.name + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }",
                'corX' => "#152D56"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculasSituacao', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            ?>
        </td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<!-- Tabela Indicadores-->
		<td class="fundo_padrao" rowspan="1" width="70%">
            <table width="100%" align="center" border="0">
                <tr>
                    <td width="20%">&nbsp;</td>
                    <td align="center">
                        <div>
                            <?php exibirTitulo('busca', 'Monitoramento de Programas e Ações por Universidade', 'Clique na bandeira para filtrar o estado'); ?>
                        </div>
                    </td>
                    <td width="20%">&nbsp;</td>
                </tr>
             </table>
			<table width = 100%>
			<tr>
				<td width = "50%">
					<div  align=center style="width: 100%;" id="containerMapa" >
						<img src="/imagens/mapa_brasil.png" width="444" height="357" border="0" usemap="#mapaBrasil" />
						<map name="mapaBrasil" id="mapaBrasil">
							<area shape="rect" coords="48,124,74,151"  href="javascript:void(0);" onclick="listaUniversidades('AC');" title="Acre"/>
							<area shape="rect" coords="364,147,432,161" href="javascript:void(0);" onclick="listaUniversidades('AL');" title="Alagoas"/>
							<area shape="rect" coords="202,27,233,56"   href="javascript:void(0);" onclick="listaUniversidades('AP');" title="Amapá"/>
							<area shape="rect" coords="89,76,133,107"   href="javascript:void(0);" onclick="listaUniversidades('AM');" title="Amazonas"/>
							<area shape="rect" coords="294,155,320,183" href="javascript:void(0);" onclick="listaUniversidades('BA');" title="Bahia"/>
							<area shape="rect" coords="311,86,341,114"  href="javascript:void(0);" onclick="listaUniversidades('CE');" title="Ceará"/>
							<area shape="rect" coords="244,171,281,197" href="javascript:void(0);" onclick="listaUniversidades('DF');" title="Distrito Federal"/>
							<area shape="rect" coords="331,215,369,242" href="javascript:void(0);" onclick="listaUniversidades('ES');" title="Espírito Santo"/>
							<area shape="rect" coords="217,187,243,218" href="javascript:void(0);" onclick="listaUniversidades('GO');" title="Goiás"/>
							<area shape="rect" coords="154,155,210,186" href="javascript:void(0);" onclick="listaUniversidades('MT');" title="Mato Grosso"/>
							<area shape="rect" coords="156,219,202,246" href="javascript:void(0);" onclick="listaUniversidades('MS');" title="Mato Grosso do Sul"/>
							<area shape="rect" coords="248,80,301,111"  href="javascript:void(0);" onclick="listaUniversidades('MA');" title="Maranhão"/>
							<area shape="rect" coords="264,206,295,235" href="javascript:void(0);" onclick="listaUniversidades('MG');" title="Minas Gerais"/>
							<area shape="rect" coords="188,84,217,112"  href="javascript:void(0);" onclick="listaUniversidades('PA');" title="Pará"/>
							<area shape="rect" coords="368,112,433,130" href="javascript:void(0);" onclick="listaUniversidades('PB');" title="Paraíba"/>
							<area shape="rect" coords="201,262,231,289" href="javascript:void(0);" onclick="listaUniversidades('PR');" title="Paraná"/>
							<area shape="rect" coords="369,131,454,147" href="javascript:void(0);" onclick="listaUniversidades('PE');" title="Pernambuco"/>
							<area shape="rect" coords="285,116,313,146" href="javascript:void(0);" onclick="listaUniversidades('PI');" title="Piauí"/>
							<area shape="rect" coords="349,83,383,108"  href="javascript:void(0);" onclick="listaUniversidades('RN');" title="Rio Grande do Norte"/>
							<area shape="rect" coords="189,310,224,337" href="javascript:void(0);" onclick="listaUniversidades('RS');" title="Rio Grande do Sul"/>
							<area shape="rect" coords="302,250,334,281" href="javascript:void(0);" onclick="listaUniversidades('RJ');" title="Rio de Janeiro"/>
							<area shape="rect" coords="98,139,141,169"  href="javascript:void(0);" onclick="listaUniversidades('RO');" title="Rondônia"/>
							<area shape="rect" coords="112,24,147,56"   href="javascript:void(0);" onclick="listaUniversidades('RR');" title="Roraima"/>
							<area shape="rect" coords="228,293,272,313" href="javascript:void(0);" onclick="listaUniversidades('SC');" title="Santa Catarina"/>
							<area shape="rect" coords="233,243,268,270" href="javascript:void(0);" onclick="listaUniversidades('SP');" title="São Paulo"/>
							<area shape="rect" coords="337,161,401,178" href="javascript:void(0);" onclick="listaUniversidades('SE');" title="Sergipe"/>
							<area shape="rect" coords="227,130,270,163" href="javascript:void(0);" onclick="listaUniversidades('TO');" title="Tocantins"/>
						</map>
					</div>
				</td>
				<td>
					<div id = "popdiv">
					<?php listaUniversidadesRedeFederal(); ?>
					</div>
				</td>
			</tr>
			</table>
		</td>
		<!-- Tabela Alinhamento Estratégico -->
		<td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',3,88,'','');" align="center" width="30%">
			<?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
			<img src="cockpit/images/alinhamentoEstrategico.png" />
		</td>
		<!-- FIM Tabela Alinhamento Estratégico -->
	</tr>
<?php /* ?>
	<tr>
		<!-- INÍCIO DO FINANCEIRO -->
		<?php
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
		?>
		<td class="fundo_padrao">
			<?php
			$sql = "SELECT
						ano,
						sum(valor1) AS empenhado,
						sum(valor2) AS dotacao,
						sum(valor3) AS pago
					FROM (
						SELECT
							2011 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2011 sld
						WHERE substr(sld.plicod, 2, 4) in ('SS09','SS16')
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					UNION ALL
						SELECT
							2012 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2012 sld
						WHERE substr(sld.plicod, 2, 4) in ('SS09','SS10','SS25')
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					UNION ALL
						SELECT
							2013 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2013 sld
						WHERE substr(sld.plicod, 2, 4) in ('SS09','SS10','SS25','SS31')
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					) as foo
					GROUP BY ano
					ORDER BY ano";
			$arrDados = $db2->carregar($sql,null,3200);
			$total_dotacao = 0;
			$total_empenhado = 0;
			$total_pago = 0;
			?>
			<div>
				<?php exibirTitulo('financeiro', 'Orçamentário/Financeiro'); ?>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th class="center bold" >Ano</th>
					<th class="center bold" >Empenhado</th>
					<!--<th class="center bold" >Pago</th>-->
				</tr>
				<?php
				$count = -1;
				foreach($arrDados as $dado):
				    $count++;
					if($dado['ano']=='2011'){?>
						<tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=subacao&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&subacao[0]=SS09&subacao[1]=SS16&alterar_ano=0')" >
					<?php }elseif($dado['ano']=='2012'){?>
						<tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=subacao&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&subacao[0]=SS09&subacao[1]=SS10&subacao[2]=SS25&alterar_ano=0')" >
					<?php }else{?>
						<tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=subacao&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&subacao[0]=SS09&subacao[1]=SS10&subacao[2]=SS25&subacao[3]=SS31&alterar_ano=0')" >
					<?php }?>
						<td><?php echo $dado['ano'] ?></td>
						<td class="numero" ><?php $total_empenhado+=$dado['empenhado']; echo number_format($dado['empenhado'],2,",",".") ?></td>
						<!--<td class="numero" ><?php $total_pago+=$dado['pago'];  echo number_format($dado['pago'],2,",",".") ?></td>-->
					</tr>
				<?php endforeach; ?>
				<tr>
					<th class="bold" >Total</th>
					<th class="numero bold" ><?php echo number_format($total_empenhado,2,",",".") ?></th>
					<!--<th class="numero bold" ><?php echo number_format($total_pago,2,",",".") ?></th>-->
				</tr>
			</table>
		</td>
	</tr>
<?php */ ?>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(553);" width="50%">
			<div>
				<?php exibirTitulo('indicador', 'Matrículas em Graduação em Universidades Federais'); ?>
			</div>
			<?php
			$sql = "SELECT dpe.dpeanoref AS periodo, dsh.tidid1 AS modalidade, SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (553)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo, modalidade
                    ORDER BY periodo, modalidade";
			$arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrB = array();
            $arrTotal = array();
            $arrSituacao = array();
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['periodo']][$dado['modalidade']]['total'][] = $dado['valor'];
                }
                foreach($arrSituacao as $periodo => $sit){
                    $vA = Round((is_array($sit['1839']['total'])?array_sum($sit['1839']['total']):0));
                    $vB = Round((is_array($sit['1838']['total'])?array_sum($sit['1838']['total']):0));
                    $arrX[] = $periodo;
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                    $arrTotal[] =  $vA + $vB;
                }

                $aDados = array(
                    array('name' => 'A Distancia', 'data'=>$arrA),
                    array('name' => 'Presencial', 'data'=>$arrB),
                    array('name' => 'Total de Matriculas', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculas", "Matrículas",300, $formatoValores);
            }
			?>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(650);" width="50%">
			<div>
				<?php exibirTitulo('indicador', 'Vagas em Graduação em Universidades Federais'); ?>
			</div>
			<?php
			$sql = "SELECT dpe.dpeanoref AS periodo, dsh.tidid1 AS modalidade, SUM(dsh.dshqtde)::integer AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid in (650)
					AND sh.sehstatus <> 'I'
					GROUP BY periodo, modalidade
                    ORDER BY periodo, modalidade";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrB = array();
            $arrTotal = array();
            $arrSituacao = array();
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['periodo']][$dado['modalidade']]['total'][] = $dado['valor'];
                }
                foreach($arrSituacao as $periodo => $sit){
                    $vA = Round((is_array($sit['2211']['total'])?array_sum($sit['2211']['total']):0));
                    $vB = Round((is_array($sit['2210']['total'])?array_sum($sit['2210']['total']):0));
                    $arrX[] = $periodo;
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                    $arrTotal[] =  $vA + $vB;
                }

                $aDados = array(
                    array('name' => 'A Distancia', 'data'=>$arrA),
                    array('name' => 'Presencial', 'data'=>$arrB),
                    array('name' => 'Total de Vagas', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoVagas", "Vagas",300, $formatoValores);
            }
			?>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2890);" width="50%">
			<div>
				<?php exibirTitulo('indicador', 'Número de cursos de graduação nas Universidades Federais'); ?>
			</div>
			<?php
			$sql = "SELECT dpe.dpeanoref AS periodo, dsh.tidid1 AS modalidade, SUM(dsh.dshqtde)::integer AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid in (2890)
					AND sh.sehstatus <> 'I'
					GROUP BY periodo, modalidade
                    ORDER BY periodo, modalidade";
			$arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrB = array();
            $arrC = array();
            $arrD = array();
            $arrE = array();
            $arrF = array();
            $arrG = array();
            $arrH = array();
            $arrTotal = array();
            $arrSituacao = array();
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['periodo']][$dado['modalidade']]['total'][] = $dado['valor'];
                }
                foreach($arrSituacao as $periodo => $sit){
                    $vA = Round((is_array($sit['7804']['total'])?array_sum($sit['7804']['total']):0));
                    $vB = Round((is_array($sit['7803']['total'])?array_sum($sit['7803']['total']):0));
                    $vC = Round((is_array($sit['7801']['total'])?array_sum($sit['7801']['total']):0));
                    $vD = Round((is_array($sit['7802']['total'])?array_sum($sit['7802']['total']):0));
                    $vE = Round((is_array($sit['7800']['total'])?array_sum($sit['7800']['total']):0));
                    $vF = Round((is_array($sit['7799']['total'])?array_sum($sit['7799']['total']):0));
                    $vG = Round((is_array($sit['7798']['total'])?array_sum($sit['7798']['total']):0));
                    $vH = Round((is_array($sit['7797']['total'])?array_sum($sit['7797']['total']):0));
                    $arrX[] = $periodo;
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                    $arrC[] = $vC;
                    $arrD[] = $vD;
                    $arrE[] = $vE;
                    $arrF[] = $vF;
                    $arrG[] = $vG;
                    $arrH[] = $vH;
                    $arrTotal[] =  $vA + $vB + $vC + $vD + $vE + $vF + $vG + $vH;
                }

                $aDados = array(
                    array('name' => 'Servicos', 'data'=>$arrA),
                    array('name' => 'Saude e Bem-estar social', 'data'=>$arrB),
                    array('name' => 'Engenharia, Producao, Construcao', 'data'=>$arrC),
                    array('name' => 'Humanidades e Artes', 'data'=>$arrD),
                    array('name' => 'Educacao', 'data'=>$arrE),
                    array('name' => 'Ciencias Sociais, Negocios e Direito', 'data'=>$arrF),
                    array('name' => 'Ciencias, Matematica, Computacao', 'data'=>$arrG),
                    array('name' => 'Agricultura, Veterinaria', 'data'=>$arrH),
                    array('name' => 'Total de Cursos', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoCursos", "Cursos",300, $formatoValores);
            }
			?>
		</td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(554);" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Concluintes de graduação em Universidades Federais'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, dsh.tidid1 AS modalidade, SUM(dsh.dshqtde)::integer AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid in (554)
					AND sh.sehstatus <> 'I'
					GROUP BY periodo, modalidade
                    ORDER BY periodo, modalidade";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrB = array();
            $arrTotal = array();
            $arrSituacao = array();
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['periodo']][$dado['modalidade']]['total'][] = $dado['valor'];
                }
                foreach($arrSituacao as $periodo => $sit){
                    $vA = Round((is_array($sit['1841']['total'])?array_sum($sit['1841']['total']):0));
                    $vB = Round((is_array($sit['1840']['total'])?array_sum($sit['1840']['total']):0));
                    $arrX[] = $periodo;
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                    $arrTotal[] =  $vA + $vB;
                }

                $aDados = array(
                    array('name' => 'A Distancia', 'data'=>$arrA),
                    array('name' => 'Presencial', 'data'=>$arrB),
                    array('name' => 'Total de Concluintes', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoConcluintes", "Concluintes",300, $formatoValores);
            }
            ?>
        </td>
	</tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2895);" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Investimento Total - Universidades e Hospitais Universitários Federais'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, tid1.tidid AS modalidade, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (2895)
                    AND sh.sehstatus <> 'I'
                    --AND dpe.dpeanoref <= '2013'
                    GROUP BY periodo, modalidade
                    ORDER BY periodo, modalidade";
            $arrDados = $db->carregar( $sql, null, 3200 );
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
                    $vA = Round((is_array($sit['7831']['total'])?array_sum($sit['7831']['total']):0));
                    $vB = Round((is_array($sit['7834']['total'])?array_sum($sit['7834']['total']):0));
                    $vC = Round((is_array($sit['7835']['total'])?array_sum($sit['7835']['total']):0));
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
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoDespesaUniversidades", "Investimento Total",300, $formatoValores);
            }
            ?>
            <br><br>
            <table><tr><td class="bold">*2014: Orçamento Previsto</td></tr></table>
        </td>
    </tr>
	<tr>
		<td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Linha do tempo da criação/federalização das unidades'); ?>
            </div>
            <?php
            $dados = array(
                array('descricao'=>"Epitácio Pessoa<br />1919-1922", 'valor'=>'1'),
                array('descricao'=>"Getúlio Vargas<br />1930-1945 e 1951-1954 ", 'valor'=>'1'),
                array('descricao'=>"Gaspar Dutra<br />1946-1951", 'valor'=>'4'),
                array('descricao'=>"Café Filho<br />1954-1955", 'valor'=>'2'),
                array('descricao'=>"Juscelino Kubitschek<br />1956-1961", 'valor'=>'11'),
                array('descricao'=>"João Goulart<br />1961-1964", 'valor'=>'2'),
                array('descricao'=>"Castelo Branco<br />1964-1967", 'valor'=>'3'),
                array('descricao'=>"Costa e Silva<br />1967-1969", 'valor'=>'6'),
                array('descricao'=>"Emílio G. Médici<br />1969-1974", 'valor'=>'1'),
                array('descricao'=>"Ernesto Geisel<br />1974-1979", 'valor'=>'1'),
                array('descricao'=>"João B. Figueiredo<br />1979-1985", 'valor'=>'3'),
                array('descricao'=>"José Sarney<br />1985-1990", 'valor'=>'2'),
                array('descricao'=>"Itamar Franco<br />1992-1995", 'valor'=>'2'),
                array('descricao'=>"Fernando Henrique Cardoso<br />1995-2003", 'valor'=>'6'),
                array('descricao'=>"Luiz Inácio Lula da Silva<br />2003-2010", 'valor'=>'14'),
                array('descricao'=>"Dilma Rousseff<br />2011-2014", 'valor'=>'4'),
            );

            $config = array('rotation'=>'-45', 'dataLabels'=>'true');
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($dados, 'graficoColunaExpansao', null, null, $formatoValores, null, 'estrategico.php?modulo=principal/popuplistauniversidades&acao=A', 1000, 600, null, 300, $config);
            ?>
            <table width="100%">
                <tr>
                    <td class="titulo_box bold" align="center"><?php echo number_format(63,0,",",".") ?> universidades</td>
                </tr>
            </table>
		</td>
	</tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Linha do tempo da criação/federalização das unidades - Câmpus'); ?>
            </div>
            <?php
/*
            $sql = "SELECT
                    CASE
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1889-11-01' AND '1891-11-01' THEN 'Manuel Deodoro da Fonseca'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1891-11-02' AND '1894-11-01' THEN 'Floriano Vieira Peixoto'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1894-11-02' AND '1898-11-01' THEN 'Prudente José de Morais e Barros'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1898-11-02' AND '1902-11-01' THEN 'Manuel Ferraz de Campos Sales'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1902-11-02' AND '1906-11-01' THEN 'Francisco de Paula Rodrigues Alves'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1906-11-02' AND '1909-06-01' THEN 'Afonso Augusto Moreira Pena'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1909-06-02' AND '1910-11-01' THEN 'Nilo Procópio Peçanha'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1910-11-02' AND '1914-11-01' THEN 'Hermes Rodrigues da Fonseca'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1914-11-02' AND '1918-11-01' THEN 'Venceslau Brás Pereira Gomes'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1918-11-02' AND '1919-07-01' THEN 'Delfim Moreira da Costa Ribeiro'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1919-07-02' AND '1922-11-01' THEN 'Epitácio Pessoa'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1922-11-02' AND '1926-11-01' THEN 'Artur da Silva Bernardes'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1926-11-02' AND '1930-11-01' THEN 'Washington Luís Pereira de Souza'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1930-11-02' AND '1945-10-01' THEN 'Getúlio Vargas'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1945-10-02' AND '1946-01-01' THEN 'José Linhares'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1946-01-02' AND '1951-01-01' THEN 'Gaspar Dutra'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1951-01-02' AND '1954-08-01' THEN 'Getúlio Vargas'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1954-08-02' AND '1955-11-01' THEN 'Café Filho'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1955-11-02' AND '1956-01-01' THEN 'Nereu de Oliveira Ramos'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1956-01-02' AND '1961-01-01' THEN 'Juscelino Kubitschek'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-01-02' AND '1961-08-01' THEN 'Jânio Quadros'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-08-02' AND '1961-09-01' THEN 'Pascoal Ranieri Mazzilli'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-09-02' AND '1964-04-01' THEN 'João Goulart'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1964-04-02' AND '1967-03-01' THEN 'Castelo Branco'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1967-03-02' AND '1969-10-01' THEN 'Costa e Silva'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1969-10-02' AND '1974-03-01' THEN 'Emílio G. Médici'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1974-03-02' AND '1979-03-01' THEN 'Ernesto Geisel'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1979-03-02' AND '1985-03-01' THEN 'João B. Figueiredo'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1985-03-02' AND '1990-03-01' THEN 'José Sarney'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1990-03-02' AND '1992-12-01' THEN 'Fernando Collor'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1992-12-02' AND '1995-01-01' THEN 'Itamar Franco'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1995-01-02' AND '2003-01-01' THEN 'Fernando Henrique Cardoso'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '2003-01-02' AND '2011-01-01' THEN 'Luiz Inácio Lula da Silva'
                    ELSE 'Dilma Rousseff'
                    END AS descricao,
                    CASE
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1889-11-01' AND '1891-11-01' THEN 1
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1891-11-02' AND '1894-11-01' THEN 2
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1894-11-02' AND '1898-11-01' THEN 3
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1898-11-02' AND '1902-11-01' THEN 4
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1902-11-02' AND '1906-11-01' THEN 5
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1906-11-02' AND '1909-06-01' THEN 6
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1909-06-02' AND '1910-11-01' THEN 7
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1910-11-02' AND '1914-11-01' THEN 8
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1914-11-02' AND '1918-11-01' THEN 9
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1918-11-02' AND '1919-07-01' THEN 10
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1919-07-02' AND '1922-11-01' THEN 11
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1922-11-02' AND '1926-11-01' THEN 12
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1926-11-02' AND '1930-11-01' THEN 13
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1930-11-02' AND '1945-10-01' THEN 14
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1945-10-02' AND '1946-01-01' THEN 15
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1946-01-02' AND '1951-01-01' THEN 16
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1951-01-02' AND '1954-08-01' THEN 17
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1954-08-02' AND '1955-11-01' THEN 18
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1955-11-02' AND '1956-01-01' THEN 19
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1956-01-02' AND '1961-01-01' THEN 20
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-01-02' AND '1961-08-01' THEN 21
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-08-02' AND '1961-09-01' THEN 22
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-09-02' AND '1964-04-01' THEN 23
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1964-04-02' AND '1967-03-01' THEN 24
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1967-03-02' AND '1969-10-01' THEN 25
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1969-10-02' AND '1974-03-01' THEN 26
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1974-03-02' AND '1979-03-01' THEN 27
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1979-03-02' AND '1985-03-01' THEN 28
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1985-03-02' AND '1990-03-01' THEN 29
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1990-03-02' AND '1992-12-01' THEN 30
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1992-12-02' AND '1995-01-01' THEN 31
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1995-01-02' AND '2003-01-01' THEN 32
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '2003-01-02' AND '2011-01-01' THEN 33
                    ELSE 34
                    END AS ordem,
                    COUNT(0) AS valor
                FROM entidade.entidade entcampus
                INNER JOIN entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                INNER JOIN entidade.funentassoc a on a.fueid = fn.fueid
                INNER JOIN entidade.entidade entinst on entinst.entid = a.entid
                INNER JOIN entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
                INNER JOIN academico.campus cmp on cmp.entid = entcampus.entid
                INNER JOIN academico.existencia exi on exi.exiid = cmp.exiid
                WHERE exi.exiid NOT IN (7)
                GROUP BY descricao, ordem
                ORDER BY ordem";
            $dados = $db->carregar( $sql );

            $config = array('rotation'=>'-45', 'dataLabels'=>'true');
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($dados, 'graficoColunaExpansaoCampus', null, null, $formatoValores, null, 'estrategico.php?modulo=principal/popuplistauniversidades&acao=A&campus=1', 1000, 600, null, 300, $config);

*/
                $sql = "SELECT
                    CASE
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1889-11-01' AND '1891-11-01' THEN 'Manuel Deodoro da Fonseca'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1891-11-02' AND '1894-11-01' THEN 'Floriano Vieira Peixoto'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1894-11-02' AND '1898-11-01' THEN 'Prudente José de Morais e Barros'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1898-11-02' AND '1902-11-01' THEN 'Manuel Ferraz de Campos Sales'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1902-11-02' AND '1906-11-01' THEN 'Francisco de Paula Rodrigues Alves'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1906-11-02' AND '1909-06-01' THEN 'Afonso Augusto Moreira Pena'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1909-06-02' AND '1910-11-01' THEN 'Nilo Procópio Peçanha'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1910-11-02' AND '1914-11-01' THEN 'Hermes Rodrigues da Fonseca'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1914-11-02' AND '1918-11-01' THEN 'Venceslau Brás Pereira Gomes'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1918-11-02' AND '1919-07-01' THEN 'Delfim Moreira da Costa Ribeiro'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1919-07-02' AND '1922-11-01' THEN 'Epitácio Pessoa'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1922-11-02' AND '1926-11-01' THEN 'Artur da Silva Bernardes'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1926-11-02' AND '1930-11-01' THEN 'Washington Luís Pereira de Souza'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1930-11-02' AND '1945-10-01' THEN 'Getúlio Vargas'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1945-10-02' AND '1946-01-01' THEN 'José Linhares'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1946-01-02' AND '1951-01-01' THEN 'Gaspar Dutra'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1951-01-02' AND '1954-08-01' THEN 'Getúlio Vargas (1951-1954)'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1954-08-02' AND '1955-11-01' THEN 'Café Filho'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1955-11-02' AND '1956-01-01' THEN 'Nereu de Oliveira Ramos'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1956-01-02' AND '1961-01-01' THEN 'Juscelino Kubitschek'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-01-02' AND '1961-08-01' THEN 'Jânio Quadros'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-08-02' AND '1961-09-01' THEN 'Pascoal Ranieri Mazzilli'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-09-02' AND '1964-04-01' THEN 'João Goulart'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1964-04-02' AND '1967-03-01' THEN 'Castelo Branco'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1967-03-02' AND '1969-10-01' THEN 'Costa e Silva'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1969-10-02' AND '1974-03-01' THEN 'Emílio G. Médici'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1974-03-02' AND '1979-03-01' THEN 'Ernesto Geisel'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1979-03-02' AND '1985-03-01' THEN 'João B. Figueiredo'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1985-03-02' AND '1990-03-01' THEN 'José Sarney'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1990-03-02' AND '1992-12-01' THEN 'Fernando Collor'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1992-12-02' AND '1995-01-01' THEN 'Itamar Franco'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1995-01-02' AND '2003-01-01' THEN 'Fernando Henrique Cardoso'
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '2003-01-02' AND '2011-01-01' THEN 'Luiz Inácio Lula da Silva'
                    ELSE 'Dilma Rousseff'
                    END AS descricao,
                    CASE
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1889-11-01' AND '1891-11-01' THEN 1
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1891-11-02' AND '1894-11-01' THEN 2
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1894-11-02' AND '1898-11-01' THEN 3
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1898-11-02' AND '1902-11-01' THEN 4
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1902-11-02' AND '1906-11-01' THEN 5
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1906-11-02' AND '1909-06-01' THEN 6
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1909-06-02' AND '1910-11-01' THEN 7
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1910-11-02' AND '1914-11-01' THEN 8
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1914-11-02' AND '1918-11-01' THEN 9
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1918-11-02' AND '1919-07-01' THEN 10
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1919-07-02' AND '1922-11-01' THEN 11
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1922-11-02' AND '1926-11-01' THEN 12
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1926-11-02' AND '1930-11-01' THEN 13
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1930-11-02' AND '1945-10-01' THEN 14
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1945-10-02' AND '1946-01-01' THEN 15
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1946-01-02' AND '1951-01-01' THEN 16
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1951-01-02' AND '1954-08-01' THEN 17
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1954-08-02' AND '1955-11-01' THEN 18
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1955-11-02' AND '1956-01-01' THEN 19
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1956-01-02' AND '1961-01-01' THEN 20
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-01-02' AND '1961-08-01' THEN 21
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-08-02' AND '1961-09-01' THEN 22
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1961-09-02' AND '1964-04-01' THEN 23
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1964-04-02' AND '1967-03-01' THEN 24
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1967-03-02' AND '1969-10-01' THEN 25
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1969-10-02' AND '1974-03-01' THEN 26
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1974-03-02' AND '1979-03-01' THEN 27
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1979-03-02' AND '1985-03-01' THEN 28
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1985-03-02' AND '1990-03-01' THEN 29
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1990-03-02' AND '1992-12-01' THEN 30
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1992-12-02' AND '1995-01-01' THEN 31
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '1995-01-02' AND '2003-01-01' THEN 32
                        WHEN substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' BETWEEN '2003-01-02' AND '2011-01-01' THEN 33
                    ELSE 34
                    END AS ordem,
                    COUNT(0) AS valor,
                    CASE
                    WHEN cmp.cmpsituacao = 'F' THEN 'Funcionando'
                    WHEN cmp.cmpsituacao = 'N' THEN 'Nao Funcionando'
                    ELSE 'Não Informado'
                    END AS situacao
                FROM entidade.entidade entcampus
                INNER JOIN entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                INNER JOIN entidade.funentassoc a on a.fueid = fn.fueid
                INNER JOIN entidade.entidade entinst on entinst.entid = a.entid
                INNER JOIN entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
                INNER JOIN academico.campus cmp on cmp.entid = entcampus.entid
                INNER JOIN academico.existencia exi on exi.exiid = cmp.exiid
                WHERE exi.exiid NOT IN (7)
                GROUP BY descricao, ordem, situacao
                ORDER BY ordem";
            $arrDados = $db->carregar( $sql );

            $dadosAgrupados = agruparDadosGrafico($arrDados, 'descricao', 'situacao', 'valor');
            $config = array('rotation'=>'-45', 'dataLabels'=>'true');
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaEstado', null, null, $formatoValores, null, "estrategico.php?modulo=principal/popuplistauniversidades&acao=A&campus=1", 1000, 600, null, 300);

            $totalCampus = 0;
            foreach($arrDados as $dado){
                $totalCampus += $dado['valor'];
            }
            ?>
            <table width="100%">
                <tr>
                    <td class="titulo_box bold" align="center"><?php echo number_format($totalCampus,0,",",".") ?> câmpus</td>
                </tr>
            </table>
        </td>
    </tr>
	<tr>
		<td class="fundo_td_laranja" colspan="2">
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