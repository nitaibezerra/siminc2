<?php 
$sql = "SELECT icodescricao FROM par.subacaoitenscomposicao sit WHERE sit.icoid = {$_REQUEST['icoid']}";

$descIcone = $db->pegaUm($sql);
?>
<body>
	<script type="text/javascript" src="/includes/funcoes.js"></script>
	
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	
	<table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTitulocentro" colspan="2">HISTÓRICO PARECER</td>		
		</tr>
		<tr>
			<td class="SubTituloEsquerda" >Descrição do Item: </td>
			<td class="SubTitulocentro">
				<?=$descIcone ?>
			</td>
		</tr>
	</table>
	
	<form name=formulario id=formulario method=post enctype="multipart/form-data">
		<input type="hidden" name="salvar" value="0">
		<table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTitulocentro" colspan="2"></td>		
			</tr>
			<tr>
				<td class="SubTitulocentro" >
					<?php 
					$sql = "SELECT  DISTINCT
								CASE WHEN sip.siastatus = 'A' THEN '<b>'||tpa.atpdsc||'</b>' ELSE tpa.atpdsc END as tipoparecer,
								CASE WHEN sip.siastatus = 'A' 
									THEN '<b>'||CASE WHEN acprestritiva IS TRUE THEN 'Sim' ELSE 'Não' END||'</b>' 
									ELSE CASE WHEN acprestritiva IS TRUE THEN 'Sim' ELSE 'Não' END 
								END  as restritiva,
								CASE WHEN sip.siastatus = 'A' THEN '<b>' ELSE '' END||
								array_to_string(array(SELECT atpdsc FROM par.acompanhamentotipopendencia tpp 
											INNER JOIN par.acompanhamentotipopendencia_acompanhamentoparecer tppap ON tppap.atpid = tpp.atpid 
											WHERE tppap.acpid = acp.acpid),', <br>')||
								CASE WHEN sip.siastatus = 'A' THEN '</b>' ELSE '' END as tipopendencia,
								CASE WHEN sip.siastatus = 'A' THEN '<b>'||acpdsc||'</b>' ELSE acpdsc END as parecer,
								CASE WHEN sip.siastatus = 'A' 
									THEN '<b>'||to_char(acpdatacriacao, 'DD/MM/YYYY HH24:MI:SS')||'</b>' 
									ELSE to_char(acpdatacriacao, 'DD/MM/YYYY HH24:MI:SS') 
								END as data_criacao,
								CASE WHEN sip.siastatus = 'A' THEN '<b>Ativo</b>' ELSE 'Inativo' END as status
							FROM 
								par.subacaoitenscomposicao_acompanhamentoparecer iap 
							INNER JOIN par.acompanhamentoparecer acp ON acp.acpid = iap.acpid
							INNER JOIN par.acompanhamentotipoparecer tpa ON tpa.atpid = acp.atpid
							INNER JOIN par.subacaoitenscomposicao_acompanhamentoparecer sip ON sip.acpid = acp.acpid
							WHERE 
								iap.icoid = {$_REQUEST['icoid']}
							ORDER BY
								status, data_criacao DESC";
					
			        $cabecalho = array('Tipo do Parecer', 'Restritivo?', 'Pendência(s)',  'Parecer' , 'Data de Criação', 'Status' );
			        $db->monta_lista_simples( $sql, $cabecalho, 50, 0, 'N', '', '' );
					?>	
				</td>
			</tr>
		</table>
	</form>
</body>