<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

if($_REQUEST['campus_escolhido']){
    getDadosCampus($_REQUEST['campus_escolhido']);
    die();
}
if($_REQUEST['campus_escolhido_scrool']){
    getDadosScrool($_REQUEST['campus_escolhido_scrool']);
    die();
}

if($_REQUEST['demanda_escolhida']){
    getDadosObras($_REQUEST['campus_escolhido2'], $_REQUEST['demanda_escolhida']);
    die();
}
$_SESSION['academico']['entid'] = $_REQUEST['entid'];

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
	<script language="javascript" type="text/javascript" src="/obras/js/obras.js"></script>

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
                    jQuery('#dados_campus').load('estrategico.php?modulo=principal/cockpit_detalhe_universidade&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&campus_escolhido='+jQuery(this).attr('entid'));
					jQuery('#dados_scrool').load('estrategico.php?modulo=principal/cockpit_detalhe_universidade&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&campus_escolhido_scrool='+jQuery(this).attr('entid'));
                }
            });

            jQuery('.selecionar-demanda').click(function(){
                if(jQuery(this).attr('dinid')){
                    jQuery('#dados_demandas').html('Carregando...');
                    jQuery('#dados_demandas').load('estrategico.php?modulo=principal/cockpit_detalhe_universidade&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&campus_escolhido2='+jQuery(this).attr('entid')+'&demanda_escolhida='+jQuery(this).attr('dinid'));
                }
            });
        });
		atualizaUsuario();
		
		function abreObras1(obrid){
    		window.open('/obras/obras.php?modulo=principal/cadastro&acao=A&obrid='+obrid);
    	}

        function abreDemanda(dinid){
            window.open('/academico/academico.php?modulo=principal/mpa/popupMpaInfraestrutura&acao=A&dinid='+dinid);
        }
	</script>
