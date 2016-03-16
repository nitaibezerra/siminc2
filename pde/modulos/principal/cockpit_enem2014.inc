<?PHP
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( substr($_SESSION['favurl'],0,56) );

if(!empty($_REQUEST['atualizarAbrangencia'])){
    carregarDetalhesAbrangenciaEstado(array('esiid'=>$_REQUEST['esiid']));
    die;
}
if(!empty($_REQUEST['atualizarOcorrencia'])){
    carregarDetalhesOcorrenciaEstado(array('tipoocorrencia'=>$_REQUEST['tipoocorrencia']));
    die;
}
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
		window.location='estrategico.php?modulo=principal/cockpit_enem2014&acao=A&tipo='+tipo;
	}
	function abreRiskManager(nivel,etapasimec, situacao, tipo, origem)
	{
		url = "/pde/estrategico.php?modulo=principal/enem_producao_riskmanager2013&acao=A&nivel=" + nivel + "&situacao=" + situacao + "&tipo=" + tipo + "&etapasimec=" + etapasimec + "&origem=" + origem + "&ano=2014";
	 	window.open(url,'Risk Manager','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}
	function abreAtividade(id_atividade)
	{
		window.location='estrategico.php?modulo=principal/cockpit_enem2014&acao=A&tipo=3&id_atividade='+id_atividade;
	}
    function mudaTabelaEnem(tipotabela)
    {
        window.location.href='estrategico.php?modulo=principal/cockpit_enem2014&acao=A&tipo=1&tipotabela='+tipotabela;
    }

	jQuery(function(){
		jQuery('.toggle_consolidado1').click(function(){
			jQuery('#graficos_consolidados1').toggle('fade');
            jQuery('#graficos_consolidados2').hide();
            jQuery('#graficos_consolidados3').hide();
            jQuery('#graficos_consolidados5').hide();
            jQuery('#graficos_consolidados6').hide();
		});
		setTimeout(function() {jQuery('#graficos_consolidados1').hide()} , 500);
	});
	
	jQuery(function(){
		jQuery('.toggle_consolidado2').click(function(){
			jQuery('#graficos_consolidados2').toggle('fade');
            jQuery('#graficos_consolidados1').hide();
            jQuery('#graficos_consolidados3').hide();
            jQuery('#graficos_consolidados5').hide();
            jQuery('#graficos_consolidados6').hide();
		});
		setTimeout(function() {jQuery('#graficos_consolidados2').hide()} , 500);
	});
	
	jQuery(function(){
		jQuery('.toggle_consolidado3').click(function(){
			jQuery('#graficos_consolidados3').toggle('fade');
            jQuery('#graficos_consolidados1').hide();
            jQuery('#graficos_consolidados2').hide();
            jQuery('#graficos_consolidados5').hide();
            jQuery('#graficos_consolidados6').hide();
		});
		setTimeout(function() {jQuery('#graficos_consolidados3').hide()} , 500);
	});
	
	jQuery(function(){
		jQuery('.toggle_consolidado5').click(function(){
			jQuery('#graficos_consolidados5').toggle('fade');
            jQuery('#graficos_consolidados1').hide();
            jQuery('#graficos_consolidados2').hide();
            jQuery('#graficos_consolidados3').hide();
            jQuery('#graficos_consolidados6').hide();
		});
		setTimeout(function() {jQuery('#graficos_consolidados5').hide()} , 500);
	});
	
	jQuery(function(){
		jQuery('.toggle_consolidado6').click(function(){
			jQuery('#graficos_consolidados6').toggle('fade');
            jQuery('#graficos_consolidados1').hide();
            jQuery('#graficos_consolidados2').hide();
            jQuery('#graficos_consolidados3').hide();
            jQuery('#graficos_consolidados5').hide();
		});
		setTimeout(function() {jQuery('#graficos_consolidados6').hide()} , 500);
	});

    function visualizarXLS(tipo){
        window.location='estrategico.php?modulo=principal/cockpit_enem2014&acao=A&exporta='+tipo;
    }

    function detalharAbrangenciaEstado(esiid, obj) {
        var tabela = obj.parentNode.parentNode.parentNode;
        var linha = obj.parentNode.parentNode;
        if(obj.title=="mais") {
            obj.title    = "menos";
            obj.src      = "../imagens/menos.gif";
            var nlinha   = tabela.insertRow(linha.rowIndex+1);
            var ncol     = nlinha.insertCell(0);
            ncol.colSpan = 2;
            ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+1);
            jQuery('#'+ncol.id).html('Carregando...');
            jQuery('#'+ncol.id).load('estrategico.php?modulo=principal/cockpit_enem2014&acao=A&atualizarAbrangencia=1&esiid='+esiid);
            divCarregado();
        } else {
            obj.title    = "mais";
            obj.src      = "../imagens/mais.gif";
            tabela.deleteRow(linha.rowIndex+1);
        }
    }

    function detalharOcorrenciaEstado(tipoocorrencia, obj) {
        var tabela = obj.parentNode.parentNode.parentNode;
        var linha = obj.parentNode.parentNode;
        if(obj.title=="mais") {
            obj.title    = "menos";
            obj.src      = "../imagens/menos.gif";
            var nlinha   = tabela.insertRow(linha.rowIndex+1);
            var ncol     = nlinha.insertCell(0);
            ncol.colSpan = 2;
            ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+1);
            jQuery('#'+ncol.id).html('Carregando...');
            jQuery('#'+ncol.id).load('estrategico.php?modulo=principal/cockpit_enem2014&acao=A&atualizarOcorrencia=1&tipoocorrencia='+tipoocorrencia);
            divCarregado();
        } else {
            obj.title    = "mais";
            obj.src      = "../imagens/mais.gif";
            tabela.deleteRow(linha.rowIndex+1);
        }
    }
