<?php

// VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado( $_REQUEST["atiidraiz"] );

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
montar_titulo_projeto();

if ( $_REQUEST["formulario_filtro_arvore"] ) {
	
}

?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<table border="0" cellpading="0" cellspacing="0" width="100%">
				<tr>
					<td valign="top">
						<?php if ( $_REQUEST['acao'] != 'R' ) : ?>
							<script language="javascript" type="text/javascript">
								
								function formulario_filtro_arvore_submeter()
								{
									document.formulario_filtro_arvore.submit();
								}
								
							</script>
							<form name="formulario_filtro_arvore" action="" method="post">
								<input type="hidden" name="formulario_filtro_arvore" value="1"/>
								<table border="0" cellpadding="5" cellspacing="0" width="100%">
									<tr>
										<td align="right" width="150">
											Atividade
										</td>
										<td>
											<select id="atiidraiz" name="atiidraiz" class="CampoEstilo" style="width: 250px;">
												<option value="">
													<?php
													$sql = "select atidescricao from pde.atividade where atiid = " . PROJETO;
													echo $db->pegaUm( $sql );
													?>
												</option>
												<?php
												
												$sql = "
													select
														a.atiid,
														a.atidescricao,
														a._atiprofundidade as profundidade,
														a._atinumero as numero
													from pde.atividade a
													where
														a._atiprofundidade < 3
														and a._atiprojeto = " . PROJETO . " 
													order by
														a._atiordem
												";
												$lista = $db->carregar( $sql );
												$lista = $lista ? $lista : array();
												
												?>
												<?php foreach ( $lista as $item ) : ?>
													<option value="<?=  $item['atiid'] ?>" <?= $item['atiid'] == $_REQUEST["atiidraiz"] ? 'selected="selected"' : '' ?>>
														<?= str_repeat( '&nbsp;', $item['profundidade'] * 5 ) ?>
														<?= $item['numero'] ?>
														<?= $item['atidescricao'] ?>
													</option>
												<?php endforeach; ?>
											</select>
										</td>
									</tr>
									<tr>
										<td align="right">
											Profundidade
										</td>
										<td>
											<?php
											
											// força o preenchimento do formulário
											$_REQUEST["profundidade"] = $_REQUEST["profundidade"] ? $_REQUEST["profundidade"] : 3;
											
											?>
											<select name="profundidade" class="CampoEstilo">
												<option value="0" <?= $_REQUEST["profundidade"] == 0 ? 'selected="selected"' : '' ?>>Todos</option>
												<option value="1" <?= $_REQUEST["profundidade"] == 1 ? 'selected="selected"' : '' ?>>1 nível</option>
												<option value="2" <?= $_REQUEST["profundidade"] == 2 ? 'selected="selected"' : '' ?>>2 níveis</option>
												<option value="3" <?= $_REQUEST["profundidade"] == 3 ? 'selected="selected"' : '' ?>>3 níveis</option>
												<option value="4" <?= $_REQUEST["profundidade"] == 4 ? 'selected="selected"' : '' ?>>4 níveis</option>
												<option value="5" <?= $_REQUEST["profundidade"] == 5 ? 'selected="selected"' : '' ?>>5 níveis</option>
												<option value="6" <?= $_REQUEST["profundidade"] == 6 ? 'selected="selected"' : '' ?>>6 níveis</option>
											</select>
										</td>
									</tr>
									<tr>
										<td align="right">Situação</td>
										<td>
											<?php
											
											// força o preenchimento do formulário
											if ( $_REQUEST["formulario_filtro_arvore"] ) {
												$situacao = $_REQUEST["situacao"];
											} else {
												$situacao = array(
													STATUS_NAO_INICIADO,
													STATUS_EM_ANDAMENTO,
													STATUS_SUSPENSO,
													STATUS_CANCELADO,
													STATUS_CONCLUIDO
												);
											}
											
											?>
											<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_NAO_INICIADO ?>" <?= in_array( STATUS_NAO_INICIADO, $situacao ) ? 'checked="checked"' : '' ?>/>não iniciado</label>
											<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_EM_ANDAMENTO ?>" <?= in_array( STATUS_EM_ANDAMENTO, $situacao ) ? 'checked="checked"' : '' ?>/>em andamento</label>
											<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_SUSPENSO ?>" <?= in_array( STATUS_SUSPENSO, $situacao ) ? 'checked="checked"' : '' ?>/>suspenso</label>
											<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_CANCELADO ?>" <?= in_array( STATUS_CANCELADO, $situacao ) ? 'checked="checked"' : '' ?>/>cancelado</label>
											<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_CONCLUIDO ?>" <?= in_array( STATUS_CONCLUIDO, $situacao ) ? 'checked="checked"' : '' ?>/>concluído</label>
										</td>
									</tr>
									<tr>
										<td align="right">&nbsp;</td>
										<td>
											<input
												type="button"
												name="alterar_arvore"
												value="Atualizar Árvore"
												onclick="formulario_filtro_arvore_submeter();"
											/>
										</td>
									</tr>
								</table>
							</form>
						<?php endif; ?>
					</td>
					<td valign="top" width="250">
						<?= montar_formulario_pesquisa(); ?>
					</td>
				</tr>
			</table>
			<hr size="1" noshade="noshade" color="#dddddd" style="margin:15px 0 15px 0;"/>
			<?php
			
			$atividade = $_REQUEST["atiidraiz"] ? $_REQUEST["atiidraiz"] : PROJETO;
			$profundidade;
			$situacao;
			$usuario = $_REQUEST["usucpf"] ? $_REQUEST["usucpf"] : null;
			echo arvore( $atividade, $_REQUEST["profundidade"], $situacao, $_REQUEST["usucpf"] );
			
			?>
		</td>
	</tr>
</table>