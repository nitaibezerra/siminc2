<?php

$academico = new academico();
$habilitado_unidade = 'N';

if ( $_REQUEST['ajax'] == 1 ){
	
	$tipodeensino = '';
	$tipodeensino = $_REQUEST['tipodeensino'];
	
}

switch ( $_REQUEST["requisicao"] ){
	case '1':
		$academico->autorizacaoespecial( $_REQUEST );
		break;
	case '2':
		$dados = $academico->buscadadoautorizacao( $_REQUEST['autid'] );
		break;
	case '3':
		$academico->excluirautorizacaoespecial( $_REQUEST['autid'] );
		break;
}

$tipodeensino = !empty($tipodeensino) ? $tipodeensino : $dados['orgid'];

$sql = "";
$sql = "SELECT
			blsid
		FROM
			academico.bloqueiosistema
		WHERE
			blsstatus = 'A'";

$bloqueado_sistema = $db->pegaUm($sql);

include APPRAIZ."includes/cabecalho.inc";
print '<br>';

$titulo_modulo='Autorização Especial';
monta_titulo($titulo_modulo, '');

if ( $bloqueado_sistema ){

?>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script>

	function editarAutorizacao( autid ){
		window.location.href = "academico.php?modulo=sistema/bloqueio/autorizacaoespecial&acao=A&requisicao=2&autid=" + autid;
	}

	function excluirAutorizacao( autid ){
		if( confirm("Deseja realmente excluir a autorização?") ){
			window.location.href = "academico.php?modulo=sistema/bloqueio/autorizacaoespecial&acao=A&requisicao=3&autid=" + autid;
		}
	}

	function pegaTipodeEnsino( orgid ){
		
		if ( orgid ){
			document.getElementById('unidade').disabled = false;
			document.getElementById('unidade').options[0].text = 'Duplo clique para selecionar da lista'; 
		}else{
			document.getElementById('unidade').disabled = true;
			document.getElementById('unidade').options[0].text = 'Nenhum';
		}
		
		var url = 'academico.php?modulo=sistema/bloqueio/autorizacaoespecial&acao=A&ajax=1';
		var parametros = "&tipodeensino=" + orgid;
	
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(resp) {
					
				}
			}
		);
	}
	
	function validaAutorizacao(){
		
		var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
		var validacao = true;
		
		var orgao   = document.formulario.orgid.value;
		var inicio  = document.formulario.autdtinicio.value;
		var termino = document.formulario.autdttermino.value;
		var unidade = document.formulario.unidade.options[0].value;
		
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
		if (unidade == ''){
			mensagem += 'Unidades \n';
			validacao = false;
		}
		
		if (!validacao){
			alert(mensagem);
			return validacao;
		}else{
			selectAllOptions( document.getElementById( 'unidade' ) );
			formulario.submit();
		}
		
	}
	
</script>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="requisicao" value="1"/>
	<input type="hidden" name="autid" value="<?=$dados['autid']?>"/>
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
					$db->monta_combo("orgid", $sql, 'S', "Selecione...", 'pegaTipodeEnsino(this.value);', '', '', '', 'S', 'orgid');
				?>
			</td>
		</tr>
		<tr>
			<td class="subTituloDireita">Período</td>
			<td>
				<?php
					$autdtinicio  = formata_data($dados['autdtinicio']);
					$autdttermino = formata_data($dados['autdttermino']);
				?>
				<?= campo_data( 'autdtinicio', 'S', 'S', '', '' ); ?> &nbsp; até &nbsp;
				<?= campo_data( 'autdttermino', 'S', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subTituloDireita">Unidades</td>
			<td>
				<?php
					
					if ($dados){
						foreach($dados['unidade'] as $chave=>$valor){
							$unidade[] = $valor;
						}
						$habilitado_unidade = 'S';
					}
					
					$funid = !empty($tipodeensino) ? ' AND ef.funid = ' . ( $tipodeensino == 1 ? ACA_ID_UNIVERSIDADE : ACA_ID_ESCOLAS_TECNICAS) : '';
				
					$sql = "SELECT
								e.entid as codigo,
								upper(e.entnome) as descricao
							FROM
								entidade.entidade e
							INNER JOIN
								entidade.funcaoentidade ef ON ef.entid = e.entid
							WHERE
								e.entstatus = 'A' {$funid}
							ORDER BY
								e.entnome";
					
					combo_popup( 'unidade', $sql, 'Selecione as Unidades', '500x500', 0, array(), '', $habilitado_unidade, false, false, 10, 400, '');
												
				?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<input type="button" value="Autorizar" style="cursor: pointer" onclick="validaAutorizacao();" />
				<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
			</td>
		</tr>
	</table>
</form>
<?php

	$entidadesautorizadas = array();

	$sql = "SELECT
				autid,
				ao.orgdesc as tipodeensino, 
				to_char(autdtinicio, 'DD/MM/YYYY') as inicio,
				to_char(autdttermino, 'DD/MM/YYYY') as termino,
				su.usunome as nome
			FROM
				academico.autorizacaoespecial ae
			INNER JOIN
				academico.orgao ao ON ao.orgid = ae.orgid
			INNER JOIN
				seguranca.usuario su ON su.usucpf = ae.usucpf
			WHERE
				autstatus = 'A'";
	
	$dados = $db->carregar($sql);
	
	if ($dados) {
		for ($k = 0; $k < count($dados); $k++){
			
			$sql = "SELECT
						e.entnome
					FROM
						academico.autorizacaoentidade ae
					INNER JOIN
						entidade.entidade e ON e.entid = ae.entid
					WHERE
						autid = {$dados[$k]['autid']}";
			
			$entidadesautorizadas[$dados[$k]['autid']] = $db->carregar($sql); 
			
		}
	?>
		<table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%" style="color: rgb(51, 51, 51);">
			<thead>
				<tr>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Ação
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Tipo de Ensino
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Unidades Autorizadas
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Data de Início
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Data de Término
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Autorizado Por
					</td>
				</tr>
			</thead>
			<tbody>
				<?php
		
					for ($k = 0; $k < count($dados); $k++){
						if (fmod($k,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
						?>
							<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
								<td align="center" valign="middle">
									<img src="../imagens/alterar.gif" style="cursor:pointer;" onclick="editarAutorizacao(<?=$dados[$k]['autid']?>)"/>
									<img src="../imagens/excluir.gif" style="cursor:pointer;" onclick="excluirAutorizacao(<?=$dados[$k]['autid']?>)"/>
								</td>
								<td align="center"><?=$dados[$k]['tipodeensino']?></td>
								<td>
									<?php
									
										foreach ($entidadesautorizadas[$dados[$k]['autid']] as $chave=>$valor){
											foreach($valor as $chave1=>$valor1){
												echo $valor1 . '<br/>';
											}
										}
									
									?>
								</td>
								<td align="center"><?=$dados[$k]['inicio']?></td>
								<td align="center"><?=$dados[$k]['termino']?></td>
								<td align="center"><?=$dados[$k]['nome']?></td>
							</tr>
						<?php
					}
				
				?>
			</tbody>
		</table>
	<?php }else{ ?>
		<table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%" style="color: rgb(51, 51, 51);">
			<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>
		</table>
	<?php }

}else{?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr align="center">
			<td style="color: red;">Para autorizar o acesso de uma unidade, é necessário que o sistema esteja bloqueado.</td>
		</tr>
	</table>
<?}?>