</script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.css" />
 <style>
  	.fundo_td{background-color:#0D3502}
  	.titulo_pagina{font-weight:bold;font-size:20px;color:#FFFFFF}
  	.titulo_box{font-weight:bold;font-size:18px;color:#FFFFFF;margin-top:15px;text-shadow:#000000 0px 1px 2px}
  	.subtitulo_box{font-weight:normal;font-size:10px;color:#FFFFFF}
  	.fundo_td:hover {background-color:#104202}
  	.fundo_td{text-align:left;vertical-align:top;}
  	.tabela_painel{font-weight:bold;font-size:8px;color:#FFFFFF;font-family:fantasy}
  	.lista_metas{float:left}
  	#busca{background: none repeat scroll 0% 0% rgb(255, 255, 255); width:400px;border-width: 1px; border-style: solid; border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204); color: rgb(0, 0, 0); font: 18px arial,sans-serif bold; height: 35px;}
  	.tabela_box{color:#FFFFFF;}
  	.tabela_box td{background-color:#166002;text-shadow:#000000 0px 2px 2px}
  	.tabela_box_azul td{background-color:#63B8FF;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
  	.tabela_box_azul_escuro td{background-color:#204481;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
  	.fundo_td_azul{background-color:#01A2D8}
  	.fundo_td_azul:hover{background-color:#2B86CC}
  	.fundo_td_azul_hover{background-color:#2B86CC}
  	.fundo_td_verde{background-color:#166002}
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
    #popdiv2{display:block;width:100%;height:600px;overflow-y: scroll;}
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
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_enem2014&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<?php
	//Tipos
	//1 - INSCRIÇÕES
	//2 - MONITORAMENTO/CERTIFICAÇÃO
	//3 - OPERAÇÃO LOGÍSTICA
    //4 - GESTÃO DE TI
	if($_REQUEST['tipo']){
		$tipo = $_REQUEST['tipo'];
	}else{
		$tipo = 1;
	}

    if($_REQUEST['tipotabela']){
        $tipotabela = $_REQUEST['tipotabela'];
    }else{
        $tipotabela = 1;
    }

    if($tipotabela==2){
        $tabelaEnem = "ts_enem_pre_inscricao_2014";
    }else{
        $tabelaEnem = "ts_enem_inscricao_2014";
    }

    //VERIFICA PERMISSÃO DE ACESSOS A DETERMINADOS USUÁRIOS
    if($tipo==2 || $tipo==3 || $tipo==4){
        if($_SESSION['usucpf']=='' || $_SESSION['usucpf']=='' || $_SESSION['usucpf']==''){
            $db->sucesso( 'estrategico.php?modulo=principal/cockpit_enem2014&acao=A', '', 'O seu perfil não dá permissão para acessar essa informação!', 'N', 'N' );
        }
    }
	?>
	<!-- Título-->
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >Monitoramento ENEM 2014</div></td>
	</tr>
	<!-- Fim Título-->
	<tr>
		<td class="<?=($tipo==1)?'fundo_td_azul_hover':'fundo_td_azul'?>" valign="top" width="25%">
            <div>
                <?php exibirTitulo('doc', '<span class="link" onclick="abreTipo(1);">INSCRIÇÕES</span>'); ?>
                <br>
                <br>
                <br>
                <input type="radio" name="tabelaENEM" value="1" <?php echo 1 == $tipotabela ? 'checked="checked"': ''; ?> onclick="mudaTabelaEnem(1);" />Inscrições
                <input type="radio" name="tabelaENEM" value="2" <?php echo 2 == $tipotabela ? 'checked="checked"': ''; ?> onclick="mudaTabelaEnem(2);" />Pré-inscrições
            </div>
		</td>
		<td class="<?=($tipo==2)?'fundo_td_azul_hover':'fundo_td_azul'?> link" valign="top" width="25%" onclick="abreTipo(2);">
		<div>
			<?php exibirTitulo('casas', 'MONITORAMENTO'); ?>
		</div>
		</td>
		<td class="<?=($tipo==3)?'fundo_td_azul_hover':'fundo_td_azul'?> link" valign="top" width="25%" onclick="abreTipo(3);">
		<div>
			<?php exibirTitulo('configs', 'OPERAÇÃO LOGÍSTICA'); ?>
		</div>
		</td>
        <td class="<?=($tipo==4)?'fundo_td_azul_hover':'fundo_td_azul'?> link" valign="top" width="25%" onclick="abreTipo(4);">
            <div>
                <?php exibirTitulo('configs', 'GESTÃO DE TI'); ?>
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
		$sql = "SELECT SUM(qt_total) as total FROM enem.{$tabelaEnem}";
		$Inscricao = $db->pegaUm($sql);
		?>
		<tr>
			<td class="fundo_td_laranja" valign="top" width="34%" >
				<?php
				//RETIRAR APÓS A CORREÇÃO DA CARGA
				$correcao = 0;
				$sql = "SELECT to_char(data_atualizacao, 'DD/MM/YYYY') as dataatualizacao, to_char(data_atualizacao, 'HH24:MI') as hora
					FROM enem.{$tabelaEnem}
					ORDER BY dataatualizacao DESC
					LIMIT 1";
				$arrInscricao = $db->pegaLinha($sql);
				 ?>
				<div>
					<?php exibirTitulo('indicador', number_format($Inscricao,0,",",".").' Inscrições', 'Atualizado em : '.$arrInscricao['dataatualizacao'].' às '.$arrInscricao['hora'].''); ?>
				</div>
			</td>
			<td class="fundo_td" width="33%" >
				<div>
					<?php exibirTitulo('doc', 'Por Categoria'); ?>
				</div>
				<?php
				$sql = "SELECT (SUM(qt_sem_pagamento) + SUM(qt_carencia_indeferida) + SUM(qt_pagamento_efetuado)) AS pagantes, (SUM(qt_carencia_aceita) + SUM(qt_inscricao_gratuita)) AS isentos,
						(SUM(qt_sem_pagamento) + SUM(qt_carencia_indeferida) + SUM(qt_pagamento_efetuado) + SUM(qt_carencia_aceita) + SUM(qt_inscricao_gratuita)) AS total
						FROM enem.{$tabelaEnem}";
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
					<?php exibirTitulo('mundo', 'Por Região'); ?>
				</div>
				<?php
				$sql = "SELECT no_regiao, SUM(qt_total) as total
					FROM enem.{$tabelaEnem}
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
				$sql = "SELECT sum(qt_pediu_certificado) FROM enem.{$tabelaEnem}";
				$total_certificado = $db->pegaUm($sql);
				?>
				<div>
					<?php exibirTitulo('indicador', number_format($total_certificado,0,",",".").' Inscrições para <br />Certificação', 'Atualizado em : '.$arrInscricao['dataatualizacao'].' às '.$arrInscricao['hora'].''); ?>
				</div>
			</td>

			<td class="fundo_td" >
				<div>
					<?php exibirTitulo('executive', 'Sexo'); ?>
				</div>
				<?php
				$sql = "SELECT sum(qt_sexo_masculino) as sexo_masculino, sum(qt_sexo_feminino) as sexo_feminino, (sum(qt_sexo_masculino) + sum(qt_sexo_feminino)) as total
						FROM enem.{$tabelaEnem}";
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
					<?php exibirTitulo('doc', 'Por Modalidade'); ?>
				</div>
				<?php
				$sql = "SELECT SUM(qt_concluiu_em) as concluiu_em, SUM(qt_concluira_em) as concluira_em, SUM(qt_concluira_apos_em) as concluira_apos_em, SUM(qt_nao_concluiu) as nao_concluiu, (SUM(qt_concluiu_em) + SUM(qt_concluira_em) + SUM(qt_concluira_apos_em) + SUM(qt_nao_concluiu)) as totalsituacao,
						SUM(qt_ensino_regular) AS ensino_regular, SUM(qt_eja) AS eja, SUM(qt_ensino_especial) AS ensino_especial, SUM(qt_ensino_outros) as ensino_outros, (SUM(qt_ensino_regular) + SUM(qt_eja) + SUM(qt_ensino_especial) + SUM(qt_ensino_outros)) AS totalmodalidade
						FROM enem.{$tabelaEnem}";
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
					<?php exibirTitulo('executive', 'Cor/Raça'); ?>
				</div>
				<?php
				$sql = "SELECT sum(qt_cor_raca_branca) as cor_raca_branca, sum(qt_cor_raca_preta) as cor_raca_preta, sum(qt_cor_raca_parda) as cor_raca_parda, sum(qt_cor_raca_amarela) as cor_raca_amarela, sum(qt_cor_raca_indigena) as cor_raca_indigena, sum(qt_cor_raca_nao_declarada) + $correcao as cor_raca_nao_declarada,
						(sum(qt_cor_raca_branca) + sum(qt_cor_raca_preta) + sum(qt_cor_raca_parda) + sum(qt_cor_raca_amarela) + sum(qt_cor_raca_indigena) + sum(qt_cor_raca_nao_declarada) + $correcao) as total
						FROM enem.{$tabelaEnem}";
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
					<?php exibirTitulo('mundo', 'Por Estado'); ?>
				</div>
				<?php
				$sql = "SELECT sgl_uf, SUM(qt_total) as totalinscricao, sum(qt_pediu_certificado) as totalcertificado
					FROM enem.{$tabelaEnem}
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
				$sql = "SELECT sum(qt_concluiu_em) as concluiu_em, sum(qt_concluira_em) as concluira_em, sum(qt_concluira_apos_em) as concluira_apos_em, sum(qt_nao_concluiu) + $correcao as nao_concluiu, (sum(qt_concluiu_em) + sum(qt_concluira_em) + sum(qt_concluira_apos_em) + sum(qt_nao_concluiu) + $correcao) as totalsituacao,
						sum(qt_ensino_regular) AS ensino_regular, sum(qt_eja) AS eja, sum(qt_ensino_especial) AS ensino_especial, sum(qt_ensino_outros) as ensino_outros, (sum(qt_ensino_regular) + sum(qt_eja) + sum(qt_ensino_especial) + sum(qt_ensino_outros)) AS totalmodalidade
						FROM enem.{$tabelaEnem}";
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
						<td>Concluirá em 2014</td>
						<td class="numero" ><?php echo number_format($arrIns['concluira_em'],0,",",".") ?></td>
					</tr>
					<tr>
						<td>Concluirá após 2014</td>
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
				$sql = "SELECT SUM(qt_sem_pagamento) AS sem_pagamento, SUM(qt_carencia_indeferida) AS carencia_indeferida, SUM(qt_pagamento_efetuado) AS pagamento_efetuado, SUM(qt_carencia_aceita) AS carencia_aceita, SUM(qt_inscricao_gratuita) AS inscricao_gratuita,
						(SUM(qt_sem_pagamento) + SUM(qt_carencia_indeferida) + SUM(qt_pagamento_efetuado) + SUM(qt_carencia_aceita) + SUM(qt_inscricao_gratuita)) AS total
						FROM enem.{$tabelaEnem}";
				$pag = $db->pegaLinha($sql);
				?>
				<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Situação</td>
						<td class="center bold" >Quantidade</td>
					</tr>
					<tr>
						<td class="" >Pagamento Efetuado</td>
						<td class="numero" ><?php echo number_format($pag['pagamento_efetuado'],0,",",".") ?></td>
					</tr>
                    <tr>
                        <td class="" >Pendente de Pagamento</td>
                        <td class="numero" ><?php echo number_format($pag['sem_pagamento'],0,",",".") ?></td>
                    </tr>
					<tr>
						<td class="" >Carência Deferida (Baixa Renda)</td>
						<td class="numero" ><?php echo number_format($pag['carencia_aceita'],0,",",".") ?></td>
					</tr>
                    <tr>
                        <td class="" >Carência Indeferida</td>
                        <td class="numero" ><?php echo number_format($pag['carencia_indeferida'],0,",",".") ?></td>
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
				$sql = "SELECT sum(qt_menor_de_16) as menor_de_16, sum(qt_igual_a_16) as igual_a_16, sum(qt_igual_a_17) as igual_a_17, sum(qt_igual_a_18) as igual_a_18,
						sum(qt_igual_a_19) as igual_a_19, sum(qt_igual_a_20) as igual_a_20, sum(qt_entre_21_a_30) as entre_21_a_30, sum(qt_maior_de_30) as maior_de_30,
						(sum(qt_menor_de_16) + sum(qt_igual_a_16) + sum(qt_igual_a_17) + sum(qt_igual_a_18) +
						sum(qt_igual_a_19) + sum(qt_igual_a_20) + sum(qt_entre_21_a_30) + sum(qt_maior_de_30)) as total
						FROM enem.{$tabelaEnem}";
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
                        Atendimento Específico<br/>
					</div>
				</div>
				<?php
				$sql = "SELECT sum(qt_sala_mais_facil_acesso) as sala_mais_facil_acesso,
							   sum(qt_prova_ampliada) as prova_ampliada,
							   sum(qt_prova_ampliada_18) as prova_ampliada_18,
							   sum(qt_prova_braille) as prova_braille,
							   sum(qt_auxilio_ledor) as auxilio_ledor,
							   sum(qt_interprete_libras) as interprete_libras,
							   sum(qt_auxilio_transcricao) as auxilio_transcricao,
							   sum(qt_classe_hospitalar) as classe_hospitalar,
							   sum(qt_cid) as qt_cid,
                                sum(qt_apoio_perna) as qt_apoio_perna,
                                sum(qt_guia_interprete) as qt_guia_interprete,
                                sum(qt_leitura_labial) as qt_leitura_labial,
                                sum(qt_mesa_cadeira_rodas) as qt_mesa_cadeira_rodas,
                                sum(qt_mesa_cadeira_separada) as qt_mesa_cadeira_separada,
                                sum(qt_dispensa_recurso) as qt_dispensa_recurso,
							   sum(qt_sabatista) as sabatista,
							   sum(qt_gestante) as gestante,
							   sum(qt_lactante) as lactante,
							   sum(qt_idoso) as idoso,
							   (sum(qt_sala_mais_facil_acesso) + sum(qt_prova_ampliada) + sum(qt_prova_ampliada_18) + sum(qt_prova_braille) +
							   sum(qt_auxilio_ledor) + sum(qt_interprete_libras) + sum(qt_auxilio_transcricao) + sum(qt_classe_hospitalar) +
							   sum(qt_cid) + sum(qt_sabatista) + sum(qt_gestante) + sum(qt_lactante) + sum(qt_idoso)) AS total
						FROM enem.{$tabelaEnem}";
				$mod = $db->pegaLinha($sql);
                $sql = "SELECT
                            SUM(qt_cpf_atd_especifico) AS qt_cpf_atd_especifico,
							SUM(qt_cpf_atd_especializado) AS qt_cpf_atd_especializado,
							SUM(qt_cpf_atd_ambos) AS qt_cpf_atd_ambos
						FROM enem.{$tabelaEnem}";
				$cpfDistintos = $db->pegaLinha($sql);
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
						<td class="bold numero" ><?php echo number_format($cpfDistintos['qt_cpf_atd_especifico'],0,",",".") ?></td>
					</tr>
				</table>
			</td>
			<td class="fundo_td" rowspan="1">
				<div>
					<img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
                        Atendimento Especializado<br>por deficiência e/ou Condição Especial<br/>
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
                        <td class="" >Apoio para perna</td>
                        <td class="numero" ><?php echo number_format($mod['qt_apoio_perna'],0,",",".") ?></td>
                    </tr>
                    <tr>
                        <td class="" >Guia Interprete</td>
                        <td class="numero" ><?php echo number_format($mod['qt_guia_interprete'],0,",",".") ?></td>
                    </tr>
                    <tr>
                        <td class="" >Leitura Labial</td>
                        <td class="numero" ><?php echo number_format($mod['qt_leitura_labial'],0,",",".") ?></td>
                    </tr>
                    <tr>
                        <td class="" >Mesa para cadeira de rodas</td>
                        <td class="numero" ><?php echo number_format($mod['qt_mesa_cadeira_rodas'],0,",",".") ?></td>
                    </tr>
                    <tr>
                        <td class="" >Mesa com cadeira separada</td>
                        <td class="numero" ><?php echo number_format($mod['qt_mesa_cadeira_separada'],0,",",".") ?></td>
                    </tr>
                    <tr>
                        <td class="" >Dispensa Recurso</td>
                        <td class="numero" ><?php echo number_format($mod['qt_dispensa_recurso'],0,",",".") ?></td>
                    </tr>
					<tr>
						<td class="" >Outro Atendimento (CID)</td>
						<td class="numero" ><?php echo number_format($mod['qt_cid'],0,",",".") ?></td>
					</tr>
					<tr>
						<td class="bold" >Total de Inscritos/CPF</td>
						<td class="bold numero" ><?php echo number_format($cpfDistintos['qt_cpf_atd_especializado'],0,",",".") ?></td>
					</tr>
				</table>
			</td>
			<td class="fundo_td_azul_escuro" rowspan="1">
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
                <table>
                    <tr>
                        <td class="fundo_padrao" align="left">
                            <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(1);" style="cursor: pointer;"/>
                        </td>
                    </tr>
                </table>
			</td>
		</tr>
    <?php /* ?>
        <tr>
            <td class="fundo_td" rowspan="1">
                <div>
                    <img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                    <div style="float:left" class="titulo_box" >
                        Atendimento Específico / Especializado<br/>
                    </div>
                </div>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <td class="bold" >Total de Inscritos/CPF</td>
                        <td class="bold numero" ><?php echo number_format($cpfDistintos['qt_cpf_atd_ambos'],0,",",".") ?></td>
                    </tr>
                </table>
            </td>
        </tr>
 <?php */ ?>
		<tr>
			<td class="fundo_td_azul center"><p class="toggle_consolidado1 titulo_box link">Tipo de Atendimento Específico</p></td>
			<td class="fundo_td_azul center"><p class="toggle_consolidado2 titulo_box link">Atendimento Especializado</p></td>
			<td class="fundo_td_azul center" rowspan="2"><p class="toggle_consolidado3 titulo_box link">Ausentes</p></td>
		</tr>
		<tr>
			<td class="fundo_td_azul center"><p class="toggle_consolidado5 titulo_box link">Inscritos em 2013</p></td>
			<td class="fundo_td_azul center"><p class="toggle_consolidado6 titulo_box link">Ausentes em Potencial - Atendimento Específico</p></td>
		</tr>
	</table>
	<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
		<tr id="graficos_consolidados1">
			<td class="fundo_td" valign="top" colspan="2">
				<div>
					<img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
                        Tipo de Atendimento Específico
					</div>
				</div>
				<?php
				$sql = "SELECT
							est.estuf,
							sum(qt_sabatista) as sabatista,
							sum(qt_gestante) as gestante,
							sum(qt_lactante) as lactante,
							sum(qt_idoso) as idoso,
							sum(qt_classe_hospitalar) as classe_hospitalar,
							(sum(qt_sabatista) + sum(qt_gestante) + sum(qt_lactante) + sum(qt_idoso) + sum(qt_classe_hospitalar)) AS total
						FROM territorios.estado est
						LEFT JOIN enem.{$tabelaEnem} ins ON ins.sgl_uf = est.estuf
						GROUP BY est.estuf
						ORDER BY est.estuf";
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
							<td class="" ><?php echo $uf['estuf'] ?></td>
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
		</tr>
		<tr id="graficos_consolidados2">
			<td class="fundo_td" valign="top" colspan="2">
				<div>
					<img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						Atendimento Especializado<br/>
					</div>
				</div>
				<?php
				$sql = "SELECT
							est.estuf,
							sum(qt_sala_mais_facil_acesso) as sala_mais_facil_acesso,
							sum(qt_prova_ampliada) as prova_ampliada,
							sum(qt_prova_ampliada_18) as prova_ampliada_18,
							sum(qt_prova_braille) as prova_braille,
							sum(qt_auxilio_ledor) as auxilio_ledor,
							sum(qt_interprete_libras) as interprete_libras,
							sum(qt_auxilio_transcricao) as auxilio_transcricao,
							sum(qt_cid) as qt_cid,
                            sum(qt_apoio_perna) as qt_apoio_perna,
                            sum(qt_guia_interprete) as qt_guia_interprete,
                            sum(qt_leitura_labial) as qt_leitura_labial,
                            sum(qt_mesa_cadeira_rodas) as qt_mesa_cadeira_rodas,
                            sum(qt_mesa_cadeira_separada) as qt_mesa_cadeira_separada,
                            sum(qt_dispensa_recurso) as qt_dispensa_recurso,
							(sum(qt_sala_mais_facil_acesso) + sum(qt_prova_ampliada) + sum(qt_prova_ampliada_18) + sum(qt_prova_braille) +
							sum(qt_auxilio_ledor) + sum(qt_interprete_libras) + sum(qt_auxilio_transcricao) + sum(qt_cid) +
							sum(qt_apoio_perna) + sum(qt_guia_interprete) + sum(qt_leitura_labial) + sum(qt_mesa_cadeira_rodas) +
							sum(qt_mesa_cadeira_separada) + sum(qt_dispensa_recurso)) AS total
						FROM territorios.estado est
						LEFT JOIN enem.{$tabelaEnem} ins ON ins.sgl_uf = est.estuf
						GROUP BY est.estuf
						ORDER BY est.estuf";
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
                        <td class="center bold" >Apoio para perna</td>
                        <td class="center bold" >Guia Interprete</td>
                        <td class="center bold" >Leitura Labial</td>
                        <td class="center bold" >Mesa para cadeira de rodas</td>
                        <td class="center bold" >Mesa com cadeira separada</td>
                        <td class="center bold" >Dispensa Recurso</td>
						<td class="center bold" >Outro Atendimento (CID)</td>
						<td class="center bold" >Total</td>
					</tr>
					<?php
					if($arrUF){
						foreach($arrUF as $uf): ?>
						<tr>
							<td class="" ><?php echo $uf['estuf'] ?></td>
							<td class="numero" ><?php echo number_format($uf['sala_mais_facil_acesso'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['prova_ampliada'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['prova_ampliada_18'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['prova_braille'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['auxilio_ledor'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['interprete_libras'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['auxilio_transcricao'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($uf['qt_apoio_perna'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($uf['qt_guia_interprete'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($uf['qt_leitura_labial'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($uf['qt_mesa_cadeira_rodas'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($uf['qt_mesa_cadeira_separada'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($uf['qt_dispensa_recurso'],0,",",".") ?></td>
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
                        $totalqt_apoio_perna+=$uf['qt_apoio_perna'];
						$totalqt_guia_interprete+=$uf['qt_guia_interprete'];
						$totalqt_leitura_labial+=$uf['qt_leitura_labial'];
						$totalqt_mesa_cadeira_rodas+=$uf['qt_mesa_cadeira_rodas'];
						$totalqt_mesa_cadeira_separada+=$uf['qt_mesa_cadeira_separada'];
						$totalqt_dispensa_recurso+=$uf['qt_dispensa_recurso'];
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
                        <td class="numero bold" ><?php echo number_format($totalqt_apoio_perna,0,",",".") ?></td>
                        <td class="numero bold" ><?php echo number_format($totalqt_guia_interprete,0,",",".") ?></td>
                        <td class="numero bold" ><?php echo number_format($totalqt_leitura_labial,0,",",".") ?></td>
                        <td class="numero bold" ><?php echo number_format($totalqt_mesa_cadeira_rodas,0,",",".") ?></td>
                        <td class="numero bold" ><?php echo number_format($totalqt_mesa_cadeira_separada,0,",",".") ?></td>
                        <td class="numero bold" ><?php echo number_format($totalqt_dispensa_recurso,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalqt_cid,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalgeraldiferenciado,0,",",".") ?></td>
					</tr>
				</table>
				<div>
					<img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						Deficiências<br/>
					</div>
				</div>
				<?php
				$sql = "SELECT
								est.estuf AS estado,
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
								SUM(qt_discalculia) AS qt_discalculia,
								(sum(qt_deficiencia_fisica) + sum(qt_baixa_visao) + sum(qt_cegueira) + sum(qt_deficiencia_auditiva) +
							    sum(qt_surdez) + sum(qt_surdo_cequeira) + sum(qt_deficiencia_mental) + sum(qt_dislexia) + sum(qt_deficit_atencao) +
							    sum(qt_autismo) + sum(qt_discalculia)) AS total
							FROM territorios.estado est
							LEFT JOIN enem.{$tabelaEnem} ins ON ins.sgl_uf = est.estuf
							GROUP BY estado
							ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
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
							<td class="center bold" >Discalculia</td>
                            <td class="center bold" >Total</td>
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
									<td class="numero" ><?php echo number_format($att['qt_discalculia'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['total'],0,",",".") ?></td>
								</tr>
							    <?php
                                $totalqt_deficiencia_fisica+=$att['qt_deficiencia_fisica'];
                                $totalqt_baixa_visao+=$att['qt_baixa_visao'];
                                $totalqt_cegueira+=$att['qt_cegueira'];
                                $totalqt_deficiencia_auditiva+=$att['qt_deficiencia_auditiva'];
                                $totalqt_surdez+=$att['qt_surdez'];
                                $totalqt_surdo_cequeira+=$att['qt_surdo_cequeira'];
                                $totalqt_deficiencia_mental+=$att['qt_deficiencia_mental'];
                                $totalqt_dislexia+=$att['qt_dislexia'];
                                $totalqt_deficit_atencao+=$att['qt_deficit_atencao'];
                                $totalqt_autismo+=$att['qt_autismo'];
                                $totalqt_discalculia+=$att['qt_discalculia'];
                                $totalgeraldeficiencia+=$att['total'];
                            endforeach;
						}
						?>
                        <tr>
                            <td class="bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalqt_deficiencia_fisica,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalqt_baixa_visao,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalqt_cegueira,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalqt_deficiencia_auditiva,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalqt_surdez,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalqt_surdo_cequeira,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalqt_deficiencia_mental,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalqt_dislexia,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalqt_deficit_atencao,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalqt_autismo,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalqt_discalculia,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalgeraldeficiencia,0,",",".") ?></td>
                        </tr>
					</table>
			</td>
		</tr>
		<tr id="graficos_consolidados3">
			<td class="fundo_td" valign="top" colspan="2">
				<div>
					<img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						Inscritos em 2014<br>(Ausentes anos anteriores)<br/>
					</div>
				</div>
				<?php
				$sql = "SELECT
                            sgl_uf AS estuf,
                            SUM(qt_ausente_12) AS ausentes2012,
                            SUM(qt_ausente_13) AS ausentes2013,
                            SUM(qt_ausente_12_13) AS ausentes20122013
                        FROM enem.{$tabelaEnem}
                        GROUP BY estuf
                        ORDER BY estuf";
				$arrUF = $db->carregar($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >UF</td>
						<td class="center bold" >Ausentes em 2012</td>
						<td class="center bold" >Ausentes em 2013</td>
                        <td class="center bold" >Ausentes em 2012 e 2013</td>
					</tr>
					<?php
					if($arrUF){
						foreach($arrUF as $uf): ?>
						<tr>
							<td class="" ><?php echo $uf['estuf'] ?></td>
							<td class="numero" ><?php echo number_format($uf['ausentes2012'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['ausentes2013'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($uf['ausentes20122013'],0,",",".") ?></td>
						</tr>
						<?php
						$totalausentes2012+=$uf['ausentes2012'];
						$totalausentes2013+=$uf['ausentes2013'];
                        $totalausentes20122013+=$uf['ausentes20122013'];
						endforeach;
					}
					?>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($totalausentes2012,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalausentes2013,0,",",".") ?></td>
                        <td class="numero bold" ><?php echo number_format($totalausentes20122013,0,",",".") ?></td>
					</tr>
				</table>
			</td>
			<td class="fundo_td" valign="top" colspan="2">
				<div>
					<img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						Ausentes em Potencial<br/>
					</div>
				</div>
				<?php
				$sql = "SELECT
                            est.estuf, qt_inscrito_perfil_1 AS perfil1, qt_inscrito_perfil_2 AS perfil2
                        FROM territorios.estado est
                        LEFT JOIN enem.tb_possivel_ausente_2014 pa ON est.estuf = pa.sgl_uf
                        ORDER BY est.estuf";
				$arrUF = $db->carregar($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >UF</td>
						<td class="center bold" >Perfil 1*</td>
						<td class="center bold" >Perfil 2*</td>
					</tr>
					<?php
					if($arrUF){
						foreach($arrUF as $uf): ?>
						<tr>
							<td class="" ><?php echo $uf['estuf'] ?></td>
							<td class="numero" ><?php echo number_format($uf['perfil1'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['perfil2'],0,",",".") ?></td>
						</tr>
						<?php
						$totalperfil1+=$uf['perfil1'];
						$totalperfil2+=$uf['perfil2'];
						endforeach;
					}
					?>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($totalperfil1,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalperfil2,0,",",".") ?></td>
					</tr>
                </table>
                <table>
                    <tr>
                        <td class="bold" >
                            * Perfil 1: Inscritos em 2014 na condição de concluintes do Ensino Médio, carência deferida, maiores de 21 anos, com renda familiar de até 1,5 salários mínimos, e que trabalham ou já trabalharam.
                            <br><br>
                            * Perfil 2: Inscritos em 2014 na condição de concluintes do Ensino Médio, carência deferida, e que trabalham ou já trabalharam.
                        </td>
                    </tr>
                </table>
			</td>
		</tr>
		<tr id="graficos_consolidados5">
			<td class="fundo_td" valign="top" colspan="2">
				<div>
					<img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						Inscritos em 2013<br/>
					</div>
				</div>
				<?php
				$sql = "SELECT sgl_uf AS estuf, SUM(total_uf) AS inscritos, 0 AS selecionadossisu, 0 AS naoselecionadossisu
                        FROM enem.ts_enem_inscricao_2013
                        GROUP BY sgl_uf
                        ORDER BY sgl_uf";
				$arrUF = $db->carregar($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >UF</td>
						<td class="center bold" >Inscritos em 2013</td>
						<td class="center bold" >Selecionados no Sisu em 2013</td>
						<td class="center bold" >Não selecionados no Sisu em 2013</td>
					</tr>
					<?php
					if($arrUF){
						foreach($arrUF as $uf): ?>
						<tr>
							<td class="" ><?php echo $uf['estuf'] ?></td>
							<td class="numero" ><?php echo number_format($uf['inscritos'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['selecionadossisu'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['naoselecionadossisu'],0,",",".") ?></td>
						</tr>
						<?php
						$totalinscritos+=$uf['inscritos'];
						$totalselecionadossisu+=$uf['selecionadossisu'];
						$totalnaoselecionadossisu+=$uf['naoselecionadossisu'];
						endforeach;
					}
					?>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($totalinscritos,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalselecionadossisu,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalnaoselecionadossisu,0,",",".") ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr id="graficos_consolidados6">
			<td class="fundo_td" valign="top" colspan="2">
				<div>
					<img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >
						Ausentes em Potencial - Atendimentos Específicos<br/>
					</div>
				</div>
				<?php
				$sql = "SELECT
							estuf, 0 AS idosos, 0 AS gestantes, 0 AS sabatistas, 0 AS classehospitalar
						FROM territorios.estado
						ORDER BY estuf";
				$arrUF = $db->carregar($sql);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >UF</td>
						<td class="center bold" >Idosos</td>
						<td class="center bold" >Gestantes</td>
						<td class="center bold" >Sabatistas</td>
						<td class="center bold" >Classe Hospitalar</td>
					</tr>
					<?php
					if($arrUF){
						foreach($arrUF as $uf): ?>
						<tr>
							<td class="" ><?php echo $uf['estuf'] ?></td>
							<td class="numero" ><?php echo number_format($uf['idosos'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['gestantes'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['sabatistas'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($uf['classehospitalar'],0,",",".") ?></td>
						</tr>
						<?php
						$totalidosos+=$uf['idosos'];
						$totalgestantes+=$uf['gestantes'];
						$totalsabatistas+=$uf['sabatistas'];
						$totalclassehospitalar+=$uf['classehospitalar'];
						endforeach;
					}
					?>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero bold" ><?php echo number_format($totalidosos,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalgestantes,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalsabatistas,0,",",".") ?></td>
						<td class="numero bold" ><?php echo number_format($totalclassehospitalar,0,",",".") ?></td>
					</tr>
				</table>
			</td>
		</tr>
	</table>
	<?php
	//========================================================================================================================================
	//MONITORAMENTO/CERTIFICAÇÃO
	//========================================================================================================================================
	}elseif($tipo==2){
	?>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_td" width="50%" >
			<?php
			montaTabelaExamesMec('Pontos de Atenção<br>(Certificação de Processos)', array(26,35));
			?>
            <br>
            <br>
            <?php
            $sql = "SELECT
                        '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharAbrangenciaEstado(\''||esi.esiid||'\',this);\">&nbsp;' || esi.esidesc as acao,
                        SUM(esd.esdinscritos) AS total
                    FROM enem.estudoinscrito esi
                    LEFT JOIN enem.estudoinscritodados_2014 esd ON esd.esiid = esi.esiid
                    WHERE esi.esiid IN (9)
                    GROUP BY esi.esiid, esi.esidesc, esi.esiordem
                    ORDER BY esi.esiordem";
            $arrDados = $db->carregar($sql);
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <td class="center bold" colspan="2">Estudo de dados de inscritos</td>
                </tr>
                <tr>
                    <td class="center bold" >&nbsp;</td>
                    <td class="center bold" >nº de inscritos</td>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $chave => $dado): ?>
                        <tr>
                            <td class=""><?php echo $dado['acao'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['total'],0,",",".") ?></td>
                        </tr>
                    <?php endforeach; }?>
            </table>
        </td>
        <td class="fundo_td" width="50%">
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
                    AND ( _atinumero LIKE '26%' OR _atinumero LIKE '35%' )
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
                <tr class="link" onclick="abreRelatorio('form=1&agrupadores[0]=atividades&etapa_campo_flag=1&etapa[0]=148910&etapa[1]=148964&etapa[2]=149128&etapa[3]=149591&gestorProcessos_campo_flag=1&gestorProcessos[0]=<?php echo $dado['usucpf'] ?>&prazovencido=nao&arvore_=true');">
                    <td class=""><?php echo $dado['gestor'] ?></td>
                    <td class="numero" ><?php echo number_format($dado['atividade'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['item'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['executado'],0,",",".") ?></td>
                </tr>
                <?php
                    $totalAtividade += $dado['atividade'];
                    $totalItem += $dado['item'];
                    $totalExecutado += $dado['executado'];
                endforeach;
            }?>
            <tr>
                <td class="bold" >Total</td>
                <td class="bold numero" ><?php echo number_format($totalAtividade,0,",",".") ?></td>
                <td class="bold numero" ><?php echo number_format($totalItem,0,",",".") ?></td>
                <td class="bold numero" ><?php echo number_format($totalExecutado,0,",",".") ?></td>
            </tr>
            </table>
        </td>
    </tr>
		</table>
		<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
		<tr>
			<td class="fundo_td" width="34%" >
			<div>
                <img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					Risk Manager da Operação<br/>
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
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '1.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '2.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '3.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '4.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '5.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '6.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '7.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '8.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '9.%' AND statusalerta = 'Aberto'
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
					<td class="bold" rowspan="2">ETAPAS DO ENEM/2014</td>
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
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '1.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '2.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '3.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '4.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '5.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '6.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '7.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '8.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta = 'RM - Operação' AND etapasimec ilike '9.%' AND statusalerta = 'Fechado'
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
					<td class="bold" rowspan="2">ETAPAS DO ENEM/2014</td>
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
					Risk Manager da Certificação<br>de Processos / Caminho Crítico<br/>
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
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '1.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '2.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '3.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '4.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '5.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '6.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '7.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '8.%' AND statusalerta = 'Aberto'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '9.%' AND statusalerta = 'Aberto'
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
					<td class="bold" rowspan="2">ETAPAS DO ENEM/2014</td>
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
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '1.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '2.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '3.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '4.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '5.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '6.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '7.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '8.%' AND statusalerta = 'Fechado'
						GROUP BY
							etapasimec, nivelalerta
						UNION ALL
						SELECT
							etapasimec,
							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
						WHERE
							origem_alerta ilike 'RM - Certificação de Processos%' AND etapasimec ilike '9.%' AND statusalerta = 'Fechado'
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
					<td class="bold" rowspan="2">ETAPAS DO ENEM/2014</td>
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

							CASE WHEN nivelalerta = '1 - Baixo' 	THEN count(*) END as baixo,
							CASE WHEN nivelalerta = '2 - Atenção' 	THEN count(*) END as atencao,
							CASE WHEN nivelalerta = '3 - Elevado' 	THEN count(*) END as elevado,
							CASE WHEN nivelalerta = '4 - Alto' 		THEN count(*) END as alto,
							CASE WHEN nivelalerta = '5 - Severo' 	THEN count(*) END as severo
						FROM
							enem.riskmanager_2014
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
</table>
	<?php
	//========================================================================================================================================
	//OPERAÇÃO LOGÍSTICA
	//========================================================================================================================================
	}elseif($tipo==3){
	?>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
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
								WHEN 1 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 112)
								WHEN 30 THEN (SELECT COALESCE(COUNT(DISTINCT co_local),0) FROM enem.tb_enem_reg_arq_esp_fisico_2014)
								--WHEN 30 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 112)
								WHEN 3 THEN (SELECT COALESCE(COUNT(DISTINCT co_local),0) FROM enem.tb_enem_reg_arq_esp_fisico_2014)
								WHEN 2 THEN (SELECT COALESCE(SUM(qt_total),0) FROM enem.ts_enem_inscricao_2014)
								--WHEN 7 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 11)
								WHEN 7 THEN (SELECT COALESCE(SUM(total_kit),0) FROM enem.tb_enem_qtd_kits_material_adm_2014 WHERE id_situacao_kit IN (12, 18))
								WHEN 19 THEN 0
								WHEN 15 THEN (SELECT COALESCE(SUM(qt_total),0) FROM enem.ts_enem_inscricao_2014)
								WHEN 8 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2)
								WHEN 29 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 51)
								WHEN 12 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 37)



								--WHEN 19 THEN 0
								--WHEN 15 THEN 0
								--WHEN 8 THEN 0
								--WHEN 29 THEN 0
								--WHEN 12 THEN 0
							END as meta,
							CASE etpid
								WHEN 1 THEN (SELECT COALESCE(COUNT(DISTINCT co_local),0) FROM enem.tb_enem_reg_arq_esp_fisico_2014)
								WHEN 30 THEN (SELECT COUNT(DISTINCT co_local) FROM enem.tb_enem_reg_arq_esp_fisico_2014 WHERE tp_avaliacao = 2)
								WHEN 3 THEN (SELECT COALESCE(COUNT(DISTINCT co_local),0) FROM enem.tb_enem_reg_arq_esp_fisico_2014 WHERE tp_avaliacao_risco IN (2,3))
								WHEN 2 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 1)
								--WHEN 7 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 115)
								WHEN 7 THEN (SELECT COALESCE(SUM(total_kit),0) FROM enem.tb_enem_qtd_kits_material_adm_2014 WHERE id_situacao_kit = 18)
								WHEN 19 THEN 0
								WHEN 15 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 14)
								WHEN 8 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 114)
								WHEN 29 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 116)
								WHEN 12 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 41)


								--WHEN 19 THEN 0
								--WHEN 15 THEN 0
								--WHEN 8 THEN 0
								--WHEN 29 THEN 0
								--WHEN 12 THEN 0
							END as execucao
						FROM enem.etapasoperacao
						WHERE etpid IN (1,3,2,7,19,15,8,29,12,30)
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
						<td ><span style="cursor:pointer;" onclick="abreAtividade(<?=$att['id_atividade'] ?>);"><?php echo $att['no_atividade'] ?></td>
						<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($att['execucao'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
					</tr>
				<?php endforeach; ?>
			</table>
		</td>
		<td class="fundo_td_azul_escuro" width="50%" rowspan="5">
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
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(localagendado,0) AS localagendado,
                                COALESCE(capacidade,0) AS capacidade,
                                inscritos,
                                ((capacidade::NUMERIC/inscritos::NUMERIC)*100) AS percentual
                            FROM territorios.estado est
                            LEFT JOIN (SELECT DISTINCT sgl_uf AS estuf, nu_meta AS meta FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 112) met ON met.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf AS estuf, COUNT(DISTINCT co_local) AS localagendado, SUM(nu_capacidade_max) AS capacidade FROM enem.tb_enem_reg_arq_esp_fisico_2014 GROUP BY estuf) esf ON esf.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf, SUM(qt_total) AS inscritos FROM enem.ts_enem_inscricao_2014 GROUP BY sgl_uf) ins ON ins.sgl_uf = est.estuf
                            ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
                    <div id="popdiv2">
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" rowspan="2" >UF</td>
							<td class="center bold" colspan="5" >Agendamento</td>
						</tr>
						<tr>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Quantidade de Locais Agendados</td>
                            <td class="center bold" >Capacidade</td>
							<td class="center bold" >%</td>
							<td class="center bold" >Inscritos</td>
						</tr>
						<?php
						$totalMeta = 0;
                        $totallocalagendado = 0;
						$totalInscritos = 0;
						$totalpercentual = 0;
                        $totalCapacidade = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalMeta += $att['meta'];
        						$totallocalagendado += $att['localagendado'];
                                $totalCapacidade += $att['capacidade'];
                                $totalInscritos += $att['inscritos'];
							    ?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['localagendado'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['capacidade'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
									<td class="numero" ><?php echo number_format($att['inscritos'],0,",",".") ?></td>
								</tr>
							<?php endforeach;
						}?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totallocalagendado,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalCapacidade,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format(($totalCapacidade/$totalInscritos)*100,2,",",".") ?>%</td>
							<td class="numero bold" ><?php echo number_format($totalInscritos,0,",",".") ?></td>
						</tr>
					</table>
                    <br>
                    <br>
                    <br>
                    <br>
                    <?php
                    $sql = "SELECT recordem, estuf, recmunicipio, recies, recsituacao
                            FROM enem.recusalocaisaplicacao_2014
                            ORDER BY recordem";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="titulo_box bold center" colspan="5">RECUSA DE LOCAIS DE APLICAÇÃO</td>
                        </tr>
                        <tr>
                            <td class="center bold" >N</td>
                            <td class="center bold" >UF</td>
                            <td class="center bold" >Cidade</td>
                            <td class="center bold" >IES</td>
                            <td class="center bold" >Situação</td>
                        </tr>
                        <?php
                        if($arrItem){
                            foreach($arrItem as $att):
                                ?>
                                <tr>
                                    <td><?php echo $att['recordem'] ?></td>
                                    <td><?php echo $att['estuf'] ?></td>
                                    <td><?php echo $att['recmunicipio'] ?></td>
                                    <td><?php echo $att['recies'] ?></td>
                                    <td><?php echo ($att['recsituacao']=='Cedido' ? '<font color="#00FF00">' : '') . $att['recsituacao'] ?></td>
                                </tr>
                            <?php endforeach;
                        }?>
                    </table>
                    <br>
                    <br>
                    <br>
                    <br>
                    <?php
                    $sql = "SELECT estuf, mpemunicipiosaturado, mpemunicipioconsorcio, mpemunicipioinep
                            FROM enem.municipiosposedital_2014
                            ORDER BY estuf";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="titulo_box bold center" colspan="4">INCLUSÃO DE MUNICÍPIOS PÓS EDITAL</td>
                        </tr>
                        <tr>
                            <td class="center bold" >UF</td>
                            <td class="center bold" >Município Saturado</td>
                            <td class="center bold" >Município Novo - Consórcio</td>
                            <td class="center bold" >Município Novo - INEP</td>
                        </tr>
                        <?php
                        if($arrItem){
                            foreach($arrItem as $att):
                                ?>
                                <tr>
                                    <td><?php echo $att['estuf'] ?></td>
                                    <td><?php echo $att['mpemunicipiosaturado'] ?></td>
                                    <td><?php echo $att['mpemunicipioconsorcio'] ?></td>
                                    <td><?php echo $att['mpemunicipioinep'] ?></td>
                                </tr>
                            <?php endforeach;
                        }?>
                    </table>
				<?php
				break;
				//ENSALAMENTO
				case 2:
					$sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(qtd_cordenacoes,0) AS coordenacao,
                                COALESCE(inscritos,0) AS inscritos,
                                COALESCE(inscritosprova,0) AS inscritosprova,
                                COALESCE(ensalado,0) AS ensalado,
                                ((COALESCE(ensalado,0)::NUMERIC/COALESCE(inscritosprova,0)::NUMERIC)*100) AS percentual
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS ensalado, nu_percentual_execucao AS percentual, nu_meta AS inscritosprova FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 1) esf ON esf.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, SUM(qt_total) AS inscritos FROM enem.ts_enem_inscricao_2014 GROUP BY sgl_uf) ins ON ins.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf_prova AS estuf, qtd_cordenacoes FROM enem.tb_enem_qtd_coord_recebida_2014) cor ON cor.estuf = est.estuf
                            ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Nº de Coordenações</td>
							<td class="center bold" >Nº Participantes Inscritos<br>por município de residência</td>
							<td class="center bold" >Nº Participantes Inscritos<br>por município de prova</td>
							<td class="center bold" >Nº Participantes Ensalados</td>
							<td class="center bold" >% Ensalado</td>
						</tr>
						<?php
						$totalcoordenacao = 0;
						$totalinscritos = 0;
						$totalinscritosprova = 0;
						$totalensalado = 0;
						if($arrItem){
							foreach($arrItem as $att):
								$totalcoordenacao += $att['coordenacao'];
								$totalinscritos += $att['inscritos'];
								$totalinscritosprova += $att['inscritosprova'];
								$totalensalado += $att['ensalado'];
								?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['coordenacao'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['inscritos'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['inscritosprova'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['ensalado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
								</tr>
							<?php endforeach;
						}?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalcoordenacao,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalinscritos,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalinscritosprova,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalensalado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format(($totalensalado/$totalinscritosprova)*100,2,",",".") ?>%</td>
						</tr>
					</table>
                    </div>
				<?php
				break;
				//HOMOLOGAÇÃO DE SEGURANÇA E LOCAL DE APLICAÇÃO
				case 3:
					$sql = "SELECT est.estuf AS estado, COALESCE(localagendado,0) AS localagendado, COALESCE(naoavaliado,0) AS naoavaliado, COALESCE(semrisco,0) AS semrisco, COALESCE(comrisco,0) AS comrisco
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sg_uf AS estuf, COUNT(DISTINCT co_local) AS localagendado FROM enem.tb_enem_reg_arq_esp_fisico_2014 WHERE tp_situacao_alocacao = 1 GROUP BY estuf) AS c1 ON c1.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf AS estuf, COUNT(DISTINCT co_local) AS naoavaliado FROM enem.tb_enem_reg_arq_esp_fisico_2014 WHERE tp_avaliacao_risco = 1 GROUP BY estuf) AS c2 ON c2.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf AS estuf, COUNT(DISTINCT co_local) AS semrisco FROM enem.tb_enem_reg_arq_esp_fisico_2014 WHERE tp_avaliacao_risco = 2 GROUP BY estuf) AS c3 ON c3.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf AS estuf, COUNT(DISTINCT co_local) AS comrisco FROM enem.tb_enem_reg_arq_esp_fisico_2014 WHERE tp_avaliacao_risco = 3 GROUP BY estuf) AS c4 ON c4.estuf = est.estuf
                            ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Nº de locais agendados</td>
							<td class="center bold" >Locais não avaliados</td>
							<td class="center bold" >Locais que não apresentam risco</td>
							<td class="center bold" >Locais que apresentam risco</td>
						</tr>
						<?php
						$totallocalagendado = 0;
						$totalnaoavaliado = 0;
						$totalsemrisco = 0;
						$totalcomrisco = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totallocalagendado += $att['localagendado'];
        						$totalnaoavaliado += $att['naoavaliado'];
        						$totalsemrisco += $att['semrisco'];
        						$totalcomrisco += $att['comrisco'];
    							?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['localagendado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['naoavaliado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['semrisco'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['comrisco'],0,",",".") ?></td>
								</tr>
							<?php endforeach;
						}?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totallocalagendado,0,",",".") ?></td>
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
                                est.estuf AS estado,
                                COALESCE(planejado,0) AS planejado,
                                COALESCE(montado,0) AS montado,
                                COALESCE(coletado,0) AS coletado,
                                COALESCE(entregue,0) AS entregue,
                                CASE WHEN COALESCE(planejado,0) > 0 THEN ((COALESCE(entregue,0)::NUMERIC/COALESCE(planejado,0)::NUMERIC)*100) ELSE 0 END AS percentual
                            FROM territorios.estado est
                            --LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS planejado FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 11) esf1 ON esf1.estuf = est.estuf
                            --LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS montado FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 115) esf2 ON esf2.estuf = est.estuf
                            --LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS coletado FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 12) esf3 ON esf3.estuf = est.estuf
                            --LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS entregue FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 13) esf4 ON esf4.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf_escola AS estuf, SUM(total_kit) AS planejado FROM enem.tb_enem_qtd_kits_material_adm_2014 WHERE id_situacao_kit IN (12, 18) GROUP BY estuf) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf_escola AS estuf, SUM(total_kit) AS montado FROM enem.tb_enem_qtd_kits_material_adm_2014 WHERE id_situacao_kit IN (13, 18) GROUP BY estuf) esf2 ON esf2.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf_escola AS estuf, SUM(total_kit) AS coletado FROM enem.tb_enem_qtd_kits_material_adm_2014 WHERE id_situacao_kit IN (15, 18) GROUP BY estuf) esf3 ON esf3.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf_escola AS estuf, SUM(total_kit) AS entregue FROM enem.tb_enem_qtd_kits_material_adm_2014 WHERE id_situacao_kit = 18 GROUP BY estuf) esf4 ON esf4.estuf = est.estuf
                            ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Kits Planejados</td>
							<td class="center bold" >Kits Montados</td>
							<td class="center bold" >Kits Coletados</td>
							<td class="center bold" >Kits Entregues</td>
							<td class="center bold" >% de Entrega</td>
						</tr>
						<?php
                        $totalplanejado = 0;
                        $totalmontado = 0;
                        $totalcoletado = 0;
                        $totalentregue = 0;
						if($arrItem){
							foreach($arrItem as $att):
								$totalplanejado += $att['planejado'];
								$totalmontado += $att['montado'];
								$totalcoletado += $att['coletado'];
								$totalentregue += $att['entregue'];
								?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['planejado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['montado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['coletado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['entregue'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
								</tr>
							<?php endforeach;
						}?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalplanejado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalmontado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcoletado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalentregue,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format(($totalentregue/($totalplanejado > 0 ? $totalplanejado : 1))*100,2,",",".") ?>%</td>
						</tr>
					</table>
				<?php
				break;
				//DISTRIBUIÇÃO DE PROVAS
				case 8:
					$sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(executado,0) AS executado,
                                COALESCE(coletado,0) AS coletado,
                                COALESCE(centralizadora,0) AS centralizadora,
                                COALESCE(distribuidora,0) AS distribuidora,
                                COALESCE(saiuentrega,0) AS saiuentrega,
                                COALESCE(entregue,0) AS entregue
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_meta AS meta FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS coletado FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 3) esf2 ON esf2.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS centralizadora FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 4) esf3 ON esf3.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS distribuidora FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 5) esf4 ON esf4.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS saiuentrega FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 6) esf5 ON esf5.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS entregue FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 7) esf6 ON esf6.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS executado FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 114) esf7 ON esf7.estuf = est.estuf
                            ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Malotamento Executado</td>
							<td class="center bold" >Malotamento Coletado</td>
							<td class="center bold" >Recebido na Centralizadora</td>
							<td class="center bold" >Recebido na Distribuidora</td>
							<td class="center bold" >Saiu para Entrega</td>
							<td class="center bold" >Entregue</td>
						</tr>
						<?php
						$totalmeta = 0;
						$totalexecutado = 0;
						$totalcoletado = 0;
						$totalRecebidoCentralizadora = 0;
						$totalRecebidoDistribuidora = 0;
						$totalSaiuEntrega = 0;
						$totalEntregue = 0;
						if($arrItem){
							foreach($arrItem as $att):
								$totalmeta += $att['meta'];
								$totalexecutado += $att['executado'];
								$totalcoletado += $att['coletado'];
								$totalRecebidoCentralizadora += $att['centralizadora'];
								$totalRecebidoDistribuidora += $att['distribuidora'];
								$totalSaiuEntrega += $att['saiuentrega'];
								$totalEntregue += $att['entregue'];
								?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['executado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['coletado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['centralizadora'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['distribuidora'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['saiuentrega'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['entregue'],0,",",".") ?></td>
								</tr>
							<?php endforeach;
						} ?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalmeta,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalexecutado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcoletado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalRecebidoCentralizadora,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalRecebidoDistribuidora,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalSaiuEntrega,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalEntregue,0,",",".") ?></td>
						</tr>
					</table>
				<?php
				break;
				//CERTIFICAÇÃO DE APLICADORES
				case 9:
                    $sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(cadastrados,0) AS cadastrados,
                                COALESCE(certificados,0) AS certificados,
                                CASE WHEN COALESCE(cadastrados,0) > 0 THEN ((COALESCE(certificados,0)::NUMERIC/COALESCE(cadastrados,0)::NUMERIC)*100) ELSE 0 END AS percentual
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_meta AS meta, nu_execucao AS cadastrados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 3700) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS certificados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 3701) esf2 ON esf2.estuf = est.estuf
                            ORDER BY estado";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" >UF</td>
                            <td class="center bold" >Meta</td>
                            <td class="center bold" >Cadastrados</td>
                            <td class="center bold" >Certificados</td>
                            <td class="center bold" >% Certificados</td>
                        </tr>
                        <?php
                        $totalMeta = 0;
                        $totalCadastrados = 0;
                        $totalCertificados = 0;
                        if($arrItem){
                            foreach($arrItem as $att):
                                $totalMeta += $att['meta'];
                                $totalCadastrados += $att['cadastrados'];
                                $totalCertificados += $att['certificados'];
                                ?>
                                <tr>
                                    <td class="" ><?php echo $att['estado']; ?></td>
                                    <td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['cadastrados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['certificados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
                                </tr>
                            <?php endforeach;
                        }
                        ?>
                        <tr>
                            <td class="bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCadastrados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCertificados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format(($totalCertificados/($totalCadastrados > 0 ? $totalCadastrados : 1))*100,2,",",".") ?>%</td>
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
								enem.TB_MAPA_ENEM_2014 ef ON ef.sgl_uf = e.estuf
							LEFT JOIN
								(select COALESCE(nu_execucao,0) as confirmados, sgl_uf from enem.TB_MAPA_ENEM_2014 WHERE id_indicador = 122) foo1 ON foo1.sgl_uf = e.estuf
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
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(cadastrada,0) AS cadastrada,
                                COALESCE(homologada,0) AS homologada,
                                CASE WHEN COALESCE(meta,0) > 0 THEN ((COALESCE(homologada,0)::NUMERIC/COALESCE(meta,0)::NUMERIC)*100) ELSE 0 END AS percentual
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_meta AS meta, nu_execucao AS cadastrada FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 37) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS homologada FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 41) esf2 ON esf2.estuf = est.estuf
                            ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Rotas Criadas</td>
							<td class="center bold" >Rotas Homologadas</td>
							<td class="center bold" >% Rotas Homologadas</td>
						</tr>
						<?php
						$totalmeta = 0;
						$totalcadastrada = 0;
						$totalhomologada = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalmeta += $att['meta'];
        						$totalcadastrada += $att['cadastrada'];
        						$totalhomologada += $att['homologada'];
    							?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['cadastrada'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['homologada'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
								</tr>
							<?php endforeach;
						}
						?>
						<tr>
							<td class="bold">Total</td>
							<td class="numero bold" ><?php echo number_format($totalmeta,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalcadastrada,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalhomologada,0,",",".") ?></td>
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
							FROM enem.tb_enem_ensalado_2014
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
							<td class="center bold" >Outras deficiências</td>
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
						FROM enem.tb_enem_ensalado_2014
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
					$sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(inscritos,0) AS inscritos,
                                COALESCE(postado,0) AS postado,
                                CASE WHEN COALESCE(inscritos,0) > 0 THEN ((COALESCE(postado,0)::NUMERIC/COALESCE(inscritos,0)::NUMERIC)*100) ELSE 0 END AS percentualpostado,
                                COALESCE(devolvido,0) AS devolvido,
                                CASE WHEN COALESCE(inscritos,0) > 0 THEN ((COALESCE(devolvido,0)::NUMERIC/COALESCE(inscritos,0)::NUMERIC)*100) ELSE 0 END AS percentualdevolvido
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, SUM(qt_total) AS inscritos FROM enem.ts_enem_inscricao_2014 GROUP BY sgl_uf) ins ON ins.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS postado FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 14) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS devolvido FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 15) esf2 ON esf2.estuf = est.estuf
                            ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Nº de Inscritos</td>
							<td class="center bold" >Postados</td>
							<td class="center bold" >Percentual Postado</td>
							<td class="center bold" >Devolução-Cedo</td>
							<td class="center bold" >Percentual Devolvido</td>
						</tr>
						<?php
						$totalinscritos = 0;
						$totalpostado = 0;
						$totaldevolvido = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalinscritos += $att['inscritos'];
        						$totalpostado += $att['postado'];
        						$totaldevolvido += $att['devolvido'];
    							?>
								<tr>
									<td class="" ><?php echo $att['estado']; ?></td>
									<td class="numero" ><?php echo number_format($att['inscritos'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['postado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['percentualpostado'],2,",",".") ?>%</td>
									<td class="numero" ><?php echo number_format($att['devolvido'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['percentualdevolvido'],2,",",".") ?>%</td>
								</tr>
							<?php endforeach;
						}
						?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalinscritos,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalpostado,0,",",".") ?></td>
							<td class="numero bold"  >-</td>
							<td class="numero bold"  ><?php echo number_format($totaldevolvido,0,",",".") ?></td>
							<td class="numero bold" > - </td>
						</tr>
					</table>
				<?php
				break;
				
				//DEVOLUÇÃO DE MALOTES
				case 16:
					$sql = "SELECT
                                sgl_uf,
                                SUM(malotes1) AS malotes1,
                                SUM(malotes2) AS malotes2,
                                SUM(malotesrecebidos1) AS malotesrecebidos1,
                                SUM(malotesrecebidos2) AS malotesrecebidos2,
                                SUM(malotesprocessados1) AS malotesprocessados1,
                                SUM(malotesprocessados2) AS malotesprocessados2
                            FROM(
                                SELECT sg_uf AS sgl_uf, SUM(total_malotes) AS malotes1, 0 AS malotes2, SUM(total_recebidos) AS malotesrecebidos1, 0 AS malotesrecebidos2, SUM(total_processados) AS malotesprocessados1, 0 AS malotesprocessados2
                                FROM enem.tb_enem_malotes_recebidos
                                WHERE nu_dia = 1
                                GROUP BY sgl_uf
                            UNION ALL
                                SELECT sg_uf AS sgl_uf, 0 AS malotes1, SUM(total_malotes) AS malotes2, 0 AS malotesrecebidos1, SUM(total_recebidos) AS malotesrecebidos2, 0 AS malotesprocessados1, SUM(total_processados)AS malotesprocessados2
                                FROM enem.tb_enem_malotes_recebidos
                                WHERE nu_dia = 2
                                GROUP BY sgl_uf
                            ) AS FOO
                            LEFT JOIN territorios.estado est ON est.estuf = foo.sgl_uf
                            GROUP BY sgl_uf
                            ORDER BY sgl_uf";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" rowspan="2">UF</td>
							<td class="center bold" colspan="2">Qtd. Malotes</td>
							<td class="center bold" colspan="2">Malotes Recebidos</td>
							<td class="center bold" colspan="2">Malotes Processados</td>
						</tr>
                        <tr>
                            <td class="center bold" >1º Dia</td>
                            <td class="center bold" >2º Dia</td>
                            <td class="center bold" >1º Dia</td>
                            <td class="center bold" >2º Dia</td>
                            <td class="center bold" >1º Dia</td>
                            <td class="center bold" >2º Dia</td>
                        </tr>
						<?php
						$totalmalotes1 = 0;
						$totalmalotes2 = 0;
						$totalmalotesrecebidos1 = 0;
						$totalmalotesrecebidos2 = 0;
						$totalmalotesprocessados1 = 0;
						$totalmalotesprocessados2 = 0;
						if($arrItem){
							foreach($arrItem as $att):
                                $totalmalotes1 += $att['malotes1'];
                                $totalmalotes2 += $att['malotes2'];
                                $totalmalotesrecebidos1 += $att['malotesrecebidos1'];
                                $totalmalotesrecebidos2 += $att['malotesrecebidos2'];
                                $totalmalotesprocessados1 += $att['malotesprocessados1'];
                                $totalmalotesprocessados2 += $att['malotesprocessados2'];
    							?>
								<tr>
									<td class="" ><?php echo $att['sgl_uf']; ?></td>
									<td class="numero" ><?php echo number_format($att['malotes1'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['malotes2'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['malotesrecebidos1'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['malotesrecebidos2'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['malotesprocessados1'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['malotesprocessados2'],0,",",".") ?></td>
								</tr>
							<?php endforeach;
						}
						?>
						<tr>
							<td class="center bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalmalotes1,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalmalotes2,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalmalotesrecebidos1,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalmalotesrecebidos2,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalmalotesprocessados1,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalmalotesprocessados2,0,",",".") ?></td>
						</tr>
					</table>
                    <br>
                    <br>
				<?php
                $sql = "SELECT
                            ente,
                            SUM(malotes) AS malotes,
                            SUM(malotesrecebidos) AS malotesrecebidos,
                            SUM(malotesprocessados) AS malotesprocessados
                        FROM (
                            SELECT responsavel_aplicacao AS ente, SUM(total_malotes) AS malotes, SUM(total_recebidos) AS malotesrecebidos, SUM(total_processados) AS malotesprocessados
                            FROM enem.tb_enem_malotes_recebidos
                            WHERE tipo_malote = 'PROVA'
                            GROUP BY ente
                            UNION ALL
                            SELECT 'CEBRASPE' AS ente, 0 AS malotes, 0 AS malotesrecebidos, 0 AS malotesprocessados
                            UNION ALL
                            SELECT '(RESERVA)' AS ente, SUM(total_malotes) AS malotes, SUM(total_recebidos) AS malotesrecebidos, SUM(total_processados) AS malotesprocessados
                            FROM enem.tb_enem_malotes_recebidos
                            WHERE tipo_malote = 'RESERVA'
                            GROUP BY ente
                        ) AS FOO
                        GROUP BY ente";
                $arrItem = $db->carregar($sql);
                ?>
                <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <td class="center bold">Ente Consorciado</td>
                        <td class="center bold">Qtd. Malotes</td>
                        <td class="center bold">Malotes Recebidos</td>
                        <td class="center bold">Malotes Processados</td>
                    </tr>
                    <?php
                    $totalmalotes = 0;
                    $totalmalotesrecebidos = 0;
                    $totalmalotesprocessados = 0;
                    if($arrItem){
                        foreach($arrItem as $att):
                            $totalmalotes += $att['malotes'];
                            $totalmalotesrecebidos += $att['malotesrecebidos'];
                            $totalmalotesprocessados += $att['malotesprocessados'];
                            ?>
                            <tr>
                                <td class="" ><?php echo $att['ente']; ?></td>
                                <td class="numero" ><?php echo number_format($att['malotes'],0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($att['malotesrecebidos'],0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($att['malotesprocessados'],0,",",".") ?></td>
                            </tr>
                        <?php endforeach;
                    }
                    ?>
                    <tr>
                        <td class="center bold" >Total</td>
                        <td class="numero bold" ><?php echo number_format($totalmalotes,0,",",".") ?></td>
                        <td class="numero bold"  ><?php echo number_format($totalmalotesrecebidos,0,",",".") ?></td>
                        <td class="numero bold"  ><?php echo number_format($totalmalotesprocessados,0,",",".") ?></td>
                    </tr>
                </table>
                <?php
				break;
				
				//DIGITALIZAÇÃO
				case 17:
					$sql = "SELECT estuf AS sgl_uf, 0 AS meta, 0 AS execucao, 0 AS percentual FROM territorios.estado ORDER BY estuf";
					$arrItem = $db->carregar($sql);
					?>
					<br><br><br><br>
					<div style="width:100%;text-align:center" >
						<input type="radio" class="filtrar_semestre" name="semestre" value="1" checked="checked" onclick="" />Cartões do 1º dia
						<input type="radio" class="filtrar_semestre" name="semestre" value="2" onclick="" />Cartões do 2º dia
						<input type="radio" class="filtrar_semestre" name="semestre" value="3" onclick="" />Redações
					</div>
					<br><br>
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
									<td class="" ><?php echo $att['sgl_uf']; ?></td>
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
				
				//CORREÇÃO DE REDAÇÕES
				case 18:
					$sql = "SELECT estuf AS sgl_uf, 0 AS meta, 0 AS correcao1, 0 AS correcao2, 0 AS banca, 0 AS ausente, 0 AS branco FROM territorios.estado ORDER BY estuf";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >1ª e 2ª correção</td>
							<td class="center bold" >3ª correção</td>
							<td class="center bold" >Banca</td>
							<td class="center bold" >Ausentes</td>
							<td class="center bold" >Em branco</td>
						</tr>
						<?php
						$totalMeta = 0;
						$totalcorrecao1 = 0;
						$totalcorrecao2 = 0;
						$totalbanca = 0;
						$totalausente = 0;
						$totalbranco = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalMeta += $att['meta'];
    							?>
								<tr>
									<td class="" ><?php echo $att['sgl_uf']; ?></td>
									<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['correcao1'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['correcao2'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['banca'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['ausente'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['branco'],0,",",".") ?></td>
								</tr>
							<?php endforeach;
						}
						?>
						<tr>
							<td class="center bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalcorrecao1,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalcorrecao2,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalbanca,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalausente,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalbranco,0,",",".") ?></td>
						</tr>
					</table>
				<?php
				break;
				
				//RECEBIMENTO DO MATERIAL ADMINISTRATIVO
				case 19:
					$sql = "SELECT e.estuf as uf, 0 as kitentregue, 0 as confirmacao, 0 AS kitaprovado, 0 AS kitnaoaprovado
							FROM territorios.estado e
							order by e.estuf";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Kits entregues</td>
							<td class="center bold" >Confirmação do Coordenador</td>
							<td class="center bold" >Kit Aprovado</td>
							<td class="center bold" >Kit não aprovado</td>
						</tr>
						<?php
						$totalkitentregue = 0;
						$totalconfirmacao = 0;
						$totalkitaprovado = 0;
						$totalkitnaoaprovado = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalExecucao += $att['execucao'];
        						$totalMeta += $att['meta'];
    							?>
								<tr>
									<td class="" ><?php echo $att['uf']; ?></td>
									<td class="numero" ><?php echo number_format($att['kitentregue'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['confirmacao'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['kitaprovado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['kitnaoaprovado'],0,",",".") ?></td>
								</tr>
							<?php endforeach;
						}
						?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalkitentregue,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalconfirmacao,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalkitaprovado,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalkitnaoaprovado,0,",",".") ?></td>
						</tr>
					</table>
				<?php
				break;

                //OCORRÊNCIAS
                case 20:
                    $sql = "SELECT tipo, SUM(total) AS total
                            FROM (
                                SELECT 1 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(1,this);\">&nbsp;Eliminado Portando Celular (*)' AS tipo, SUM(nu_eliminacao_portar_celular) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 2 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(2,this);\">&nbsp;Recusou-se a ser submetido ao detector de metais' AS tipo, SUM(nu_elim_porta_equip_eletronico) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 3 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(3,this);\">&nbsp;Comunicar-se indevidamente, de forma verbal' AS tipo, SUM(nu_elim_comuni_indevida_verbal) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 4 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(4,this);\">&nbsp;Comunicar-se indevidamente, de forma escrita' AS tipo, SUM(nu_elim_comuni_indevida_escrit) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 5 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(5,this);\">&nbsp;Utilizar livros, notas e impressos' AS tipo, SUM(nu_elim_utilizar_impressos) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 6 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(6,this);\">&nbsp;Postar imagens durante a realização das provas' AS tipo, SUM(nu_eliminacao_postar_imagens) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 7 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(7,this);\">&nbsp;Descumprimento equipamentos eletrônicos (*)' AS tipo, SUM(nu_eliminacao_meio_fraudulento) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 8 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(8,this);\">&nbsp;Outros (**)' AS tipo, COUNT(ds_eliminacao_outros) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014 WHERE ds_eliminacao_outros <> ''
                            ) AS FOO
                            GROUP BY ordem, tipo
                            ORDER BY ordem";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" colspan="2" >ELIMINAÇÕES</td>
                        </tr>
                        <tr>
                            <td class="center bold" >Tipo de Eliminação</td>
                            <td class="center bold" >Eliminados</td>
                        </tr>
                        <?php
                        $totalEliminados = 0;
                        if($arrItem){
                            foreach($arrItem as $att):
                                $totalEliminados += $att['total'];
                                ?>
                                <tr>
                                    <td class="" ><?php echo $att['tipo']; ?></td>
                                    <td class="numero" ><?php echo number_format($att['total'],0,",",".") ?></td>
                                </tr>
                            <?php endforeach;
                        }?>
                        <tr>
                            <td class="bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalEliminados,0,",",".") ?></td>
                        </tr>
                    </table>
                    <br>
                    <br>
                    <?php
                    $sql = "SELECT tipo, SUM(total) AS total
                            FROM (
                                SELECT 9 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(9,this);\">&nbsp;Falta de energia' AS tipo, SUM(in_falta_energia::INTEGER) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 10 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(10,this);\">&nbsp;Falta de água' AS tipo, SUM(in_falta_agua::INTEGER) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 11 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(11,this);\">&nbsp;Aglomeração' AS tipo, SUM(in_aglomeracao::INTEGER) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 12 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(12,this);\">&nbsp;Inundação' AS tipo, SUM(in_inundacao::INTEGER) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 13 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(13,this);\">&nbsp;Ausência de aplicadores', SUM(in_ausencia_aplicadores::INTEGER) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 14 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(14,this);\">&nbsp;Interrupção de transporte público' AS tipo, SUM(in_interrupcao_transp_publico::INTEGER) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 15 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(15,this);\">&nbsp;Número de provas insuficiente' AS tipo, SUM(in_numero_provas_inferior::INTEGER) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 16 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(16,this);\">&nbsp;Número de cartão resposta insuficiente' AS tipo, SUM(in_numero_cartao_resposta_inf::INTEGER) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 17 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(17,this);\">&nbsp;Instalacões comprometidas' AS tipo, SUM(in_instalacoes_comprometidas::INTEGER) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            UNION ALL
                                SELECT 18 AS ordem, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharOcorrenciaEstado(18,this);\">&nbsp;Tumulto' AS tipo, SUM(in_tumulto::INTEGER) AS total FROM enem.tb_enem_ocorrencias_aplicacao_2014
                            ) AS FOO
                            GROUP BY ordem, tipo
                            ORDER BY ordem";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" colspan="2" >OCORRÊNCIAS</td>
                        </tr>
                        <tr>
                            <td class="center bold" >Tipo de Ocorrência</td>
                            <td class="center bold" >Ocorrências</td>
                        </tr>
                        <?php
                        $totalOcorrencias = 0;
                        if($arrItem){
                            foreach($arrItem as $att):
                                $totalOcorrencias += $att['total'];
                                ?>
                                <tr>
                                    <td class="" ><?php echo $att['tipo']; ?></td>
                                    <td class="numero" ><?php echo number_format($att['total'],0,",",".") ?></td>
                                </tr>
                            <?php endforeach;
                        }?>
                        <tr>
                            <td class="bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalOcorrencias,0,",",".") ?></td>
                        </tr>
                    </table>
                <?php
                break;
				//CAPACITAÇÃO DA REDE DE OBSERVADORES
				case 22:
                    $sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(cadastrados,0) AS cadastrados,
                                COALESCE(certificados,0) AS certificados,
                                CASE WHEN COALESCE(cadastrados,0) > 0 THEN ((COALESCE(certificados,0)::NUMERIC/COALESCE(cadastrados,0)::NUMERIC)*100) ELSE 0 END AS percentual
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_meta AS meta, nu_execucao AS cadastrados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2708) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS certificados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2709) esf2 ON esf2.estuf = est.estuf
                            ORDER BY estado";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" >UF</td>
                            <td class="center bold" >Meta</td>
                            <td class="center bold" >Cadastrados</td>
                            <td class="center bold" >Certificados</td>
                            <td class="center bold" >% Certificados</td>
                        </tr>
                        <?php
                        $totalMeta = 0;
                        $totalCadastrados = 0;
                        $totalCertificados = 0;
                        if($arrItem){
                            foreach($arrItem as $att):
                                $totalMeta += $att['meta'];
                                $totalCadastrados += $att['cadastrados'];
                                $totalCertificados += $att['certificados'];
                                ?>
                                <tr>
                                    <td class="" ><?php echo $att['estado']; ?></td>
                                    <td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['cadastrados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['certificados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
                                </tr>
                            <?php endforeach;
                        }
                        ?>
                        <tr>
                            <td class="bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCadastrados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCertificados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format(($totalCertificados/($totalCadastrados > 0 ? $totalCadastrados : 1))*100,2,",",".") ?>%</td>
                        </tr>
                    </table>
				<?php
				break;
				
				//CERTIFICAÇÃO DE COORDENADORES ESTADUAIS
				case 23:
					$sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(cadastrados,0) AS cadastrados,
                                COALESCE(certificados,0) AS certificados,
                                CASE WHEN COALESCE(cadastrados,0) > 0 THEN ((COALESCE(certificados,0)::NUMERIC/COALESCE(cadastrados,0)::NUMERIC)*100) ELSE 0 END AS percentual
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_meta AS meta, nu_execucao AS cadastrados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 117) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS certificados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 118) esf2 ON esf2.estuf = est.estuf
                            ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Cadastrados</td>
							<td class="center bold" >Certificados</td>
							<td class="center bold" >% Certificados</td>
						</tr>
						<?php
						$totalMeta = 0;
						$totalCadastrados = 0;
						$totalCertificados = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalMeta += $att['meta'];
        						$totalCadastrados += $att['cadastrados'];
        						$totalCertificados += $att['certificados'];
    							?>
								<tr>
									<td class="" ><?php echo $att['estado']; ?></td>
									<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['cadastrados'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['certificados'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
								</tr>
							<?php endforeach;
						}
						?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalCadastrados,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalCertificados,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format(($totalCertificados/($totalCadastrados > 0 ? $totalCadastrados : 1))*100,2,",",".") ?>%</td>
						</tr>
					</table>
				<?php
				break;
				
				//CERTIFICAÇÃO DE COORDENADORES MUNICIPAIS 
				case 24:
                    $sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(cadastrados,0) AS cadastrados,
                                COALESCE(certificados,0) AS certificados,
                                CASE WHEN COALESCE(cadastrados,0) > 0 THEN ((COALESCE(certificados,0)::NUMERIC/COALESCE(cadastrados,0)::NUMERIC)*100) ELSE 0 END AS percentual
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_meta AS meta, nu_execucao AS cadastrados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2700) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS certificados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2701) esf2 ON esf2.estuf = est.estuf
                            ORDER BY estado";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" >UF</td>
                            <td class="center bold" >Meta</td>
                            <td class="center bold" >Cadastrados</td>
                            <td class="center bold" >Certificados</td>
                            <td class="center bold" >% Certificados</td>
                        </tr>
                        <?php
                        $totalMeta = 0;
                        $totalCadastrados = 0;
                        $totalCertificados = 0;
                        if($arrItem){
                            foreach($arrItem as $att):
                                $totalMeta += $att['meta'];
                                $totalCadastrados += $att['cadastrados'];
                                $totalCertificados += $att['certificados'];
                                ?>
                                <tr>
                                    <td class="" ><?php echo $att['estado']; ?></td>
                                    <td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['cadastrados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['certificados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
                                </tr>
                            <?php endforeach;
                        }
                        ?>
                        <tr>
                            <td class="bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCadastrados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCertificados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format(($totalCertificados/($totalCadastrados > 0 ? $totalCadastrados : 1))*100,2,",",".") ?>%</td>
                        </tr>
                    </table>
				<?php
				break;
				
				//CERTIFICAÇÃO DE COORDENADORES DE LOCAL
				case 25:
                    $sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(cadastrados,0) AS cadastrados,
                                COALESCE(certificados,0) AS certificados,
                                CASE WHEN COALESCE(cadastrados,0) > 0 THEN ((COALESCE(certificados,0)::NUMERIC/COALESCE(cadastrados,0)::NUMERIC)*100) ELSE 0 END AS percentual
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_meta AS meta, nu_execucao AS cadastrados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2702) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS certificados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2703) esf2 ON esf2.estuf = est.estuf
                            ORDER BY estado";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" >UF</td>
                            <td class="center bold" >Meta</td>
                            <td class="center bold" >Cadastrados</td>
                            <td class="center bold" >Certificados</td>
                            <td class="center bold" >% Certificados</td>
                        </tr>
                        <?php
                        $totalMeta = 0;
                        $totalCadastrados = 0;
                        $totalCertificados = 0;
                        if($arrItem){
                            foreach($arrItem as $att):
                                $totalMeta += $att['meta'];
                                $totalCadastrados += $att['cadastrados'];
                                $totalCertificados += $att['certificados'];
                                ?>
                                <tr>
                                    <td class="" ><?php echo $att['estado']; ?></td>
                                    <td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['cadastrados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['certificados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
                                </tr>
                            <?php endforeach;
                        }
                        ?>
                        <tr>
                            <td class="bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCadastrados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCertificados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format(($totalCertificados/($totalCadastrados > 0 ? $totalCadastrados : 1))*100,2,",",".") ?>%</td>
                        </tr>
                    </table>
				<?php
				break;
				
				//CERTIFICAÇÃO DE AGENTES DE SEGURANÇA
				case 26:
                    $sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(cadastrados,0) AS cadastrados,
                                COALESCE(certificados,0) AS certificados,
                                CASE WHEN COALESCE(cadastrados,0) > 0 THEN ((COALESCE(certificados,0)::NUMERIC/COALESCE(cadastrados,0)::NUMERIC)*100) ELSE 0 END AS percentual
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_meta AS meta, nu_execucao AS cadastrados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2706) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS certificados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2707) esf2 ON esf2.estuf = est.estuf
                            ORDER BY estado";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" >UF</td>
                            <td class="center bold" >Meta</td>
                            <td class="center bold" >Cadastrados</td>
                            <td class="center bold" >Certificados</td>
                            <td class="center bold" >% Certificados</td>
                        </tr>
                        <?php
                        $totalMeta = 0;
                        $totalCadastrados = 0;
                        $totalCertificados = 0;
                        if($arrItem){
                            foreach($arrItem as $att):
                                $totalMeta += $att['meta'];
                                $totalCadastrados += $att['cadastrados'];
                                $totalCertificados += $att['certificados'];
                                ?>
                                <tr>
                                    <td class="" ><?php echo $att['estado']; ?></td>
                                    <td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['cadastrados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['certificados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
                                </tr>
                            <?php endforeach;
                        }
                        ?>
                        <tr>
                            <td class="bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCadastrados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCertificados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format(($totalCertificados/($totalCadastrados > 0 ? $totalCadastrados : 1))*100,2,",",".") ?>%</td>
                        </tr>
                    </table>
				<?php
				break;
				
				//CORRETORES
				case 27:
					$sql = "SELECT e.estuf as uf, 0 as Capacitados, 0 as Habilitados, 0 AS Correcoes, 0 AS Eliminados
							FROM territorios.estado e
							order by e.estuf";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Capacitados</td>
							<td class="center bold" >Habilitados</td>
							<td class="center bold" >Correções Realizadas</td>
							<td class="center bold" >Eliminados</td>
						</tr>
						<?php
						$totalCapacitados = 0;
						$totalHabilitados = 0;
						$totalCorrecoes = 0;
						$totalEliminados = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalExecucao += $att['execucao'];
        						$totalMeta += $att['meta'];
    							?>
								<tr>
									<td class="" ><?php echo $att['uf']; ?></td>
									<td class="numero" ><?php echo number_format($att['Capacitados'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['Habilitados'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['Correcoes'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['Eliminados'],0,",",".") ?></td>
								</tr>
							<?php endforeach;
						}
						?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalCapacitados,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalHabilitados,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalCorrecoes,0,",",".") ?></td>
							<td class="numero bold"  ><?php echo number_format($totalEliminados,0,",",".") ?></td>
						</tr>
					</table>
				<?php
				break;
				
				//CERTIFICAÇÃO DOS CHEFES DE SALA
				case 28:
                    $sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(cadastrados,0) AS cadastrados,
                                COALESCE(certificados,0) AS certificados,
                                CASE WHEN COALESCE(cadastrados,0) > 0 THEN ((COALESCE(certificados,0)::NUMERIC/COALESCE(cadastrados,0)::NUMERIC)*100) ELSE 0 END AS percentual
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_meta AS meta, nu_execucao AS cadastrados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 119) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS certificados FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 120) esf2 ON esf2.estuf = est.estuf
                            ORDER BY estado";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" >UF</td>
                            <td class="center bold" >Meta</td>
                            <td class="center bold" >Cadastrados</td>
                            <td class="center bold" >Certificados</td>
                            <td class="center bold" >% Certificados</td>
                        </tr>
                        <?php
                        $totalMeta = 0;
                        $totalCadastrados = 0;
                        $totalCertificados = 0;
                        if($arrItem){
                            foreach($arrItem as $att):
                                $totalMeta += $att['meta'];
                                $totalCadastrados += $att['cadastrados'];
                                $totalCertificados += $att['certificados'];
                                ?>
                                <tr>
                                    <td class="" ><?php echo $att['estado']; ?></td>
                                    <td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['cadastrados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['certificados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
                                </tr>
                            <?php endforeach;
                        }
                        ?>
                        <tr>
                            <td class="bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCadastrados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalCertificados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format(($totalCertificados/($totalCadastrados > 0 ? $totalCadastrados : 1))*100,2,",",".") ?>%</td>
                        </tr>
                    </table>
				<?php
				break;
				
				//MALOTAMENTO/DISTRIBUIÇÃO DA RESERVA TÉCNICA DAS PROVAS
				case 29:
					$sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(planejado,0) AS planejado,
                                COALESCE(executado,0) AS executado,
                                COALESCE(coletado,0) AS coletado,
                                COALESCE(centralizadora,0) AS centralizadora,
                                COALESCE(entregue,0) AS entregue
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_meta AS meta, nu_execucao AS planejado FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 51) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS executado FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 116) esf2 ON esf2.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS coletado FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 52) esf3 ON esf3.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS centralizadora FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 54) esf4 ON esf4.estuf = est.estuf
                            LEFT JOIN (SELECT sgl_uf AS estuf, nu_execucao AS entregue FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 57) esf5 ON esf5.estuf = est.estuf
                            ORDER BY estado";
					$arrItem = $db->carregar($sql);
					?>
					<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >UF</td>
							<td class="center bold" >Meta</td>
							<td class="center bold" >Malotamento Planejado</td>
							<td class="center bold" >Malotamento Executado</td>
							<td class="center bold" >Malotamento Coletado</td>
							<td class="center bold" >Recebido na Centralizadora</td>
							<td class="center bold" >Entregue</td>
						</tr>
						<?php
						$totalMeta = 0;
						$totalMalotamentoPlanejado = 0;
						$totalMalotamentoExecutado = 0;
						$totalMalotamentoColetado = 0;
						$totalRecebidoCentralizadora = 0;
						$totalEntregue = 0;
						if($arrItem){
							foreach($arrItem as $att):
        						$totalMeta += $att['meta'];
        						$totalMalotamentoPlanejado += $att['planejado'];
        						$totalMalotamentoExecutado += $att['executado'];
        						$totalMalotamentoColetado += $att['coletado'];
        						$totalRecebidoCentralizadora += $att['centralizadora'];
        						$totalEntregue += $att['entregue'];
							    ?>
								<tr>
									<td><?php echo $att['estado'] ?></td>
									<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['planejado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['executado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['coletado'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['centralizadora'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($att['entregue'],0,",",".") ?></td>
								</tr>
							<?php endforeach;
						}?>
						<tr>
							<td class="bold" >Total</td>
							<td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalMalotamentoPlanejado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalMalotamentoExecutado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalMalotamentoColetado,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalRecebidoCentralizadora,0,",",".") ?></td>
							<td class="numero bold" ><?php echo number_format($totalEntregue,0,",",".") ?></td>
						</tr>
					</table>
				<?php
				break;
                //CERTIFICAÇÃO DE ENDEREÇOS
                case 30:
                    $sql = "SELECT
                                est.estuf AS estado,
                                COALESCE(meta,0) AS meta,
                                COALESCE(certificado,0) AS certificado,
                                COALESCE(naocertificado,0) AS naocertificado,
                                COALESCE(pendente,0) AS pendente
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sg_uf AS estuf, COUNT(DISTINCT co_local) AS meta FROM enem.tb_enem_reg_arq_esp_fisico_2014 GROUP BY estuf) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf AS estuf, COUNT(DISTINCT co_local) AS pendente FROM enem.tb_enem_reg_arq_esp_fisico_2014 WHERE tp_avaliacao = 1 GROUP BY estuf) esf2 ON esf2.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf AS estuf, COUNT(DISTINCT co_local) AS certificado FROM enem.tb_enem_reg_arq_esp_fisico_2014 WHERE tp_avaliacao = 2 GROUP BY estuf) esf3 ON esf3.estuf = est.estuf
                            LEFT JOIN (SELECT sg_uf AS estuf, COUNT(DISTINCT co_local) AS naocertificado FROM enem.tb_enem_reg_arq_esp_fisico_2014 WHERE tp_avaliacao = 3 GROUP BY estuf) esf4 ON esf4.estuf = est.estuf
                            ORDER BY estado";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" >UF</td>
                            <td class="center bold" >Meta</td>
                            <td class="center bold" >Locais Certificados</td>
                            <td class="center bold" >Locais Pendentes de Análise</td>
                            <td class="center bold" >Locais Não Certificados</td>
                        </tr>
                        <?php
                        $totalMeta = 0;
                        $totalcertificado = 0;
                        $totalnaocertificado = 0;
                        $totalpendente = 0;
                        if($arrItem){
                            foreach($arrItem as $att):
                                $totalMeta += $att['meta'];
                                $totalcertificado += $att['certificado'];
                                $totalnaocertificado += $att['naocertificado'];
                                $totalpendente += $att['pendente'];
                                ?>
                                <tr>
                                    <td><?php echo $att['estado'] ?></td>
                                    <td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['certificado'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['pendente'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['naocertificado'],0,",",".") ?></td>
                                </tr>
                            <?php endforeach;
                        }?>
                        <tr>
                            <td class="bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalMeta,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalcertificado,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalpendente,0,",",".") ?></td>
                            <td class="numero bold" ><?php echo number_format($totalnaocertificado,0,",",".") ?></td>
                        </tr>
                    </table>
                <?php
                break;
                //PROCESSAMENTO DOS CADEADOS ELETÔNICOS
                case 34:
                    $sql = "SELECT
                                sgl_uf,
                                SUM(cadeados1) AS cadeados1,
                                SUM(cadeados2) AS cadeados2,
                                SUM(cadeadosprocessados1) AS cadeadosprocessados1,
                                (CASE WHEN COALESCE(SUM(cadeados1),0) > 0 THEN (SUM(cadeadosprocessados1)/SUM(cadeados1))*100 ELSE 0 END) AS percentualprocessados1,
                                SUM(cadeadosprocessados2) AS cadeadosprocessados2,
                                (CASE WHEN COALESCE(SUM(cadeados2),0) > 0 THEN (SUM(cadeadosprocessados2)/SUM(cadeados2))*100 ELSE 0 END) AS percentualprocessados2
                            FROM (
                                SELECT
                                    sgl_uf, SUM(cadeados1) AS cadeados1, SUM(cadeados2) AS cadeados2, SUM(cadeadosprocessados1) AS cadeadosprocessados1, SUM(cadeadosprocessados2) AS cadeadosprocessados2
                                FROM (
                                    SELECT sg_uf AS sgl_uf, SUM(total_malotes) AS cadeados1, 0 AS cadeados2, 0 AS cadeadosprocessados1, 0 AS cadeadosprocessados2
                                    FROM enem.tb_enem_malotes_recebidos
                                    WHERE nu_dia = 1
                                    GROUP BY sgl_uf
                                UNION ALL
                                    SELECT sg_uf AS sgl_uf, 0 AS cadeados1, SUM(total_malotes) AS cadeados2, 0 AS cadeadosprocessados1, 0 AS cadeadosprocessados2
                                    FROM enem.tb_enem_malotes_recebidos
                                    WHERE nu_dia = 2
                                    GROUP BY sgl_uf
                                ) AS FOO1
                                GROUP BY sgl_uf
                            UNION ALL
                                SELECT
                                    sgl_uf, SUM(cadeados1) AS cadeados1, SUM(cadeados2) AS cadeados2, SUM(cadeadosprocessados1) AS cadeadosprocessados1, SUM(cadeadosprocessados2) AS cadeadosprocessados2
                                FROM (
                                    SELECT sg_uf AS sgl_uf, 0 AS cadeados1, 0 AS cadeados2, SUM(total_malotes) AS cadeadosprocessados1, 0 AS cadeadosprocessados2
                                    FROM enem.tb_enem_cadeados_eletronicos
                                    WHERE nu_dia = 1
                                    GROUP BY sgl_uf
                                UNION ALL
                                    SELECT sg_uf AS sgl_uf, 0 AS cadeados1, 0 AS cadeados2, 0 AS cadeadosprocessados1, SUM(total_malotes) AS cadeadosprocessados2
                                    FROM enem.tb_enem_cadeados_eletronicos
                                    WHERE nu_dia = 2
                                    GROUP BY sgl_uf
                                ) AS FOO2
                                GROUP BY sgl_uf
                            ) AS FOO3
                            GROUP BY sgl_uf
                            ORDER BY sgl_uf";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" rowspan="3">UF</td>
                            <td class="center bold" rowspan="2" colspan="2">Qtd. Cadeados</td>
                            <td class="center bold" colspan="4">Cadeados Processados</td>
                        </tr>
                        <tr>
                            <td class="center bold" colspan="2">1º Dia</td>
                            <td class="center bold" colspan="2">2º Dia</td>
                        </tr>
                        <tr>
                            <td class="center bold" >1º Dia</td>
                            <td class="center bold" >2º Dia</td>
                            <td class="center bold" >Qtd.</td>
                            <td class="center bold" >%</td>
                            <td class="center bold" >Qtd.</td>
                            <td class="center bold" >%</td>
                        </tr>
                        <?php
                        $totalcadeados1 = 0;
                        $totalcadeados2 = 0;
                        $totalcadeadosprocessados1 = 0;
                        $totalcadeadosprocessados2 = 0;
                        if($arrItem){
                            foreach($arrItem as $att):
                                $totalcadeados1 += $att['cadeados1'];
                                $totalcadeados2 += $att['cadeados2'];
                                $totalcadeadosprocessados1 += $att['cadeadosprocessados1'];
                                $totalcadeadosprocessados2 += $att['cadeadosprocessados2'];
                                ?>
                                <tr>
                                    <td class="" ><?php echo $att['sgl_uf']; ?></td>
                                    <td class="numero" ><?php echo number_format($att['cadeados1'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['cadeados2'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['cadeadosprocessados1'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['percentualprocessados1'],2,",",".") ?>%</td>
                                    <td class="numero" ><?php echo number_format($att['cadeadosprocessados2'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['percentualprocessados2'],2,",",".") ?>%</td>
                                </tr>
                            <?php endforeach;
                        }
                        ?>
                        <tr>
                            <td class="center bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalcadeados1,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalcadeados2,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalcadeadosprocessados1,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format(($totalcadeados1/($totalcadeadosprocessados1 > 0 ? $totalcadeados1 : 1))*100,2,",",".") ?>%</td>
                            <td class="numero bold"  ><?php echo number_format($totalcadeadosprocessados2,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format(($totalcadeados2/($totalcadeadosprocessados2 > 0 ? $totalcadeados2 : 1))*100,2,",",".") ?>%</td>
                        </tr>
                    </table>
                <?php
                break;
                //HORÁRIO DE ABERTURA DOS CADEADOS ELETRÔNICOS
                case 35:
                    $sql = "SELECT
                                est.estuf AS sgl_uf,
                                cadeados,
                                horario1,
                                horario2,
                                horario3,
                                horario4,
                                horario5,
                                horario6
                            FROM territorios.estado est
                            LEFT JOIN (SELECT sg_uf AS estuf, SUM(total_malotes) AS cadeados FROM enem.tb_enem_malotes_recebidos WHERE nu_dia = 1 GROUP BY estuf) esf1 ON esf1.estuf = est.estuf
                            LEFT JOIN (
                                SELECT
                                    estuf,
                                    SUM(horario1) AS horario1,
                                    SUM(horario2) AS horario2,
                                    SUM(horario3) AS horario3,
                                    SUM(horario4) AS horario4,
                                    SUM(horario5) AS horario5,
                                    SUM(horario6) AS horario6
                                FROM (
                                    SELECT
                                        sg_uf AS estuf,
                                        CASE WHEN hr_abertura::INTEGER <= 7 THEN total_malotes END AS horario1,
                                        CASE WHEN hr_abertura::INTEGER = 8 THEN total_malotes END AS horario2,
                                        CASE WHEN hr_abertura::INTEGER = 9 THEN total_malotes END AS horario3,
                                        CASE WHEN hr_abertura::INTEGER = 10 THEN total_malotes END AS horario4,
                                        CASE WHEN hr_abertura::INTEGER = 11 THEN total_malotes END AS horario5,
                                        CASE WHEN hr_abertura::INTEGER >= 12 THEN total_malotes END AS horario6
                                    FROM enem.tb_enem_cadeados_eletronicos
                                    WHERE nu_dia = 1
                                    AND hr_abertura <> ''
                                ) AS FOO
                                GROUP BY estuf
                                ) AS esf2 ON esf2.estuf = est.estuf
                            ORDER BY sgl_uf";
                    $arrItem = $db->carregar($sql);
                    ?>
                    <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" colspan="8">Leitura dos cadeados eletrônicos do 1º dia de provas</td>
                        </tr>
                        <tr>
                            <td class="center bold" rowspan="2">UF</td>
                            <td class="center bold" rowspan="2">Qtd.</td>
                            <td class="center bold" colspan="6">Horário de Abertura dos cadeados eletrônicos</td>
                        </tr>
                        <tr>
                            <td class="center bold" >7h ou antes</td>
                            <td class="center bold" >Entre 8 e 9h</td>
                            <td class="center bold" >Entre 9 e 10h</td>
                            <td class="center bold" >Entre 10 e 11h</td>
                            <td class="center bold" >Entre 11 e 12h</td>
                            <td class="center bold" >Após às 12h</td>
                        </tr>
                        <?php
                        $totalcadeados = 0;
                        $totalhorario1 = 0;
                        $totalhorario2 = 0;
                        $totalhorario3 = 0;
                        $totalhorario4 = 0;
                        $totalhorario5 = 0;
                        $totalhorario6 = 0;
                        if($arrItem){
                            foreach($arrItem as $att):
                                $totalcadeados += $att['cadeados'];
                                $totalhorario1 += $att['horario1'];
                                $totalhorario2 += $att['horario2'];
                                $totalhorario3 += $att['horario3'];
                                $totalhorario4 += $att['horario4'];
                                $totalhorario5 += $att['horario5'];
                                $totalhorario6 += $att['horario6'];
                                ?>
                                <tr>
                                    <td class="" ><?php echo $att['sgl_uf']; ?></td>
                                    <td class="numero" ><?php echo number_format($att['cadeados'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['horario1'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['horario2'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['horario3'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['horario4'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['horario5'],0,",",".") ?></td>
                                    <td class="numero" ><?php echo number_format($att['horario6'],0,",",".") ?></td>
                                </tr>
                            <?php endforeach;
                        }
                        ?>
                        <tr>
                            <td class="center bold" >Total</td>
                            <td class="numero bold" ><?php echo number_format($totalcadeados,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalhorario1,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalhorario2,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalhorario3,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalhorario4,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalhorario5,0,",",".") ?></td>
                            <td class="numero bold"  ><?php echo number_format($totalhorario6,0,",",".") ?></td>
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
								WHEN 4 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 29)
								WHEN 5 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 26)
								WHEN 6 THEN (SELECT COALESCE(SUM(nu_meta),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 33)
								WHEN 21 THEN (SELECT COALESCE(SUM(qt_total),0) FROM enem.ts_enem_inscricao_2014)

								--WHEN 4 THEN 47889898
								--WHEN 5 THEN 18294576
								--WHEN 6 THEN 20486
							END as meta,
							CASE etpid
								WHEN 4 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 29)
								WHEN 5 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 26)
								WHEN 6 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 33)
								WHEN 21 THEN (SELECT COALESCE(SUM(nu_execucao),0) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 700)

								--WHEN 4 THEN 0
								--WHEN 5 THEN 0
								--WHEN 6 THEN 0
								--WHEN 21 THEN 0
							END as execucao
						FROM enem.etapasoperacao
						WHERE etpid IN (4,5,6,21)
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
				<?php
				if($arrAtividade){
					foreach($arrAtividade as $att): ?>
						<tr>
							<td ><?php echo $att['no_atividade'] ?></td>
							<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($att['execucao'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
						</tr>
					<?php endforeach;
				} ?>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" width="50%">
			<div>
                <img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					Etapa de Certificação / Capacitação de Pessoas
                </div>
            </div>
			<?php
			$sql = "SELECT
						id_atividade,
						no_atividade,
						meta,
						acesso,
						concluido,
						CASE
							WHEN COALESCE(acesso, 0) = 0 THEN 0
							ELSE
							(COALESCE(concluido, 0) * 100)/COALESCE(acesso, 0)::numeric
						END AS percentual
					FROM(
						SELECT
							etpid as id_atividade,
							etpetapa as no_atividade,
							CASE etpid
								WHEN 22 THEN (SELECT SUM(nu_meta) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2708)
								WHEN 23 THEN (SELECT SUM(nu_meta) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 117 AND sgl_uf IS NOT NULL)
								WHEN 24 THEN (SELECT SUM(nu_meta) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2700 AND sgl_uf IS NOT NULL)
								WHEN 25 THEN (SELECT SUM(nu_meta) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2702 AND sgl_uf IS NOT NULL)
								WHEN 26 THEN (SELECT SUM(nu_meta) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2706 AND sgl_uf IS NOT NULL)
								WHEN 28 THEN (SELECT SUM(nu_meta) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 119 AND sgl_uf IS NOT NULL)
								WHEN 9 THEN (SELECT SUM(nu_meta) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 3700 AND sgl_uf IS NOT NULL)
								WHEN 31 THEN 33
								WHEN 32 THEN 378
								WHEN 33 THEN 13828

                                --WHEN 22 THEN 1500
								--WHEN 23 THEN (SELECT SUM(cermeta) FROM enem.certificacao WHERE id_atividade = 23)
								--WHEN 24 THEN (SELECT SUM(cermeta) FROM enem.certificacao WHERE id_atividade = 24)
								--WHEN 25 THEN 35000
								--WHEN 26 THEN 20000
								--WHEN 28 THEN 172000
								--WHEN 9 THEN 35000
							END as meta,
							CASE etpid
								WHEN 22 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2708)
								WHEN 23 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 117 AND sgl_uf IS NOT NULL)
								WHEN 24 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2700 AND sgl_uf IS NOT NULL)
								WHEN 25 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2702 AND sgl_uf IS NOT NULL)
								WHEN 26 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2706 AND sgl_uf IS NOT NULL)
								WHEN 28 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 119 AND sgl_uf IS NOT NULL)
								WHEN 9 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 3700 AND sgl_uf IS NOT NULL)
								WHEN 31 THEN 33
								WHEN 32 THEN 378
								WHEN 33 THEN 13828

                                --WHEN 22 THEN 0
								--WHEN 23 THEN (SELECT SUM(cercadastrados) FROM enem.certificacao WHERE id_atividade = 23)
								--WHEN 24 THEN (SELECT SUM(cercadastrados) FROM enem.certificacao WHERE id_atividade = 24)
								--WHEN 25 THEN 0
								--WHEN 26 THEN 0
								--WHEN 28 THEN 0
								--WHEN 9 THEN 0
							END as acesso,
							CASE etpid
								WHEN 22 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2709)
								WHEN 23 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 118 AND sgl_uf IS NOT NULL)
								WHEN 24 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2701 AND sgl_uf IS NOT NULL)
								WHEN 25 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2703 AND sgl_uf IS NOT NULL)
								WHEN 26 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 2707 AND sgl_uf IS NOT NULL)
								WHEN 28 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 120 AND sgl_uf IS NOT NULL)
								WHEN 9 THEN (SELECT SUM(nu_execucao) FROM enem.tb_mapa_enem_2014 WHERE id_indicador = 3701 AND sgl_uf IS NOT NULL)
								WHEN 31 THEN 33
								WHEN 32 THEN 378
								WHEN 33 THEN 0

								--WHEN 22 THEN 0
								--WHEN 23 THEN 0
								--WHEN 24 THEN 0
								--WHEN 25 THEN 0
								--WHEN 26 THEN 0
								--WHEN 28 THEN 0
								--WHEN 9 THEN 0
							END as concluido
						FROM enem.etapasoperacao
						WHERE etpid IN (22,23,24,25,26,28,9,31,32,33)
						AND etpstatus = 'A'
						ORDER BY etpordem
					) AS FOO";
            $arrAtividade = $db->carregar($sql);
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="bold" >Etapa</td>
					<td class="center bold" >Meta</td>
					<td class="center bold" >Acesso</td>
					<td class="center bold" >Concluído</td>
					<td class="center bold" >% Executado</td>
				</tr>
				<?php
				if($arrAtividade){
					foreach($arrAtividade as $att):
                        $label="";
                        if($att['id_atividade']==28){
                            $label = "(80%)";
                        }elseif($att['id_atividade']==9){
                            $label = "(50%)";
                        }
                        ?>
						<tr>
                            <?php if($att['id_atividade']==22 || $att['id_atividade']==31 || $att['id_atividade']==32 || $att['id_atividade']==33) {?>
							    <td><?php echo $att['no_atividade'] . ' '. $label ?></td>
                            <?php }else{ ?>
                                <td><span style="cursor:pointer;" onclick="abreAtividade(<?=$att['id_atividade'] ?>);"><?php echo $att['no_atividade'] . ' '. $label ?></span></td>
                            <?php } ?>
							<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($att['acesso'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($att['concluido'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
						</tr>
					<?php endforeach;
				} ?>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" width="50%">
			<div>
                <img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					Etapa de Operação Reversa
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
								WHEN 16 THEN 0
								WHEN 34 THEN 0
								WHEN 35 THEN 0
								WHEN 17 THEN 0
								WHEN 20 THEN 0
							END as meta,
							CASE etpid
								WHEN 16 THEN 0
								WHEN 34 THEN 0
								WHEN 35 THEN 0
								WHEN 17 THEN 0
								WHEN 20 THEN 0
							END as execucao
						FROM enem.etapasoperacao
						WHERE etpid IN (16,34,35,17,20)
						AND etpstatus = 'A'
						ORDER BY etpordem
					) AS FOO";
            $arrAtividade = $db->carregar($sql);
            ?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="bold" >Etapa</td>
                    <!--
					<td class="center bold" >Meta</td>
					<td class="center bold" >Executado</td>
					<td class="center bold" >% Executado</td>
					-->
				</tr>
				<?php
				if($arrAtividade){
					foreach($arrAtividade as $att): ?>
						<tr>
							<td ><span style="cursor:pointer;" onclick="abreAtividade(<?=$att['id_atividade'] ?>);"><?php echo str_replace(array("1-","2-","3-","4-","5-","6-","7-","8-"),"",$att['no_atividade']) ?></td>
                            <!--
							<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($att['execucao'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
							-->
						</tr>
					<?php endforeach;
				} ?>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" width="50%">
			<div>
                <img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					Etapa de Correção de Redações
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
								WHEN 18 THEN 0
								WHEN 27 THEN 0
							END as meta,
							CASE etpid
								WHEN 18 THEN 0
								WHEN 27 THEN 0
							END as execucao
						FROM enem.etapasoperacao
						WHERE etpid IN (18,27)
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
				<?php
				if($arrAtividade){
					foreach($arrAtividade as $att): ?>
						<tr>
							<td ><span style="cursor:pointer;" onclick="abreAtividade(<?=$att['id_atividade'] ?>);"><?php echo str_replace(array("1-","2-","3-","4-","5-","6-","7-","8-"),"",$att['no_atividade']) ?></td>
							<td class="numero" ><?php echo number_format($att['meta'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($att['execucao'],0,",",".") ?></td>
							<td class="numero" ><?php echo number_format($att['percentual'],2,",",".") ?>%</td>
						</tr>
					<?php endforeach;
				} ?>
			</table>
		</td>
	</tr>
</table>
	<?php
	}elseif($tipo==4){
	//========================================================================================================================================
    //GESTÃO DE TI
    //========================================================================================================================================
    ?>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_td_azul_escuro" valign="top" width="100%" colspan="2">
            <div>
                <img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
                    Lista de Arquivos Enem 2014
                </div>
            </div>
            <?php
            $sql = "SELECT cod_arquivo, resumo_do_conteudo, origem, destino, layout_aprovado, em_operacao, status
                    FROM enem.gestao_ti_2014";
            $arrGestaoTI = $db->carregar($sql);
            ?>
            <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <td class="center bold">COD</td>
                    <td class="center bold">Resumo do Conteúdo</td>
                    <td class="center bold">Origem</td>
                    <td class="center bold">Destino</td>
                    <td class="center bold">Layout Aprovado</td>
                    <td class="center bold">Em Operação</td>
                    <td class="center bold">Status</td>
                </tr>
                <?php
                if($arrGestaoTI){
                    foreach($arrGestaoTI as $gti): ?>
                        <tr>
                            <td class="" ><?php echo $gti['cod_arquivo'] ?></td>
                            <td class="" ><?php echo $gti['resumo_do_conteudo'] ?></td>
                            <td class="" ><?php echo $gti['origem'] ?></td>
                            <td class="" ><?php echo $gti['destino'] ?></td>
                            <td class="" ><?php echo $gti['layout_aprovado'] ?></td>
                            <td class="" ><?php echo $gti['em_operacao'] ?></td>
                            <td class="" ><?php echo ($gti['status']=='PENDENTE' || $gti['status']=='EXCLUÍDO' ? '<font color="red">':($gti['status']=='REVISADO' || $gti['status']=='EM OPERAÇÃO' ? '<font color="#00FF00">':'')) . $gti['status'] ?></td>
                        </tr>
                    <?php endforeach;
                } ?>
            </table>
            <br><br>
            <table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="30%" >
                <tr>
                    <td class="bold" >STATUS</td>
                </tr>
                <tr>
                    <td class="bold" >Agendado: já existe proposta de data para definição do layout</td>
                </tr>
                <tr>
                    <td class="bold" >Revisado: layout e regras foram validados</td>
                </tr>
                <tr>
                    <td class="bold" >Em operação: arquivo está sendo enviado conforme planejado</td>
                </tr>
                <tr>
                    <td class="bold" >Pendente: Atraso em definições e/ou operação</td>
                </tr>
            </table>
        </td>
     </tr>
</table>
<?php
}
?>
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
        <td class="fundo_td_azul_escuro link" onclick="window.location='/pde/estrategico.php?modulo=principal/cockpit_enem2013&acao=A';">
            <div>
                <img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
                    Monitoramento<br>ENEM 2013
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
</body>
</html>

<?php
//GERAR DADOS EM EXCEL
$gerarExcel = $_REQUEST['exporta'];
if($gerarExcel==1){
    //Ingressantes em cursos técnicos regulares – Meta Pronatec
    $sql = "SELECT sgl_uf, co_municipio, no_municipio, no_regiao, qt_total
            FROM enem.ts_enem_inscricao_2014
            ORDER BY sgl_uf, no_municipio, no_regiao";
    $dadosExcel = $db->carregar( $sql );
    ob_clean();
    $db->sql_to_excel( $dadosExcel, "Inscritos_ENEM_2014" );
}

function carregarDetalhesAbrangenciaEstado($dados) {
    global $db;
    $sql = "SELECT est.estuf, esd.esdinscritos
            FROM territorios.estado est
            LEFT JOIN enem.estudoinscritodados_2014 esd ON esd.estuf = est.estuf
            WHERE esd.esiid = {$dados['esiid']}
            ORDER BY est.estuf";
    $arrEstado = $db->carregar($sql);
    //$cabecalho = array("UF","Inscrições");
    //$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
    ?>
    <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
        <tr>
            <td class="fundo_td">
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <td class="center bold">UF</td>
                        <td class="center bold">Inscrições</td>
                    </tr>
                    <?php
                    if($arrEstado){
                        foreach($arrEstado as $gti): ?>
                            <tr>
                                <td class="center" ><?php echo $gti['estuf'] ?></td>
                                <td class="center numero" ><?php echo $gti['esdinscritos'] ?></td>
                            </tr>
                        <?php endforeach;
                    } ?>
                </table>
            </td>
        </tr>
    </table>
<?php
}

function carregarDetalhesOcorrenciaEstado($dados) {
    global $db;

    switch($dados['tipoocorrencia']){
        case 1:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(nu_eliminacao_portar_celular) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE nu_eliminacao_portar_celular > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
        break;
        case 2:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(nu_elim_porta_equip_eletronico) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE nu_elim_porta_equip_eletronico > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 3:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(nu_elim_comuni_indevida_verbal) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE nu_elim_comuni_indevida_verbal > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 4:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(nu_elim_comuni_indevida_escrit) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE nu_elim_comuni_indevida_escrit > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 5:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(nu_elim_utilizar_impressos) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE nu_elim_utilizar_impressos > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 6:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(nu_eliminacao_postar_imagens) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE nu_eliminacao_postar_imagens > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 7:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(nu_eliminacao_meio_fraudulento) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE nu_eliminacao_meio_fraudulento > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 8:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, COUNT(ds_eliminacao_outros) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    HAVING COUNT(ds_eliminacao_outros) > 0
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 9:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(in_falta_energia::INTEGER) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE in_falta_energia::INTEGER > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 10:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(in_falta_agua::INTEGER) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE in_falta_agua::INTEGER > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 11:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(in_aglomeracao::INTEGER) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE in_aglomeracao::INTEGER > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 12:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(in_inundacao::INTEGER) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE in_inundacao::INTEGER > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 13:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(in_ausencia_aplicadores::INTEGER) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE in_ausencia_aplicadores::INTEGER > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 14:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(in_interrupcao_transp_publico::INTEGER) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE in_interrupcao_transp_publico::INTEGER > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 15:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(in_numero_provas_inferior::INTEGER) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE in_numero_provas_inferior::INTEGER > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 16:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(in_numero_cartao_resposta_inf::INTEGER) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE in_numero_cartao_resposta_inf::INTEGER > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 17:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(in_instalacoes_comprometidas::INTEGER) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE in_instalacoes_comprometidas::INTEGER > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
        case 18:
            $sql = 'SELECT "SGL_UF" AS uf, "NO_MUNICIPIO" AS municipio, SUM(in_tumulto::INTEGER) AS total
                    FROM enem.tb_enem_ocorrencias_aplicacao_2014
                    WHERE in_tumulto::INTEGER > 0
                    GROUP BY "SGL_UF", "NO_MUNICIPIO"
                    ORDER BY "SGL_UF", "NO_MUNICIPIO"';
            break;
    }
    $arrEstado = $db->carregar($sql);
    ?>
    <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
        <tr>
            <td class="fundo_td">
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <td class="center bold">UF</td>
                        <td class="center bold">Município</td>
                        <td class="center bold">Total</td>
                    </tr>
                    <?php
                    if($arrEstado){
                        foreach($arrEstado as $gti): ?>
                            <tr>
                                <td ><?php echo $gti['uf'] ?></td>
                                <td ><?php echo $gti['municipio'] ?></td>
                                <td class="numero" ><?php echo $gti['total'] ?></td>
                            </tr>
                        <?php endforeach;
                    } ?>
                </table>
            </td>
        </tr>
    </table>
<?php
}
?>