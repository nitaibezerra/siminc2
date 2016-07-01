<?php

$academico = new academico();

switch ( $_REQUEST["requisicao"] ){
	case '1':
		$academico->bloqueiasistema( $_REQUEST );
		break;
	case '2':
		$dados = $academico->buscadadobloqueio( $_REQUEST['blsid'] );
		break;
	case '3':
		$academico->desbloqueiasistema( $_REQUEST['blsid'] );
		break; 
}

include APPRAIZ."includes/cabecalho.inc";
print '<br>';

$titulo_modulo='Bloqueio do Sistema';
monta_titulo($titulo_modulo, '');

?>
<script src="../includes/calendario.js"></script>
<script>
	function validaBloqueio(){
		
		var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
		var validacao = true;
		
		var orgao   = document.formulario.orgid.value;
		var inicio  = document.formulario.blsdtinicio.value;
		var termino = document.formulario.blsdttermino.value;
		var motivo  = document.formulario.blsmotivo.value;
	
		if (inicio != ""){
			if (!validaData(document.formulario.blsdtinicio)){
				alert("A data de início informada é inválida");
				document.formulario.blsdtinicio.focus();
				return false;
			}
		}
		
		if (termino != ""){
			if (!validaData(document.formulario.blsdttermino)){
				alert("A data de termino informada é inválida");
				document.formulario.blsdttermino.focus();
				return false;
			}
		}	
		
		if (inicio != "" && termino != ""){
			if (!validaDataMaior(document.formulario.blsdtinicio, document.formulario.blsdttermino)){
				alert("A data de término não deve ser maior do que a de início.");
				document.formulario.obrdtinicio.focus();
				return false;
			}
		}	
	
		if (orgao == ''){
			mensagem += 'Tipo de Ensino \n';
			validacao = false;
		}
		if (inicio == ''){
			mensagem += 'Data de Início \n';
			validacao = false;
		}
		if (termino == ''){
			mensagem += 'Data de Término \n';
			validacao = false;
		}
		if (motivo == ''){
			mensagem += 'Motivo \n';
			validacao = false;
		}
		
		if (motivo.length > 499){
			alert("O campo Motivo deve ter no máximo 500 caracteres!");
			return false;
		}
		
		if (!validacao){
			alert(mensagem);
			return validacao;
		}else{
			formulario.submit();
		}
		
	}
	
	function editarBloqueio( blsid ){
			window.location.href = "academico.php?modulo=sistema/bloqueio/bloqueio_sistema&acao=A&requisicao=2&blsid=" + blsid;
	}
	
	function desbloqueiaSistema( blsid ){
	
		if( confirm("Deseja realmente desbloquear o sistema?") ){
			window.location.href = "academico.php?modulo=sistema/bloqueio/bloqueio_sistema&acao=A&requisicao=3&blsid=" + blsid;
		}	
	
	}
	
</script>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="requisicao" value="1"/>
	<input type="hidden" name="blsid" value="<?php echo $dados['blsid']; ?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="subtitulodireita">Tipo de Ensino</td>
			<td>
				<?php
					$orgid = $dados['orgid'];
					$sql = "SELECT 
								orgid as codigo,
								orgdesc as descricao
							FROM
								academico.orgao
							ORDER BY
								orgdesc";
					$db->monta_combo("orgid", $sql, 'S', "Selecione...", '', '', '', '', 'S', 'orgid');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Período de Bloqueio</td>
			<td>
				<?php
					$blsdtinicio  = formata_data($dados['blsdtinicio']);
					$blsdttermino = formata_data($dados['blsdttermino']);
				?>
				<?= campo_data( 'blsdtinicio', 'S', 'S', '', '' ); ?> &nbsp; até &nbsp;
				<?= campo_data( 'blsdttermino', 'S', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Motivo</td>
			<td>
				<?php $blsmotivo  = $dados['blsmotivo']; ?>
				<?= campo_textarea( 'blsmotivo', 'S', 'S', '', '68', '6', '500', '' , 0, ''); ?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<input type="button" value="Bloquear" style="cursor: pointer" onclick="validaBloqueio();"; />
				<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
			</td>
		</tr>
	</table>
</form>
<?php
	
	$sql = "SELECT
				CASE WHEN blsstatus = 'A' THEN
					'<center>
						<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"editarBloqueio(' ||blsid || ')\"/>
						<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"desbloqueiaSistema(' ||blsid || ')\"/>
					</center>'
					ELSE '' 
				END as acao,
				orgdesc,
				to_char(blsdtinicio, 'DD/MM/YYYY') as inicio,
				to_char(blsdttermino, 'DD/MM/YYYY') as termino,
				blsmotivo, 
				su.usunome,
				to_char(blsdtinclusao, 'DD/MM/YYYY') as dtbloqueio,
				CASE WHEN blsdtdesbloqueio is not null THEN 
					to_char(blsdtdesbloqueio, 'DD/MM/YYYY') 
					ELSE
						'Não informado'
				END as dtdesbloqueio
			FROM
				academico.bloqueiosistema bs
			INNER JOIN
				academico.orgao ao ON ao.orgid = bs.orgid
			INNER JOIN
				seguranca.usuario su ON su.usucpf = bs.usucpf
			ORDER BY
				blsdtinclusao";
	
	$cabecalho = array('Ação', 'Tipo de Ensino', 'Data de Início', 'Data de Término', 'Motivo', 'Bloqueado Por', 'Bloqueado Em','Desbloqueado Em'); 
	$db->monta_lista_simples($sql, $cabecalho, 500, 10, 'N', '95%', 'N');
	
?>