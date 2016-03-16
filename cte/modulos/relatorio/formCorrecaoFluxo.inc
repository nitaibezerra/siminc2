<?php

if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include("resultCorrecaoFluxo.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Correção de Fluxo', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript"><!--
function gerarRelatorio(){
	var formulario = document.formulario;

	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}	
	
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.estado );
	selectAllOptions( formulario.municipio );
			
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
//	ajaxRelatorio();
	formulario.submit();
	
	janela.focus();
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

//function ajaxRelatorio(){
//	var formulario = document.formulario;
//	var divRel 	   = document.getElementById('resultformulario');
//
//	divRel.style.textAlign='center';
//	divRel.innerHTML = 'carregando...';
//
//	var agrupador = new Array();	
//	for(i=0; i < formulario.agrupador.options.length; i++){
//		agrupador[i] = formulario.agrupador.options[i].value; 
//	}	
//	
//	var tipoensino = new Array();
//	for(i=0; i < formulario.f_tipoensino.options.length; i++){
//		tipoensino[i] = formulario.f_tipoensino.options[i].value; 
//	}		
//	
//	var param =  '&agrupador=' + agrupador + 
//				 '&f_tipoensino=' + tipoensino +
//				 '&f_tipoensino_campo_excludente=' + formulario.f_tipoensino_campo_excludente.checked +
//				 '&f_tipoensino_campo_flag=' + formulario.f_tipoensino_campo_flag.value;
//	 
//    var req = new Ajax.Request('academico.php?modulo=inicio&acao=C', {
//						        method:     'post',
//						        parameters: param,
//						        onComplete: function (res)
//						        {
//							    	divRel.innerHTML = res.responseText;
//						        }
//							});
//	
//}
--></script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Agrupadores</td>
		<td>
			<?
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
		<?php
			
				// Filtros do relatório
				
				$stSql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						  FROM
						  	territorios.estado";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' ); 
				
				$stSql = "SELECT
							muncod AS codigo,
							estuf || ' - ' || mundescricao AS descricao
						  FROM
						  	territorios.municipio	
						  WHERE
						  	1=1
						  ORDER BY
						  	descricao";
				$stSqlCarregados = "";
				$where = array(
								array(
										"codigo" 	=> "estuf",
										"descricao" => "Estado <b style='size: 8px;'>(sigla)</b>",
										"numeric"	=> false
							   		  )
							   );
				mostrarComboPopup( 'Município', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)', $where ); 
			
			?>
					
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio();"/>
		</td>
	</tr>
