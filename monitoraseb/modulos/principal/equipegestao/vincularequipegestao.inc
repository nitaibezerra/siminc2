<?php
 
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

include APPRAIZ."monitoraseb/classes/SubacaoCoordenacao.class.inc";
monta_titulo( "Equipe de Gestão", '' );

$oSubacaoCoordenacao = new SubacaoCoordenacao();

$coordenacoes = $oSubacaoCoordenacao->carregaCoordenacoes();

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />
<!--<link rel="stylesheet" href="../includes/jquery-treeview/red-treeview.css" />
<link rel="stylesheet" href="screen.css" />
--><script type="text/javascript" src="../includes/jquery-treeview/lib/jquery.cookie.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview-simec.js"></script>
<script type="text/javascript">
<!--

var janela;

$(document).ready(function(){
	
	$("#_arvore").treeview({
		collapsed: true,
		animated: "medium",
		control:"#sidetreecontrol",
		persist: "cookie"
	});

	$('#abrirTodos').click();

	$(window).unload(function() {
		janela.close();
	});

});

function associarSubacao(cooid){
	janela = window.open('?modulo=principal/equipegestao/popup/selecionarsubacao&acao=A&cooid='+cooid,'Associar','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=900,height=700');
	janela.focus();
	return false;
}

function excluirVinculo(vinId){
	if ( confirm( 'Deseja desassociar esta Subação?' ) ) {
		location.href= window.location+'&excluirVinId='+vinId;
	}
	return false;
}

//-->
</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td width="100%" valign="top" style="background: none repeat scroll 0% 0%;" id="_arvore">
			<div id="main" style="background: #f5f5f5">
				<div id="sidetree">
					<div class="treeheader">&nbsp;</div>
					<div id="sidetreecontrol"><a href="?#">Fechar Todos</a> | <a href="?#" id="abrirTodos">Abrir Todos</a> </div>
					<ul id="tree" class="filetree treeview-famfamfam">
						<li><span class="folder">Equipes de Gestão</span>
							<ul>
								<?php if(count($coordenacoes) && $coordenacoes[0]): ?>
									<?php foreach($coordenacoes as $coordenacao) : ?>
										<li>
											<span><img alt="" src="../imagens/gif_inclui.gif" title="Associar" style="cursor: pointer" onclick="associarSubacao('<?php echo $coordenacao['coonid']; ?>')" align="top" border="0" /> <img alt="" src="../imagens/pessoas.png" align="top" border="0" /> <?php echo $coordenacao['coodsc']; ?>(<?=$coordenacao['usunome']?>)</span>
											<?php 
												$vinculos = $oSubacaoCoordenacao->carregaVinculos($coordenacao['coonid']);
											?>
											<ul>
												<?php if(count($vinculos) && $vinculos[0]): ?>
													<?php foreach($vinculos as $subacaocoordenacao) : ?>
														<li><span> <img src="../imagens/icones/qb.png" align="top" border="0" /> <?php echo $subacaocoordenacao['sbatitulo']; ?>(<?=$subacaocoordenacao['usunome']?>)</span>
														<?php 
															$listaAcoes = $oSubacaoCoordenacao->listarAcoes($subacaocoordenacao['sbaid']);
															
															if(count($listaAcoes) && $listaAcoes[0]){ ?>
																<ul> 
																<?php foreach($listaAcoes as $acao) : ?>
																	<li><span> <img src="../imagens/icones/br.png" align="top" border="0" /> <?php echo $acao['descricao']; ?></span></li>
																<?php endforeach; 
																?></ul><?php 
															}?>
														</li>
													<?php endforeach;?>
												<?php else: ?>
														<li><span class="file"> Não existe Subação associada, para associar <a href="#" onclick="associarSubacao('<?php echo $coordenacao['coonid']; ?>')">clique aqui.</a></span></li>
												<?php endif;?>
											</ul>
										</li>
									<?php endforeach;?>
								<?php else: ?>
										<li><span class="file"> Não existe nenhuma Coordenação.</span></li>
								<?php endif; ?>
							</ul>
						</li>
					</ul> <!-- ul #tree -->
				</div><!-- div #sidetree -->
			</div><!-- div #main -->
		</td>
	</tr>
</table>