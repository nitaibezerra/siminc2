<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/calendario.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Volta para a tela de pesquisa
	 * @name voltar
	 * @return void
	 */	
	function voltar(){
		window.location = 'sap.php?modulo=principal/patrimonio/pesquisarPatrimonio&acao=A'
	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> RGP: </td>
			<td class="campo">
				<?=campo_texto('rgpnum','N','N','',15,6,'','','left','',0,'id="rgpnum"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o do Patrim&ocirc;nio: </td>
			<td class="campo">
				<?=campo_texto('matdsc','N','N','',82,100,'','','left','',0,'id="matdsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Processo da Entrada: </td>
			<td class="campo">
				<?=campo_texto('bennumproc','N','N','',30,20,'','','left','',0,'id="bennumproc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data da Entrada: </td>
			<td class="campo">
				<?=campo_texto('bendtentrada','N','N','',15,15, '','','left','',0,'id="bendtentrada"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Processo de Baixa: </td>
			<td class="campo">
				<?=campo_texto('bebnumprocesso','N','N','',30,20,'','','left','',0,'id="bebnumprocesso"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data da Baixa: </td>
			<td class="campo">
				<?=campo_texto('bebdata','N','N','',15,15, '','','left','',0,'id="bebdata"');?>
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero de Série: </td>
			<td class="campo">
				<?=campo_texto('rgpnumserie','N','N','',30,20,'','','left','',0,'id="rgpnumserie"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Situação: </td>
			<td class="campo">
				<?=campo_texto('sbmdsc','N','N','',30,20,'','','left','',0,'id="sbmdsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Estado do Bem: </td>
			<td class="campo">
				<?=campo_texto('ecodsc','N','N','',82,100,'','','left','',0,'id="ecodsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Motivo Para Estado do Bem: </td>
			<td class="campo">
				<?=campo_texto('mecdsc','N','N','',82,100,'','','left','',0,'id="mecdsc"');?>
            </td>
		</tr>
				
		<tr>
			<td class="SubtituloDireita"> SIAPE do Respons&aacute;vel: </td>
			<td class="campo">
				<?=campo_texto( 'nu_matricula_siape','N','N','',20,20,'','','left','',0,'id="nu_matricula_siape"', '', '', '' );?>&nbsp;&nbsp;
				<?=campo_texto( 'no_servidor','N','N','',57,50,'','','left','',0,'id="no_servidor"' );?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Unidade Organizacional: </td>
			<td class="campo">
				<?=campo_texto( 'uorco_uorg_lotacao_servidor','N','N','',20,7,'','','left','',0,'id="uorco_uorg_lotacao_servidor"', '', '', "" );?>&nbsp;&nbsp;
				<?=campo_texto( 'uorno','N','N','',57,200,'','','left','',0,'id="uorno"' );?>
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo">
				<?php echo campo_texto('enduf', 'N', 'N', '', 15, 10, '', '', '', '', '', 'id="enduf"', '', '', '');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Cidade: </td>
			<td class="campo">
				<?php echo campo_texto('endcid', 'N', 'N', '', 100, 100, '', '', '', '', '', 'id="endcid"', '', '', '');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> CEP: </td>
			<td class="campo">
				<?php echo campo_texto('endcep', 'N', 'N', '', 15, 10, '', '', '', '', '', 'id="endcep"', '', '', '');?>
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Endereco: </td>
			<td class="campo">
				<?php echo campo_texto('endereco', 'N', 'N', '', 100, 100, '', '', '', '', '', 'id="endereco"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Andar: </td>
			<td class="campo">
				<?php echo campo_texto('enadescricao', 'N', 'N', '', 100, 100, '', '', '', '', '', 'id="enadescricao"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Sala: </td>
			<td class="campo">
				<?php echo campo_texto('easdescricao', 'N', 'N', '', 100, 100, '', '', '', '', '', 'id="easdescricao"');?>
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Valor Histórico: </td>
			<td class="campo">
                                <?php
                                    $valhistorico = number_format($valhistorico,2,',','.'); 
                                ?>
				<?php echo campo_texto('valhistorico','N','N','',15,15,'[###.]###,##','','left','',0,'id="valhistorico"'); ?>
            </td>
		</tr>
				<tr>
			<td class="SubtituloDireita"> Valor Depreciado: </td>
			<td class="campo">
                                <?php
                                    $valdepreciado = number_format($valdepreciado,2,',','.'); 
                                ?>
				<?php echo campo_texto('valdepreciado','N','N','',15,15,'[###.]###,##','','left','',0,'id="valdepreciado"'); ?>
            </td>
		</tr>
		
		<tr class="buttons">
			<td colspan='2'>
				<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar();" />
		    </td>
		</tr>
	</table>
</form>
<?php 
include_once APPSAP . 'modulos/principal/patrimonio/views/partialsViews/listaDadosPatrimonio.inc';
?>