</table>
</form>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Municípios que ainda não responderam o questionário</label></td>
	</tr>
	<tr>
		<td colspan="2">
		<div  id='test' style="display: ''">
			<?php 
				$sql = "SELECT
							-- muncod,
							mu.estuf, 
							mu.mundescricao,
							q.quetitulo
						FROM
							cte.fluxoescolar fx
						JOIN cte.instrumentounidade USING (inuid)
						JOIN territorios.municipio mu USING (muncod)
						JOIN questionario.questionario q USING (queid)
						WHERE
							muncod not in (
						
						
									(
										-- Verifica desistente
										SELECT
										--	qr.qrpid AS id,
										--	q.quetitulo as questionario,
										--	qq.inuid,
										--	iu.mun_estuf AS estado,
											iu.muncod AS municipio
										--	m.mundescricao,
										--	qr.qrpdata AS data_cadastro,
										--	'1' as idpergunta,
										--	'Desistente?' AS pergunta,
										--	case when fe.flestatus = 'I' then 'Desistente' else 'Não Desistente' end AS resposta,
										--	1 as campo
										FROM
											cte.ctequestionario qq
										JOIN cte.instrumentounidade iu USING (inuid)
										JOIN cte.fluxoescolar fe USING (inuid)
										JOIN questionario.questionarioresposta qr USING (qrpid)
										JOIN questionario.questionario q ON q.queid = qr.queid
										JOIN territorios.municipio m USING (muncod)
										WHERE
											1 = 1
										--order by estado, mundescricao
						
									)UNION ALL(
										-- Perguntas filhas de questionarios e de resposta textual
										SELECT
										--	qr.qrpid AS id,
										--	q.quetitulo as questionario,
										--	qq.inuid,
										--	iu.mun_estuf AS estado,
											iu.muncod AS municipio
										--	m.mundescricao,
										--	qr.qrpdata AS data_cadastro,
										--	2 as idpergunta,
										--	p.pertitulo AS pergunta,
										--	r.resdsc AS resposta,
										--	1 as campo	
										FROM
											cte.ctequestionario qq
										JOIN cte.instrumentounidade iu USING (inuid)
										JOIN questionario.questionarioresposta qr USING (qrpid)
										JOIN questionario.questionario q ON q.queid = qr.queid
										JOIN questionario.pergunta p ON q.queid = p.queid
										JOIN questionario.resposta r ON r.perid = p.perid
														AND r.qrpid = qr.qrpid
														AND r.itpid IS NULL
										JOIN territorios.municipio m USING (muncod)
										WHERE
											1 = 1
						
									)UNION ALL(
										-- Perguntas vinculadas a grupos filhos de questionarios e de resposta textual
										SELECT
										--	qr.qrpid AS id,
										--	q.quetitulo as questionario,
										--	qq.inuid,
										--	iu.mun_estuf AS estado,
											iu.muncod AS municipio
										--	m.mundescricao,
										--	qr.qrpdata AS data_cadastro,
										--	p.perid as idpergunta,
										--	p.pertitulo AS pergunta,
										--	r.resdsc AS resposta,
										--	1 as campo	
										FROM
											cte.ctequestionario qq
										JOIN cte.instrumentounidade iu USING (inuid)
										JOIN questionario.questionarioresposta qr USING (qrpid)
										JOIN questionario.questionario q ON q.queid = qr.queid
										JOIN questionario.grupopergunta gp ON gp.queid = q.queid
										JOIN questionario.pergunta p ON p.grpid = gp.grpid
										JOIN questionario.resposta r ON r.perid = p.perid
														AND r.qrpid = qr.qrpid
														AND r.itpid IS NULL
										JOIN territorios.municipio m USING (muncod)
										WHERE
											1 = 1
						
										---WHERE 
											--qq.estuf IS NULL
											--AND qq.mun_estuf IS NOT NULL
											--AND qq.muncod IS NOT NULL
											--AND estuf = 'RS'
											--AND (pertitulo like '5%' OR pertitulo like '2%')
									)UNION ALL(
										-- Perguntas vinculadas a grupos filhos de questionarios e que possuem item como resposta
										SELECT
										--	qr.qrpid AS id,
										--	q.quetitulo as questionario,
										--	qq.inuid,
										--	iu.mun_estuf AS estado,
											iu.muncod AS municipio
										--	m.mundescricao,
										--	qr.qrpdata AS data_cadastro,
										--	p.perid as idpergunta,
										--	p.pertitulo AS pergunta,
										--	ip.itptitulo AS resposta,
										--	1 as campo
											
										FROM
											cte.ctequestionario qq
										JOIN cte.instrumentounidade iu USING (inuid)
										JOIN questionario.questionarioresposta qr USING (qrpid)
										JOIN questionario.questionario q ON q.queid = qr.queid
										JOIN questionario.grupopergunta gp ON gp.queid = q.queid
										JOIN questionario.pergunta p ON p.grpid = gp.grpid
										JOIN questionario.itempergunta ip ON ip.perid = p.perid
										JOIN questionario.resposta r ON r.perid = p.perid
														AND r.qrpid = qr.qrpid
														AND r.itpid = ip.itpid
										JOIN territorios.municipio m USING (muncod)
										WHERE
											1 = 1
						
										--WHERE 
											--qq.estuf IS NULL
											--AND qq.mun_estuf IS NOT NULL
											--AND qq.muncod IS NOT NULL
											--AND estuf = 'RS'
											--AND (pertitulo like '5%' OR pertitulo like '2%')
									)UNION ALL(
										-- Perguntas de resposta textual filhas de itens de perguntas vinculadas a grupos filhos de questionarios
										SELECT
										--	qr.qrpid AS id,
										--	q.quetitulo as questionario,
										--	qq.inuid,
										--	iu.mun_estuf AS estado,
											iu.muncod AS municipio
										--	m.mundescricao,
										--	qr.qrpdata AS data_cadastro,
										--	p.perid as idpergunta,
										--	p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
										--	r.resdsc AS resposta,
										--	1 as campo
											
										FROM
											cte.ctequestionario qq
										JOIN cte.instrumentounidade iu USING (inuid)
										JOIN questionario.questionarioresposta qr USING (qrpid)
										JOIN questionario.questionario q ON q.queid = qr.queid
										JOIN questionario.grupopergunta gp ON gp.queid = q.queid
										JOIN questionario.pergunta p ON p.grpid = gp.grpid
										JOIN questionario.itempergunta ip ON ip.perid = p.perid
										JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
										JOIN questionario.resposta r ON r.perid = p1.perid
														AND r.qrpid = qr.qrpid
														AND r.itpid IS NULL
										JOIN territorios.municipio m USING (muncod)
										WHERE
											1 = 1
						
										--WHERE 
										--	qq.estuf IS NULL
										--	AND qq.mun_estuf IS NOT NULL
										--	AND qq.muncod IS NOT NULL
										--	--AND estuf = 'RS'
										--	AND (p.pertitulo like '5%' OR p.pertitulo like '2%')
									)UNION ALL(
									-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas vinculadas a grupos filhos de questionarios
										SELECT
										--	qr.qrpid AS id,
										--	q.quetitulo as questionario,
										--	qq.inuid,
										--	iu.mun_estuf AS estado,
											iu.muncod AS municipio
										--	m.mundescricao,
										--	qr.qrpdata AS data_cadastro,
										--	p.perid as idpergunta,
										--	p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
										--	r.resdsc AS resposta,
										--	1 as campo
											
										FROM
											cte.ctequestionario qq
										JOIN cte.instrumentounidade iu USING (inuid)
										JOIN questionario.questionarioresposta qr USING (qrpid)
										JOIN questionario.questionario q ON q.queid = qr.queid
										JOIN questionario.grupopergunta gp ON gp.queid = q.queid
										JOIN questionario.pergunta p ON p.grpid = gp.grpid
										JOIN questionario.itempergunta ip ON ip.perid = p.perid
										JOIN questionario.grupopergunta gp1 ON gp1.itpid = ip.itpid
										JOIN questionario.pergunta p1 ON p1.grpid = gp1.grpid
										JOIN questionario.resposta r ON r.perid = p1.perid
														AND r.qrpid = qr.qrpid
														AND r.itpid IS NULL
										JOIN territorios.municipio m USING (muncod)
										WHERE
											1 = 1
						
									--	WHERE 
									--		qq.estuf IS NULL
									--		AND qq.mun_estuf IS NOT NULL
									--		AND qq.muncod IS NOT NULL
									--		--AND estuf = 'RS'
									--		AND (p.pertitulo like '5%' OR p.pertitulo like '2%')
									)UNION ALL(
										-- Perguntas de resposta textual filhas de grupos vinculadas a grupos filhos de questionarios
										SELECT
										--	qr.qrpid AS id,
										--	q.quetitulo as questionario,
										--	qq.inuid,
										--	iu.mun_estuf AS estado,
											iu.muncod AS municipio
										--	m.mundescricao,
										--	qr.qrpdata AS data_cadastro,
										--	p.perid as idpergunta,
										--	p.pertitulo AS pergunta,
										--	r.resdsc AS resposta,
										--	1 as campo
											
										FROM
											cte.ctequestionario qq
										JOIN cte.instrumentounidade iu USING (inuid)
										JOIN questionario.questionarioresposta qr USING (qrpid)
										JOIN questionario.questionario q ON q.queid = qr.queid
										JOIN questionario.grupopergunta gp ON gp.queid = q.queid
										JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
										JOIN questionario.pergunta p ON p.grpid = gp1.grpid
										JOIN questionario.resposta r ON r.perid = p.perid
														AND r.qrpid = qr.qrpid
														AND r.itpid IS NULL
										JOIN territorios.municipio m USING (muncod)
										WHERE
											1 = 1
						
										--WHERE 
										--	qq.estuf IS NULL
										--	AND qq.mun_estuf IS NOT NULL
										--	AND qq.muncod IS NOT NULL
											--AND estuf = 'RS'
										--	AND (p.pertitulo like '5%' OR p.pertitulo like '2%')
									)UNION ALL(
										-- Perguntas de resposta por itens filhas de grupos vinculadas a grupos filhos de questionarios
										SELECT
										--	qr.qrpid AS id,
										--	q.quetitulo as questionario,
										--	qq.inuid,
										--	iu.mun_estuf AS estado,
											iu.muncod AS municipio
										--	m.mundescricao,
										--	qr.qrpdata AS data_cadastro,
										--	p.perid as idpergunta,
										--	p.pertitulo AS pergunta,
										--	ip.itptitulo AS resposta,
										--	1 as campo
											
										FROM
											cte.ctequestionario qq
										JOIN cte.instrumentounidade iu USING (inuid)
										JOIN questionario.questionarioresposta qr USING (qrpid)
										JOIN questionario.questionario q ON q.queid = qr.queid
										JOIN questionario.grupopergunta gp ON gp.queid = q.queid
										JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
										JOIN questionario.pergunta p ON p.grpid = gp1.grpid
										JOIN questionario.itempergunta ip ON ip.perid = p.perid
										JOIN questionario.resposta r ON r.perid = p.perid
														AND r.itpid = ip.itpid
														AND r.qrpid = qr.qrpid
														AND r.itpid IS NOT NULL
										JOIN territorios.municipio m USING (muncod)
										WHERE
											1 = 1
						
									--	WHERE 
									--		qq.estuf IS NULL
									--		AND qq.mun_estuf IS NOT NULL
									--		AND qq.muncod IS NOT NULL
											--AND estuf = 'RS'
									--		AND (p.pertitulo like '5%' OR p.pertitulo like '2%')
									)UNION ALL(
										-- Perguntas de resposta textual filhas de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
										SELECT
										--	qr.qrpid AS id,
										--	q.quetitulo as questionario,
										--	qq.inuid,
										--	iu.mun_estuf AS estado,
											iu.muncod AS municipio
										--	m.mundescricao,
										--	qr.qrpdata AS data_cadastro,
										--	p.perid as idpergunta,
										--	p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
										--	r.resdsc AS resposta,
										--	1 as campo
											
										FROM
											cte.ctequestionario qq
										JOIN cte.instrumentounidade iu USING (inuid)
										JOIN questionario.questionarioresposta qr USING (qrpid)
										JOIN questionario.questionario q ON q.queid = qr.queid
										JOIN questionario.grupopergunta gp ON gp.queid = q.queid
										JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
										JOIN questionario.pergunta p ON p.grpid = gp1.grpid
										JOIN questionario.itempergunta ip ON ip.perid = p.perid
										JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
										JOIN questionario.resposta r ON r.perid = p1.perid
														AND r.qrpid = qr.qrpid
														AND r.itpid IS NULL
										JOIN territorios.municipio m USING (muncod)
										WHERE
											1 = 1
						
										--WHERE 
										--	qq.estuf IS NULL
										--	AND qq.mun_estuf IS NOT NULL
										--	AND qq.muncod IS NOT NULL
											--AND estuf = 'RS'
										--	AND (p.pertitulo like '5%' OR p.pertitulo like '2%')
									)UNION ALL(
									-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
										SELECT
										--	qr.qrpid AS id,
										--	q.quetitulo as questionario,
										--	qq.inuid,
										--	iu.mun_estuf AS estado,
											iu.muncod AS municipio
										--	m.mundescricao,
										--	qr.qrpdata AS data_cadastro,
										--	p.perid as idpergunta,
										--	p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
										--	r.resdsc AS resposta,
										--	1 as campo
											
										FROM
											cte.ctequestionario qq
										JOIN cte.instrumentounidade iu USING (inuid)
										JOIN questionario.questionarioresposta qr USING (qrpid)
										JOIN questionario.questionario q ON q.queid = qr.queid
										JOIN questionario.grupopergunta gp ON gp.queid = q.queid
										JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
										JOIN questionario.pergunta p ON p.grpid = gp1.grpid
										JOIN questionario.itempergunta ip ON ip.perid = p.perid
										JOIN questionario.grupopergunta gp2 ON gp2.itpid = ip.itpid
										JOIN questionario.pergunta p1 ON p1.grpid = gp2.grpid
										JOIN questionario.resposta r ON r.perid = p1.perid
														AND r.qrpid = qr.qrpid
														AND r.itpid IS NULL
										JOIN territorios.municipio m USING (muncod)
										WHERE
											1 = 1
						
									) --order by estado, mundescricao
						)
						ORDER BY
							estuf, mundescricao";
				$cabecalho = array("Estado","Município","Questionário");
				$alinha = array("5%","25%","70%");		
				$align = array("center", "left", "left");	
				$db->monta_lista( $sql, $cabecalho, 15, 10, 'N', '95%', 'N', '', $alinha, $align );	
				?>
		</div>
		</td>
	</tr>
</table>
</body>
</html>
<?
function agrupador(){
	return array(
				array('codigo' 	  => 'estado',
					  'descricao' => 'Estado'),	
				array('codigo' 	  => 'municipio',
					  'descricao' => 'Município'),				
				array('codigo' 	  => 'questionario',
					  'descricao' => 'Questionário'),
				array('codigo' 	  => 'idpergunta',
					  'descricao' => 'Pergunta'),
				array('codigo' 	  => 'resposta',
					  'descricao' => 'Resposta')
				);
}
?>