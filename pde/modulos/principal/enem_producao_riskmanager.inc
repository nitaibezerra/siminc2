<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=7" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<body> 
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.js"></script>
<script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.pieRenderer.min.js"></script>
<script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.donutRenderer.min.js"></script>
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
		<div style="float:right;cursor:pointer;" onclick="window.close();">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="2" ><div style="margin:28px" >Monitoramento ENEM 2012</div></td>
	</tr>
	<tr>
		<td class="fundo_td_azul_escuro" >
			<?php
			$stWhereNivelalerta = strtoupper($_REQUEST['nivel']);
			$stWhereEtapasimec = strtoupper($_REQUEST['etapasimec']);
			$stWhereSituacao = $_REQUEST['situacao'];
			$stWhereTipo = '';

			if ( $_REQUEST['tipo'] == '1' ) {
				$stWhereTipo = "AND statusalerta = '{$stWhereSituacao}' AND tipo = 'Alerta Externo' ";
			} elseif ( $_REQUEST['tipo'] == '2' ) {
					$stWhereTipo = "AND statusalerta = '{$stWhereSituacao}' AND tipo = 'Alerta Externo' ";
				} elseif ( $_REQUEST['tipo'] == '3' ) {
						$stWhereTipo = "AND( statusalerta = 'Fechado' or statusalerta = 'Aberto' ) and ( tipo = 'Alerta Externo' or tipo = 'Alerta Interno' ) ";
					}


            $sql = "SELECT upper(nivelalerta), topico, ocorrencia, risco, status, responsavel, prazo, etapasimec
					FROM enem.riskmanager_2012
					WHERE 
						upper(nivelalerta) ilike '%{$stWhereNivelalerta}%' 
						AND upper(etapasimec) ilike upper('%{$stWhereEtapasimec}%') 
						{$stWhereTipo}
					ORDER BY etapasimec";
            $arrRiskmanager = $db->carregar($sql);
			$stWhere = strtoupper(str_replace('a', 'ã', str_replace('c','ç',$_REQUEST['nivel'])));
            ?>
			<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="4" width="100%" >
				<tr>
					<td class=" center bold" colspan=8><?php echo $stWhere ?></td>
				</tr>
				<tr>
					<td class="bold" >Tópico</td>
					<td class="bold" >Ocorrência</td>
					<td class="bold" >Risco</td>
					<td class="bold" >Status</td>
					<td class="bold" >Responsável</td>
					<td class="bold" >Prazo</td>
					<td class="bold" >Etapas</td>
				</tr>
				<?php if( is_array($arrRiskmanager) ){?>
				<?php foreach($arrRiskmanager as $atr): ?>
					<tr>
						<td ><?php echo $atr['topico'] ?></td>
						<td ><?php echo $atr['ocorrencia'] ?></td>
						<td ><?php echo $atr['risco'] ?></td>
						<td ><?php echo $atr['status'] ?></td>
						<td ><?php echo $atr['responsavel'] ?></td>
						<td ><?php echo $atr['prazo'] ?></td>
						<td ><?php echo $atr['etapasimec'] ?></td>
					</tr>
				<?php endforeach; ?>
				<?php }else{?>
					<tr>
						<td  class="center" style="color:red" colspan="7">Não possui registros.</td>
					</tr>
				<?php }?>
			</table>
		</td>
	</tr>
</table>
</script>
</body>
</html>