</head>
<body>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
		<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
        	<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_detalhe_universidade&acao=A&entid=<?=$_REQUEST['entid'] ?>';">
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
						) AS entreitor ON entreitor.entid = ent.entid
					LEFT JOIN (
						SELECT entinterlocutor.entnome, ent.entid
						FROM entidade.entidade ent
						INNER JOIN entidade.funentassoc fea ON fea.entid = ent.entid
						INNER JOIN entidade.funcaoentidade fue ON fue.fueid = fea.fueid AND fue.fuestatus = 'A' AND fue.funid = 40
						INNER JOIN entidade.entidade entinterlocutor ON entinterlocutor.entid = fue.entid
						) AS entinterlocutor ON entinterlocutor.entid = ent.entid
					WHERE ent.entid = " . $_REQUEST['entid'];
			$arrDados = $db->pegaLinha($sql);
			$nomeUniversidade = $arrDados['sigla'] . ' - ' . $arrDados['universidade'];
			?>
			<table width="100%" border="0" >
				<tr>
					<td width="10%" valign="center">
						<br><img width="110px" src="cockpit/images/universidades/<?=strtolower($arrDados['sigla'])?>.jpg">
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
		<td class="fundo_padrao" >
			<div>
				<?php exibirTitulo('indicador', 'Indicadores Acadêmicos'); ?>
			</div>
			<?php 
			$sql = "select idaigc as igc, idacpcnumcursos as cpc, idacpcperc as percentual
					from academico.indicadoresacademicos
					where entid = {$_REQUEST['entid']}";
			$dados = $db->carregar($sql);
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
			<tr>
				<th class="center bold" >IGC</th>
				<th class="center bold" >CPC Nº de Cursos Avaliados</th>
				<th class="center bold" >Cursos com CPC < 3</th>
			</tr>
			<?php
			if ($dados)
				{
				$count=-1;
				foreach($dados as $dado):
				$count++;
				?>
				<tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
					<td class="numero" ><?php echo $dado['igc'] ?></td>
					<td class="numero" ><?php echo $dado['cpc'] ?></td>
					<td class="numero" ><?php echo $dado['percentual'] ?></td>
				</tr>
			<?php endforeach; 
			}else{
				echo "<tr><td class='center'>-</td><td class='center'>-</td><td class='center'>-</td></tr>";
			}?>
          </table>
		</td>
        <td class="fundo_padrao" colspan="1" >
            <?php
            $sql="SELECT sh.indid AS indicador, dpe.dpeanoref AS periodo
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND sh.indid IN (650, 553, 554, 675)";
            $arrDados = $db->carregar($sql);
            $periodo = 0;
            $periodosIguais = true;
            if($arrDados){
                foreach($arrDados as $dado){
                    if($periodo==0){
                        $periodo = $dado['periodo'];
                    }else{
                        if($dado['periodo'] == $periodo){
                            $periodo = $dado['periodo'];
                        }else{
                            $periodosIguais = false;
                        }
                    }
                    $arrPeriodo[$dado['indicador']] = $dado['periodo'];
                }
            }
            ?>
            <div>
                <?php exibirTitulo('indicador', 'Censo da<br>Educação<br>Superior'.($periodosIguais ? '<br>('.$periodo.')' : '')); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 650
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(650);">
                    <td class="bold subtitulo_box">Vagas em graduação<?=(!$periodosIguais ? '<br>('.$arrPeriodo[650].')' :'')?></td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 553
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(553);">
                    <td class="bold subtitulo_box">Matrículas em graduação<?=(!$periodosIguais ? '<br>('.$arrPeriodo[553].')' :'')?></td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 554
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(554);">
                    <td class="bold subtitulo_box">Concluintes de graduação<?=(!$periodosIguais ? '<br>('.$arrPeriodo[554].')' :'')?></td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 675
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(675);">
                    <td class="bold subtitulo_box">Número de cursos de graduação<?=(!$periodosIguais ? '<br>('.$arrPeriodo[675].')' :'')?></td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3180);">
            <div>
                <?php exibirTitulo('indicador', 'Percentual de<br>Cotas'); ?>
            </div>
            <?php
            $sql = "SELECT descricao, (SUM(valortotal)/SUM(valor))*100 AS valor
                    FROM(
                        SELECT dpe.dpedsc as descricao, dshqtde as valor, CASE WHEN dsh.tidid1 NOT IN (8894) THEN dshqtde ELSE 0 END valortotal
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::INTEGER
                        WHERE sh.indid in (3180)
                        AND sh.sehstatus <> 'I'
                        AND sh.dpeid IN (1222,1248) --1º Semestre/2013 e 1º Semestre/2014
                        AND ies.entid = {$_REQUEST['entid']}
                    ) AS FOO
                    GROUP BY descricao
                    ORDER BY descricao";
            $dados = $db->carregar($sql);
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th class="center bold" >Semestre</th>
                    <th class="center bold" >Total</th>
                </tr>
                <?php
                if ($dados)
                {
                    $count=-1;
                    foreach($dados as $dado):
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td class="" ><?php echo $dado['descricao'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['valor'],2,",",".") ?>%</td>
                        </tr>
                    <?php endforeach;
                }else{
                    echo "<tr><td class='center'>-</td><td class='center'>-</td></tr>";
                }?>
            </table>
        </td>
        <td class="fundo_padrao"  >
            <div>
                <?php exibirTitulo('indicador', 'SISU<br>Edição 1/2014'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = 1248 --1º Semestre/2014
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
						AND sh.dpeid = 1248 --1º Semestre/2014
						AND sh.indid = 1435
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(1435);">
                    <td class="bold subtitulo_box">Inscrições</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
            <br>
            <br>
            <div>
                <?php exibirTitulo('indicador', 'SISU<br>Edição 2/2014'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = 1249 --2º Semestre/2014
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
						AND sh.dpeid = 1249 --2º Semestre/2014
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
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(3150);"  >
			<div>
				<?php exibirTitulo('indicador', 'Adesão à EBSERH'); ?>
			</div>
            <?php
            $sql = "SELECT DISTINCT
                        e.entnome,
                        CASE
                            WHEN dsh.tidid1 IN (8671) AND dsh.tidid2 IN (8674) THEN 'Em processo - Fase I (Dimensionamento)'
                            WHEN dsh.tidid1 IN (8671) AND dsh.tidid2 IN (8675) THEN 'Em processo - Fase II (DEST/MP)'
                            WHEN dsh.tidid1 IN (8671) AND dsh.tidid2 IN (8676) THEN 'Em processo - Fase III (Trâmites finais)'
                            WHEN dsh.tidid1 IN (8670) THEN 'Contrato assinado'
                            WHEN dsh.tidid1 IN (8668) THEN 'Sem adesão'
                        ELSE tid1.tiddsc
                        END AS situacao
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    INNER JOIN entidade.entidade e ON e.entid = dsh.entid
                    INNER JOIN entidade.funcaoentidade fn ON fn.entid = e.entid
                    INNER JOIN entidade.funentassoc fa ON fa.fueid = fn.fueid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND sh.indid = 3150
                    AND fa.entid = {$_REQUEST['entid']}
                    ORDER BY e.entnome";
            $arrDados = $db->carregar($sql);
            if($arrDados){
            ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Hospital</th>
                    <th>Situação</th>
                </tr>
                <?php
                $count=-1;
                foreach($arrDados as $dado):
                    $count++;
                    ?>
                    <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                        <td class="" ><?php echo $dado['entnome'] ?></td>
                        <td class="" ><?php echo $dado['situacao'] ?></td>
                    </tr>
                <?php endforeach; ?>
                </table>
            <?php
            }
            ?>
		</td>
        <td class="fundo_padrao"  >
            <div>
                <?php exibirTitulo('indicador', 'Mais Médicos'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT COUNT(0) AS valor
						FROM maismedicos.universidade uni
						INNER JOIN maismedicos.universidademunicipio unm ON uni.uniid = unm.uniid
						INNER JOIN maismedicos.medico mdc ON mdc.muncod = unm.muncod
						WHERE unistatus = 'A'
						AND uni.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(2291);">
                    <td class="bold subtitulo_box">Instituições Supervisoras</td>
                    <td class="numero" ><?php echo ($valor > 0 ? 'SIM' : 'NÃO') ?></td>
                </tr>
                <?php
                $sql = "SELECT
							COUNT(DISTINCT tut.tutid) AS valor
						FROM maismedicos.universidade uni
						INNER JOIN maismedicos.tutor tut ON tut.uniid = uni.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade IS TRUE
						WHERE unistatus = 'A'
						AND uni.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(2292);">
                    <td class="bold subtitulo_box">Tutores</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
                <?php
                $sql = "SELECT
							COUNT(DISTINCT tut.tutid) AS valor
						FROM maismedicos.universidade uni
						INNER JOIN maismedicos.tutor tut ON tut.uniid = uni.uniid and tut.tuttipo = 'S' and tut.tutstatus = 'A' and tut.tutvalidade IS TRUE
						WHERE unistatus = 'A'
						AND uni.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(2293);">
                    <td class="bold subtitulo_box">Supervisores</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
                <?php
                $sql = "SELECT
							count(distinct mdc.mdcid) AS dshqtde
						FROM maismedicos.universidade uni
						inner join maismedicos.universidademunicipio unm  ON uni.uniid = unm.uniid
						inner join maismedicos.medico mdc ON mdc.muncod = unm.muncod
						WHERE unistatus = 'A'
						AND uni.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(2792);">
                    <td class="bold subtitulo_box">Médicos</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao"  >
            <div>
                <?php exibirTitulo('indicador', 'Inglês sem Fronteiras'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT COUNT(dsh.dshqtde) as valor
                            FROM painel.seriehistorica sh
                            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                            INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
                            WHERE sh.sehstatus <> 'I'
                            AND sh.indid = 2025
                            AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(2025);">
                    <td class="bold subtitulo_box">Centro aplicador</td>
                    <td class="numero" ><?php echo ($valor > 0 ? 'SIM' : 'NÃO') ?></td>
                </tr>
                <?php
                $sql = "SELECT COUNT(dsh.dshqtde) as valor
                            FROM painel.seriehistorica sh
                            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                            INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
                            WHERE sh.sehstatus <> 'I'
                            AND sh.indid = 2024
                            AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(2024);">
                    <td class="bold subtitulo_box">Núcleo de líguas</td>
                    <td class="numero" ><?php echo ($valor > 0 ? 'SIM' : 'NÃO') ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao"  >
            <?php
            $sql = "SELECT MIN(dpe.dpeanoref) || ' - ' || MAX(dpe.dpeanoref) as periodo
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I'
                        AND sh.indid = 2019";
            $edicao = $db->pegaUm($sql);
            ?>
            <div>
                <?php exibirTitulo('indicador', 'Bolsa<br>Permanência<br>('.$edicao.')'); ?>
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
                <?php exibirTitulo('indicador', 'PNAES'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor, MIN(dpe.dpeanoref) || ' - ' || MAX(dpe.dpeanoref) as periodo
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I'
                        AND sh.indid = 331
                        AND ies.entid = {$_REQUEST['entid']}";
                $arrDados = $db->pegaLinha($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(331);">
                    <td class="bold subtitulo_box">Número de benefícios<br>(<?=$arrDados['periodo']?>)</td>
                    <td class="numero" ><?php echo number_format($arrDados['valor'],0,",",".") ?></td>
                </tr>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor, MAX(dpe.dpeanoref) as periodo
                            FROM painel.seriehistorica sh
                            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                            INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
                            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                            WHERE sh.sehstatus <> 'I'
                            AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                            AND sh.indid = 780
                            AND ies.entid = {$_REQUEST['entid']}";
                $arrDados = $db->pegaLinha($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(780);">
                    <td class="bold subtitulo_box">Recursos disponibilizados<br>(<?=$arrDados['periodo']?>)</td>
                    <td class="numero" >R$<?php echo number_format($arrDados['valor'],2,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao"  >
            <?php
            $sql="SELECT dpe.dpeanoref AS edicao
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND sh.indid = 781";
            $edicao = $db->pegaUm($sql);
            ?>
            <div>
                <?php exibirTitulo('indicador', 'Incluir<br>('.$edicao.')'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 66
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                //RETIRADO TEMPORARIAMENTE A PEDIDO DA JAANA - 17/07/2014
                ?>
                <!--
                <tr class="link" onclick="abreIndicadorPopUp(66);">
                    <td class="bold subtitulo_box">Projetos Financiados</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
                -->
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 781
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(781);">
                    <td class="bold subtitulo_box">Recursos disponibilizados</td>
                    <td class="numero" ><?php echo "R$" . number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" colspan="2" >
            <?php
            $sql="SELECT MIN(dpe.dpeanoref) || ' - ' || MAX(dpe.dpeanoref) AS edicao
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid = 334";
            $edicao = $db->pegaUm($sql);
            ?>
            <div>
                <?php exibirTitulo('indicador', 'PROEXT<br>('.$edicao.')'); ?>
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
<?php /* ?>
		<td class="fundo_padrao"  >
			<div>
				<?php exibirTitulo('indicador', 'PARFOR'); ?>
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
                //RETIRADO TEMPORARIAMENTE A PEDIDO DA JAANA - 17/07/2014
				?>
				<tr class="link" onclick="abreIndicadorPopUp(1367);">
					<td class="bold subtitulo_box">Turmas ativas</td>
					<td class="numero" >-</td>
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
                //RETIRADO TEMPORARIAMENTE A PEDIDO DA JAANA - 17/07/2014
				?>
				<tr class="link" onclick="abreIndicadorPopUp(1513);">
					<td class="bold subtitulo_box">Matrículas ativas</td>
					<td class="numero" >-</td>
				</tr>
			</table>
		</td>
 <?php */ ?>
	</tr>
    <tr>
        <td class="fundo_padrao"  >
            <?php
            $sql="SELECT MIN(dpe.dpeanoref) || ' - ' || MAX(dpe.dpeanoref) AS edicao
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid = 108";
            $edicao = $db->pegaUm($sql);
            ?>
            <div>
                <?php exibirTitulo('indicador', 'PIBID<br>('.$edicao.')'); ?>
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
            <?php
            $sql="SELECT MIN(dpe.dpeanoref) || ' - ' || MAX(dpe.dpeanoref) AS edicao
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.indid = 580";
            $edicao = $db->pegaUm($sql);
            ?>
            <div>
                <?php exibirTitulo('indicador', 'PET<br>('.$edicao.')'); ?>
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
        <td class="fundo_padrao" colspan="2" >
            <?php
            $sql="SELECT sh.indid AS indicador, dpe.dpeanoref AS periodo
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND sh.indid IN (305, 558)";
            $arrDados = $db->carregar($sql);
            $periodo = 0;
            $periodosIguais = true;
            if($arrDados){
                foreach($arrDados as $dado){
                    if($periodo==0){
                        $periodo = $dado['periodo'];
                    }else{
                        if($dado['periodo'] == $periodo){
                            $periodo = $dado['periodo'];
                        }else{
                            $periodosIguais = false;
                        }
                    }
                    $arrPeriodo[$dado['indicador']] = $dado['periodo'];
                }
            }
            ?>
            <div>
                <?php exibirTitulo('indicador', 'UAB'.($periodosIguais ? '<br>('.$periodo.')' : '')); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT COUNT(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 305
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(305);">
                    <td class="bold subtitulo_box">Integrante do Sistema UAB<?=(!$periodosIguais ? '<br>('.$arrPeriodo[305].')' :'')?></td>
                    <td class="numero"><?php echo ($valor > 0 ? 'SIM' : 'NÃO') ?></td>
                </tr>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 558
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(558);">
                    <td class="bold subtitulo_box">Cursando e provável formando<?=(!$periodosIguais ? '<br>('.$arrPeriodo[558].')' :'')?></td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 558
						AND tidid1 IN (1870) --FORMADO
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(558);">
                    <td class="bold subtitulo_box">Formados<?=(!$periodosIguais ? '<br>('.$arrPeriodo[558].')' :'')?></td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" colspan="2" >
            <?php
            $sql="SELECT sh.indid AS indicador, dpe.dpeanoref AS periodo
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND sh.indid IN (2961, 726, 574)";
            $arrDados = $db->carregar($sql);
            $periodo = 0;
            $periodosIguais = true;
            if($arrDados){
                foreach($arrDados as $dado){
                    if($periodo==0){
                        $periodo = $dado['periodo'];
                    }else{
                        if($dado['periodo'] == $periodo){
                            $periodo = $dado['periodo'];
                        }else{
                            $periodosIguais = false;
                        }
                    }
                    $arrPeriodo[$dado['indicador']] = $dado['periodo'];
                }
            }
            ?>
            <div>
                <?php exibirTitulo('indicador', 'Pós-graduação<br><i>stricto sensu</i>'.($periodosIguais ? '<br>('.$periodo.')' : '')); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
<?php /* ?>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 2961
						AND ies.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(2961);">
                    <td class="bold subtitulo_box">Número de cursos</td>
                    <td class="numero" ><?php echo ($valor ? number_format($valor,0,",",".") : '-') ?></td>
                </tr>
 <?php */ ?>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.iepg iep ON dsh.iepid = iep.iepid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 726
						AND iep.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(726);">
                    <td class="bold subtitulo_box">Número de matrículas<?=(!$periodosIguais ? '<br>('.$arrPeriodo[726].')' :'')?></td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.iepg iep ON dsh.iepid = iep.iepid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 574
						AND iep.entid = {$_REQUEST['entid']}";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(574);">
                    <td class="bold subtitulo_box">Número de titulados<?=(!$periodosIguais ? '<br>('.$arrPeriodo[574].')' :'')?></td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%"  cellspacing="4" cellpadding="1" class="tabela_painel">
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(553);" width="50%">
			<div>
				<?php exibirTitulo('indicador', 'Matrículas em Graduação<br>('.$nomeUniversidade.')'); ?>
			</div>
			<?php
			$sql = "SELECT dpe.dpeanoref AS periodo, dsh.tidid1 AS modalidade, SUM(dsh.dshqtde)::integer AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
					INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid in (553)
					AND sh.sehstatus <> 'I'
					AND ies.entid = {$_REQUEST['entid']}
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
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(650);" width="50%"  >
			<div>
				<?php exibirTitulo('indicador', 'Vagas em Graduação<br>('.$nomeUniversidade.')'); ?>
			</div>
			<?php
			$sql = "SELECT dpe.dpeanoref AS periodo, dsh.tidid1 AS modalidade, SUM(dsh.dshqtde)::integer AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
					INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid in (650)
					AND sh.sehstatus <> 'I'
					AND ies.entid = {$_REQUEST['entid']}
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
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2890);" colspan="1"  >
			<div>
				<?php exibirTitulo('indicador', 'Número de cursos de graduação<br>('.$nomeUniversidade.')'); ?>
			</div>
			<?php
			$sql = "SELECT dpe.dpeanoref AS periodo, dsh.tidid1 AS modalidade, SUM(dsh.dshqtde)::integer AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
					INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid in (2890)
					AND sh.sehstatus <> 'I'
					AND ies.entid = {$_REQUEST['entid']}
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
                <?php exibirTitulo('indicador', 'Concluintes de Graduação<br>('.$nomeUniversidade.')'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, dsh.tidid1 AS modalidade, SUM(dsh.dshqtde)::integer AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.ies ies ON dsh.dshcod::integer = ies.iesid
					INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid in (554)
					AND sh.sehstatus <> 'I'
					AND ies.entid = {$_REQUEST['entid']}
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

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoConcluintes", "Concluintes",300, $formatoValores);
            }
            ?>
        </td>
	</tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2895);" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Investimentos <br>('.$nomeUniversidade.')'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, tid1.tidid AS modalidade, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN entidade.entidade e ON e.entunicod::integer = dsh.unicod::integer
                    WHERE sh.indid in (2895)
                    AND sh.sehstatus <> 'I'
                    --AND dpe.dpeanoref <= '2013'
                    AND e.entid = {$_REQUEST['entid']}
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

                echo geraGraficoLinha($aDados, $arrX, "graficoDespesaUniversidades", "Investimentos",300, $formatoValores);
            }
            ?>
            <br><br>
            <table><tr><td class="bold">*2014: Orçamento Previsto</td></tr></table>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%"  cellspacing="4" cellpadding="1" class="tabela_painel">
	<tr>
        <td width="30%" class="fundo_padrao">
            <?php
            $sql = "SELECT entcampus.entnome AS campus, entcampus.entid
                    FROM entidade.entidade entcampus
                        INNER JOIN entidade.funcaoentidade fn ON entcampus.entid = fn.entid AND fn.funid = 18 AND entcampus.entstatus = 'A' AND fn.fuestatus = 'A'
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
function getDadosCampus($entid){
    global $db;

    $sql = "SELECT entnome AS campus FROM entidade.entidade WHERE entid = '$entid'";
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
			inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
			inner join academico.campus cmp on cmp.entid = entcampus.entid
			inner join academico.existencia exi on exi.exiid = cmp.exiid
			left join academico.novoscampus nvc on nvc.entidcampus = cmp.entid
			where entcampus.entid = '$entid'
			order by status, situacao, instalacao";
	$arrDadosCampus = $db->pegaLinha($sql);
    ?>
    <table class="" cellspacing="4" cellpadding="5" width="100%" >
        <tr>
            <td class="fundo_padrao">
				<h1><?=$nomeCampus ?></h1>
				<h2>
				Status: <?=$arrDadosCampus['status']?><br>
				Situação: <?=$arrDadosCampus['situacao']?><br>
				Instalações: <?=$arrDadosCampus['instalacao']?><br>
                <?php if($arrDadosCampus['situacao'] != 'FUNCIONANDO'){ ?>
				    Previsão de Funcionamento: <?=$arrDadosCampus['previsao']?>
                <?php } ?>
				</h2>
			</td>
        </tr>
        <tr>
            <td class="fundo_padrao">
                <div>
                    <?php exibirTitulo('executiverel', 'Resumo do Campus'); ?>
                </div>
                <?php
                $sql = "SELECT descricao, valor
                        FROM(
                            SELECT 'Matrículas' AS descricao, SUM(dsh.dshqtde) AS valor
                            FROM painel.seriehistorica sh
                            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                            WHERE sh.indid in (3094)
                            AND sh.sehstatus <> 'I'
                            AND entid = '$entid'
                            GROUP BY descricao
                        UNION ALL
                            SELECT 'Vagas' AS descricao, SUM(dsh.dshqtde) AS valor
                            FROM painel.seriehistorica sh
                            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                            WHERE sh.indid in (3175)
                            AND sh.sehstatus <> 'I'
                            AND entid = '$entid'
                            GROUP BY descricao
                        UNION ALL
                            SELECT 'Concluintes' AS descricao, SUM(dsh.dshqtde) AS valor
                            FROM painel.seriehistorica sh
                            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                            WHERE sh.indid in (3176)
                            AND sh.sehstatus <> 'I'
                            AND entid = '$entid'
                            GROUP BY descricao
                        UNION ALL
                            SELECT 'Cursos' AS descricao, SUM(dsh.dshqtde) AS valor
                            FROM painel.seriehistorica sh
                            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                            WHERE sh.indid in (3177)
                            AND sh.sehstatus <> 'I'
                            AND entid = '$entid'
                            GROUP BY descricao
                        ) AS FOO
                        ORDER BY descricao";
                $arrDados = $db->carregar($sql,null,3200);
                $count = -1;
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <th>&nbsp;</th>
                        <th>Total</th>
                    </tr>
                    <?php
                    if ($arrDados){
                        foreach($arrDados as $dado):
                            $count++;
                            ?>
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                <td class="" ><?php echo $dado['descricao'] ?></td>
                                <td class="numero" ><?php echo number_format($dado['valor'],0,",",".") ?></td>
                            </tr>
                        <?php
                        endforeach;
                    }else{
                        echo "<tr><td class='center'>-</td><td class='center'>-</td></tr>";
                    }
                    ?>
                </table>
            </td>
        </tr>
        <tr>
            <td class="fundo_padrao">
				<div>
					<?php exibirTitulo('configs', 'Necessidade de Infraestrutura', 'Demandas cadastradas em 2013'); ?>
				</div>
                <?php
                $sql = "SELECT
                                din.dinid,
								dindsc AS demandas,
								spedsc AS projeto,
								dinsitlicitacao, 
								din.speid, 
								CASE dinsitlicitacao
									WHEN 'L' THEN 'Licitado'
									WHEN 'N' THEN 'Não Licitado'
									WHEN 'E' THEN 'Em Licitação'
								ELSE '<center>-</center>'
								END AS status,
								to_char(dindtprevisaotermino, 'DD/MM/YYYY') AS licitacao,
								(dinvlr2014 + dinvlr2015 + dinvlr2016 + dinvlr2017) AS valor
							FROM academico.demandainfra din
							LEFT JOIN academico.sitprojetoexec spe ON spe.speid = din.speid
							WHERE din.dinstatus = 'A' and entidcampus = '$entid'
							ORDER BY demandas, projeto";
                $arrDados = $db->carregar($sql,null,3200);
                $count = -1;
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <th class="center bold" >Demandas</th>
                        <th class="center bold" >Projeto Executivo</th>
                        <th class="center bold" >Licitação</th>
                        <th class="center bold" >Previsão do fim da licitação</th>
                        <th class="center bold" >Valor</th>
                    </tr>
                    <?php
                    if ($arrDados){
                        foreach($arrDados as $dado):
                            $count++;
							$mudaCor = "";
							if($dado['speid']!= 1 || $dado['dinsitlicitacao']!= 'L'){
								//$mudaCor = "<font color='red'>";
							}
                            ?>
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?> link selecionar-demanda" entid='<?=$entid?>' dinid='<?=$dado['dinid']?>'>
                                <td class="" ><?php echo $mudaCor. $dado['demandas'] ?></td>
                                <td class="" ><?php echo $mudaCor. $dado['projeto'] ?></td>
                                <td class="" ><?php echo $mudaCor. $dado['status'] ?></td>
                                <td class="" ><?php echo $mudaCor. $dado['licitacao'] ?></td>
                                <td class="numero" ><?php echo $mudaCor. ($dado['valor'] ? "R$ " . number_format($dado['valor'],2,",","."):'<center>-</center>') ?></td>
                            </tr>
                        <?php
                        endforeach;
                    }else{
					echo "<tr><td class='center'>-</td><td class='center'>-</td><td class='center'>-</td><td class='center'>-</td><td class='center'>-</td></tr>";
					}
                    ?>
                </table>
            </td>
        </tr>

        <tr>
            <td class="fundo_padrao">
                <div>
                    <?php exibirTitulo('obras', 'Situação das Obras'); ?>
                </div>
                <div id="dados_demandas">
                <?php
                getDadosObras($entid, $dinid);
                ?>
                </div>
            </td>
        </tr>
    </table>
<?php
}

function getDadosObras($entid, $dinid){

    global $db;

    if($dinid){
        $where = "AND din.dinid = {$dinid}";
    }

    $sql = "SELECT
                obr.obrid,
                obr.obrdesc AS nome,
                tpo.tpodsc AS tipologia,
                to_char(obrdtinicio, 'DD/MM/YYYY') AS datainicio,
                CASE
                    WHEN max(ta.tradtinclusao) IS NOT NULL THEN to_char(max(ta.traterminoexec), 'DD/MM/YYYY')
                    ELSE to_char(obr.obrdttermino, 'DD/MM/YYYY')
                END as datatermino,
                sto.stodesc AS situacao,
                obr.obrpercexec as percentual,
                obr.obrvlrrealobra as valor,
                din.dindsc AS demandas,
                din.dinid
            FROM obras.obrainfraestrutura obr
            LEFT JOIN obras.termoaditivo ta ON ta.obrid = obr.obrid
            INNER JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
            LEFT JOIN obras.tipologiaobra tpo ON tpo.tpoid = obr.tpoid
            LEFT JOIN academico.demandaobras dob ON dob.obrid = obr.obrid
            LEFT JOIN academico.demandainfra din ON din.dinid = dob.dinid
            WHERE obr.obsstatus = 'A'
            AND obr.entidcampus = {$entid}
            $where
            AND obr.stoid NOT IN (99) --Em Planejamento pelo Proponente
            GROUP BY obr.obrid, nome, tipologia, obr.obrdtinicio, obr.obrdttermino, sto.stodesc, obr.obrpercexec, obr.obrvlrrealobra, din.dindsc, din.dinid
            ORDER BY demandas, nome";
    $arrDados = $db->carregar($sql,null,3200);
    $count = -1;
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr>

            <th class="center bold" >Nome</th>
            <th class="center bold" >Tipologia</th>
            <th class="center bold" >Data de início</th>
            <th class="center bold" >Data de término</th>
            <th class="center bold" >Situação</th>
            <th class="center bold" >%</th>
            <th class="center bold" >Valor</th>
            <th class="center bold" >Demandas</th>
        </tr>
        <?php
        if ($arrDados){
            foreach($arrDados as $dado):
                $count++;
                ?>
                <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">

                    <td class="link" onclick="abreObras1(<?=$dado['obrid']?>);" ><?php echo $dado['nome'] ?></td>
                    <td class="link" onclick="abreObras1(<?=$dado['obrid']?>);" ><?php echo $dado['tipologia'] ?></td>
                    <td class="link" onclick="abreObras1(<?=$dado['obrid']?>);" ><?php echo $dado['datainicio'] ?></td>
                    <td class="link" onclick="abreObras1(<?=$dado['obrid']?>);" ><?php echo $dado['datatermino'] ?></td>
                    <td class="link" onclick="abreObras1(<?=$dado['obrid']?>);" ><?php echo $dado['situacao'] ?></td>
                    <td class="numero link" onclick="abreObras1(<?=$dado['obrid']?>);"><?php echo number_format($dado['percentual'],2,",",".") ?></td>
                    <td class="numero link" onclick="abreObras1(<?=$dado['obrid']?>);" >R$ <?php echo number_format($dado['valor'],2,",",".") ?></td>
                    <?php if($dado['demandas']) { ?>
                        <td class="link" onclick="abreDemanda(<?=$dado['dinid']?>);" ><?php echo $dado['demandas'] ?></td>
                    <?php }else{ ?>
                        <td class="center">-</td>
                    <?php } ?>
                </tr>
            <?php
            endforeach;
        }else{
            echo "<tr><td class='center'>-</td><td class='center'>-</td><td class='center'>-</td><td class='center'>-</td><td class='center'>-</td><td class='center'>-</td><td class='center'>-</td><td class='center'>-</td></tr>";
        }
        ?>
    </table>
<?php
}
?>