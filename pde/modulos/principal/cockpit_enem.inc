<?PHP
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
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
<link rel="stylesheet" type="text/css" href="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.css" />
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
        	<div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_enem&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<?php
	$sql = "SELECT SUM(total_uf) as total FROM enem.ts_enem_inscricao_2012";
    $Inscricao = $db->pegaUm($sql);
	if($Inscricao!=0){
	?>
	<!-- Título-->
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Monitoramento ENEM 2012</div></td>
	</tr>
	<!-- Fim Título-->
	<tr>
		<td class="fundo_td_laranja" valign="top" >
			<?php
			//RETIRAR APÓS A CORREÇÃO DA CARGA
			$correcao = 0;
			$sql = "SELECT to_char(data_atualizacao, 'DD/MM/YYYY') as dataatualizacao, to_char(data_atualizacao, 'HH24:MI') as hora
				FROM enem.ts_enem_inscricao_2012
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
		<td class="fundo_td" rowspan="2" >
			<div>
                <img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" >Por Categoria<br/></a>
                </div>
            </div>
            <?php
            //$sql = "SELECT sum(pendentes) as pendentes, sum(baixa_renda) as baixa_renda, sum(escola_publica) as escola_publica, sum(escola_particular) as escola_particular, (sum(pendentes) + sum(baixa_renda) + sum(escola_publica) + sum(escola_particular)) as total
			//		FROM enem.ts_enem_inscricao_2012";
			$sql = "SELECT (SUM(sem_pagamento) + SUM(carencia_indeferida) + SUM(pagamento_efetuado)) AS pagantes, (SUM(carencia_aceita) + SUM(inscricao_gratuita)) AS isentos,
					(SUM(sem_pagamento) + SUM(carencia_indeferida) + SUM(pagamento_efetuado) + SUM(carencia_aceita) + SUM(inscricao_gratuita)) AS total
					FROM enem.ts_enem_inscricao_2012";
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
		<td class="fundo_td">
			<div onclick="window.location.href='estrategico.php?modulo=principal/cockpit_enem_producao&acao=A'"  >
                <img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
				<div style="float:left;cursor:pointer;" class="titulo_box" >Produção<br/></div>
			</div>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" >
			<?php
            $sql = "SELECT sum(pediu_certificado) FROM enem.ts_enem_inscricao_2012";
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
		<td class="fundo_td_azul_escuro" rowspan="2" >
			<div>
                <img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" >Por Região<br/></a>
                </div>
            </div>
            <?php
            $sql = "SELECT no_regiao, SUM(total_uf) as total
				FROM enem.ts_enem_inscricao_2012
				group by no_regiao
				order by no_regiao";
            $arrPR = $db->carregar($sql);
            ?>
			<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" >Região</td>
					<td class="center bold" >Quantidade</td>
				</tr>
            	<?php foreach($arrPR as $pr): ?>
				<tr>
					<td class="" ><?php echo $pr['no_regiao'] ?></td>
					<td class="numero" ><?php $total_regiao+=$pr['total']; echo number_format($pr['total'],0,",",".") ?></td>
				</tr>
				<?php endforeach; ?>
				<tr>
					<td class="bold" >Total</td>
					<td class="numero bold" ><?php echo number_format($total_regiao,0,",",".") ?></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" >
			<div>
                <img style="float:left" src="../imagens/icones/icons/indicador.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" onclick="abreIndicadores(129773,'')"  >Indicadores<br/>
                	<span class="subtitulo_box" >Impacto | Produto | Processo</span>
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
					FROM enem.ts_enem_inscricao_2012";
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
					FROM enem.ts_enem_inscricao_2012";
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
					FROM enem.ts_enem_inscricao_2012";
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
		<td class="fundo_td_azul_escuro" rowspan="5" valign="top" >
			<div>
                <img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" >Por Estado<br/></a>
                </div>
            </div>
            <?php
            $sql = "SELECT sgl_uf, SUM(total_uf) as totalinscricao, SUM(pediu_certificado) as totalcertificado
				FROM enem.ts_enem_inscricao_2012
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
            	<?php foreach($arrUF as $uf): ?>
			<tr>
				<td class="" ><?php echo $uf['sgl_uf'] ?></td>
				<td class="numero" ><?php $total_uf_inscrito+=$uf['totalinscricao']; echo number_format($uf['totalinscricao'],0,",",".") ?></td>
				<td class="numero" ><?php $total_uf_certificado+=$uf['totalcertificado']; echo number_format($uf['totalcertificado'],0,",",".") ?></td>
			</tr>
			<?php endforeach; ?>
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
					FROM enem.ts_enem_inscricao_2012";
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
					<td>Concluirá em 2012</td>
					<td class="numero" ><?php echo number_format($arrIns['concluira_em'],0,",",".") ?></td>
				</tr>
				<tr>
					<td>Concluirá após 2012</td>
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
		<td class="fundo_td_azul" >
			<div>
                <img style="float:left" src="../imagens/icones/icons/financeiro.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" >Pagamento<br/></a>
                </div>
            </div>
            <?php
            $sql = "SELECT SUM(sem_pagamento) AS sem_pagamento, SUM(carencia_indeferida) AS carencia_indeferida, SUM(pagamento_efetuado) AS pagamento_efetuado, SUM(carencia_aceita) AS carencia_aceita, SUM(inscricao_gratuita) AS inscricao_gratuita,
					(SUM(sem_pagamento) + SUM(carencia_indeferida) + SUM(pagamento_efetuado) + SUM(carencia_aceita) + SUM(inscricao_gratuita)) AS total
					FROM enem.ts_enem_inscricao_2012";
            $pag = $db->pegaLinha($sql);
            ?>
			<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" >Situação</td>
					<td class="center bold" >Quantidade</td>
				</tr>
				<tr>
					<td class="" >Aguardando Pagamento</td>
					<td class="numero" ><?php echo number_format($pag['sem_pagamento'],0,",",".") ?></td>
				</tr>
				<tr>
					<td class="" >Carência Indeferida (Pagantes)</td>
					<td class="numero" ><?php echo number_format($pag['carencia_indeferida'],0,",",".") ?></td>
				</tr>
				<tr>
					<td class="" >Pagamento Efetuado</td>
					<td class="numero" ><?php echo number_format($pag['pagamento_efetuado'],0,",",".") ?></td>
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
                <img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" >Atendimento Especial<br/></a>
                </div>
            </div>
            <?php
            $sql = "SELECT SUM(sala_mais_facil_acesso) as sala_mais_facil_acesso, SUM(prova_ampliada) as prova_ampliada, SUM(prova_braille) as prova_braille, SUM(auxilio_ledor) as auxilio_ledor,
					SUM(interprete_libras) as interprete_libras, SUM(auxilio_transcricao) as auxilio_transcricao, SUM(classe_hospitalar) as classe_hospitalar, SUM(sabatista) as sabatista,
					(SUM(sala_mais_facil_acesso) + SUM(prova_ampliada) + SUM(prova_braille) + SUM(auxilio_ledor) +
					SUM(interprete_libras) + SUM(auxilio_transcricao) + SUM(classe_hospitalar) + SUM(sabatista)) AS total
					FROM enem.ts_enem_inscricao_2012";
            $mod = $db->pegaLinha($sql);
            ?>
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
					<td class="" >Classe Hospitalar</td>
					<td class="numero" ><?php echo number_format($mod['classe_hospitalar'],0,",",".") ?></td>
				</tr>
				<tr>
					<td class="" >Guarda o sábado</td>
					<td class="numero" ><?php echo number_format($mod['sabatista'],0,",",".") ?></td>
				</tr>
				<!--<tr>
					<td class="bold" >Total</td>
					<td class="numero bold" ><?php echo number_format($mod['total'],0,",",".") ?></td>
				</tr>-->
			</table>
		</td>
		<td class="fundo_td_azul" >
		<?php
		$sql = "SELECT
					n.atiid,
					numero,
					n.atidescricao,
					SUM(QtdExecutada)+SUM(QtdValidada)+SUM(QtdCertificada)+SUM(EmAtrasoExecucao)+SUM(EmAtrasoValidacao)+SUM(EmAtrasoCertificacao)+SUM(QtdNaoExecutada)+SUM(QtdNaoValidada)+SUM(QtdNaoCertificada) AS TOTAL,
					SUM(QtdExecutada)+SUM(QtdValidada)+SUM(QtdCertificada) AS Executados,
					SUM(EmAtrasoExecucao)+SUM(EmAtrasoValidacao)+SUM(EmAtrasoCertificacao) AS EmAtraso,
					SUM(QtdNaoExecutada)+SUM(QtdNaoValidada)+SUM(QtdNaoCertificada) AS Pendentes
				FROM
				(

				(select numero,
						SUM(QtdExecucao) AS QtdExecucao, SUM(QtdExecutada) AS QtdExecutada, Sum(QtdNaoExecutada) AS QtdNaoExecutada,
						0 AS QtdValidacao, 0 AS QtdValidada, 0 AS QtdNaoValidada,
						0 AS QtdCertificacao, 0 AS QtdCertificada, 0 AS QtdNaoCertificada,
						SUM(EmAtrasoExecucao) AS EmAtrasoExecucao, 0 AS EmAtrasoValidacao, 0 AS EmAtrasoCertificacao
					from (
					SELECT SUBSTR(ati._atinumero,1,3) AS numero,
											COUNT(et1.etcid) AS QtdExecucao, COUNT(d1.docid) AS QtdExecutada, 0 AS QtdNaoExecutada,
											0 AS QtdValidacao, 0 AS QtdValidada, 0 AS QtdNaoValidada,
											0 AS QtdCertificacao, 0 AS QtdCertificada, 0 AS QtdNaoCertificada,
											0 AS EmAtrasoExecucao, 0 AS EmAtrasoValidacao, 0 AS EmAtrasoCertificacao
										FROM pde.itemchecklist icl
										INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
										INNER JOIN pde.etapascontrole et1 ON et1.iclid = icl.iclid AND et1.tpvid = 1
										LEFT JOIN workflow.documento d1 ON d1.docid = icl.docid AND d1.esdid = 284
										LEFT JOIN workflow.documento d2 ON d2.docid = icl.docid AND d2.esdid != 284
										WHERE ati.atistatus = 'A'
										AND _atiprojeto = 114098 AND ( _atinumero LIKE '5%' )
										AND icl.iclid NOT IN (SELECT icl.iclid
													FROM pde.itemchecklist icl
													INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
													INNER JOIN pde.etapascontrole ec ON ec.iclid = icl.iclid
													WHERE  ati.atistatus = 'A' AND _atiprojeto = 114098
													AND ( _atinumero LIKE '5%' ) AND ec.tpvid = 2 )
										GROUP BY SUBSTR(ati._atinumero,1,3)
					union all
					SELECT SUBSTR(a._atinumero,1,3), 0 AS QtdExecucao, 0 AS QtdExecutada, 0 AS QtdNaoExecutada,
								0 AS QtdValidacao, 0 AS QtdValidada, 0 AS QtdNaoValidada,
								0 AS QtdCertificacao, 0 AS QtdCertificada, 0 AS QtdNaoCertificada,
								count(icl.iclid) AS EmAtrasoExecucao, 0 AS EmAtrasoValidacao, 0 AS EmAtrasoCertificacao
						FROM pde.atividade a
						INNER JOIN pde.itemchecklist icl ON icl.atiid = a.atiid
						INNER JOIN pde.etapascontrole ec ON ec.iclid = icl.iclid
						INNER JOIN workflow.documento d ON d.docid = icl.docid
						WHERE atistatus = 'A' AND esdid = 281
						AND ( _atinumero LIKE '5%' )
						AND tpvid = 1 AND iclprazo < date(now())
						AND icl.iclid NOT IN (SELECT icl.iclid
									FROM pde.itemchecklist icl
									INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
									INNER JOIN pde.etapascontrole ec ON ec.iclid = icl.iclid
									INNER JOIN pde.validacao v ON v.iclid = icl.iclid
									WHERE v.tpvid = 1 AND ati.atistatus = 'A' AND _atiprojeto = 114098
									AND ( _atinumero LIKE '5%' ) AND ec.tpvid = 1 AND vldsituacao = true )
					group by SUBSTR(a._atinumero,1,3)
	union all
SELECT SUBSTR(a._atinumero,1,3), 0 AS QtdExecucao, 0 AS QtdExecutada, count(icl.iclid) AS QtdNaoExecutada,
								0 AS QtdValidacao, 0 AS QtdValidada, 0 AS QtdNaoValidada,
								0 AS QtdCertificacao, 0 AS QtdCertificada, 0 AS QtdNaoCertificada,
								0 AS EmAtrasoExecucao, 0 AS EmAtrasoValidacao, 0 AS EmAtrasoCertificacao
						FROM pde.atividade a
						INNER JOIN pde.itemchecklist icl ON icl.atiid = a.atiid
						INNER JOIN pde.etapascontrole ec ON ec.iclid = icl.iclid
						INNER JOIN workflow.documento d ON d.docid = icl.docid
						WHERE atistatus = 'A' AND esdid != 284
						AND ( _atinumero LIKE '5%' )
						AND tpvid = 1 AND iclprazo > date(now())
						AND icl.iclid NOT IN (SELECT icl.iclid
									FROM pde.itemchecklist icl
									INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
									INNER JOIN pde.etapascontrole ec ON ec.iclid = icl.iclid
									INNER JOIN pde.validacao v ON v.iclid = icl.iclid
									WHERE v.tpvid = 1 AND ati.atistatus = 'A' AND _atiprojeto = 114098
									AND ( _atinumero LIKE '5%' ) AND ec.tpvid = 1 AND vldsituacao = true )
					group by SUBSTR(a._atinumero,1,3)

					) as foo
					group by numero)

								UNION ALL

				(select numero,
						0 AS QtdExecucao, 0 AS QtdExecutada, 0 AS QtdNaoExecutada,
						SUM(QtdValidacao) AS QtdValidacao, SUM(QtdValidada) AS QtdValidada, 0 AS QtdNaoValidada,
						0 AS QtdCertificacao, 0 AS QtdCertificada, 0 AS QtdNaoCertificada,
						0 AS EmAtrasoExecucao, SUM(EmAtrasoValidacao) AS EmAtrasoValidacao, 0 AS EmAtrasoCertificacao
					from (
					SELECT SUBSTR(ati._atinumero,1,3) AS numero,
											0 AS QtdExecucao, 0 AS QtdExecutada, 0 AS QtdNaoExecutada,
											COUNT(et1.etcid) AS QtdValidacao, COUNT(d1.docid) AS QtdValidada, COUNT(d2.docid) AS QtdNaoValidada,
											0 AS QtdCertificacao, 0 AS QtdCertificada, 0 AS QtdNaoCertificada,
											0 AS EmAtrasoExecucao, 0 AS EmAtrasoValidacao, 0 AS EmAtrasoCertificacao
										FROM pde.itemchecklist icl
										INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
										INNER JOIN pde.etapascontrole et1 ON et1.iclid = icl.iclid AND et1.tpvid = 2
										LEFT JOIN workflow.documento d1 ON d1.docid = icl.docid AND d1.esdid = 284
										LEFT JOIN workflow.documento d2 ON d2.docid = icl.docid AND d2.esdid != 284
										WHERE ati.atistatus = 'A'
										AND _atiprojeto = 114098 AND ( _atinumero LIKE '5%' )
										AND icl.iclid NOT IN (SELECT icl.iclid
													FROM pde.itemchecklist icl
													INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
													INNER JOIN pde.etapascontrole ec ON ec.iclid = icl.iclid
													WHERE  ati.atistatus = 'A' AND _atiprojeto = 114098
													AND ( _atinumero LIKE '5%' ) AND ec.tpvid = 3 )
										GROUP BY SUBSTR(ati._atinumero,1,3)
					union all
					SELECT SUBSTR(a._atinumero,1,3), 0 AS QtdExecucao, 0 AS QtdExecutada, 0 AS QtdNaoExecutada,
								0 AS QtdValidacao, 0 AS QtdValidada, 0 AS QtdNaoValidada,
								0 AS QtdCertificacao, 0 AS QtdCertificada, 0 AS QtdNaoCertificada,
								0 AS EmAtrasoExecucao, count(icl.iclid) AS EmAtrasoValidacao, 0 AS EmAtrasoCertificacao
						FROM pde.atividade a
						INNER JOIN pde.itemchecklist icl ON icl.atiid = a.atiid
						INNER JOIN pde.etapascontrole ec ON ec.iclid = icl.iclid
						INNER JOIN workflow.documento d ON d.docid = icl.docid
						WHERE atistatus = 'A' AND esdid = 282
						AND ( _atinumero LIKE '5%' )
						AND tpvid = 2 AND iclprazo < date(now())
						AND icl.iclid NOT IN (SELECT icl.iclid
									FROM pde.itemchecklist icl
									INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
									INNER JOIN pde.etapascontrole ec ON ec.iclid = icl.iclid
									INNER JOIN pde.validacao v ON v.iclid = icl.iclid
									WHERE v.tpvid = 2 AND ati.atistatus = 'A' AND _atiprojeto = 114098
									AND ( _atinumero LIKE '5%' ) AND ec.tpvid = 2 AND vldsituacao = true )
					group by SUBSTR(a._atinumero,1,3)
					) as foo
					group by numero)

				UNION ALL

				(select numero,
						0 AS QtdExecucao, 0 AS QtdExecutada, 0 AS QtdNaoExecutada,
						0 AS QtdValidacao, 0 AS QtdValidada, 0 AS QtdNaoValidada,
						SUM(QtdCertificacao) AS QtdCertificacao, SUM(QtdCertificada) AS QtdCertificada, 0 AS QtdNaoCertificada,
						0 AS EmAtrasoExecucao, 0 AS EmAtrasoValidacao, SUM(EmAtrasoCertificacao) AS EmAtrasoCertificacao
					from (
					SELECT SUBSTR(ati._atinumero,1,3) AS numero,
											0 AS QtdExecucao, 0 AS QtdExecutada, 0 AS QtdNaoExecutada,
											0 AS QtdValidacao, 0 AS QtdValidada, 0 AS QtdNaoValidada,
											COUNT(et1.etcid) AS QtdCertificacao, COUNT(d1.docid) AS QtdCertificada, COUNT(d2.docid) AS QtdNaoCertificada,
											0 AS EmAtrasoExecucao, 0 AS EmAtrasoValidacao, 0 AS EmAtrasoCertificacao
										FROM pde.itemchecklist icl
										INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
										INNER JOIN pde.etapascontrole et1 ON et1.iclid = icl.iclid AND et1.tpvid = 3
										LEFT JOIN workflow.documento d1 ON d1.docid = icl.docid AND d1.esdid = 284
										LEFT JOIN workflow.documento d2 ON d2.docid = icl.docid AND d2.esdid != 284
										WHERE ati.atistatus = 'A'
										AND _atiprojeto = 114098 AND ( _atinumero LIKE '5%' )
									GROUP BY SUBSTR(ati._atinumero,1,3)
					union all
					SELECT SUBSTR(a._atinumero,1,3), 0 AS QtdExecucao, 0 AS QtdExecutada, 0 AS QtdNaoExecutada,
								0 AS QtdValidacao, 0 AS QtdValidada, 0 AS QtdNaoValidada,
								0 AS QtdCertificacao, 0 AS QtdCertificada, 0 AS QtdNaoCertificada,
								0 AS EmAtrasoExecucao, 0 AS EmAtrasoValidacao, count(icl.iclid) AS EmAtrasoCertificacao
						FROM pde.atividade a
						INNER JOIN pde.itemchecklist icl ON icl.atiid = a.atiid
						INNER JOIN pde.etapascontrole ec ON ec.iclid = icl.iclid
						INNER JOIN workflow.documento d ON d.docid = icl.docid
						WHERE atistatus = 'A' AND esdid = 283
						AND ( _atinumero LIKE '5%' )
						AND tpvid = 3 AND iclprazo < date(now())
						AND icl.iclid NOT IN (SELECT icl.iclid
									FROM pde.itemchecklist icl
									INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
									INNER JOIN pde.etapascontrole ec ON ec.iclid = icl.iclid
									INNER JOIN pde.validacao v ON v.iclid = icl.iclid
									WHERE v.tpvid = 3 AND ati.atistatus = 'A' AND _atiprojeto = 114098
									AND ( _atinumero LIKE '5%' ) AND ec.tpvid = 3 AND vldsituacao = true )
					group by SUBSTR(a._atinumero,1,3)
					) as foo
					group by numero)
			) AS t
				LEFT JOIN (SELECT atiid, _atinumero, atidescricao
					FROM pde.atividade
					WHERE _atiprojeto = 114098
					AND _atiprofundidade = 2
					AND atistatus = 'A'
					AND ( _atinumero LIKE '5%' OR _atinumero LIKE '6%' OR _atinumero LIKE '7%' OR _atinumero LIKE '8%' OR _atinumero LIKE '9%' OR _atinumero LIKE '10%' OR _atinumero LIKE '11%' )
					) n ON n._atinumero = t.numero
			--where numero != '5.3'
				GROUP BY n.atiid, numero, n.atidescricao
				ORDER BY LENGTH(numero), numero";
		$arrDados = $db->carregar($sql);
		?>
			<div>
                <img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					<a href="#" >Pontos de Atenção (Certificação de Processos)<br/></a>
                </div>
            </div>
            <table class="tabela_box_azul link" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" colspan="5">Itens de Checklist</td>
				</tr>
				<tr>
					<td class="center bold" >Descrição</td>
					<td class="center bold" >Executados</td>
					<td class="center bold" >Em Atraso</td>
					<td class="center bold" >A Executar</td>
					<td class="center bold" >Total</td>
				</tr>
				<?php
		foreach($arrDados as $dado): ?>
				<tr>
					<td class="" ><a style="cursor:pointer;" onclick="montarGrafico(<?=$dado['executados'] ?>,<?=$dado['ematraso'] ?>,<?=$dado['pendentes'] ?>);"><img src="../imagens/grafico_pizza.png" width="16" height="16" border="0"></a> <?php echo $dado['atidescricao'] ?></td>
					<td class="numero" onclick="abreRelatorioEnem('form=1&agrupadores[0]=atividades&etapa_campo_flag=1&etapa[0]=<?php echo $dado['atiid'] ?>&prazovencido=nao&possuicertificacao=sim&possuivalidacao=sim&possuiexecucao=sim&arvore_=true')"><?php $total_executado+=$dado['executados']; echo number_format($dado['executados'],0,",",".") ?></td>
					<td class="numero" onclick="abreRelatorioEnem('form=1&agrupadores[0]=atividades&etapa_campo_flag=1&etapa[0]=<?php echo $dado['atiid'] ?>&prazovencido=nao&possuicertificacao=nao&possuivalidacao=nao&possuiexecucao=nao&data_inicio=01/01/2012&data_fim=<?php echo date('d/m/Y',mktime(0, 0, 0, date("m")  , date("d")-1, date("Y"))); ?>&arvore_=true')" ><?php $total_ematraso+=$dado['ematraso']; echo number_format($dado['ematraso'],0,",",".") ?></td>
					<td class="numero" onclick="abreRelatorioEnem('form=1&agrupadores[0]=atividades&etapa_campo_flag=1&etapa[0]=<?php echo $dado['atiid'] ?>&prazovencido=nao&possuicertificacao=nao&possuivalidacao=nao&possuiexecucao=nao&data_inicio=<?php echo date('d/m/Y',mktime(0, 0, 0, date("m")  , date("d")+1, date("Y"))); ?>&data_fim=<?php echo date('d/m/Y',mktime(0, 0, 0, date("m")  , date("d"), date("Y")+1)); ?>&arvore_=true')" ><?php $total_pendentes+=$dado['pendentes']; echo number_format($dado['pendentes'],0,",",".") ?></td>
					<td class="numero" onclick="abreRelatorioEnem('form=1&agrupadores[0]=atividades&etapa_campo_flag=1&etapa[0]=<?php echo $dado['atiid'] ?>&prazovencido=nao&arvore_=true')" ><?php $total_geral+=$dado['total']; echo number_format($dado['total'],0,",",".") ?></td>
				</tr>
				<?php endforeach; ?>
				<tr>
					<td class="bold" >Total</td>
					<td class="bold numero" ><?php echo number_format($total_executado,0,",",".") ?></td>
					<td class="bold numero" ><?php echo number_format($total_ematraso,0,",",".") ?></td>
					<td class="bold numero" ><?php echo number_format($total_pendentes,0,",",".") ?></td>
					<td class="bold numero" ><?php echo number_format($total_geral,0,",",".") ?></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" >
			<div onclick="abreIndicadorPopUp(271)" >
				<?php
				$sql = "select sum(sehqtde) as qtde,sum(sehvalor) as valor from painel.seriehistorica where indid = 135 and sehstatus != 'I';";
				$arrValores = $db->pegaLinha($sql);
				$sql = "select
							count(distinct dshcodmunicipio)
						from
							painel.seriehistorica seh
						inner join
							painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
						where
							indid = 135
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
					FROM enem.ts_enem_inscricao_2012";
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
<?php
}else{
?>
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:15px" >O INEP está enviando os dados.<br>Por favor aguarde...</div></td>
	</tr>
<?php } ?>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_td_azul_escuro link" onclick="window.location='/pde/estrategico.php?modulo=principal/cockpit_enem2013&acao=A';">
            <div>
                <img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
                    Monitoramento<br>ENEM 2013
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