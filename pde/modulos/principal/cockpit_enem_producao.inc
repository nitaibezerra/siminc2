<?PHP
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=9" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<body onload="refreshAutomatico();">
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.js"></script>
<script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.pieRenderer.min.js"></script>
<script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.donutRenderer.min.js"></script>
<script language="javascript" type="text/javascript" src="js/estrategico.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
<script>
	jQuery.noConflict();
	jQuery('.div_fotos_interno').cycle({
		fx: 'scrollDown'
	});

	atualizaUsuario();
</script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.css" />
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
  	.tabela_box_azul_escuro td{background-color:#204481;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
  	.fundo_td_azul{background-color:#2B86EE}
  	.fundo_td_azul:hover{background-color:#01A2D8}
  	.fundo_td_verde{background-color:#0F6D39}
  	.fundo_td_verde:hover{background-color:#32CD32}
   	.fundo_td_laranja{background-color:#EE9200}
  	.fundo_td_laranja:hover{background-color:#EBB513}
  	.fundo_td_vermelho{background-color:#BB0000}
  	.fundo_td_vermelho:hover{background-color:#DD0000}
  	.fundo_td_roxo{background-color:#5333AD}
  	.fundo_td_roxo:hover{background-color:#6A5ACD}
  	.fundo_td_azul_escuro{background-color:#152D56}
  	.fundo_td_azul_escuro:hover{background-color:#1F3864}
  	.div_fotos{background-color:#204481;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:450px;margin-bottom:2px}
  	 body{background-image:url('../imagens/degrade-fundo-preto.png');background-repeat:repeat-x;background-color:#330000;margin:0px;padding-top:0px;}
  	.fundo_titulo{background-image:url('../imagens/cockpit/fundo_enem.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
  	.numero{text-align:right;font-size:11px;font-family:verdana}
  	.center{text-align:center}
  	.titulo_box a{color:#FFFFFF;text-decoration:none;}
  	.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
  	.div_fotos_interno{width:98%;height:85px}
  	.bold{font-weight:bold}
  	.esquerda{text-align:left;}
  	.link{cursor:pointer}
 </style>
<style>#mask{position:absolute;left:0;top:0;z-index:9000;background-color:#000;display:none;}#boxes .window{position:fixed;left:0;top:0;width:440px;height:200px;display:none;z-index:9999;padding:20px;}#boxes #dialog{width:375px;height:300px;padding:10px;background-color:#ffffff;}</style>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
		<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
        	<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_enem_producao&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<script>
	function buscar(busca){
		window.open('/painel/painel.php?modulo=principal/painel_controle&acao=A&buscacockpit='+busca,'Observações','scrollbars=yes,height=800,width=1500,status=no,toolbar=no,menubar=no,location=no');
	}
	function abreAtividade(detativ)
	{
		window.location='estrategico.php?modulo=principal/cockpit_enem_producao&acao=A&atividade='+detativ;
	}
	function abreRiskManager(nivel,etapasimec, situacao, tipo)
	{
		url = "/pde/estrategico.php?modulo=principal/enem_producao_riskmanager&acao=A&nivel=" + nivel + "&situacao=" + situacao + "&tipo=" + tipo + "&etapasimec=" + etapasimec;
	 	window.open(url,'Risk Manager','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}
</script>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr style=cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_enem&acao=A';">
		<td class="fundo_titulo" style="text-align:center" colspan="2" ><div style="margin:28px" >Monitoramento ENEM 2012</div></td>
	</tr>
	<tr>
		<td class="fundo_td" width="50%">
			<div>
                <img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" >Por Atividade<br/></a>
                </div>
            </div>
            <?php
            $sql = "SELECT
						CASE UPPER(no_atividade)
						WHEN 'LOCAÇÃO DE ESPAÇOS' THEN '1-Locação de Espaços'
						WHEN 'PRODUÇÃO DE PROVAS NORMAIS' THEN '2-Produção de Provas Normais'
						WHEN 'PRODUÇÃO DE PROVAS ESPECIAIS' THEN '3-Produção de Provas Especiais'
						WHEN 'ENSALAMENTO' THEN '4-Ensalamento'
						WHEN 'MALOTAMENTO DO MATERIAL ADMINISTRATIVO' THEN '5-Kit do Material Administrativo'
						WHEN 'MALOTAMENTO DAS PROVAS' THEN '6-Malotamento das Provas'
						END AS atividadepai,
						no_atividade,
						CASE UPPER(no_atividade)
						WHEN 'LOCAÇÃO DE ESPAÇOS' THEN '1'
						WHEN 'PRODUÇÃO DE PROVAS NORMAIS' THEN '2'
						WHEN 'PRODUÇÃO DE PROVAS ESPECIAIS' THEN '3'
						WHEN 'ENSALAMENTO' THEN '4'
						WHEN 'MALOTAMENTO DO MATERIAL ADMINISTRATIVO' THEN '5'
						WHEN 'MALOTAMENTO DAS PROVAS' THEN '6'
						END AS idatividade,
						COALESCE(SUM(nu_meta), NULL, 0) AS meta,
						COALESCE(SUM(nu_execucao), NULL, 0) AS execucao,
						(COALESCE(SUM(nu_execucao), NULL, 0) * 100)/COALESCE(SUM(nu_meta), NULL, 0) AS percentual
					FROM enem.tb_mapa_enem_2012
					where no_atividade in ( 'Produção de Provas Normais', 'Produção de Provas Especiais', 'ENSALAMENTO', 'MALOTAMENTO DO MATERIAL ADMINISTRATIVO',
								'MALOTAMENTO DAS PROVAS', 'LOCAÇÃO DE ESPAÇOS'
								)
					GROUP BY no_atividade
					ORDER BY atividadepai";
            $arrAtividade = $db->carregar($sql);
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="bold" >Atividade</td>
					<td class="center bold" >Meta</td>
					<td class="center bold" >Executado</td>
					<td class="center bold" >% Executado</td>
				</tr>
				<?php foreach($arrAtividade as $att): ?>
					<tr>
						<td ><span style="cursor:pointer;" onclick="abreAtividade('<?=$att['no_atividade'] ?>');"><?php echo str_replace(array("1-","2-","3-","4-","5-","6-","7-","8-"),"",$att['atividadepai']) ?></td>
						<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($att['execucao'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
					</tr>
				<?php endforeach; ?>
			</table>
		</td>
		<td class="fundo_td_azul_escuro" width="50%" rowspan="2">
			<?php
				switch ( $_REQUEST['atividade'] ){
					case 1:
						$stWhere = "Locação de Espaços";
					break;
					case 2:
						$stWhere = "Gráfica";
					break;
					case 3:
						$stWhere = "Ensalamento";
					break;
					case 4:
						$stWhere = "Malotamento do Material Administrativo";
					break;
					case 5:
						$stWhere = "Transporte de Material Administrativo Ida";
					break;
					case 6:
						$stWhere = "Malotamento das Provas";
					break;
					case 7:
						$stWhere = "Transporte de Provas Ida";
					break;
					case 8:
						$stWhere = "Transporte de Provas Volta";
					break;
					default:
						$stWhere = "Locação de Espaços";
				}

			if ( $_REQUEST['atividade'] )
				{	$stWhere = $_REQUEST['atividade'];	}
			else
				{	$stWhere = 'LOCAÇÃO DE ESPAÇOS';	}

            $sql = "SELECT
						UPPER(nome_atividade_pai) AS nomeatividade,
						COALESCE(sgl_uf, NULL, 'Não Informado') AS estado,
						COALESCE(SUM(nu_meta), NULL, 0) AS meta,
						COALESCE(SUM(nu_execucao), NULL, 0) AS execucao,
						to_char(dt_final_meta, 'DD/MM/YYYY') as datameta,
						(COALESCE(SUM(nu_execucao), NULL, 0) * 100)/COALESCE(SUM(nu_meta), NULL, 0) AS percentual
					FROM enem.tb_mapa_enem_2012
					WHERE no_atividade ILIKE '%{$stWhere}%'
					GROUP BY nomeatividade, estado, dt_final_meta
					ORDER BY nomeatividade, estado";
            $arrItem = $db->carregar($sql);

			if ( $_REQUEST['atividade'] == 'MALOTAMENTO DO MATERIAL ADMINISTRATIVO')
				{
					$descricaoMeta = 'Metas/Aluno';
					$descricaoExec = 'Executado/Aluno';
				}
			else
				{
					$descricaoMeta = 'Meta';
					$descricaoExec = 'Executado';
				}

			?>
			<div>
                <img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" ><?php echo $stWhere ?><br/></a>
                </div>
            </div>
			<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="bold" >UF</td>
					<td class="center bold" ><?php echo $descricaoMeta ?></td>
					<td class="center bold" >Data da Meta</td>
					<td class="center bold" ><?php echo $descricaoExec ?></td>
					<td class="center bold" >% Executado</td>
				</tr>
				<?php foreach($arrItem as $att): ?>
					<tr>
						<td><?php echo $att['estado'] ?></td>
						<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
						<td class="numero" ><?php echo $att['datameta'] ?></td>
						<td class="numero" ><?php echo number_format($att['execucao'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
					</tr>
				<?php endforeach; ?>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" >
			<div>
                <img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" >Risk Manager<br/></a>
                </div>
            </div>
            <?php
            $sql = "SELECT
            			SUBSTR(etapasimec,7,length(etapasimec)) as etapasimec,
						SUM(baixo) as baixo,
						SUM(atencao) as atencao,
						SUM(elevado) as elevado,
						SUM(alto) as alto,
						SUM(severo) as severo
					FROM (
						SELECT
							etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%gt%' and statusalerta = 'Aberto' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%pessoas%' and statusalerta = 'Aberto' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%operação%' and statusalerta = 'Aberto' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%execução%' and etapasimec not ilike '%ppl%' and statusalerta = 'Aberto' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%encerramento%' and etapasimec not ilike '%ppl%' and statusalerta = 'Aberto' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%operação%' and etapasimec ilike '%ppl%' and statusalerta = 'Aberto' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%execução%' and etapasimec ilike '%ppl%' and statusalerta = 'Aberto' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
						UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec = '9.0 - NÃO POSSUI' and statusalerta = 'Aberto' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					) as foo
					GROUP BY etapasimec
					ORDER BY SUBSTR(etapasimec,1,1)";
            $arrRiskmanager = $db->carregar($sql);
            ?>



			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" colspan="7" >ALERTAS EM TRATAMENTO</td>
				</tr>
				<tr>
					<td class="bold" rowspan="2">ETAPAS DO ENEM/2012</td>
					<td class="center bold" colspan="6" >NÍVEIS DE ALERTA</td>
				</tr>
				<tr>
					<td class="center bold" style="color:rgb(8,255,2)">BAIXO</td>
					<td class="center bold" style="color:rgb(0,41,255)">ATENÇÃO</td>
					<td class="center bold" style="color:rgb(255,255,0)">ELEVADO</td>
					<td class="center bold" style="color:rgb(255,145,0)">ALTO</td>
					<td class="center bold" style="color:rgb(255,2,2)">SEVERO</td>
					<td class="center bold" ">TOTAL</td>
				</tr>
				<?php if( is_array( $arrRiskmanager ) ){?>
				<?php foreach($arrRiskmanager as $atr):

						$tot = $atr['baixo'] + $atr['atencao'] + $atr['elevado'] + $atr['alto'] + $atr['severo'];
				?>
					<tr>
						<td>
							<?php echo $atr['etapasimec'] ?>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Baixo','<?=$atr['etapasimec']?>', 'Aberto', 1);">
								<?= $atr['baixo'] == 0 ? '-' : number_format($atr['baixo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Atenção','<?=$atr['etapasimec']?>', 'Aberto', 1);">
								<?= $atr['atencao'] == 0 ? '-' : number_format($atr['atencao'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Elevado','<?=$atr['etapasimec']?>', 'Aberto', 1);">
								<?= $atr['elevado'] == 0 ? '-' : number_format($atr['elevado'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Alto','<?=$atr['etapasimec']?>', 'Aberto', 1);">
								<?= $atr['alto'] == 0 ? '-' 	 : number_format($atr['alto'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Severo','<?=$atr['etapasimec']?>', 'Aberto', 1);">
								<?= $atr['severo'] == 0 ? '-'  : number_format($atr['severo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;">
								<?= $tot ?>
							</span>
						</td>
					</tr>
				<?php
					$totbaixo += $atr['baixo'];
					$totatencao += $atr['atencao'];
					$totelevado += $atr['elevado'];
					$totalto += $atr['alto'];
					$totmedio += $atr['severo'];
					$totgeral += $atr['baixo'] + $atr['atencao'] + $atr['elevado'] + $atr['alto'] + $atr['severo'];
					endforeach;

					?>
				<tr>
					<td class="center bold" >TOTAL</td>
					<td class="center bold" style="color:rgb(8,255,2)"><?=$totbaixo?></td>
					<td class="center bold" style="color:rgb(0,41,255)"><?=$totatencao?></td>
					<td class="center bold" style="color:rgb(255,255,0)"><?=$totelevado?></td>
					<td class="center bold" style="color:rgb(255,145,0)"><?=$totalto?></td>
					<td class="center bold" style="color:rgb(255,2,2)"><?=$totmedio?></td>
					<td class="center bold"><?=$totgeral?></td>
				</tr>
				<?php }else{?>
					<tr>
						<td  class="center" style="color:red" colspan="7">Não possui registros.</td>
					</tr>
				<?php }
					$totbaixo = 0;
					$totatencao = 0;
					$totelevado = 0;
					$totalto = 0;
					$totmedio = 0;
					$totgeral = 0;
					?>
			</table>

			<table class="fundo_td" cellpadding="2" cellspacing="1" width="100%" >
				<tr><td>&nbsp;</td></tr>
			</table>

            <?php
            $sql = "SELECT
            			SUBSTR(etapasimec,7,length(etapasimec)) as etapasimec,
						SUM(baixo) as baixo,
						SUM(atencao) as atencao,
						SUM(elevado) as elevado,
						SUM(alto) as alto,
						SUM(severo) as severo
					FROM (
						SELECT
							etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%gt%' and statusalerta = 'Fechado' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%pessoas%' and statusalerta = 'Fechado' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%operação%' and statusalerta = 'Fechado' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%execução%' and etapasimec not ilike '%ppl%' and statusalerta = 'Fechado' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%encerramento%' and etapasimec not ilike '%ppl%' and statusalerta = 'Fechado' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%operação%' and etapasimec ilike '%ppl%' and statusalerta = 'Fechado' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%execução%' and etapasimec ilike '%ppl%' and statusalerta = 'Fechado' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
						UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec = '9.0 - NÃO POSSUI' and statusalerta = 'Fechado' and tipo = 'Alerta Externo'
					 	GROUP BY
					 		etapasimec, nivelalerta
					) as foo
					GROUP BY etapasimec
					ORDER BY SUBSTR(etapasimec,1,1)";
            $arrRiskmanager = $db->carregar($sql);
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" colspan="7" >ALERTAS TRATADOS</td>
				</tr>
				<tr>
					<td class="bold" rowspan="2">ETAPAS DO ENEM/2012</td>
					<td class="center bold" colspan="6" >NÍVEIS DE ALERTA</td>
				</tr>
				<tr>
					<td class="center bold" style="color:rgb(8,255,2)">BAIXO</td>
					<td class="center bold" style="color:rgb(0,41,255)">ATENÇÃO</td>
					<td class="center bold" style="color:rgb(255,255,0)">ELEVADO</td>
					<td class="center bold" style="color:rgb(255,145,0)">ALTO</td>
					<td class="center bold" style="color:rgb(255,2,2)">SEVERO</td>
					<td class="center bold" ">TOTAL</td>
				</tr>
				<?php if( is_array( $arrRiskmanager ) ){?>
				<?php foreach($arrRiskmanager as $atr):

						$tot = $atr['baixo'] + $atr['atencao'] + $atr['elevado'] + $atr['alto'] + $atr['severo'];
				?>
					<tr>
						<td>
							<?php echo $atr['etapasimec'] ?>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Baixo','<?=$atr['etapasimec']?>', 'Fechado', 2);">
								<?= $atr['baixo'] == 0 ? '-' : number_format($atr['baixo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Atenção','<?=$atr['etapasimec']?>', 'Fechado', 2);">
								<?= $atr['atencao'] == 0 ? '-' : number_format($atr['atencao'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Elevado','<?=$atr['etapasimec']?>', 'Fechado', 2);">
								<?= $atr['elevado'] == 0 ? '-' : number_format($atr['elevado'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Alto','<?=$atr['etapasimec']?>', 'Fechado', 2);">
								<?= $atr['alto'] == 0 ? '-' 	 : number_format($atr['alto'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Severo','<?=$atr['etapasimec']?>', 'Fechado', 2);">
								<?= $atr['severo'] == 0 ? '-'  : number_format($atr['severo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;">
								<?= $tot ?>
							</span>
						</td>
					</tr>
				<?php
					$totbaixo += $atr['baixo'];
					$totatencao += $atr['atencao'];
					$totelevado += $atr['elevado'];
					$totalto += $atr['alto'];
					$totmedio += $atr['severo'];
					$totgeral += $atr['baixo'] + $atr['atencao'] + $atr['elevado'] + $atr['alto'] + $atr['severo'];
					endforeach;

					?>
				<tr>
					<td class="center bold" >TOTAL</td>
					<td class="center bold" style="color:rgb(8,255,2)"><?=$totbaixo?></td>
					<td class="center bold" style="color:rgb(0,41,255)"><?=$totatencao?></td>
					<td class="center bold" style="color:rgb(255,255,0)"><?=$totelevado?></td>
					<td class="center bold" style="color:rgb(255,145,0)"><?=$totalto?></td>
					<td class="center bold" style="color:rgb(255,2,2)"><?=$totmedio?></td>
					<td class="center bold"><?=$totgeral?></td>
				</tr>
				<?php }else{?>
					<tr>
						<td  class="center" style="color:red" colspan="7">Não possui registros.</td>
					</tr>
				<?php
					}
					$totbaixo = 0;
					$totatencao = 0;
					$totelevado = 0;
					$totalto = 0;
					$totmedio = 0;
					$totgeral = 0;

					?>
			</table>


			<table class="fundo_td" cellpadding="2" cellspacing="1" width="100%" >
				<tr><td>&nbsp;</td></tr>
			</table>

            <?php
            $sql = "SELECT
            			SUBSTR(etapasimec,7,length(etapasimec)) as etapasimec,
						SUM(baixo) as baixo,
						SUM(atencao) as atencao,
						SUM(elevado) as elevado,
						SUM(alto) as alto,
						SUM(severo) as severo
					FROM (
						SELECT
							etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%gt%' and ( statusalerta = 'Fechado' or statusalerta = 'Aberto' ) and ( tipo = 'Alerta Externo' or tipo = 'Alerta Interno' )
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%pessoas%' and ( statusalerta = 'Fechado' or statusalerta = 'Aberto' ) and ( tipo = 'Alerta Externo' or tipo = 'Alerta Interno' )
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%operação%' and ( statusalerta = 'Fechado' or statusalerta = 'Aberto' ) and ( tipo = 'Alerta Externo' or tipo = 'Alerta Interno' )
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%execução%' and etapasimec not ilike '%ppl%' and ( statusalerta = 'Fechado' or statusalerta = 'Aberto' ) and ( tipo = 'Alerta Externo' or tipo = 'Alerta Interno' )
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%encerramento%' and etapasimec not ilike '%ppl%' and ( statusalerta = 'Fechado' or statusalerta = 'Aberto' ) and ( tipo = 'Alerta Externo' or tipo = 'Alerta Interno' )
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%operação%' and etapasimec ilike '%ppl%' and ( statusalerta = 'Fechado' or statusalerta = 'Aberto' ) and ( tipo = 'Alerta Externo' or tipo = 'Alerta Interno' )
					 	GROUP BY
					 		etapasimec, nivelalerta
					 	UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec ilike '%execução%' and etapasimec ilike '%ppl%' and ( statusalerta = 'Fechado' or statusalerta = 'Aberto' ) and ( tipo = 'Alerta Externo' or tipo = 'Alerta Interno' )
					 	GROUP BY
					 		etapasimec, nivelalerta
						UNION ALL
					 	SELECT
					 		etapasimec,
						  	CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
						  	CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
						  	CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
						  	CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
						  	CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
					 	FROM
					 		enem.riskmanager_2012
					 	WHERE
					 		etapasimec = '9.0 - NÃO POSSUI' and ( statusalerta = 'Fechado' or statusalerta = 'Aberto' ) and ( tipo = 'Alerta Externo' or tipo = 'Alerta Interno' )
					 	GROUP BY
					 		etapasimec, nivelalerta
					) as foo
					GROUP BY etapasimec
					ORDER BY SUBSTR(etapasimec,1,1)";
            $arrRiskmanager = $db->carregar($sql);
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" colspan="7" >TODAS AS OCORRÊNCIAS</td>
				</tr>
				<tr>
					<td class="bold" rowspan="2">ETAPAS DO ENEM/2012</td>
					<td class="center bold" colspan="6" >NÍVEIS DE ALERTA</td>
				</tr>
				<tr>
					<td class="center bold" style="color:rgb(8,255,2)">BAIXO</td>
					<td class="center bold" style="color:rgb(0,41,255)">ATENÇÃO</td>
					<td class="center bold" style="color:rgb(255,255,0)">ELEVADO</td>
					<td class="center bold" style="color:rgb(255,145,0)">ALTO</td>
					<td class="center bold" style="color:rgb(255,2,2)">SEVERO</td>
					<td class="center bold" ">TOTAL</td>
				</tr>
				<?php if( is_array( $arrRiskmanager ) ){?>
				<?php foreach($arrRiskmanager as $atr):

						$tot = $atr['baixo'] + $atr['atencao'] + $atr['elevado'] + $atr['alto'] + $atr['severo'];
				?>
					<tr>
						<td>
							<?php echo $atr['etapasimec'] ?>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Baixo','<?=$atr['etapasimec']?>', 'Fechado', 3);">
								<?= $atr['baixo'] == 0 ? '-' : number_format($atr['baixo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Atenção','<?=$atr['etapasimec']?>', 'Fechado', 3);">
								<?= $atr['atencao'] == 0 ? '-' : number_format($atr['atencao'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Elevado','<?=$atr['etapasimec']?>', 'Fechado', 3);">
								<?= $atr['elevado'] == 0 ? '-' : number_format($atr['elevado'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Alto','<?=$atr['etapasimec']?>', 'Fechado', 3);">
								<?= $atr['alto'] == 0 ? '-' 	 : number_format($atr['alto'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Severo','<?=$atr['etapasimec']?>', 'Fechado', 3);">
								<?= $atr['severo'] == 0 ? '-'  : number_format($atr['severo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;">
								<?= $tot ?>
							</span>
						</td>
					</tr>
				<?php
					$totbaixo += $atr['baixo'];
					$totatencao += $atr['atencao'];
					$totelevado += $atr['elevado'];
					$totalto += $atr['alto'];
					$totmedio += $atr['severo'];
					$totgeral += $atr['baixo'] + $atr['atencao'] + $atr['elevado'] + $atr['alto'] + $atr['severo'];
					endforeach;

					?>
				<tr>
					<td class="center bold" >TOTAL</td>
					<td class="center bold" style="color:rgb(8,255,2)"><?=$totbaixo?></td>
					<td class="center bold" style="color:rgb(0,41,255)"><?=$totatencao?></td>
					<td class="center bold" style="color:rgb(255,255,0)"><?=$totelevado?></td>
					<td class="center bold" style="color:rgb(255,145,0)"><?=$totalto?></td>
					<td class="center bold" style="color:rgb(255,2,2)"><?=$totmedio?></td>
					<td class="center bold"><?=$totgeral?></td>
				</tr>
				<?php }else{?>
					<tr>
						<td  class="center" style="color:red" colspan="7">Não possui registros.</td>
					</tr>
				<?php }?>
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
</script>
</body>
</html>