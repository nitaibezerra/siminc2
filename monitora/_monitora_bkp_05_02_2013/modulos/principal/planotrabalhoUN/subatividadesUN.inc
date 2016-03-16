<?php
// carrega as funções de integração
include_once "planotrabalhoUN/unidade_atividade_funcoes.php";
include_once "planotrabalhoUN/_constantes.php";
include_once "planotrabalhoUN/_funcoes.php";
include_once "planotrabalhoUN/_componentes.php";

// verifica qual unidade selecionada
if($_REQUEST['unicod']) {
	$_SESSION['monitora_var']['unicod']=$_REQUEST['unicod'];
}


// obtém dados da atividade vinculada à ação
$atividade = retornaTarefaUnidade( $_SESSION['monitora_var']['unicod'], $_SESSION['exercicio'] );


$_SESSION["projeto"] = $atividade["_atiprojeto"];
define( "PROJETO", $atividade["_atiprojeto"] );
projeto_verifica_selecionado( $atividade["atiid"] );


if ($_REQUEST["atiid"]) {
	$atividade = atividade_pegar( $_REQUEST["atiid"] );
} else {
	$atividade = atividade_pegar( $atividade["atiid"] );
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

extract( $atividade ); # mantém o formulário preenchido

if($atividade['_atiprofundidade'] > 1) {
	// montando o menu
	echo montarAbasArray(carregardadosplanotrabalhoUN_sub(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&atiid=".$_REQUEST['atiid']);
} else {
	// montando o menu
	echo montarAbasArray(carregardadosplanotrabalhoUN_raiz(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=".$_SESSION['monitora_var']['unicod']);

}


monta_titulo("Plano de Trabalho",$atividade['atidescricao']);

?>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="2" align="center">
	<tr>
		<td>
			<?php
			echo montar_resumo_atividade( $atividade, $numeracao_relativa = true );
			?>
			
			<table cellspacing="0" border="0" width="100%" cellpading="0">
				<!--<tbody>
					<tr>
						<td valign="top">
							<div style="margin: 5px;">
								<p style="margin: 0pt 0pt 5px 0px;">
									<a href="?modulo=principal/planotrabalhoUN/subatividadesUN&amp;acao=A&amp;atiid=61869"> 26285 - Fundação Universidade Federal de São João Del Rei</a>
								</p>
								<p style="margin: 0pt 0pt 5px 20px; font-weight: bold; font-size: 120%;">
									<img border="0" align="absmiddle" src="../imagens/seta_filho.gif">&nbsp; 2 Objetivo Estratégico - Aprimorar e ampliar os canais de comunicação e os espaços de diálogo com a comunidade externa e interna
								</p>
							</div>
						</td>
					</tr>
				</tbody>-->
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
										<?php 
										
											/*if( isset($_POST['profundidade']) ){
												$profundidade += 1;
											}else{
												$profundidade = 4;
											}*/
										
											$sql = "select
													a.atidescricao
												from pde.atividade a
												where
													a.atistatus = 'A'
													and a._atiprofundidade < 4
													and a._atiprojeto = " . $_SESSION['projeto'] . " 
													and a.unicod = '" . $_REQUEST['unicod'] . "'
													--and a.atiidpai <> " . $_SESSION['projeto'] . "
												order by
													a._atiordem";
										?>
										<td>
											<select id="atiidraiz" name="atiidraiz" class="CampoEstilo" style="width: 250px;">
												<option value="">
													<?php
													//$sql = "select atidescricao from pde.atividade where atiid = " . $_SESSION['projeto'];
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
														a.atistatus = 'A'
														and a._atiprofundidade < 4
														and a._atiprojeto = " . $_SESSION['projeto'] . "
														and a.unicod = '" . $_REQUEST['unicod'] . "'
														and a.atiidpai <> " . $_SESSION['projeto'] . " 
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
												<option value="1" <?= $_REQUEST["profundidade"] == 1 ? 'selected="selected"' : '' ?>>1 nível</option>
												<option value="2" <?= $_REQUEST["profundidade"] == 2 ? 'selected="selected"' : '' ?>>2 níveis</option>
												<option value="3" <?= $_REQUEST["profundidade"] == 3 ? 'selected="selected"' : '' ?>>3 níveis</option>
												<option value="4" <?= $_REQUEST["profundidade"] == 4 ? 'selected="selected"' : '' ?>>4 níveis</option>
												<option value="5" <?= $_REQUEST["profundidade"] == 5 ? 'selected="selected"' : '' ?>>5 níveis</option>
												<option value="6" <?= $_REQUEST["profundidade"] == 6 ? 'selected="selected"' : '' ?>>6 níveis</option>
											</select>
										</td>
									</tr>
									<script language="javascript" type="text/javascript">
										
										function SetAllCheckBoxes( FormName, FieldName, CheckValue ) {
											if(!document.forms[FormName])
												return;
											var objCheckBoxes = document.forms[FormName].elements[FieldName];
											if(!objCheckBoxes)
												return;
											var countCheckBoxes = objCheckBoxes.length;
											if(!countCheckBoxes)
												objCheckBoxes.checked = CheckValue;
											else
												for(var i = 0; i < countCheckBoxes; i++)
													objCheckBoxes[i].checked = CheckValue;
										}
										
									</script>
									<tr>
										<td align="right">
											Situação
											(<a href="" onclick="SetAllCheckBoxes( 'formulario_filtro_arvore', 'situacao[]', true ); return false;">todos</a>)
										</td>
										<td>
											<?php
											
											// força o preenchimento do formulário
											if ( $_REQUEST["formulario_filtro_arvore"] ) {
												$situacao = $_REQUEST["situacao"];
												if( isset($_REQUEST['atiidraiz']) ){
													$atividade["atiid"] = $_REQUEST['atiidraiz'];
												}
											} else {
												$situacao = array(
													STATUS_NAO_INICIADO,
													STATUS_EM_ANDAMENTO
												);
											}
											
											$situacao = (array) $situacao;
											
											?>
											<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_NAO_INICIADO ?>" <?= in_array( STATUS_NAO_INICIADO, $situacao ) ? 'checked="checked"' : '' ?>/>não iniciado</label>
											<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_EM_ANDAMENTO ?>" <?= in_array( STATUS_EM_ANDAMENTO, $situacao ) ? 'checked="checked"' : '' ?>/>em andamento</label>
											<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_SUSPENSO ?>" <?= in_array( STATUS_SUSPENSO, $situacao ) ? 'checked="checked"' : '' ?>/>suspenso</label>
											<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_CANCELADO ?>" <?= in_array( STATUS_CANCELADO, $situacao ) ? 'checked="checked"' : '' ?>/>cancelado</label>
											<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_CONCLUIDO ?>" <?= in_array( STATUS_CONCLUIDO, $situacao ) ? 'checked="checked"' : '' ?>/>concluído</label>
										</td>
									</tr>
									<?php if( atividade_verificar_responsabilidade( $_SESSION['projeto'], $_SESSION["usucpf"] ) ): ?>
										<tr>
											<td align="right">Sob Responsabilidade</td>
											<td>
												<?php
												
												// força o preenchimento do formulário
												$usucpf = $_REQUEST["usucpf"];
												
												$sql = "
													select
														u.usucpf as codigo,
														u.usunome as descricao
													from seguranca.usuario u
														inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
														inner join pde.usuarioresponsabilidade ur on ur.usucpf = u.usucpf
														inner join seguranca.perfil p on p.pflcod = ur.pflcod
														inner join seguranca.perfilusuario pu on pu.pflcod = p.pflcod and pu.usucpf = u.usucpf
														inner join pde.atividade a on a.atiid = ur.atiid
													where
														u.suscod = 'A'
														and us.suscod = 'A'
														and us.sisid = ". $_SESSION["sisid"] ."
														and ur.rpustatus = 'A'
														and ur.pflcod = ". PERFIL_GERENTE ."
														and a.atistatus = 'A'
														and a._atiprojeto = ". $_SESSION['projeto'] ."
													group by u.usucpf, u.usunomeguerra, u.usunome
													order by u.usunome
												";
												$db->monta_combo(
													"usucpf",
													$sql,
													"S",
													"- selecione -",
													"", ""
												);
												
												?>
											</td>
										</tr>
									<?php endif; ?>
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
			<hr size="1" noshade="noshade" color="#dddddd" style="margin: 15px 0pt;">
			<?php
			$situacao = array(
				STATUS_NAO_INICIADO,
				STATUS_EM_ANDAMENTO,
				STATUS_SUSPENSO,
				STATUS_CANCELADO,
				STATUS_CONCLUIDO
			);
			$diretorio = $_SESSION["sisdiretorio"] . "/planotrabalhoUN";
			echo arvore( $atividade["atiid"], $profundidade, $situacao, null, $diretorio, $numeracao_relativa = true );
			?>
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr style="background-color: #cccccc">	
		<td>
			<input type="hidden" name="atiid" value="<?php echo $_GET[atiid]; ?>">
		</td>
	</tr>		
</table>