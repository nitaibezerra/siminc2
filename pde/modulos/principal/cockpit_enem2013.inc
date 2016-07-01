<?PHP
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( substr($_SESSION['favurl'],0,56) );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=9" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<body>
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
  	.fundo_td{background-color:#104302}
  	.titulo_pagina{font-weight:bold;font-size:20px;color:#FFFFFF}
  	.titulo_box{font-weight:bold;font-size:18px;color:#FFFFFF;margin-top:15px;text-shadow:#000000 0px 1px 2px}
  	.subtitulo_box{font-weight:normal;font-size:10px;color:#FFFFFF}
  	.fundo_td:hover {background-color:#155703}
  	.fundo_td{text-align:left;vertical-align:top;}
  	.tabela_painel{font-weight:bold;font-size:8px;color:#FFFFFF;font-family:fantasy}
  	.lista_metas{float:left}
  	#busca{background: none repeat scroll 0% 0% rgb(255, 255, 255); width:400px;border-width: 1px; border-style: solid; border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204); color: rgb(0, 0, 0); font: 18px arial,sans-serif bold; height: 35px;}
  	.tabela_box{color:#FFFFFF;}
  	.tabela_box td{background-color:#0F6D39;text-shadow:#000000 0px 2px 2px}
  	.tabela_box_azul td{background-color:#63B8FF;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
  	.tabela_box_azul_escuro td{background-color:#204481;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
  	.fundo_td_azul{background-color:#01A2D8}
  	.fundo_td_azul:hover{background-color:#2B86CC}
  	.fundo_td_azul_hover{background-color:#2B86CC}
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
	.fundo_td_azul_escuro{text-align:left;vertical-align:top;}
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
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_enem2013&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<script>
	function abreIndicadores(atiprojeto)
	{
		window.open('/pde/estrategico.php?modulo=principal/painel_estrategico&acao=A&atiprojeto='+atiprojeto,'Indicadores','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}
	function abreRelatorio(params)
	{
		window.open('/pde/enem.php?modulo=relatorio_enem/relatorio_checklist&acao=A&'+params,'Relatorio','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}
	function abreIndicadorPopUp(indid)
 	{
 		var url = "../painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&indid=" + indid + "&abreMapa=1&cockpit=1";
 		window.open(url,'Indicador','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
 	}
	function buscar(busca){
		window.open('/painel/painel.php?modulo=principal/painel_controle&acao=A&buscacockpit='+busca,'Observações','scrollbars=yes,height=800,width=1500,status=no,toolbar=no,menubar=no,location=no');
	}
	function abreTipo(tipo)
	{
		window.location='estrategico.php?modulo=principal/cockpit_enem2013&acao=A&tipo='+tipo;
	}
	function abreRiskManager(nivel,etapasimec, situacao, tipo, origem)
	{
		url = "/pde/estrategico.php?modulo=principal/enem_producao_riskmanager2013&acao=A&nivel=" + nivel + "&situacao=" + situacao + "&tipo=" + tipo + "&etapasimec=" + etapasimec + "&origem=" + origem + "&ano=2013";
	 	window.open(url,'Risk Manager','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}
	function abreAtividade(id_atividade)
	{
		window.location='estrategico.php?modulo=principal/cockpit_enem2013&acao=A&tipo=3&id_atividade='+id_atividade;
	}
</script>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<?php
	//Tipos
	//1 - INSCRIÇÕES
	//2 - MONITORAMENTO/CERTIFICAÇÃO
	//3 - OPERAÇÃO LOGÍSTICA
	if($_REQUEST['tipo']){
		$tipo = $_REQUEST['tipo'];
	}else{
		$tipo = 1;
	}
	?>
	<!-- Título-->
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Monitoramento ENEM 2013</div></td>
	</tr>
	<!-- Fim Título-->
	<tr>
		<td class="<?=($tipo==1)?'fundo_td_azul_hover':'fundo_td_azul'?> link" valign="top" width="34%" onclick="abreTipo(1);">
		<div>
			<img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
			<div style="float:left;" class="titulo_box" >INSCRIÇÕES</div>
		</div>
		</td>
		<td class="<?=($tipo==2)?'fundo_td_azul_hover':'fundo_td_azul'?> link" valign="top" width="33%" onclick="abreTipo(2);">
		<div>
			<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
			<div style="float:left;" class="titulo_box" >MONITORAMENTO / CERTIFICAÇÃO</div>
		</div>
		</td>
		<td class="<?=($tipo==3)?'fundo_td_azul_hover':'fundo_td_azul'?> link" valign="top" width="33%" onclick="abreTipo(3);">
		<div>
			<img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
			<div style="float:left;" class="titulo_box" >OPERAÇÃO LOGÍSTICA</div>
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<?php
	//========================================================================================================================================
	//INSCRIÇÕES
	//========================================================================================================================================
	if($tipo==1){
		$sql = "SELECT SUM(total_uf) as total FROM enem.ts_enem_inscricao_2013";
		$Inscricao = $db->pegaUm($sql);
		?>
		<tr>
			<td class="fundo_td_laranja" valign="top" width="34%" >
				<?php
				//RETIRAR APÓS A CORREÇÃO DA CARGA
				$correcao = 0;
				$sql = "SELECT to_char(data_atualizacao, 'DD/MM/YYYY') as dataatualizacao, to_char(data_atualizacao, 'HH24:MI') as hora
					FROM enem.ts_enem_inscricao_2013
					ORDER BY dataatualizacao DESC
					LIMIT 1";
				$arrInscricao = $db->pegaLinha($sql);
				 ?>
				<div>
					<img style="float:left" src="../imagens/icones/icons/indicador.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" ><?php echo number_format($Inscricao,0,",",".") ?> Inscrições<br/>
						<span class="subtitulo_box" >Atualizado em : <?php echo $arrInscricao['dataatualizacao']?> às <?php echo $arrInscricao['hora'] ?></span>
						</a>
					</div>
				</div>
			</td>
			<td class="fundo_td" width="33%" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Por Categoria<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT (SUM(sem_pagamento) + SUM(carencia_indeferida) + SUM(pagamento_efetuado)) AS pagantes, (SUM(carencia_aceita) + SUM(inscricao_gratuita)) AS isentos,
						(SUM(sem_pagamento) + SUM(carencia_indeferida) + SUM(pagamento_efetuado) + SUM(carencia_aceita) + SUM(inscricao_gratuita)) AS total
						FROM enem.ts_enem_inscricao_2013";
				$arrIns = $db->pegaLinha($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Categoria</td>
						<td class="center bold" >Inscrições</td>
					</tr>
					<tr>
						<td>Pagantes</td>
						<td class="numero" ><?php echo number_format($arrIns['pagantes'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Isentos de Pagamento</td>
						<td class="numero" ><?php echo number_format($arrIns['isentos'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($arrIns['total'],0,",",".") ?></td>
					</tr>
				</table>
			</td>
			<td class="fundo_td_azul_escuro" rowspan="2" width="33%" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Por Região<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT no_regiao, SUM(total_uf) as total
					FROM enem.ts_enem_inscricao_2013
					group by no_regiao
					order by no_regiao";
				$arrPR = $db->carregar($sql);
				?>
				<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Região</td>
						<td class="center bold" >Quantidade</td>
					</tr>
					<?php
					if($arrPR){
						foreach($arrPR as $pr): ?>
						<tr>
							<td class="" ><?php echo $pr['no_regiao'] ?></td>
							<td class="numero" ><?php $total_regiao+=$pr['total']; echo number_format($pr['total'],0,",",".") ?></td>
						</tr>
						<?php
						endforeach;
					}
					?>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($total_regiao,0,",",".") ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="fundo_td">
				<?php
				$sql = "SELECT sum(pediu_certificado) FROM enem.ts_enem_inscricao_2013";
				$total_certificado = $db->pegaUm($sql);
				?>
				<div>
					<img style="float:left" src="../imagens/icones/icons/indicador.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" ><?php echo number_format($total_certificado,0,",",".") ?> Inscrições para <br />Certificação<br/>
						<span class="subtitulo_box" >Atualizado em : <?php echo $arrInscricao['dataatualizacao']?> às <?php echo $arrInscricao['hora'] ?></span>
						</a>
					</div>
				</div>
			</td>

			<td class="fundo_td" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/executive.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Sexo<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT sum(sexo_masculino) as sexo_masculino, sum(sexo_feminino) as sexo_feminino, (sum(sexo_masculino) + sum(sexo_feminino)) as total
						FROM enem.ts_enem_inscricao_2013";
				$arrIns = $db->pegaLinha($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Sexo</td>
						<td class="center bold" >Inscrições</td>
					</tr>
					<tr>
						<td>Masculino</td>
						<td class="numero" ><?php echo number_format($arrIns['sexo_masculino'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Feminino</td>
						<td class="numero" ><?php echo number_format($arrIns['sexo_feminino'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($arrIns['total'],0,",",".") ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="fundo_td" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Por Modalidade<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT SUM(concluiu_em) as concluiu_em, SUM(concluira_em) as concluira_em, SUM(concluira_apos_em) as concluira_apos_em, SUM(nao_concluiu) as nao_concluiu, (SUM(concluiu_em) + SUM(concluira_em) + SUM(concluira_apos_em) + SUM(nao_concluiu)) as totalsituacao,
						SUM(ensino_regular) AS ensino_regular, SUM(eja) AS eja, SUM(ensino_especial) AS ensino_especial, SUM(ensino_outros) as ensino_outros, (SUM(ensino_regular) + SUM(eja) + SUM(ensino_especial) + SUM(ensino_outros)) AS totalmodalidade
						FROM enem.ts_enem_inscricao_2013";
				$arrIns = $db->pegaLinha($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Modalidade</td>
						<td class="center bold" >Inscrições</td>
					</tr>
					<tr>
						<td>Ensino Médio Regular</td>
						<td class="numero" ><?php echo number_format($arrIns['ensino_regular'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Educação de Jovens e Adultos</td>
						<td class="numero" ><?php echo number_format($arrIns['eja'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Ensino Médio Ed. Especial</td>
						<td class="numero" ><?php echo number_format($arrIns['ensino_especial'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Outros</td>
						<td class="numero" ><?php echo number_format($arrIns['ensino_outros'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="bold">Total</td>
						<td class="numero bold" ><?php echo number_format($arrIns['totalmodalidade'],0,",",".") ?></td>
					</tr>
				</table>
			</td>
			<td class="fundo_td" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/executive.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Cor/Raça<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT sum(cor_raca_branca) as cor_raca_branca, sum(cor_raca_preta) as cor_raca_preta, sum(cor_raca_parda) as cor_raca_parda, sum(cor_raca_amarela) as cor_raca_amarela, sum(cor_raca_indigena) as cor_raca_indigena, sum(cor_raca_nao_declarada) + $correcao as cor_raca_nao_declarada,
						(sum(cor_raca_branca) + sum(cor_raca_preta) + sum(cor_raca_parda) + sum(cor_raca_amarela) + sum(cor_raca_indigena) + sum(cor_raca_nao_declarada) + $correcao) as total
						FROM enem.ts_enem_inscricao_2013";
				$arrIns = $db->pegaLinha($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Cor/Raça</td>
						<td class="center bold" >Inscrições</td>
					</tr>
					<tr>
						<td>Branca</td>
						<td class="numero" ><?php echo number_format($arrIns['cor_raca_branca'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Preta</td>
						<td class="numero" ><?php echo number_format($arrIns['cor_raca_preta'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Parda</td>
						<td class="numero" ><?php echo number_format($arrIns['cor_raca_parda'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Amarela</td>
						<td class="numero" ><?php echo number_format($arrIns['cor_raca_amarela'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Indígena</td>
						<td class="numero" ><?php echo number_format($arrIns['cor_raca_indigena'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Não Declarada</td>
						<td class="numero" ><?php echo number_format($arrIns['cor_raca_nao_declarada'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($arrIns['total'],0,",",".") ?></td>
					</tr>
				</table>
			</td>
			<td class="fundo_td_azul_escuro" rowspan="3" valign="top" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Por Estado<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT sgl_uf, SUM(total_uf) as totalinscricao, SUM(pediu_certificado) as totalcertificado
					FROM enem.ts_enem_inscricao_2013
					GROUP BY sgl_uf
					ORDER BY sgl_uf";
				$arrUF = $db->carregar($sql);
				?>
				<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >UF</td>
						<td class="center bold" >Inscrições</td>
						<td class="center bold" >Certificados</td>
					</tr>
					<?php
					if($arrUF){
						foreach($arrUF as $uf): ?>
						<tr>
							<td class="" ><?php echo $uf['sgl_uf'] ?></td>
							<td class="numero" ><?php $total_uf_inscrito+=$uf['totalinscricao']; echo number_format($uf['totalinscricao'],0,",",".") ?></td>
							<td class="numero" ><?php $total_uf_certificado+=$uf['totalcertificado']; echo number_format($uf['totalcertificado'],0,",",".") ?></td>
						</tr>
					<?php endforeach;
					}
					?>
				<tr>
					<td class="bold" >Total</td>
					<td class="numero bold" ><?php echo number_format($total_uf_inscrito,0,",",".") ?></td>
					<td class="numero bold" ><?php echo number_format($total_uf_certificado,0,",",".") ?></td>
				</tr>
				</table>
			</td>
		</tr>

		<tr>
			<td class="fundo_td" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Por Situação<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT SUM(concluiu_em) as concluiu_em, SUM(concluira_em) as concluira_em, SUM(concluira_apos_em) as concluira_apos_em, SUM(nao_concluiu) + $correcao as nao_concluiu, (SUM(concluiu_em) + SUM(concluira_em) + SUM(concluira_apos_em) + SUM(nao_concluiu) + $correcao) as totalsituacao,
						SUM(ensino_regular) AS ensino_regular, SUM(eja) AS eja, SUM(ensino_especial) AS ensino_especial, SUM(ensino_outros) as ensino_outros, (SUM(ensino_regular) + SUM(eja) + SUM(ensino_especial) + SUM(ensino_outros)) AS totalmodalidade
						FROM enem.ts_enem_inscricao_2013";
				$arrIns = $db->pegaLinha($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Situação</td>
						<td class="center bold" >Inscrições</td>
					</tr>
					<tr>
						<td>Concluiu o Ensino Médio</td>
						<td class="numero" ><?php echo number_format($arrIns['concluiu_em'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Concluirá em 2013</td>
						<td class="numero" ><?php echo number_format($arrIns['concluira_em'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Concluirá após 2013</td>
						<td class="numero" ><?php echo number_format($arrIns['concluira_apos_em'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Não Concluiu e não está cursando o Ensino Médio</td>
						<td class="numero" ><?php echo number_format($arrIns['nao_concluiu'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="bold">Total</td>
						<td class="numero bold" ><?php echo number_format($arrIns['totalsituacao'],0,",",".") ?></td>
					</tr>
				</table>
			</td>
			<td class="fundo_td_azul" rowspan="2" valign="top">
				<div>
					<img style="float:left" src="../imagens/icones/icons/financeiro.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Pagamento<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT SUM(sem_pagamento) AS sem_pagamento, SUM(carencia_indeferida) AS carencia_indeferida, SUM(pagamento_efetuado) AS pagamento_efetuado, SUM(carencia_aceita) AS carencia_aceita, SUM(inscricao_gratuita) AS inscricao_gratuita,
						(SUM(sem_pagamento) + SUM(carencia_indeferida) + SUM(pagamento_efetuado) + SUM(carencia_aceita) + SUM(inscricao_gratuita)) AS total
						FROM enem.ts_enem_inscricao_2013";
				$pag = $db->pegaLinha($sql);
				?>
				<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Situação</td>
						<td class="center bold" >Quantidade</td>
					</tr>
					<tr>
						<td class="" >Pagamento Efetuado</td>
						<td class="numero" ><?php echo number_format(($pag['pagamento_efetuado'] + $pag['sem_pagamento']),0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Carência Deferida (Baixa Renda)</td>
						<td class="numero" ><?php echo number_format($pag['carencia_aceita'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Escola Pública (Inscrição gratuita)</td>
						<td class="numero" ><?php echo number_format($pag['inscricao_gratuita'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($pag['total'],0,",",".") ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="fundo_td" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/executive.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Por Faixa Etária<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT sum(menor_de_16) as menor_de_16, sum(igual_a_16) as igual_a_16, sum(igual_a_17) as igual_a_17, sum(igual_a_18) as igual_a_18,
						sum(igual_a_19) as igual_a_19, sum(igual_a_20) as igual_a_20, sum(entre_21_a_30) as entre_21_a_30, sum(maior_de_30) as maior_de_30,
						(sum(menor_de_16) + sum(igual_a_16) + sum(igual_a_17) + sum(igual_a_18) +
						sum(igual_a_19) + sum(igual_a_20) + sum(entre_21_a_30) + sum(maior_de_30)) as total
						FROM enem.ts_enem_inscricao_2013";
				$idade = $db->pegaLinha($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Idade</td>
						<td class="center bold" >Quantidade</td>
					</tr>
					<tr>
						<td class="" >Menor que 16</td>
						<td class="numero" ><?php echo number_format($idade['menor_de_16'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Igual a 16</td>
						<td class="numero" ><?php echo number_format($idade['igual_a_16'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Igual a 17</td>
						<td class="numero" ><?php echo number_format($idade['igual_a_17'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Igual a 18</td>
						<td class="numero" ><?php echo number_format($idade['igual_a_18'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Igual a 19</td>
						<td class="numero" ><?php echo number_format($idade['igual_a_19'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Igual a 20</td>
						<td class="numero" ><?php echo number_format($idade['igual_a_20'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >De 21 a 30</td>
						<td class="numero" ><?php echo number_format($idade['entre_21_a_30'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Maior de 30</td>
						<td class="numero" ><?php echo number_format($idade['maior_de_30'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($idade['total'],0,",",".") ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="fundo_td" rowspan="1">
				<div>
					<img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Atendimento Específico<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT SUM(sala_mais_facil_acesso) as sala_mais_facil_acesso,
							   SUM(prova_ampliada) as prova_ampliada,
							   SUM(prova_ampliada_18) as prova_ampliada_18,
							   SUM(prova_braille) as prova_braille,
							   SUM(auxilio_ledor) as auxilio_ledor,
							   SUM(interprete_libras) as interprete_libras,
							   SUM(auxilio_transcricao) as auxilio_transcricao,
							   SUM(classe_hospitalar) as classe_hospitalar,
							   SUM(qt_cid) as qt_cid,
							   SUM(sabatista) as sabatista,
							   SUM(gestante) as gestante,
							   SUM(lactante) as lactante,
							   SUM(idoso) as idoso,
							   (SUM(sala_mais_facil_acesso) + SUM(prova_ampliada) + SUM(prova_ampliada_18) + SUM(prova_braille) +
							   SUM(auxilio_ledor) + SUM(interprete_libras) + SUM(auxilio_transcricao) + SUM(classe_hospitalar) +
							   SUM(qt_cid) + SUM(sabatista) + SUM(gestante) + SUM(lactante) + SUM(idoso)) AS total
						FROM enem.ts_enem_inscricao_2013";
				$mod = $db->pegaLinha($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Modalidade</td>
						<td class="center bold" >Quantidade</td>
					</tr>
					<tr>
						<td class="" >Guarda o sábado</td>
						<td class="numero" ><?php echo number_format($mod['sabatista'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Gestante</td>
						<td class="numero" ><?php echo number_format($mod['gestante'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Lactante</td>
						<td class="numero" ><?php echo number_format($mod['lactante'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Idoso</td>
						<td class="numero" ><?php echo number_format($mod['idoso'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Classe Hospitalar</td>
						<td class="numero" ><?php echo number_format($mod['classe_hospitalar'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="bold" >Total de Inscritos/CPF</td>
						<td class="bold numero" ><?php echo number_format(105775,0,",",".") ?></td>
					</tr>
				</table>
			</td>
			<td class="fundo_td" rowspan="1">
				<div>
					<img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Atendimento Diferenciado<br/></a>
					</div>
				</div>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Modalidade</td>
						<td class="center bold" >Quantidade</td>
					</tr>
					<tr>
						<td class="" >Sala de mais fácil acesso</td>
						<td class="numero" ><?php echo number_format($mod['sala_mais_facil_acesso'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Prova ampliada</td>
						<td class="numero" ><?php echo number_format($mod['prova_ampliada'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Prova super ampliada</td>
						<td class="numero" ><?php echo number_format($mod['prova_ampliada_18'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Prova em Braile</td>
						<td class="numero" ><?php echo number_format($mod['prova_braille'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Auxílio de Ledor</td>
						<td class="numero" ><?php echo number_format($mod['auxilio_ledor'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Intérprete de Libras</td>
						<td class="numero" ><?php echo number_format($mod['interprete_libras'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Auxílio para transcrição</td>
						<td class="numero" ><?php echo number_format($mod['auxilio_transcricao'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="" >Outro Atendimento (CID)</td>
						<td class="numero" ><?php echo number_format($mod['qt_cid'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="bold" >Total de Inscritos/CPF</td>
						<td class="bold numero" ><?php echo number_format(42513,0,",",".") ?></td>
					</tr>
				</table>
			</td>
			<td class="fundo_td_azul_escuro">
				<div onclick="abreIndicadorPopUp(271)" >
					<?php
					$sql = "select sum(sehqtde) as qtde,sum(sehvalor) as valor from painel.seriehistorica where indid = 271 and sehstatus != 'I';";
					$arrValores = $db->pegaLinha($sql);
					$sql = "select
								count(distinct dshcodmunicipio)
							from
								painel.seriehistorica seh
							inner join
								painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
							where
								indid = 271
							and
								seh.sehstatus != 'I';";
					$totalMun = $db->pegaUm($sql);
					?>
					<img style="float:left" src="../imagens/icones/icons/mapas.png" style="vertical-align:middle;"  />
					<div style="float:left;cursor:pointer;" class="titulo_box" >Mapa das inscrições<br/><span class="subtitulo_box" ><a href="#" >Clique no mapa para visualizar os detalhes.</a></span>
					</div>
					<img class="link" onclick="abreIndicadorPopUp(271)" src="../imagens/cockpit/mapa_cockpit.png"  />
				</div>
			</td>
		</tr>
		<tr>
			<td class="fundo_td" valign="top" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Atendimento Específico<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT
							sgl_uf,
							SUM(sabatista) as sabatista,
							SUM(gestante) as gestante,
							SUM(lactante) as lactante,
							SUM(idoso) as idoso,
							SUM(classe_hospitalar) as classe_hospitalar,
							(SUM(sabatista) + SUM(gestante) + SUM(lactante) + SUM(idoso) + SUM(classe_hospitalar)) AS total
						FROM enem.ts_enem_inscricao_2013
						GROUP BY sgl_uf
						ORDER BY sgl_uf";
				$arrUF = $db->carregar($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >UF</td>
						<td class="center bold" >Guarda o sábado</td>
						<td class="center bold" >Gestante</td>
						<td class="center bold" >Lactante</td>
						<td class="center bold" >Idoso</td>
						<td class="center bold" >Classe Hospitalar</td>
						<td class="center bold" >Total</td>
					</tr>
					<?php
					if($arrUF){
						foreach($arrUF as $uf): ?>
						<tr>
							<td class="" ><?php echo $uf['sgl_uf'] ?></td>
							<td class="numero" ><?php echo number_format($uf['sabatista'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['gestante'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['lactante'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['idoso'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['classe_hospitalar'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['total'],0,",",".") ?></td>
						</tr>
					<?php
						$totalsabatista+=$uf['sabatista'];
						$totalgestante+=$uf['gestante'];
						$totallactante+=$uf['lactante'];
						$totalidoso+=$uf['idoso'];
						$totalclasse_hospitalar+=$uf['classe_hospitalar'];
						$totalgeral+=$uf['total'];

						endforeach;
					}
					?>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($totalsabatista,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalgestante,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totallactante,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalidoso,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalclasse_hospitalar,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalgeral,0,",",".") ?></td>
					</tr>
				</table>
			</td>

			<td class="fundo_td" valign="top" colspan="2" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Atendimento Diferenciado<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT
							sgl_uf,
							SUM(sala_mais_facil_acesso) as sala_mais_facil_acesso,
							SUM(prova_ampliada) as prova_ampliada,
							SUM(prova_ampliada_18) as prova_ampliada_18,
							SUM(prova_braille) as prova_braille,
							SUM(auxilio_ledor) as auxilio_ledor,
							SUM(interprete_libras) as interprete_libras,
							SUM(auxilio_transcricao) as auxilio_transcricao,
							SUM(qt_cid) as qt_cid,
							(SUM(sala_mais_facil_acesso) + SUM(prova_ampliada) + SUM(prova_ampliada_18) + SUM(prova_braille) +
							SUM(auxilio_ledor) + SUM(interprete_libras) + SUM(auxilio_transcricao) + SUM(qt_cid)) AS total
						FROM enem.ts_enem_inscricao_2013
						GROUP BY sgl_uf
						ORDER BY sgl_uf";
				$arrUF = $db->carregar($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >UF</td>
						<td class="center bold" >Sala de mais fácil acesso</td>
						<td class="center bold" >Prova ampliada</td>
						<td class="center bold" >Prova super ampliada</td>
						<td class="center bold" >Prova em Braile</td>
						<td class="center bold" >Auxílio de Ledor</td>
						<td class="center bold" >Intérprete de Libras</td>
						<td class="center bold" >Auxílio para transcrição</td>
						<td class="center bold" >Outro Atendimento (CID)</td>
						<td class="center bold" >Total</td>
					</tr>
					<?php
					if($arrUF){
						foreach($arrUF as $uf): ?>
						<tr>
							<td class="" ><?php echo $uf['sgl_uf'] ?></td>
							<td class="numero" ><?php echo number_format($uf['sala_mais_facil_acesso'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['prova_ampliada'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['prova_ampliada_18'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['prova_braille'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['auxilio_ledor'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['interprete_libras'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['auxilio_transcricao'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['qt_cid'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['total'],0,",",".") ?></td>
						</tr>
					<?php
						$totalsala_mais_facil_acesso+=$uf['sala_mais_facil_acesso'];
						$totalprova_ampliada+=$uf['prova_ampliada'];
						$totalprova_ampliada_18+=$uf['prova_ampliada_18'];
						$totalprova_braille+=$uf['prova_braille'];
						$totalauxilio_ledor+=$uf['auxilio_ledor'];
						$totalinterprete_libras+=$uf['interprete_libras'];
						$totalauxilio_transcricao+=$uf['auxilio_transcricao'];
						$totalqt_cid+=$uf['qt_cid'];
						$totalgeraldiferenciado+=$uf['total'];

						endforeach;
					}
					?>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($totalsala_mais_facil_acesso,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalprova_ampliada,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalprova_ampliada_18,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalprova_braille,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalauxilio_ledor,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalinterprete_libras,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalauxilio_transcricao,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalqt_cid,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalgeraldiferenciado,0,",",".") ?></td>
					</tr>
				</table>
			</td>
		</tr>
	<?php
	//========================================================================================================================================
	//MONITORAMENTO/CERTIFICAÇÃO
	//========================================================================================================================================
	}elseif($tipo==2){
	?>
	<tr>
			<td class="fundo_td" width="50%" >
			<?php
				montaTabelaExamesMec('Pontos de Atenção<br>(Certificação de Processos)', array(18,19));
			?>
			</td>
			<td class="fundo_td" width="50%" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/executive.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Gestor de Processos<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT usu.usucpf, usu.usunome as gestor, COUNT(DISTINCT ati.atiid) AS atividade, COUNT(DISTINCT icl.iclid) as item, COUNT(d1.docid) AS executado
						FROM pde.usuarioresponsabilidade rpu
						INNER JOIN seguranca.usuario usu ON usu.usucpf = rpu.usucpf
						INNER JOIN pde.atividade ati ON ati.atiid = rpu.atiid
						INNER JOIN pde.itemchecklist icl ON icl.atiid = ati.atiid
						LEFT JOIN workflow.documento d1 ON d1.docid = icl.docid AND d1.esdid = 284
						WHERE rpu.rpustatus = 'A'
						AND rpu.pflcod in (90,890) --Gestor de Processos
						AND ati.atistatus = 'A'
						AND _atiprojeto = 114098
						AND ( _atinumero LIKE '18%' OR _atinumero LIKE '19%' )
						GROUP BY usu.usucpf, gestor
						ORDER BY gestor";
				$arrDados = $db->carregar($sql);
				$totalAtividade = 0;
				$totalItem = 0;
				$totalExecutado = 0;
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
						<td class="center bold" >Gestor</td>
						<td class="center bold" >Atividades</td>
						<td class="center bold" >Itens</td>
						<td class="center bold" >Executados</td>
				</tr>
				<?php foreach($arrDados as $chave => $dado): ?>
				<tr class="link" onclick="abreRelatorio('form=1&agrupadores[0]=atividades&etapa_campo_flag=1&etapa[0]=141258&etapa[1]=141259&etapa[2]=141260&etapa[3]=141261&etapa[4]=141262&etapa[5]=141263&etapa[6]=141265&etapa[7]=141266&gestorProcessos_campo_flag=1&gestorProcessos[0]=<?php echo $dado['usucpf'] ?>&prazovencido=nao&arvore_=true');">
					<td class=""><?php echo $dado['gestor'] ?></td>
					<td class="numero" ><?php echo number_format($dado['atividade'],0,",",".") ?></td>
					<td class="numero" ><?php echo number_format($dado['item'],0,",",".") ?></td>
					<td class="numero" ><?php echo number_format($dado['executado'],0,",",".") ?></td>
				</tr>
				<?php
					$totalAtividade += $dado['atividade'];
					$totalItem += $dado['item'];
					$totalExecutado += $dado['executado'];
				endforeach; ?>
				<tr>
					<td class="bold" >Total</td>
					<td class="bold numero" ><?php echo number_format($totalAtividade,0,",",".") ?></td>
					<td class="bold numero" ><?php echo number_format($totalItem,0,",",".") ?></td>
					<td class="bold numero" ><?php echo number_format($totalExecutado,0,",",".") ?></td>
				</tr>
				</table>
			</td>
<?php
if(1==2){
?>
			<td class="fundo_td" width="33%" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/executive.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						<a href="#" >Gestor de Riscos<br/></a>
					</div>
				</div>
				<?php
				$sql = "SELECT usu.usucpf, usu.usunome as gestor, COUNT(DISTINCT ati.atiid) AS atividade, COUNT(DISTINCT icl.iclid) as item, COUNT(d1.docid) AS executado
						FROM pde.usuarioresponsabilidade rpu
						INNER JOIN seguranca.usuario usu ON usu.usucpf = rpu.usucpf
						INNER JOIN pde.atividade ati ON ati.atiid = rpu.atiid
						INNER JOIN pde.itemchecklist icl ON icl.atiid = ati.atiid
						LEFT JOIN workflow.documento d1 ON d1.docid = icl.docid AND d1.esdid = 284
						WHERE rpu.rpustatus = 'A'
						AND rpu.pflcod in (91,891) --Gestor de Risco
						AND ati.atistatus = 'A'
						AND _atiprojeto = 114098
						AND ( _atinumero LIKE '18%' OR _atinumero LIKE '19%' )
						GROUP BY usu.usucpf, gestor
						ORDER BY gestor";
				$arrDados = $db->carregar($sql);
				$totalAtividade = 0;
				$totalItem = 0;
				$totalExecutado = 0;
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
						<td class="center bold" >Gestor</td>
						<td class="center bold" >Atividades</td>
						<td class="center bold" >Itens</td>
						<td class="center bold" >Executados</td>
				</tr>
				<?php
				if($arrDados){
					foreach($arrDados as $chave => $dado): ?>
					<tr class="link" onclick="abreRelatorio('form=1&agrupadores[0]=atividades&etapa_campo_flag=1&etapa[0]=141258&etapa[1]=141259&etapa[2]=141260&etapa[3]=141261&etapa[4]=141262&etapa[5]=141263&etapa[6]=141265&etapa[7]=141266&gestorRiscos_campo_flag=1&gestorRiscos[0]=<?php echo $dado['usucpf'] ?>&prazovencido=nao&arvore_=true');">
						<td class="" ><?php echo $dado['gestor'] ?></td>
						<td class="numero" ><?php echo number_format($dado['atividade'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($dado['item'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($dado['executado'],0,",",".") ?></td>
					</tr>
					<?php
						$totalAtividade += $dado['atividade'];
						$totalItem += $dado['item'];
						$totalExecutado += $dado['executado'];
					endforeach;
				}
				?>
				<tr>
					<td class="bold" >Total</td>
					<td class="bold numero" ><?php echo number_format($totalAtividade,0,",",".") ?></td>
					<td class="bold numero" ><?php echo number_format($totalItem,0,",",".") ?></td>
					<td class="bold numero" ><?php echo number_format($totalExecutado,0,",",".") ?></td>
				</tr>
				</table>
			</td>
<?php
}
?>
		</tr>
		</table>
		<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
		<tr>
			<td class="fundo_td" width="34%" >
			<div>
                <img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" >Risk Manager da Operação<br/></a>
                </div>
            </div>
            <?php
            $sql = "SELECT
						SUBSTR(etapasimec,1,1) as id,
						etapasimec,
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '1.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '2.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '3.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '4.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '5.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '6.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '7.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '8.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '9.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
					) as foo
					GROUP BY etapasimec, id
					ORDER BY etapasimec";
            $arrRiskmanager = $db->carregar($sql);
			$totbaixo = 0;
			$totatencao = 0;
			$totelevado = 0;
			$totalto = 0;
			$totmedio = 0;
			$totgeral = 0;
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" colspan="7" >ALERTAS EM TRATAMENTO</td>
				</tr>
				<tr>
					<td class="bold" rowspan="2">ETAPAS DO ENEM/2013</td>
					<td class="center bold" colspan="6" >NÍVEIS DE ALERTA</td>
				</tr>
				<tr>
					<td class="center bold" style="color:rgb(8,255,2)">BAIXO</td>
					<td class="center bold" style="color:#2B86CC">ATENÇÃO</td>
					<td class="center bold" style="color:rgb(255,255,0)">ELEVADO</td>
					<td class="center bold" style="color:rgb(255,145,0)">ALTO</td>
					<td class="center bold" style="color:rgb(255,2,2)">SEVERO</td>
					<td class="center bold">TOTAL</td>
				</tr>
				<?php if( is_array( $arrRiskmanager ) ){?>
				<?php foreach($arrRiskmanager as $atr):

						$tot = $atr['baixo'] + $atr['atencao'] + $atr['elevado'] + $atr['alto'] + $atr['severo'];
				?>
					<tr>
						<td width="50%">
							<?php echo $atr['etapasimec'] ?>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Baixo','<?=$atr['id']?>', 'Aberto', 1, 'RM - Operação');">
								<?= $atr['baixo'] == 0 ? '-' : number_format($atr['baixo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Atenção','<?=$atr['id']?>', 'Aberto', 1, 'RM - Operação');">
								<?= $atr['atencao'] == 0 ? '-' : number_format($atr['atencao'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Elevado','<?=$atr['id']?>', 'Aberto', 1, 'RM - Operação');">
								<?= $atr['elevado'] == 0 ? '-' : number_format($atr['elevado'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Alto','<?=$atr['id']?>', 'Aberto', 1, 'RM - Operação');">
								<?= $atr['alto'] == 0 ? '-' 	 : number_format($atr['alto'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Severo','<?=$atr['id']?>', 'Aberto', 1, 'RM - Operação');">
								<?= $atr['severo'] == 0 ? '-'  : number_format($atr['severo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span>
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
					<td class="center bold" style="color:#2B86CC"><?=$totatencao?></td>
					<td class="center bold" style="color:rgb(255,255,0)"><?=$totelevado?></td>
					<td class="center bold" style="color:rgb(255,145,0)"><?=$totalto?></td>
					<td class="center bold" style="color:rgb(255,2,2)"><?=$totmedio?></td>
					<td class="center bold"><?=$totgeral?></td>
				</tr>
				<?php }else{?>
					<tr>
						<td  class="center" style="color:red" colspan="7">Não possui registros.</td>
					</tr>
				<?php } ?>
			</table>

			<table class="fundo_td" cellpadding="2" cellspacing="1" width="100%" >
				<tr><td>&nbsp;</td></tr>
			</table>

            <?php
            $sql = "SELECT
						SUBSTR(etapasimec,1,1) as id,
						etapasimec,
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '1.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '2.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '3.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '4.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '5.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '6.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '7.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '8.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta = 'RM - Operação' AND etapasimec ilike '9.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
					) as foo
					GROUP BY etapasimec, id
					ORDER BY etapasimec";
            $arrRiskmanager = $db->carregar($sql);
			$totbaixo = 0;
			$totatencao = 0;
			$totelevado = 0;
			$totalto = 0;
			$totmedio = 0;
			$totgeral = 0;
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" colspan="7" >ALERTAS TRATADOS</td>
				</tr>
				<tr>
					<td class="bold" rowspan="2">ETAPAS DO ENEM/2013</td>
					<td class="center bold" colspan="6" >NÍVEIS DE ALERTA</td>
				</tr>
				<tr>
					<td class="center bold" style="color:rgb(8,255,2)">BAIXO</td>
					<td class="center bold" style="color:#2B86CC">ATENÇÃO</td>
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
						<td width="50%">
							<?php echo $atr['etapasimec'] ?>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Baixo','<?=$atr['id']?>', 'Fechado', 2, 'RM - Operação');">
								<?= $atr['baixo'] == 0 ? '-' : number_format($atr['baixo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Atenção','<?=$atr['id']?>', 'Fechado', 2, 'RM - Operação');">
								<?= $atr['atencao'] == 0 ? '-' : number_format($atr['atencao'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Elevado','<?=$atr['id']?>', 'Fechado', 2, 'RM - Operação');">
								<?= $atr['elevado'] == 0 ? '-' : number_format($atr['elevado'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Alto','<?=$atr['id']?>', 'Fechado', 2, 'RM - Operação');">
								<?= $atr['alto'] == 0 ? '-' 	 : number_format($atr['alto'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Severo','<?=$atr['id']?>', 'Fechado', 2, 'RM - Operação');">
								<?= $atr['severo'] == 0 ? '-'  : number_format($atr['severo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span>
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
					<td class="center bold" style="color:#2B86CC"><?=$totatencao?></td>
					<td class="center bold" style="color:rgb(255,255,0)"><?=$totelevado?></td>
					<td class="center bold" style="color:rgb(255,145,0)"><?=$totalto?></td>
					<td class="center bold" style="color:rgb(255,2,2)"><?=$totmedio?></td>
					<td class="center bold"><?=$totgeral?></td>
				</tr>
				<?php }else{?>
					<tr>
						<td  class="center" style="color:red" colspan="7">Não possui registros.</td>
					</tr>
				<?php } ?>
			</table>
		</td>

		<td class="fundo_td" width="33%" >
			<div>
                <img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" >Risk Manager da Certificação<br>de Processos / Caminho Crítico<br/></a>
                </div>
            </div>
            <?php
            $sql = "SELECT
						SUBSTR(etapasimec,1,1) as id,
						etapasimec,
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '1.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '2.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '3.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '4.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '5.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '6.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '7.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '8.%' AND statusalerta = 'Aberto'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '9.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
					) as foo
					GROUP BY etapasimec, id
					ORDER BY etapasimec";
            $arrRiskmanager = $db->carregar($sql);
			$totbaixo = 0;
			$totatencao = 0;
			$totelevado = 0;
			$totalto = 0;
			$totmedio = 0;
			$totgeral = 0;
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" colspan="7" >ALERTAS EM TRATAMENTO</td>
				</tr>
				<tr>
					<td class="bold" rowspan="2">ETAPAS DO ENEM/2013</td>
					<td class="center bold" colspan="6" >NÍVEIS DE ALERTA</td>
				</tr>
				<tr>
					<td class="center bold" style="color:rgb(8,255,2)">BAIXO</td>
					<td class="center bold" style="color:#2B86CC">ATENÇÃO</td>
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
						<td width="50%">
							<?php echo $atr['etapasimec'] ?>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Baixo','<?=$atr['id']?>', 'Aberto', 1, 'RM - Certificação de Processos');">
								<?= $atr['baixo'] == 0 ? '-' : number_format($atr['baixo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Atenção','<?=$atr['id']?>', 'Aberto', 2, 'RM - Certificação de Processos');">
								<?= $atr['atencao'] == 0 ? '-' : number_format($atr['atencao'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Elevado','<?=$atr['id']?>', 'Aberto', 1, 'RM - Certificação de Processos');">
								<?= $atr['elevado'] == 0 ? '-' : number_format($atr['elevado'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Alto','<?=$atr['id']?>', 'Aberto', 2, 'RM - Certificação de Processos');">
								<?= $atr['alto'] == 0 ? '-' 	 : number_format($atr['alto'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Severo','<?=$atr['id']?>', 'Aberto', 1, 'RM - Certificação de Processos');">
								<?= $atr['severo'] == 0 ? '-'  : number_format($atr['severo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span>
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
					<td class="center bold" style="color:#2B86CC"><?=$totatencao?></td>
					<td class="center bold" style="color:rgb(255,255,0)"><?=$totelevado?></td>
					<td class="center bold" style="color:rgb(255,145,0)"><?=$totalto?></td>
					<td class="center bold" style="color:rgb(255,2,2)"><?=$totmedio?></td>
					<td class="center bold"><?=$totgeral?></td>
				</tr>
				<?php }else{?>
					<tr>
						<td  class="center" style="color:red" colspan="7">Não possui registros.</td>
					</tr>
				<?php } ?>
			</table>

			<table class="fundo_td" cellpadding="2" cellspacing="1" width="100%" >
				<tr><td>&nbsp;</td></tr>
			</table>

            <?php
            $sql = "SELECT
						SUBSTR(etapasimec,1,1) as id,
						etapasimec,
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '1.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '2.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '3.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '4.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '5.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '6.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '7.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '8.%' AND statusalerta = 'Fechado'
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
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo' AND origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '9.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
					) as foo
					GROUP BY etapasimec, id
					ORDER BY etapasimec";
            $arrRiskmanager = $db->carregar($sql);
			$totbaixo = 0;
			$totatencao = 0;
			$totelevado = 0;
			$totalto = 0;
			$totmedio = 0;
			$totgeral = 0;
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" colspan="7" >ALERTAS TRATADOS</td>
				</tr>
				<tr>
					<td class="bold" rowspan="2">ETAPAS DO ENEM/2013</td>
					<td class="center bold" colspan="6" >NÍVEIS DE ALERTA</td>
				</tr>
				<tr>
					<td class="center bold" style="color:rgb(8,255,2)">BAIXO</td>
					<td class="center bold" style="color:#2B86CC">ATENÇÃO</td>
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
						<td width="50%">
							<?php echo $atr['etapasimec'] ?>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Baixo','<?=$atr['id']?>', 'Fechado', 2, 'RM - Certificação de Processos');">
								<?= $atr['baixo'] == 0 ? '-' : number_format($atr['baixo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Atenção','<?=$atr['id']?>', 'Fechado', 2, 'RM - Certificação de Processos');">
								<?= $atr['atencao'] == 0 ? '-' : number_format($atr['atencao'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Elevado','<?=$atr['id']?>', 'Fechado', 2, 'RM - Certificação de Processos');">
								<?= $atr['elevado'] == 0 ? '-' : number_format($atr['elevado'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Alto','<?=$atr['id']?>', 'Fechado', 2, 'RM - Certificação de Processos');">
								<?= $atr['alto'] == 0 ? '-' 	 : number_format($atr['alto'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span style="cursor:pointer;"
								  onclick="abreRiskManager('Severo','<?=$atr['id']?>', 'Fechado', 2, 'RM - Certificação de Processos');">
								<?= $atr['severo'] == 0 ? '-'  : number_format($atr['severo'],0,",",".") ?>
							</span>
						</td>
						<td class="numero center">
							<span>
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
					<td class="center bold" style="color:#2B86CC"><?=$totatencao?></td>
					<td class="center bold" style="color:rgb(255,255,0)"><?=$totelevado?></td>
					<td class="center bold" style="color:rgb(255,145,0)"><?=$totalto?></td>
					<td class="center bold" style="color:rgb(255,2,2)"><?=$totmedio?></td>
					<td class="center bold"><?=$totgeral?></td>
				</tr>
				<?php }else{?>
					<tr>
						<td  class="center" style="color:red" colspan="7">Não possui registros.</td>
					</tr>
				<?php }	?>
			</table>
		</td>

		<td class="fundo_td" width="33%" >
			<div>
                <img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" >Risk Manager - Todas as Ocorrências<br/></a>
                </div>
            </div>
			<?php
			$sql = "SELECT
						COALESCE(SUM(baixo),0) as baixo,
						COALESCE(SUM(atencao),0) as atencao,
						COALESCE(SUM(elevado),0) as elevado,
						COALESCE(SUM(alto),0) as alto,
						COALESCE(SUM(severo),0) as severo,
						(COALESCE(SUM(baixo),0) + COALESCE(SUM(atencao),0) + COALESCE(SUM(elevado),0) + COALESCE(SUM(alto),0) + COALESCE(SUM(severo),0)) AS total
					FROM (
						SELECT

							CASE WHEN nivelalerta = 'Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = 'Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = 'Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = 'Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = 'Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2013
						WHERE
							tipo = 'Alerta Externo'
						GROUP BY
							nivelalerta
					) as foo";
			$arrRiskmanager = $db->pegaLinha($sql);
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold">NÍVEIS DE ALERTA</td>
					<td class="center bold">TOTAL</td>
				</tr>
				<tr class="link" onclick="abreRiskManager('Baixo','', '', '', '');">
					<td class="bold" style="color:rgb(8,255,2)">BAIXO</td>
					<td class="numero"><?php echo number_format($arrRiskmanager['baixo'],0,",",".") ?></td>
				</tr>
				<tr class="link" onclick="abreRiskManager('Atenção','', '', '', '');">
					<td class="bold" style="color:#2B86CC">ATENÇÃO</td>
					<td class="numero" ><?php echo number_format($arrRiskmanager['atencao'],0,",",".") ?></td>
				</tr>
				<tr class="link" onclick="abreRiskManager('Elevado','', '', '', '');">
					<td class="bold" style="color:rgb(255,255,0)">ELEVADO</td>
					<td class="numero" ><?php echo number_format($arrRiskmanager['elevado'],0,",",".") ?></td>
				</tr>
				<tr class="link" onclick="abreRiskManager('Alto','', '', '', '');">
					<td class="bold" style="color:rgb(255,145,0)">ALTO</td>
					<td class="numero" ><?php echo number_format($arrRiskmanager['alto'],0,",",".") ?></td>
				</tr>
				<tr class="link" onclick="abreRiskManager('Severo','', '', '', '');">
					<td class="bold" style="color:rgb(255,2,2)">SEVERO</td>
					<td class="numero" ><?php echo number_format($arrRiskmanager['severo'],0,",",".") ?></td>
				</tr>
				<tr>
					<td class="bold">TOTAL</td>
					<td class="bold numero" ><?php echo number_format($arrRiskmanager['total'],0,",",".") ?></td>
				</tr>
			</table>
		</td>
	</tr>
	<?php
	//========================================================================================================================================
	//OPERAÇÃO LOGÍSTICA
	//========================================================================================================================================
	}elseif($tipo==3){
	?>
	<tr>
		<td class="fundo_td" width="50%">
			<div>
                <img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					Etapas da Operação
                </div>
            </div>
            <?php
			$sql = "SELECT
						id_atividade,
						no_atividade,
						meta,
						execucao,
						CASE
							WHEN COALESCE(meta, 0) = 0 THEN 0
							ELSE
							(COALESCE(execucao, 0) * 100)/COALESCE(meta, 0)::numeric
						END AS percentual
					FROM(
						SELECT
							etpid as id_atividade,
							etpetapa as no_atividade,
							CASE etpid
								WHEN 1 THEN (SELECT SUM(nu_meta) FROM enem.TB_MAPA_ENEM_2013 WHERE id_indicador = 112 AND sgl_uf IS NOT NULL)
								WHEN 2 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 112 AND sgl_uf IS NOT NULL)
								WHEN 3 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 112 AND sgl_uf IS NOT NULL)
								WHEN 7 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 11 AND sgl_uf IS NOT NULL)
								WHEN 8 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 2 AND sgl_uf IS NOT NULL)
								WHEN 9 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 117 AND sgl_uf IS NOT NULL)
								WHEN 10 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 121 AND sgl_uf IS NOT NULL)
								WHEN 11 THEN 0
								WHEN 12 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador in (37,38,39,40) AND sgl_uf IS NOT NULL)
								WHEN 15 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 14 AND sgl_uf IS NOT NULL)
							END as meta,
							CASE etpid

							    -- ATENÇÃO: FOI SOLICITADO PELA CARLA EM 18/10/2013 PARA DEIXAR O EXECUTADO DOS ITENS COMENTADOS ABAIXO IGUAL À META,
							    -- DEPOIS É SÓ DESCOMENTAR E APAGAR A LINHA CORRESPONDENTE

								-- WHEN 1 THEN (SELECT SUM(nu_execucao) FROM enem.TB_MAPA_ENEM_2013 WHERE id_indicador = 112 AND sgl_uf IS NOT NULL)
								WHEN 1 THEN (SELECT SUM(nu_meta) FROM enem.TB_MAPA_ENEM_2013 WHERE id_indicador = 112 AND sgl_uf IS NOT NULL)

								-- WHEN 2 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 113 AND sgl_uf IS NOT NULL)
								WHEN 2 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 112 AND sgl_uf IS NOT NULL)

								-- WHEN 3 THEN (SELECT COUNT(0) FROM enem.tb_enem_esp_fisico_seg_2013 WHERE tp_situacao_avaliacao_risco IN (2,3))
                                WHEN 3 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 112 AND sgl_uf IS NOT NULL)

								-- WHEN 7 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 11 AND sgl_uf IS NOT NULL)
								WHEN 7 THEN 6891366

								-- WHEN 8 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 2 AND sgl_uf IS NOT NULL)
								WHEN 8 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 2 AND sgl_uf IS NOT NULL)

								WHEN 9 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 118 AND sgl_uf IS NOT NULL)

								WHEN 10 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 121 AND sgl_uf IS NOT NULL)

								WHEN 11 THEN 0

								-- WHEN 12 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador in (37,38,39,40) AND sgl_uf IS NOT NULL)
								WHEN 12 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador in (37,38,39,40) AND sgl_uf IS NOT NULL)

								WHEN 15 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 14 AND sgl_uf IS NOT NULL)
							END as execucao
						FROM enem.etapasoperacao
						WHERE etpid IN (1,3,2,7,8,9,15,12)
						AND etpstatus = 'A'
						ORDER BY etpordem
					) AS FOO";
            $arrAtividade = $db->carregar($sql);
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="bold" >Etapa</td>
					<td class="center bold" >Meta</td>
					<td class="center bold" >Executado</td>
					<td class="center bold" >% Executado</td>
				</tr>
				<?php foreach($arrAtividade as $att): ?>
					<tr>
						<td ><span style="cursor:pointer;" onclick="abreAtividade(<?=$att['id_atividade'] ?>);"><?php echo str_replace(array("1-","2-","3-","4-","5-","6-","7-","8-"),"",$att['no_atividade']) ?></td>
						<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($att['execucao'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
					</tr>
				<?php endforeach; ?>
			</table>
		</td>
		<td class="fundo_td_azul_escuro" width="50%" rowspan="3">
			<?php
			if ( $_REQUEST['id_atividade'] )
				{	$stWhere = $_REQUEST['id_atividade'];	}
			else
				{	$stWhere = 1;	}

			$sql = "SELECT etpetapa
					FROM enem.etapasoperacao
					WHERE etpid = {$stWhere}";
            $Item = $db->pegaLinha($sql);
			?>
			<div>
                <img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<?php echo $Item['etpetapa'] ?>
                </div>
            </div>
			<?php
			switch( $stWhere ){
				//AGENDAMENTO E LOCAÇÃO DE ESPAÇO PARA APLICAÇÃO
				case 1:
					$sql = "SELECT
							e.estuf as estado,
							foo3.inscritos,
							meta as metaalocado,
							COALESCE(quantidadealocada, NULL, 0) as quantidadealocada,
							(COALESCE(quantidadealocada, NULL, 0) * 100)/COALESCE(meta, NULL, 0)::numeric AS percentualalocado,
							COALESCE(quantidadeconfirmada, NULL, 0) as quantidadeconfirmada,
							(COALESCE(quantidadeconfirmada, NULL, 0) * 100)/COALESCE(meta, NULL, 0)::numeric AS percentualconfirmado,
							COALESCE(capacidade, NULL, 0) as capacidade,
							COALESCE(candidato, NULL, 0) as candidato,
							(COALESCE(candidato, NULL, 0) * 100)/COALESCE(foo3.inscritos, NULL, 0)::numeric AS percentual
						FROM territorios.estado e
						INNER JOIN
							(SELECT COALESCE(nu_meta,0) AS meta, COALESCE(nu_execucao,0) as quantidadealocada, sgl_uf from enem.TB_MAPA_ENEM_2013 where id_indicador = 112) AS foo1 ON foo1.sgl_uf = e.estuf
						LEFT JOIN
							(SELECT COALESCE(nu_execucao,0) as quantidadeconfirmada, sgl_uf from enem.TB_MAPA_ENEM_2013 where id_indicador = 113) AS foo2 ON foo2.sgl_uf = e.estuf
						INNER JOIN
							(SELECT COALESCE(nu_meta,0) as inscritos, sgl_uf from enem.TB_MAPA_ENEM_2013 WHERE id_indicador = 1 AND sgl_uf IS NOT NULL) foo3 ON foo3.sgl_uf = e.estuf
						LEFT JOIN
							(SELECT tab.sg_uf, sum(tab.nu_capacidade_max) AS capacidade FROM (SELECT nu_capacidade_max, CO_MUNICIPIO, SG_UF from ENEM.tb_enem_reg_arq_esp_fisico_2013 where tp_situacao_alocacao in (1,2)) tab GROUP BY tab.sg_uf) foo4 ON foo4.sg_uf = e.estuf
						LEFT JOIN
							(SELECT tab.sg_uf, sum(tab.qt_candidatos_alocados) AS candidato FROM (SELECT qt_candidatos_alocados, CO_MUNICIPIO, SG_UF from ENEM.tb_enem_reg_arq_esp_fisico_2013 where tp_situacao_alocacao = 2) tab GROUP BY tab.sg_uf) foo5 ON foo5.sg_uf = e.estuf
						ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" rowspan="2" >UF</td>
							<td class="center bold" rowspan="2" >Quantidade<br>de<br>Inscritos</td>
							<td class="center bold" colspan="2" >Agendamento</td>
							<td class="center bold" colspan="2" >Locação Confirmada</td>
							<td class="center bold" rowspan="2" >%</td>
						</tr>
						<tr>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Quantidade de Locais Agendados</td>
							<td class="center bold" >Capacidade Máxima</td>
							<td class="center bold" >Previsão de<br>Candidatos Alocados</td>
						</tr>
						<?php
						$totalinscritos = 0;
						$totalmetaalocado = 0;
						$totalquantidadealocada = 0;
						$totalcapacidade = 0;
						$totalcandidato = 0;
						$totalinscritos = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalinscritos += $att['inscritos'];
        						$totalmetaalocado += $att['metaalocado'];
        						$totalquantidadealocada += $att['quantidadealocada'];
        						$totalcapacidade += $att['capacidade'];
        						$totalcandidato += $att['candidato'];
        						$totalinscritos += $att['percentual'];
							    ?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['inscritos'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['metaalocado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['quantidadealocada'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['capacidade'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['candidato'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
								</tr>
							<?php endforeach;
						}?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalinscritos,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalmetaalocado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalquantidadealocada,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcapacidade,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcandidato,0,",",".") ?></td>
							<td class="numero bold" > - </td>
						</tr>
					</table>
				<?php
				break;
				//ENSALAMENTO
				case 2:
					$sql = "SELECT e.estuf as estado,
							SUM(foo1.meta) AS meta,
							ef.nu_meta as inscritos,
							COALESCE(SUM(ef.nu_execucao), NULL, 0) AS execucao,
							(COALESCE(SUM(ef.nu_execucao), NULL, 0) * 100)/COALESCE(SUM(ef.nu_meta), NULL, 0) AS percentual
							FROM territorios.estado e
							INNER JOIN
								enem.TB_MAPA_ENEM_2013 ef ON ef.sgl_uf = e.estuf
							INNER JOIN
								(SELECT COALESCE(nu_execucao,0) AS meta, sgl_uf from enem.TB_MAPA_ENEM_2013 where id_indicador = 112) AS foo1 ON foo1.sgl_uf = e.estuf
							WHERE id_indicador = 1
							GROUP BY estado, inscritos
							ORDER BY e.estuf";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Locais de Provas</td>
							<td class="center bold" >Nº Participantes Inscritos</td>
							<td class="center bold" >Nº Participantes Ensalados</td>
							<td class="center bold" >% Executado</td>
						</tr>
						<?php
						$totalmeta = 0;
						$totalinscritos = 0;
						$totalexecucao = 0;
						foreach($arrItem as $att):
    						$totalmeta += $att['meta'];
    						$totalinscritos += $att['inscritos'];
    						$totalexecucao += $att['execucao'];
  					        ?>
							<tr>
								<td><?php echo $att['estado'] ?></td>
								<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['inscritos'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['execucao'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
							</tr>
						<?php endforeach; ?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalmeta,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalinscritos,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalexecucao,0,",",".") ?></td>
							<td class="numero bold" > - </td>
						</tr>
					</table>
				<?php
				break;
				//HOMOLOGAÇÃO DE SEGURANÇA E LOCAL DE APLICAÇÃO
				case 3:
					$sql = "SELECT estuf as estado, meta, COALESCE(naoavaliado,0) AS naoavaliado, COALESCE(semrisco,0) AS semrisco, COALESCE(comrisco,0) AS comrisco
							FROM territorios.estado e
							INNER JOIN
								(SELECT COALESCE(nu_execucao,0) AS meta, sgl_uf from enem.TB_MAPA_ENEM_2013 where id_indicador = 112) AS foo1 ON foo1.sgl_uf = e.estuf
							LEFT JOIN
								(SELECT count(0) as naoavaliado, sg_uf FROM enem.tb_enem_esp_fisico_seg_2013 WHERE tp_situacao_avaliacao_risco = 1 GROUP BY sg_uf) AS foo2 ON foo2.sg_uf = e.estuf
							LEFT JOIN
								(SELECT count(0) as semrisco, sg_uf FROM enem.tb_enem_esp_fisico_seg_2013 WHERE tp_situacao_avaliacao_risco = 2 GROUP BY sg_uf) AS foo3 ON foo3.sg_uf = e.estuf
							LEFT JOIN
								(SELECT count(0) as comrisco, sg_uf FROM enem.tb_enem_esp_fisico_seg_2013 WHERE tp_situacao_avaliacao_risco = 3 GROUP BY sg_uf) AS foo4 ON foo4.sg_uf = e.estuf
							ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Não Avaliado</td>
							<td class="center bold" >Não Apresenta Risco</td>
							<td class="center bold" >Risco Existente</td>
						</tr>
						<?php
						$totalmeta = 0;
						$totalnaoavaliado = 0;
						$totalsemrisco = 0;
						$totalcomrisco = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalmeta += $att['meta'];
        						$totalnaoavaliado += $att['naoavaliado'];
        						$totalsemrisco += $att['semrisco'];
        						$totalcomrisco += $att['comrisco'];
    							?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['naoavaliado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['semrisco'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['comrisco'],0,",",".") ?></td>
								</tr>
							<?php endforeach;
						}?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalmeta,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalnaoavaliado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalsemrisco,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcomrisco,0,",",".") ?></td>
						</tr>
					</table>
				<?php
				break;
				//DISTRIBUIÇÃO DO MATERIAL ADMINISTRATIVO
				case 7:
					$sql = "SELECT
								e.estuf as estado,
								COALESCE(ef.nu_meta,0) as meta,
								COALESCE(nu_execucao,0) as planejado,
								COALESCE((select nu_execucao from enem.TB_MAPA_ENEM_2013 ex where ex.sgl_uf = e.estuf and id_indicador = 115),0) as executado,
								COALESCE((select nu_execucao from enem.TB_MAPA_ENEM_2013 cl where cl.sgl_uf = e.estuf and id_indicador = 12),0) as coletado,
								COALESCE((select nu_execucao from enem.TB_MAPA_ENEM_2013 et where et.sgl_uf = e.estuf and id_indicador = 13),0) as entregue
							FROM territorios.estado e
							INNER JOIN enem.TB_MAPA_ENEM_2013 ef ON ef.sgl_uf = e.estuf
							WHERE id_indicador = 11
							ORDER BY e.estuf";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Kit Planejado</td>
							<td class="center bold" >Kit Executado</td>
							<td class="center bold" >Kit Coletado</td>
							<td class="center bold" >Kit Entregue</td>
						</tr>
						<?php
                        $totalmeta = 0;
                        $totalplanejado = 0;
                        $totalexecutado = 0;
                        $totalcoletado = 0;
                        $totalentregue = 0;
						foreach($arrItem as $att):
                            $totalmeta += $att['meta'];
                            $totalplanejado += $att['planejado'];
                            $totalexecutado += $att['executado'];
                            $totalcoletado += $att['coletado'];
                            $totalentregue += $att['entregue'];
    						?>
							<tr>
								<td><?php echo $att['estado'] ?></td>
								<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['planejado'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['executado'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['coletado'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['entregue'],0,",",".") ?></td>
							</tr>
						<?php endforeach; ?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalmeta,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalplanejado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalexecutado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcoletado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalentregue,0,",",".") ?></td>
						</tr>
					</table>
				<?php
				break;
				//DISTRIBUIÇÃO DE PROVAS
				case 8:
					$sql = "SELECT
								e.estuf as estado,
								COALESCE(ef.nu_meta,0) as meta,
								COALESCE(nu_execucao,0) as planejado,
								COALESCE((select nu_execucao from enem.TB_MAPA_ENEM_2013 ex where ex.sgl_uf = e.estuf and id_indicador = 114),0) as executado,
								COALESCE((select nu_execucao from enem.TB_MAPA_ENEM_2013 cl where cl.sgl_uf = e.estuf and id_indicador = 3),0) as coletado
							FROM territorios.estado e
							INNER JOIN enem.TB_MAPA_ENEM_2013 ef ON ef.sgl_uf = e.estuf
							WHERE id_indicador = 2
							ORDER BY e.estuf";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Malotamento Planejado</td>
							<td class="center bold" >Malotamento Executado</td>
							<td class="center bold" >Malotamento Coletado</td>
						</tr>
						<?php
						$totalmeta = 0;
						$totalplanejado = 0;
						$totalexecutado = 0;
						$totalcoletado = 0;
						foreach($arrItem as $att):

						    // A pedido de Carla em 18/10/2013
						    $att['executado'] = $att['meta'];

    						$totalmeta += $att['meta'];
    						$totalplanejado += $att['planejado'];
    						$totalexecutado += $att['executado'];
    						$totalcoletado += $att['coletado'];
    						?>
							<tr>
								<td><?php echo $att['estado'] ?></td>
								<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['planejado'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['executado'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['coletado'],0,",",".") ?></td>
							</tr>
						<?php endforeach; ?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalmeta,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalplanejado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalexecutado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcoletado,0,",",".") ?></td>
						</tr>
					</table>
				<?php
				break;
				//CERTIFICAÇÃO DE APLICADORES
				case 9:
					$sql = "SELECT
								e.estuf as estado,
								COALESCE(ef.nu_meta,0) as metacoordenador,
								COALESCE(nu_execucao,0) as coocadastrados,
								cooconfirmados,
								(COALESCE(cooconfirmados, 0) * 100)/COALESCE(nu_execucao, 0) AS coopercentual,
								metachefe,
								chefecadastrados,
								chefecooconfirmados,
								(COALESCE(chefecooconfirmados, 0) * 100)/COALESCE(chefecadastrados, 0) AS chefepercentual
							FROM territorios.estado e
							INNER JOIN
								enem.TB_MAPA_ENEM_2013 ef ON ef.sgl_uf = e.estuf
							LEFT JOIN
								(select COALESCE(nu_execucao,0) as cooconfirmados, sgl_uf from enem.TB_MAPA_ENEM_2013 WHERE id_indicador = 118) foo1 ON foo1.sgl_uf = e.estuf
							LEFT JOIN
								(select COALESCE(nu_meta,0) as metachefe, COALESCE(nu_execucao,0) as chefecadastrados, sgl_uf from enem.TB_MAPA_ENEM_2013 WHERE id_indicador = 119) foo2 ON foo2.sgl_uf = e.estuf
							LEFT JOIN
								(select COALESCE(nu_execucao,0) as chefecooconfirmados, sgl_uf from enem.TB_MAPA_ENEM_2013 WHERE id_indicador = 120) foo3 ON foo3.sgl_uf = e.estuf
							WHERE id_indicador = 117
							ORDER BY e.estuf";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" rowspan="2" >UF</td>
							<td class="center bold" colspan="4" >Coordenador/Assistente</td>
							<td class="center bold" colspan="4" >Chefe de Sala</td>
						</tr>
						<tr>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Cadastrados</td>
							<td class="center bold" >Confirmados</td>
							<td class="center bold" >%</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Cadastrados</td>
							<td class="center bold" >Confirmados</td>
							<td class="center bold" >%</td>
						</tr>
						<?php
						$totalmetacoordenador = 0;
						$totalcoocadastrados = 0;
						$totalcooconfirmados = 0;
						$totalcoopercentual = 0;
						$totalmetachefe = 0;
						$totalchefecadastrados = 0;
						$totalchefecooconfirmados = 0;
						foreach($arrItem as $att):
    						$totalmetacoordenador += $att['metacoordenador'];
    						$totalcoocadastrados += $att['coocadastrados'];
    						$totalcooconfirmados += $att['cooconfirmados'];
    						$totalmetachefe += $att['metachefe'];
    						$totalchefecadastrados += $att['chefecadastrados'];
    						$totalchefecooconfirmados += $att['chefecooconfirmados'];
    						?>
							<tr>
								<td><?php echo $att['estado'] ?></td>
								<td class="numero" ><?php echo number_format($att['metacoordenador'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['coocadastrados'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['cooconfirmados'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['coopercentual'],2,",",".") ?>%</td>
								<td class="numero" ><?php echo number_format($att['metachefe'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['chefecadastrados'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['chefecooconfirmados'],0,",",".") ?></td>
								<td class="numero" ><?php echo number_format($att['chefepercentual'],2,",",".") ?>%</td>
							</tr>
						<?php endforeach; ?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalmetacoordenador,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcoocadastrados,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcooconfirmados,0,",",".") ?></td>
							<td class="numero bold" > - </td>
							<td class="numero bold" ><?php echo number_format($totalmetachefe,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalchefecadastrados,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalchefecooconfirmados,0,",",".") ?></td>
							<td class="numero bold" > - </td>
						</tr>
					</table>
				<?php
				break;
				//KIT DE TREINAMENTO DE CAPACITAÇÃO DOS APLICADORES
				case 10:
					$sql = "SELECT
								e.estuf as estado,
								COALESCE(ef.nu_meta,0) as meta,
								COALESCE(nu_execucao,0) as cadastrados,
								confirmados,
								(COALESCE(confirmados, 0) * 100)/COALESCE(ef.nu_meta, 0) AS percentual
							FROM territorios.estado e
							INNER JOIN
								enem.TB_MAPA_ENEM_2013 ef ON ef.sgl_uf = e.estuf
							LEFT JOIN
								(select COALESCE(nu_execucao,0) as confirmados, sgl_uf from enem.TB_MAPA_ENEM_2013 WHERE id_indicador = 122) foo1 ON foo1.sgl_uf = e.estuf
							WHERE id_indicador = 121
							ORDER BY e.estuf";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Cadastrados</td>
							<td class="center bold" >Confirmados</td>
							<td class="center bold" >% Executado</td>
						</tr>
						<?php
						$totalmeta = 0;
						$totalcadastrados = 0;
						$totalconfirmados = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalmeta += $att['meta'];
        						$totalcadastrados += $att['cadastrados'];
        						$totalconfirmados += $att['confirmados'];
    							?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['cadastrados'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['confirmados'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
								</tr>
							<?php endforeach;
						}
						?>
						<tr>
							<td class="bold">Total</td>
							<td class="numero bold" ><?php echo number_format($totalmeta,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcadastrados,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalconfirmados,0,",",".") ?></td>
							<td class="numero bold" > - </td>
						</tr>
					</table>
				<?php
				break;
				//TREINAMENTO DE CORRETORES
				case 11:
					$sql = "SELECT estuf as estado, 0 as total from territorios.estado order by estuf";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Presencial</td>
							<td class="center bold" >Distância</td>
							<td class="center bold" >% Executado</td>
						</tr>
						<?php
						if($arrItem){
							foreach($arrItem as $att): ?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['total'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['total'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['total'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['total'],2,",",".") ?>%</td>
								</tr>
							<?php endforeach;
						}?>
					</table>
				<?php
				break;
				//VALIDAÇÃO DAS ROTAS
				case 12:
					$sql = "SELECT
								e.estuf as estado,
								COALESCE(SUM(ef.nu_meta),null,0) as meta,
								COALESCE(SUM(nu_execucao),null,0) as cadastrados,
								SUM(homologadas) AS homologadas,
								(COALESCE(SUM(homologadas), 0) * 100)/COALESCE(COALESCE(SUM(ef.nu_meta),0), 0) AS percentual
							FROM territorios.estado e
							INNER JOIN
								enem.TB_MAPA_ENEM_2013 ef ON ef.sgl_uf = e.estuf
							LEFT JOIN
								(select COALESCE(SUM(nu_execucao),0) as homologadas, sgl_uf from enem.TB_MAPA_ENEM_2013 WHERE id_indicador in (41,42,43,44) GROUP BY sgl_uf) foo1 ON foo1.sgl_uf = e.estuf
							WHERE id_indicador in (37,38,39,40)
							GROUP BY estado
							ORDER BY e.estuf";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Rotas Criadas</td>
							<td class="center bold" >Rotas Homologadas</td>
							<td class="center bold" >% Executado</td>
						</tr>
						<?php
						$totalmeta = 0;
						$totalcadastrados = 0;
						$totalhomologadas = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalmeta += $att['meta'];
        						$totalcadastrados += $att['cadastrados'];
        						$totalhomologadas += $att['percentual'];
    							?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['cadastrados'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['homologadas'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
								</tr>
							<?php endforeach;
						}
						?>
						<tr>
							<td class="bold">Total</td>
							<td class="numero bold" ><?php echo number_format($totalmeta,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcadastrados,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalhomologadas,0,",",".") ?></td>
							<td class="numero bold" > - </td>
						</tr>
					</table>
				<?php
				break;
				//ATENDIMENTO DIFERENCIADO
				case 13:
					$sql = "SELECT
								sgl_uf AS estado,
								SUM(qt_deficiencia_fisica) AS qt_deficiencia_fisica,
								SUM(qt_baixa_visao) AS qt_baixa_visao,
								SUM(qt_cegueira) AS qt_cegueira,
								SUM(qt_deficiencia_auditiva) AS qt_deficiencia_auditiva,
								SUM(qt_surdez) AS qt_surdez,
								SUM(qt_surdo_cequeira) AS qt_surdo_cequeira,
								SUM(qt_deficiencia_mental) AS qt_deficiencia_mental,
								SUM(qt_dislexia) AS qt_dislexia,
								SUM(qt_deficit_atencao) AS qt_deficit_atencao,
								SUM(qt_autismo) AS qt_autismo,
								SUM(qt_cid) AS qt_cid
							FROM enem.tb_enem_ensalado_2013
							GROUP BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Deficiência física</td>
							<td class="center bold" >Baixa visão</td>
							<td class="center bold" >Cegueira</td>
							<td class="center bold" >Deficiência auditiva</td>
							<td class="center bold" >Surdez</td>
							<td class="center bold" >Surdocegueira</td>
							<td class="center bold" >Deficiência intelectual (mental)</td>
							<td class="center bold" >Dislexia</td>
							<td class="center bold" >Déficit de Atenção</td>
							<td class="center bold" >Autismo</td>
							<td class="center bold" >Outras necessidades especiais</td>
						</tr>
						<?php
						if($arrItem){
							foreach($arrItem as $att): ?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['qt_deficiencia_fisica'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_baixa_visao'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_cegueira'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_deficiencia_auditiva'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_surdez'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_surdo_cequeira'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_deficiencia_mental'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_dislexia'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_deficit_atencao'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_autismo'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_cid'],0,",",".") ?></td>
								</tr>
							<?php endforeach;
						}
						?>
					</table>
				<?php
				break;
				//ATENDIMENTO ESPECÍFICO
				case 14:
					$sql = "SELECT
							sgl_uf AS estado,
							SUM(qt_sabatista) AS qt_sabatista,
							SUM(qt_gestante) AS qt_gestante,
							SUM(qt_lactante) AS qt_lactante,
							SUM(qt_idoso) AS qt_idoso,
							SUM(qt_classe_hospitalar) AS qt_classe_hospitalar
						FROM enem.tb_enem_ensalado_2013
						GROUP BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Sabatista</td>
							<td class="center bold" >Gestante</td>
							<td class="center bold" >Lactante</td>
							<td class="center bold" >Idoso</td>
							<td class="center bold" >Atendimento em<BR>Classe Hospitalar</td>
						</tr>
						<?php
						if($arrItem){
							foreach($arrItem as $att): ?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['qt_sabatista'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_gestante'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_lactante'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_idoso'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['qt_classe_hospitalar'],0,",",".") ?></td>
								</tr>
							<?php endforeach;
						}
						?>
					</table>
				<?php
				break;
				//CARTÃO DE CONFIRMAÇÃO
				case 15:
					$sql = "SELECT sgl_uf,  meta, execucao,
                                CASE
                                    WHEN COALESCE(meta, 0) = 0 THEN 0
                                    ELSE
                                    (COALESCE(execucao, 0) * 100)/COALESCE(meta, 0)::numeric
                                END AS percentual
                            from (
                                SELECT COALESCE(SUM(nu_execucao),0) as execucao, COALESCE(SUM(nu_meta),0) as meta, sgl_uf
                                FROM enem.tb_mapa_enem_2013
                                WHERE id_indicador = 14
                                AND sgl_uf IS NOT NULL
                                group by sgl_uf
                                order by sgl_uf
                            ) as foo";

					$arrItem = $db->carregar($sql);
				?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Execução</td>
							<td class="center bold" >Percentual</td>
						</tr>
						<?php
						$totalExecucao = 0;
						$totalMeta = 0;
						if($arrItem){

							foreach($arrItem as $att):
        						$totalExecucao += $att['execucao'];
        						$totalMeta += $att['meta'];
    							?>
								<tr>
									<td class="numero" ><?php echo $att['sgl_uf']; ?></td>
									<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['execucao'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['percentual'],0,",",".") ?>%</td>
								</tr>
							<?php endforeach;
						}
						?>
						<tr>
							<td class="center bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalExecucao,0,",",".") ?></td>
							<td class="numero bold" > - </td>
						</tr>
					</table>
				<?php
				break;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" width="50%">
			<div>
                <img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					Etapas da Produção
                </div>
            </div>
            <?php

			$sql = "SELECT
						id_atividade,
						no_atividade,
						meta,
						execucao,
						CASE
							WHEN meta = 0 THEN 0
							ELSE
							(COALESCE(execucao, 0) * 100)/COALESCE(meta, 0)::numeric
						END AS percentual
					FROM(
						SELECT
							etpid as id_atividade,
							etpetapa as no_atividade,
							CASE etpid
								WHEN 4 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 29)
								WHEN 5 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 26)
								WHEN 6 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 33)
							END as meta,
							CASE etpid
								WHEN 4 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 29)
								WHEN 5 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 26)
								WHEN 6 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2013 WHERE id_indicador = 33)
							END as execucao
						FROM enem.etapasoperacao
						WHERE etpid IN (4,5,6)
						AND etpstatus = 'A'
						ORDER BY etpordem
					) AS FOO";
            $arrAtividade = $db->carregar($sql);
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="bold" >Etapa</td>
					<td class="center bold" >Meta</td>
					<td class="center bold" >Executado</td>
					<td class="center bold" >% Executado</td>
				</tr>
				<?php foreach($arrAtividade as $att): ?>
					<tr>
						<td ><?php echo str_replace(array("1-","2-","3-","4-","5-","6-","7-","8-"),"",$att['no_atividade']) ?></td>
						<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($att['execucao'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
					</tr>
				<?php endforeach; ?>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" width="50%">
			<div>
                <img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					Ensalamento - Atendimento Diferenciado e Específico
                </div>
            </div>
            <?php

			$sql = "SELECT
							etpid as id_atividade,
							etpetapa as no_atividade
						FROM enem.etapasoperacao
						WHERE etpid IN (13, 14)
						AND etpstatus = 'A'
						ORDER BY etpordem";
            $arrAtividade = $db->carregar($sql);
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="bold" >Atendimento</td>
				</tr>
				<?php foreach($arrAtividade as $att): ?>
					<tr>
						<td class="link" onclick="abreAtividade(<?=$att['id_atividade'] ?>);"><?php echo $att['no_atividade'] ?></td>
					</tr>
				<?php endforeach; ?>
			</table>
		</td>
	</tr>
	<?php
	}
	?>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_td_azul_escuro link" onclick="window.location='/pde/estrategico.php?modulo=principal/cockpit_enem&acao=A';">
            <div>
                <img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
                    Monitoramento<br>ENEM 2012
                </div>
            </div>
        </td>
        <td class="fundo_td_azul_escuro link" onclick="window.location='/pde/estrategico.php?modulo=principal/cockpit_enem2014&acao=A';">
            <div>
                <img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
                    Monitoramento<br>ENEM 2014
                </div>
            </div>
        </td>
        <td class="fundo_td_azul_escuro link" onclick="window.location='/pde/estrategico.php?modulo=principal/cockpit_enem2015&acao=A';">
            <div>
                <img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
                    Monitoramento<br>ENEM 2015
                </div>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_td_laranja" colspan="3">
            <div style="text-align:center;"  >
                <img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
                <input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
                <img src="../imagens/icones/icons/Find.png"  style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
            </div>
        </td>
    </tr>
</table>
<div id="boxes">
<div id="dialog" class="window">
Pontos de Atenção (Certificação de Processos) |
<a href="#"class="close"/>Fechar</a>
<br>
<div id="chart1"></div>
</div>
<!-- Mask to cover the whole screen -->
  <div id="mask"></div>
</div>
<script>

function montarGrafico(executados,ematraso,aexecutar) {


		//Get the A tag
		var id = "#dialog";

		//Get the screen height and width
		var maskHeight = $(document).height();
		var maskWidth = $(window).width();

		//Set heigth and width to mask to fill up the whole screen
		$('#mask').css({'width':maskWidth,'height':maskHeight});

		//transition effect
		$('#mask').fadeIn(1000);
		$('#mask').fadeTo("slow",0.8);

		//Get the window height and width
		var winH = $(window).height();
		var winW = $(window).width();

		//Set the popup window to center
		$(id).css('top',  winH/2-$(id).height()/2);
		$(id).css('left', winW/2-$(id).width()/2);

		//transition effect
		$(id).fadeIn(2000);

  var data = [['Executados', executados],['Em Atraso', ematraso], ['A Executar', aexecutar]];
  var plot1 = jQuery.jqplot ('chart1', [data],
    {
      seriesDefaults: {
        // Make this a pie chart.
        renderer: jQuery.jqplot.PieRenderer,
        rendererOptions: {
          // Put data labels on the pie slices.
          // By default, labels show the percentage of the slice.
          showDataLabels: true
        }
      },
      legend: { show:true, location: 'e' }
    }
  );


}


$(document).ready(function() {

	//if close button is clicked
	$('.window .close').click(function (e) {
		//Cancel the link behavior
		e.preventDefault();

		$('#mask').hide();
		$('.window').hide();
	});

	//if mask is clicked
	$('#mask').click(function () {
		$(this).hide();
		$('.window').hide();
	});


	$(window).resize(function () {

 		var box = $('#boxes .window');

        //Get the screen height and width
        var maskHeight = $(document).height();
        var maskWidth = $(window).width();

        //Set height and width to mask to fill up the whole screen
        $('#mask').css({'width':maskWidth,'height':maskHeight});

        //Get the window height and width
        var winH = $(window).height();
        var winW = $(window).width();

        //Set the popup window to center
        box.css('top',  winH/2 - box.height()/2);
        box.css('left', winW/2 - box.width()/2);

	});

});
</script>
</body>
</html>